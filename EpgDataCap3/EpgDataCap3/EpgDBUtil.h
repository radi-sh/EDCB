#pragma once

#include "../../Common/Util.h"
#include "./Table/TableUtil.h"
#include "../../Common/EpgDataCap3Def.h"

typedef struct _SHORT_EVENT_INFO{
	BYTE tableID;		//データ追加時のtable_id （優先度 0x4E > 0x50-0x5F > 0x4F > 0x60-0x6F)
	BYTE version;		//データ追加時のバージョン
	wstring event_name;
	wstring text_char;
} SHORT_EVENT_INFO;

typedef struct _EXTENDED_EVENT_INFO{
	BYTE tableID;		//データ追加時のtable_id （優先度 0x4E > 0x50-0x5F > 0x4F > 0x60-0x6F)
	BYTE version;		//データ追加時のバージョン
	wstring text_char;
} EXTENDED_EVENT_INFO;

typedef struct _CONTENT_INFO{
	BYTE tableID;		//データ追加時のtable_id （優先度 0x4E > 0x50-0x5F > 0x4F > 0x60-0x6F)
	BYTE version;		//データ追加時のバージョン
	vector<CContentDesc::NIBBLE_DATA> nibbleList;
} CONTEN_INFO;

typedef struct _COMPONENT_INFO{
	BYTE tableID;		//データ追加時のtable_id （優先度 0x4E > 0x50-0x5F > 0x4F > 0x60-0x6F)
	BYTE version;		//データ追加時のバージョン
	BYTE stream_content;
	BYTE component_type;
	BYTE component_tag;
	wstring text_char;
} COMPONENT_INFO;

typedef struct _AUDIO_COMPONENT_INFO_DATA{
	BYTE stream_content;
	BYTE component_type;
	BYTE component_tag;
	BYTE stream_type;
	BYTE simulcast_group_tag;
	BYTE ES_multi_lingual_flag;
	BYTE main_component_flag;
	BYTE quality_indicator;
	BYTE sampling_rate;
	wstring text_char;
}AUDIO_COMPONENT_INFO_DATA;

typedef struct _AUDIO_COMPONENT_INFO{
	BYTE tableID;		//データ追加時のtable_id （優先度 0x4E > 0x50-0x5F > 0x4F > 0x60-0x6F)
	BYTE version;		//データ追加時のバージョン
	vector<AUDIO_COMPONENT_INFO_DATA> componentList;
} AUDIO_COMPONENT_INFO;

typedef struct _EVENTGROUP_INFO{
	BYTE tableID;		//データ追加時のtable_id （優先度 0x4E > 0x50-0x5F > 0x4F > 0x60-0x6F)
	BYTE version;		//データ追加時のバージョン
	BYTE group_type;
	BYTE event_count;
	vector<CEventGroupDesc::EVENT_DATA2> eventData2List;
} EVENTGROUP_INFO;

typedef struct _EVENT_INFO{
	WORD ONID;
	WORD TSID;
	WORD SID;
	WORD event_id;
	BYTE StartTimeFlag;	//start_timeの値が有効かどうか
	SYSTEMTIME start_time;
	BYTE DurationFlag; //durationの値が有効かどうか
	DWORD durationSec;

	SHORT_EVENT_INFO* shortInfo;
	EXTENDED_EVENT_INFO* extInfo;
	CONTEN_INFO* contentInfo;
	COMPONENT_INFO* componentInfo;
	AUDIO_COMPONENT_INFO* audioInfo;
	EVENTGROUP_INFO* eventGroupInfo;
	EVENTGROUP_INFO* eventRelayInfo;

	BYTE freeCAFlag;
	BOOL pfFlag;
	_EVENT_INFO(void){
		shortInfo = NULL;
		extInfo = NULL;
		contentInfo = NULL;
		componentInfo = NULL;
		audioInfo = NULL;
		eventGroupInfo = NULL;
		eventRelayInfo = NULL;
		freeCAFlag = 0;
		pfFlag = FALSE;
	};
	~_EVENT_INFO(void){
		SAFE_DELETE(shortInfo);
		SAFE_DELETE(extInfo);
		SAFE_DELETE(contentInfo);
		SAFE_DELETE(componentInfo);
		SAFE_DELETE(audioInfo);
		SAFE_DELETE(eventGroupInfo);
		SAFE_DELETE(eventRelayInfo);
	};
} EVENT_INFO;

typedef struct _SERVICE_EVENT_INFO{
	map<WORD, EVENT_INFO*> eventMap;
	EVENT_INFO* nowEvent;	//map側でdeleteされるのでdeleteする必要なし
	EVENT_INFO* nextEvent;	//map側でdeleteされるのでdeleteする必要なし
	_SERVICE_EVENT_INFO(void){
		nowEvent = NULL;
		nextEvent = NULL;
	};
	~_SERVICE_EVENT_INFO(void){
		map<WORD, EVENT_INFO*>::iterator itr;
		for( itr = eventMap.begin(); itr != eventMap.end(); itr++ ){
			SAFE_DELETE(itr->second);
		}
		this->eventMap.clear();
	};
} SERVICE_EVENT_INFO;

