#pragma once

#include "../../Common/Util.h"
#include "../../Common/EpgTimerUtil.h"
#include "../../Common/PathUtil.h"
#include "../../Common/StringUtil.h"
#include "../../Common/ParseReserveText.h"
#include "../../Common/ParseRecInfoText.h"
#include "../../Common/ParseChText5.h"
#include "../../Common/ParseSearchChgText.h"

#include "TwitterManager.h"

#include "NotifyManager.h"
#include "ReserveInfo.h"
#include "TunerManager.h"
#include "BatManager.h"
#include "NWCoopManager.h"
#include "RecInfoDBManager.h"

class CReserveManager
{
public:
	CReserveManager(void);
	~CReserveManager(void);
/*
	void SetRegistGUI(map<DWORD, DWORD> registGUIMap);
	void SetRegistTCP(map<wstring, REGIST_TCP_INFO> registTCPMap);
*/
	void SetNotifyManager(CNotifyManager* manager);
	void SetEpgDBManager(CEpgDBManager* epgDBManager);
	void ChangeRegist();
	void ReloadSetting();

	//�^��ςݏ��̓ǂݍ��݂��s��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL ReloadRecInfoData();

	//�\����̓ǂݍ��݂��s��
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL ReloadReserveData();

	//�\�����ǉ��œǂݍ���
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	BOOL AddLoadReserveData();

	//�\������擾����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// reserveList		[OUT]�\����ꗗ�i�Ăяo�����ŉ������K�v����j
	BOOL GetReserveDataAll(
		vector<RESERVE_DATA*>* reserveList
		);

	//�`���[�i�[���̗\������擾����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// reserveList		[OUT]�\����ꗗ
	BOOL GetTunerReserveAll(
		vector<TUNER_RESERVE_INFO>* list
		);

	//�\������擾����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// id				[IN]�\��ID
	// reserveData		[OUT]�\����
	BOOL GetReserveData(
		DWORD id,
		RESERVE_DATA* reserveData
		);

	//�\�����ǉ�����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// reserveList		[IN]�\����
	BOOL AddReserveData(
		vector<RESERVE_DATA>* reserveList,
		BOOL tweet = FALSE
		);

	//�\�����ύX����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// reserveList		[IN]�\����
	BOOL ChgReserveData(
		vector<RESERVE_DATA>* reserveList,
		BOOL timeChg = FALSE
		);

	//�\������폜����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// reserveList		[IN]�\��ID���X�g
	BOOL DelReserveData(
		vector<DWORD>* reserveList
		);

	//�\��̐U�蕪�����s��
	void ReloadBankMap(BOOL notify);
	
	//�^��ςݏ��ꗗ���擾����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// infoList			[OUT]�^��ςݏ��ꗗ
	BOOL GetRecFileInfoAll(
		vector<REC_FILE_INFO>* infoList
		);

	//�^��ςݏ����폜����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// idList			[IN]ID���X�g
	BOOL DelRecFileInfo(
		vector<DWORD>* idList
		);

	//�^��ςݏ��̃v���e�N�g��ύX����
	//�߂�l�F
	// TRUE�i�����j�AFALSE�i���s�j
	//�����F
	// idList			[IN]ID���X�g
	BOOL ChgProtectRecFileInfo(
		vector<REC_FILE_INFO>* infoList
		);


	BOOL IsEnableSuspend(
		BYTE* suspendMode,
		BYTE* rebootFlag
		);

	BOOL IsEnableReloadEPG(
		);

	BOOL IsSuspendOK();

	BOOL GetSleepReturnTime(
		LONGLONG* returnTime
		);

	BOOL StartEpgCap();
	void StopEpgCap();
	BOOL IsEpgCap();

	BOOL IsFindReserve(
		WORD ONID,
		WORD TSID,
		WORD SID,
		WORD eventID
		);

	BOOL IsFindReserve(
		WORD ONID,
		WORD TSID,
		WORD SID,
		LONGLONG startTime,
		DWORD durationSec
		);

	void SendNotifyUpdate(DWORD notifyId);
	void SendNotifyChgReserveAutoAdd(RESERVE_DATA* oldInfo, RESERVE_DATA* newInfo);

	BOOL GetTVTestChgCh(
		LONGLONG chID,
		TVTEST_CH_CHG_INFO* chInfo
		);

	BOOL SetNWTVCh(
		SET_CH_INFO* chInfo
		);

	BOOL CloseNWTV(
		);

	void SetNWTVMode(
		DWORD mode
		);

	void SendTweet(
		SEND_TWEET_MODE mode,
		void* param1,
		void* param2,
		void* param3
		);

