#pragma once

#include "EpgDBUtil.h"
#include "../../Common/TSPacketUtil.h"
#include "../../Common/TSBuffUtil.h"
#include "../../Common/Util.h"
#include "../../Common/ErrDef.h"
#include "../../Common/EpgDataCap3Def.h"

#include "./Table/TableUtil.h"

class CDecodeUtil
{
public:
	CDecodeUtil(void);
	~CDecodeUtil(void);

	void SetEpgDB(CEpgDBUtil* epgDBUtil);
	DWORD AddTSData(BYTE* data, DWORD dataSize);

	//解析データの現在のストリームＩＤを取得する
	//戻り値：
	// エラーコード
	//引数：
	// originalNetworkID		[OUT]現在のoriginalNetworkID
	// transportStreamID		[OUT]現在のtransportStreamID
	DWORD GetTSID(
		WORD* originalNetworkID,
		WORD* transportStreamID
		);

	//自ストリームのサービス一覧を取得する
	//戻り値：
	// エラーコード
	//引数：
	// serviceListSize			[OUT]serviceListの個数
	// serviceList				[OUT]サービス情報のリスト（DLL内で自動的にdeleteする。次に取得を行うまで有効）
	DWORD GetServiceListActual(
		DWORD* serviceListSize,
		SERVICE_INFO** serviceList
		);

	//ストリーム内の現在の時間情報を取得する
	//戻り値：
	// エラーコード
	//引数：
	// time				[OUT]ストリーム内の現在の時間
	DWORD GetNowTime(
		SYSTEMTIME* time
		);

	//PC時計を元としたストリーム時間との差を取得する
	//戻り値：
	// 差の秒数
	int GetTimeDelay(
		);

	//ストリームの変更を通知する
	void SetStreamChangeEvent(
		);

protected:
	HANDLE lockEvent;

	typedef struct _NIT_SECTION_INFO{
		WORD network_id;
		BYTE version_number;
		BYTE last_section_number;
		map<BYTE, CNITTable*> nitSection;
		_NIT_SECTION_INFO(WORD networkId, BYTE versionNumber, BYTE lastSectionNumber){
			network_id = networkId;
			version_number = versionNumber;
			last_section_number = lastSectionNumber;
		}
		_NIT_SECTION_INFO(void){
			_NIT_SECTION_INFO(0, 0xFF, 0xFF);
		}
		~_NIT_SECTION_INFO(void){
			map<BYTE, CNITTable*>::iterator itr;
			for( itr=nitSection.begin(); itr != nitSection.end(); itr++ ){
				SAFE_DELETE(itr->second);
			}
			nitSection.clear();
		};
	}NIT_SECTION_INFO;
	typedef struct _SDT_SECTION_INFO{
		WORD original_network_id;
		WORD transport_stream_id;
		BYTE version_number;
		BYTE last_section_number;
		map<BYTE, CSDTTable*> sdtSection;
		_SDT_SECTION_INFO(WORD originalNetworkId, WORD transportStreamId, BYTE versionNumber, BYTE lastSectionNumber){
			original_network_id = originalNetworkId;
			transport_stream_id = transportStreamId;
			version_number = versionNumber;
			last_section_number = lastSectionNumber;
		}
		_SDT_SECTION_INFO(void){
			_SDT_SECTION_INFO(0, 0,0xFF, 0xFF);
		}
		~_SDT_SECTION_INFO(void){
			map<BYTE, CSDTTable*>::iterator itr;
			for( itr=sdtSection.begin(); itr != sdtSection.end(); itr++ ){
				SAFE_DELETE(itr->second);
			}
			sdtSection.clear();
		};
	}SDT_SECTION_INFO;
	typedef struct _BIT_SECTION_INFO{
		WORD original_network_id;
		BYTE version_number;
		BYTE last_section_number;
		map<BYTE, CBITTable*> bitSection;
		_BIT_SECTION_INFO(WORD originalNetworkId, BYTE versionNumber, BYTE lastSectionNumber){
			original_network_id = originalNetworkId;
			version_number = versionNumber;
			last_section_number = lastSectionNumber;
		}
		_BIT_SECTION_INFO(void){
			_BIT_SECTION_INFO(0, 0xFF, 0xFF);
		}
		~_BIT_SECTION_INFO(void){
			map<BYTE, CBITTable*>::iterator itr;
			for( itr=bitSection.begin(); itr != bitSection.end(); itr++ ){
				SAFE_DELETE(itr->second);
			}
			bitSection.clear();
		};
	}BIT_SECTION_INFO;

	CEpgDBUtil* epgDBUtil;

	//PID毎のバッファリング
	//キー PID
	map<WORD, CTSBuffUtil*> buffUtilMap;

	CPATTable* patInfo;
	CCATTable* catInfo;
	map<WORD, CPMTTable*> pmtMap;
	NIT_SECTION_INFO* nitActualInfo;
	SDT_SECTION_INFO* sdtActualInfo;
	BIT_SECTION_INFO* bitActualInfo;
	map<DWORD, SDT_SECTION_INFO*> sdtOtherMap;
	CTOTTable* totInfo;
	CTDTTable* tdtInfo;
	CSITTable* sitInfo;

	DWORD serviceListSize;
	SERVICE_INFO* serviceList;

	int delaySec;

protected:
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 60*1000);
	void UnLock(LPCWSTR log = NULL);

	void Clear();
	void ClearBuff(WORD noClearPid);
	void ChangeTSIDClear(WORD noClearPid);

	BOOL CheckPAT(WORD PID, CPATTable* pat);
	BOOL CheckCAT(WORD PID, CCATTable* pat);
	BOOL CheckPMT(WORD PID, CPMTTable* pmt);
	BOOL CheckNIT(WORD PID, CNITTable* nit);
	BOOL CheckSDT(WORD PID, CSDTTable* sdt);
	BOOL CheckTOT(WORD PID, CTOTTable* tot);
	BOOL CheckTDT(WORD PID, CTDTTable* tdt);
	BOOL CheckEIT(WORD PID, CEITTable* eit);
	BOOL CheckCDT(WORD PID, CCDTTable* cdt);
	BOOL CheckSDTT(WORD PID, CSDTTTable* sdtt);
	BOOL CheckBIT(WORD PID, CBITTable* bit);
	BOOL CheckSIT(WORD PID, CSITTable* sit);
	BOOL CheckEIT_SD(WORD PID, CEITTable_SD* eit);
	BOOL CheckEIT_SD2(WORD PID, CEITTable_SD2* eit);

	//自ストリームのサービス一覧をSITから取得する
	//戻り値：
	// エラーコード
	//引数：
	// serviceListSize			[OUT]serviceListの個数
	// serviceList				[OUT]サービス情報のリスト（DLL内で自動的にdeleteする。次に取得を行うまで有効）
	DWORD GetServiceListSIT(
		DWORD* serviceListSize,
		SERVICE_INFO** serviceList
		);

};