typedef struct _SECTION_FLAG_INFO{
	ULONGLONG sectionFlag[32];	//そのセクションが取得済かどうかのbitmap
	WORD last_section_number;	//最終のセクション通し番号
	BYTE version[8];			//バージョン番号[サブテーブル番号]
	void Clear(){
		memset(&sectionFlag, 0, sizeof(sectionFlag));
		last_section_number = (WORD)~0;
		for( int i=0; i<8; i++ ){
			version[i] = (BYTE)~0;
		}
	}
	_SECTION_FLAG_INFO(void){
		_SECTION_FLAG_INFO::Clear();
	};
}SECTION_FLAG_INFO;

typedef struct _SECTION_STATUS_INFO{
	BYTE EIT_schedule_flag;				//EIT(スケジュール)が提供されている
	BYTE EIT_present_following_flag;	//EIT(p/f)が提供されている
	BYTE otherStreamFlag;				//1=他ストリーム
	SECTION_FLAG_INFO sectionBasic;		//基本情報のセクションフラグ
	SECTION_FLAG_INFO sectionExt;		//拡張情報のセクションフラグ
	SECTION_FLAG_INFO sectionPF;		//p/fのセクションフラグ
	_SECTION_STATUS_INFO(void){
		EIT_schedule_flag = 0;
		EIT_present_following_flag = 0;
		otherStreamFlag = 0;
	};
}SECTION_STATUS_INFO;

class CEpgDBUtil
{
public:
	CEpgDBUtil(void);
	~CEpgDBUtil(void);

	BOOL AddEIT(WORD PID, CEITTable* eit);
	BOOL AddEIT_SD(WORD PID, CEITTable_SD* eit);
	BOOL AddEIT_SD2(WORD PID, CEITTable_SD2* eit);

	BOOL AddServiceList(CNITTable* nit);
	BOOL AddServiceList(WORD TSID, CSITTable* sit);
	BOOL AddSDT(CSDTTable* sdt);

	BOOL SetTDTTime(SYSTEMTIME* time);

	void SetStreamChangeEvent();

	//EPGデータの蓄積状態をリセットする
	void ClearSectionStatus();

	//EPG取得内容の設定
	//引数：
	// settings		[IN]設定値
	void SetEpgGetSettings(
		WORD settings
		);

	//EPGデータの蓄積状態を取得する
	//戻り値：
	// ステータス
	//引数：
	// l_eitFlag		[IN]L-EITのステータスを取得
	EPG_SECTION_STATUS GetSectionStatus(BOOL l_eitFlag);

	//指定サービスの全EPG情報を取得する
	//戻り値：
	// エラーコード
	//引数：
	// originalNetworkID		[IN]取得対象のoriginalNetworkID
	// transportStreamID		[IN]取得対象のtransportStreamID
	// serviceID				[IN]取得対象のServiceID
	// epgInfoListSize			[OUT]epgInfoListの個数
	// epgInfoList				[OUT]EPG情報のリスト（DLL内で自動的にdeleteする。次に取得を行うまで有効）
	DWORD GetEpgInfoList(
		WORD originalNetworkID,
		WORD transportStreamID,
		WORD serviceID,
		DWORD* epgInfoListSize,
		EPG_EVENT_INFO** epgInfoList
		);

	//蓄積されたEPG情報のあるサービス一覧を取得する
	//SERVICE_EXT_INFOの情報はない場合がある
	//戻り値：
	// エラーコード
	//引数：
	// serviceListSize			[OUT]serviceListの個数
	// serviceList				[OUT]サービス情報のリスト（DLL内で自動的にdeleteする。次に取得を行うまで有効）
	DWORD GetServiceListEpgDB(
		DWORD* serviceListSize,
		SERVICE_INFO** serviceList
		);

	//指定サービスの現在or次のEPG情報を取得する
	//戻り値：
	// エラーコード
	//引数：
	// originalNetworkID		[IN]取得対象のoriginalNetworkID
	// transportStreamID		[IN]取得対象のtransportStreamID
	// serviceID				[IN]取得対象のServiceID
	// nextFlag					[IN]TRUE（次の番組）、FALSE（現在の番組）
	// nowTime					[IN]現在の時間
	// epgInfo					[OUT]EPG情報（DLL内で自動的にdeleteする。次に取得を行うまで有効）
	DWORD GetEpgInfo(
		WORD originalNetworkID,
		WORD transportStreamID,
		WORD serviceID,
		BOOL nextFlag,
		SYSTEMTIME nowTime,
		EPG_EVENT_INFO** epgInfo
		);