	BOOL GetRecFilePath(
		DWORD reserveID,
		wstring& filePath,
		DWORD* ctrlID,
		DWORD* processID
		);

	//�\��ǉ��\���`�F�b�N����
	BOOL ChkAddReserve(RESERVE_DATA* chkData, WORD* status);

	//6���ȓ��̘^�挋�ʂɓ����ԑg�����邩�`�F�b�N����
	BOOL IsFindRecEventInfo(EPGDB_EVENT_INFO* info, WORD chkDay);
	void ChgAutoAddNoRec(EPGDB_EVENT_INFO* info);

	BOOL IsRecInfoChg();
protected:
	HANDLE lockEvent;

	HANDLE bankCheckThread;
	HANDLE bankCheckStopEvent;

	WORD notifyStatus;

	/*
	map<DWORD, DWORD> registGUIMap;
	map<wstring, REGIST_TCP_INFO> registTCPMap;
	HANDLE lockNotify;
	HANDLE notifyThread;
	HANDLE notifyStopEvent;
	HANDLE notifyStatusThread;
	HANDLE notifyStatusStopEvent;

	HANDLE notifyEpgReloadThread;
	HANDLE notifyEpgReloadStopEvent;
	*/
	CNotifyManager* notifyManager;

	CParseReserveText reserveText;
	map<DWORD, CReserveInfo*> reserveInfoMap; //�L�[�@reserveID
	map<LONGLONG, DWORD> reserveInfoIDMap; //�L�[�@ONID<<48|TSID<<32|SID<<16|EventID
	CParseRecInfoText recInfoText;

	CParseChText5 chUtil;
	CParseSearchChgText chgText;

	CTunerManager tunerManager;
	CBatManager batManager;
	CTwitterManager twitterManager;
	CNWCoopManager nwCoopManager;
	CRecInfoDBManager recInfoManager;

	CEpgDBManager* epgDBManager;

	typedef struct _BANK_WORK_INFO{
		CReserveInfo* reserveInfo;
		LONGLONG startTime;//�}�C�i�X�l�}�[�W���l�������J�n����
		LONGLONG endTime;//�}�C�i�X�l�}�[�W���l�������I������
		BYTE priority;
		BOOL recWaitFlag;
		wstring sortKey;
		DWORD reserveID;
		DWORD chID;		//originalNetworkID<<16 | transportStreamID
		DWORD preTunerID;
		DWORD useTunerID;
		WORD ONID;
		WORD TSID;
		WORD SID;
	}BANK_WORK_INFO;
	typedef struct _BANK_INFO{
		DWORD tunerID;
		map<DWORD, BANK_WORK_INFO*> reserveList; //�L�[ �\��ID
	}BANK_INFO;
	map<DWORD, BANK_INFO*> bankMap; //�L�[ �`���[�i�[ID
	map<DWORD, BANK_WORK_INFO*> NGReserveMap;

	int defStartMargine;
	int defEndMargine;

	BOOL backPriorityFlag;
	BOOL sameChPriorityFlag;

	map<DWORD, CTunerBankCtrl*> tunerBankMap; //�L�[ bonID<<16 | tunerID

	BYTE enableSetSuspendMode;
	BYTE enableSetRebootFlag;
	BYTE enableEpgReload;

	BOOL epgCapCheckFlag;


	BOOL BSOnly;
	BOOL CS1Only;
	BOOL CS2Only;
	LONGLONG ngCapMin;
	LONGLONG ngCapTunerMin;
	typedef struct _EPGTIME_INFO{
		DWORD time;
		wstring swEPGType;		//	len=1�̏ꍇ�A1:��{���̂ݎ擾 ����ȊO:�ڍ׏��擾
								//	len=7�̏ꍇ�A���j����n�܂�1��1�����A0:�擾���Ȃ� 1:��{���̂ݎ擾 2:�ڍ׏��擾
	}EPGTIME_INFO;
	vector<EPGTIME_INFO> epgCapTimeList;
	int wakeTime;
	BYTE defSuspendMode;
	BYTE defRebootFlag;
	int batMargin;
	vector<wstring> noStandbyExeList;
	DWORD noStandbyTime;
	BOOL ngShareFile;
	BOOL autoDel;
	vector<wstring> delExtList;
	vector<wstring> delFolderList;
	BOOL eventRelay;
	BOOL useTweet;
	BOOL useProxy;
	wstring proxySrv;
	wstring proxyID;
	wstring proxyPWD;
	BOOL recEndTweetErr;
	DWORD recEndTweetDrop;

	vector<wstring> tvtestUseBon;

	int duraChgMarginMin;
	int notFindTuijyuHour;
	int noEpgTuijyuMin;

	BOOL autoDelRecInfo;
	int autoDelRecInfoNum;
	BOOL timeSync;
	BOOL setTimeSync;

	DWORD NWTVPID;
	wstring recExePath;
	CSendCtrlCmd sendCtrlNWTV;
	BOOL NWTVUDP;
	BOOL NWTVTCP;
	BOOL useSrvCoop;
	BOOL useResSrvCoop;
	BOOL useEpgSrvCoop;

	BOOL ngAddResSrvCoop;

	BOOL errEndBatRun;

	int reloadBankMapAlgo;
	BOOL useRecNamePlugIn;
	wstring recNamePlugInFilePath;

	BOOL chgRecInfo;
protected:
	//PublicAPI�r������p
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 60*1000);
	void UnLock(LPCWSTR log = NULL);
/*
	BOOL NotifyLock(LPCWSTR log = NULL, DWORD timeOut = 60*1000);
	void NotifyUnLock(LPCWSTR log = NULL);
	*/

	BOOL _AddReserveData(RESERVE_DATA* reserve, BOOL tweet = FALSE);
	BOOL _ChgReserveData(RESERVE_DATA* reserve, BOOL chgTime);

	void _ReloadBankMap();
	void _ReloadBankMapAlgo0();
	void _ReloadBankMapAlgo1();
	void _ReloadBankMapAlgo2();
	void _ReloadBankMapAlgo3();
	void CheckOverTimeReserve();
	void CreateWorkData(CReserveInfo* reserveInfo, BANK_WORK_INFO* workInfo, BOOL backPriority, DWORD reserveCount, DWORD reserveNum, BOOL noTuner = FALSE);
	DWORD ChkInsertStatus(BANK_INFO* bank, BANK_WORK_INFO* inItem);
	DWORD ReChkInsertStatus(BANK_INFO* bank, BANK_WORK_INFO* inItem);
	DWORD ChkInsertNGStatus(BANK_INFO* bank, BANK_WORK_INFO* inItem);
	BOOL ChangeNGReserve(BANK_WORK_INFO* inItem);
	DWORD ChkInsertSameChStatus(BANK_INFO* bank, BANK_WORK_INFO* inItem);

	void _SendNotifyUpdate(DWORD notifyId);
	void _SendNotifyStatus(WORD status);
	void _SendNotifyRecEnd(REC_FILE_INFO* item);
	void _SendNotifyChgReserve(DWORD notifyId, RESERVE_DATA* oldInfo, RESERVE_DATA* newInfo);
/*	static UINT WINAPI SendNotifyThread(LPVOID param);
	static UINT WINAPI SendNotifyStatusThread(LPVOID param);
	void _SendNotifyEpgReload();
	static UINT WINAPI SendNotifyEpgReloadThread(LPVOID param);
	*/
	BOOL _DelReserveData(
		vector<DWORD>* reserveList
	);

	static UINT WINAPI BankCheckThread(LPVOID param);
	void CheckEndReserve();
	void CheckErrReserve();
	void CheckBatWork();
	void CheckTuijyu();
	void CheckNWSrvResCoop();
	BOOL CheckEventRelay(EPGDB_EVENT_INFO* info, RESERVE_DATA* data, BOOL errEnd = FALSE);

	BOOL CheckChgEvent(EPGDB_EVENT_INFO* info, RESERVE_DATA* data, BYTE* chgMode = NULL);
	BOOL CheckChgEventID(EPGDB_EVENT_INFO* info, RESERVE_DATA* data);
	BOOL CheckNotFindChgEvent(RESERVE_DATA* data, CTunerBankCtrl* ctrl, vector<DWORD>* deleteList);
	BOOL ChgDurationChk(EPGDB_EVENT_INFO* info);

	void EnableSuspendWork(BYTE suspendMode, BYTE rebootFlag, BYTE epgReload);
	BOOL _IsSuspendOK(BOOL rebootFlag);
	BOOL IsFindNoSuspendExe();
	BOOL IsFindShareTSFile();

	BOOL GetNextEpgcapTime(LONGLONG* capTime, LONGLONG chkMargineMin,BOOL* swBasicOnly, BOOL* swShortOnly = NULL);

	BOOL _StartEpgCap(BOOL shortOnly = FALSE);
	BOOL _IsEpgCap();

	void _SendTweet(
		SEND_TWEET_MODE mode,
		void* param1,
		void* param2,
		void* param3
		);

	void GetSrvCoopEpgList(vector<wstring>* fileList);
};