	//指定イベントのEPG情報を取得する
	//戻り値：
	// エラーコード
	//引数：
	// originalNetworkID		[IN]取得対象のoriginalNetworkID
	// transportStreamID		[IN]取得対象のtransportStreamID
	// serviceID				[IN]取得対象のServiceID
	// EventID					[IN]取得対象のEventID
	// pfOnlyFlag				[IN]p/fからのみ検索するかどうか
	// epgInfo					[OUT]EPG情報（DLL内で自動的にdeleteする。次に取得を行うまで有効）
	DWORD SearchEpgInfo(
		WORD originalNetworkID,
		WORD transportStreamID,
		WORD serviceID,
		WORD eventID,
		BYTE pfOnlyFlag,
		EPG_EVENT_INFO** epgInfo
		);

protected:
	HANDLE lockEvent;

	map<ULONGLONG, SERVICE_EVENT_INFO*> serviceEventMap;
	map<ULONGLONG, SECTION_STATUS_INFO*> sectionMap;
	map<ULONGLONG, BYTE> serviceList;

	map<ULONGLONG, SERVICE_EVENT_INFO*> serviceEventMapSD;
	map<ULONGLONG, SECTION_STATUS_INFO*> sectionMapSD;


	typedef struct _DB_SERVICE_INFO{
		WORD original_network_id;	//original_network_id
		WORD transport_stream_id;	//transport_stream_id
		WORD service_id;			//service_id
		BYTE service_type;
		BYTE partialReceptionFlag;
		wstring service_provider_name;
		wstring service_name;
		_DB_SERVICE_INFO(void){
			service_type = 0;
			partialReceptionFlag = FALSE;
			service_provider_name = L"";
			service_name = L"";
		};
		~_DB_SERVICE_INFO(void){
		};
	}DB_SERVICE_INFO;
	typedef struct _DB_TS_INFO{
		WORD original_network_id;	//original_network_id
		WORD transport_stream_id;	//transport_stream_id
		wstring network_name;
		wstring ts_name;
		BYTE remote_control_key_id;
		map<WORD,DB_SERVICE_INFO*> serviceList;
		_DB_TS_INFO(void){
			network_name = L"";
			ts_name = L"";
			remote_control_key_id = 0;
		};
		~_DB_TS_INFO(void){
			map<WORD,DB_SERVICE_INFO*>::iterator itr;
			for( itr=serviceList.begin(); itr != serviceList.end(); itr++ ){
				SAFE_DELETE(itr->second);
			}
			serviceList.clear();
		};
	}DB_TS_INFO;
	map<DWORD, DB_TS_INFO*> serviceInfoList;

	DWORD epgInfoListSize;
	EPG_EVENT_INFO* epgInfoList;

	EPG_EVENT_INFO* epgInfo;

	EPG_EVENT_INFO* searchEpgInfo;

	DWORD epgSearchListSize;
	EPG_EVENT_INFO* epgSearchList;

	DWORD serviceDBListSize;
	SERVICE_INFO* serviceDBList;

	WORD ONID;
	WORD TSID;
	SYSTEMTIME tdtTime;

	WORD epgGetSettings;

protected:
	//PublicAPI排他制御用
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 60*1000);
	void UnLock(LPCWSTR log = NULL);

	void Clear();
	
	BOOL AddShortEvent(CEITTable* eit, EVENT_INFO* eventInfo, CShortEventDesc* shortEvent);
	BOOL AddExtEvent(CEITTable* eit, EVENT_INFO* eventInfo, vector<DESCRIPTOR_DATA*>* descriptorList);
	BOOL AddContent(CEITTable* eit, EVENT_INFO* eventInfo, CContentDesc* content);
	BOOL AddComponent(CEITTable* eit, EVENT_INFO* eventInfo, CComponentDesc* component);
	BOOL AddAudioComponent(CEITTable* eit, EVENT_INFO* eventInfo, vector<DESCRIPTOR_DATA*>* descriptorList);
	BOOL AddEventGroup(CEITTable* eit, EVENT_INFO* eventInfo, CEventGroupDesc* eventGroup);
	BOOL AddEventRelay(CEITTable* eit, EVENT_INFO* eventInfo, CEventGroupDesc* eventGroup);
	BOOL CheckUpdate(CEITTable* eit, BYTE tableID, BYTE version);

	BOOL AddShortEvent_SD(CEITTable_SD* eit, EVENT_INFO* eventInfo, CShortEventDesc* shortEvent);
	BOOL AddExtEvent_SD(CEITTable_SD* eit, EVENT_INFO* eventInfo, vector<DESCRIPTOR_DATA*>* descriptorList);
	BOOL AddContent_SD(CEITTable_SD* eit, EVENT_INFO* eventInfo, CContentDesc* content);
	BOOL AddComponent_SD(CEITTable_SD* eit, EVENT_INFO* eventInfo, CComponentDesc* component);
	BOOL AddAudioComponent_SD(CEITTable_SD* eit, EVENT_INFO* eventInfo, vector<DESCRIPTOR_DATA*>* descriptorList);
	BOOL CheckUpdate_SD(CEITTable_SD* eit, BYTE tableID, BYTE version);

	BOOL AddSDEventMap(CEITTable_SD* eit);
	void ClearEvent();
	void ClearSectionMap();
	void ClearEventSD();

	void CopyEpgInfo(EPG_EVENT_INFO* destInfo, EVENT_INFO* srcInfo);
};
