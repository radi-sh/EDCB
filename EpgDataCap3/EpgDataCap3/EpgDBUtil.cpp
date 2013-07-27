#include "StdAfx.h"
#include "EpgDBUtil.h"

#include "../../Common/EpgTimerUtil.h"
#include "../../Common/StringUtil.h"
#include "../../Common/TimeUtil.h"
#include "../../Common/ErrDef.h"
#include "ARIB8CharDecode.h"

CEpgDBUtil::CEpgDBUtil(void)
{
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL);

	this->epgInfoList = NULL;
	this->epgInfoListSize = 0;

	this->epgInfo = NULL;
	this->searchEpgInfo = NULL;

	this->epgSearchList = NULL;
	this->epgSearchListSize = 0;

	this->serviceDBList = NULL;
	this->serviceDBListSize = 0;

	this->ONID = 0;
	this->TSID = 0;
	SYSTEMTIME tmp = {0,};
	memcpy(&this->tdtTime, &tmp, sizeof(SYSTEMTIME));

	epgGetSettings = EpgGetServiceVideo | EpgGetLengthLong | EpgGetInfoExt | EpgGetOtherInfoExt;
}

CEpgDBUtil::~CEpgDBUtil(void)
{
	Clear();

	map<DWORD, DB_TS_INFO*>::iterator itrInfo;
	for( itrInfo = this->serviceInfoList.begin(); itrInfo != this->serviceInfoList.end(); itrInfo++ ){
		SAFE_DELETE(itrInfo->second);
	}

	SAFE_DELETE_ARRAY(this->epgInfoList);
	this->epgInfoListSize = 0;

	SAFE_DELETE(this->epgInfo);

	SAFE_DELETE(this->searchEpgInfo);

	SAFE_DELETE_ARRAY(this->epgSearchList);
	this->epgSearchListSize = 0;

	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}

	SAFE_DELETE_ARRAY(this->serviceDBList);
	this->serviceDBListSize = 0;
}

BOOL CEpgDBUtil::Lock(LPCWSTR log, DWORD timeOut)
{
	if( this->lockEvent == NULL ){
		return FALSE;
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
	DWORD dwRet = WaitForSingleObject(this->lockEvent, timeOut);
	if( dwRet == WAIT_ABANDONED || 
		dwRet == WAIT_FAILED){
		return FALSE;
	}
	return TRUE;
}

void CEpgDBUtil::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

void CEpgDBUtil::Clear()
{
	ClearEvent();

	ClearEventSD();
}

void CEpgDBUtil::SetStreamChangeEvent()
{
	if( Lock() == FALSE ) return ;
	ClearSectionMap();
	UnLock();
}

BOOL CEpgDBUtil::AddEIT(WORD PID, CEITTable* eit)
{
	if( eit == NULL ){
		return FALSE;
	}
	if( Lock() == FALSE ) return FALSE;

	BOOL alreadyGot = FALSE;

	//�Z�N�V�����X�e�[�^�X
	map<ULONGLONG, SECTION_STATUS_INFO*>::iterator itrSec;
	SECTION_STATUS_INFO* sectionInfo = NULL;
	ULONGLONG key2;
	//SKY�T�[�r�X��TSID������key
	if( eit->original_network_id == 3 ){
		key2 = _Create64Key(eit->original_network_id, 0, eit->service_id);
	}else{
		key2 = _Create64Key(eit->original_network_id, eit->transport_stream_id, eit->service_id);
	}
	itrSec = this->sectionMap.find(key2);
	if( itrSec == this->sectionMap.end() ){
		sectionInfo = new SECTION_STATUS_INFO;
		this->sectionMap.insert(pair<ULONGLONG, SECTION_STATUS_INFO*>(key2, sectionInfo));
	}else{
		sectionInfo = itrSec->second;
	}

	SECTION_FLAG_INFO* sectionFlag = NULL;
	BYTE tableIndex = 0;	//�T�u�e�[�u���̐擪����̃C���f�b�N�X
	BYTE lastTableIndex = 0; //last_table_id�̑ւ��(BS103��p/f��table_id=4f,last_table_id=4e�ő��o�����΍�)
	BYTE sectionIndex = eit->section_number;  //���̃T�u�e�[�u���̃Z�N�V�����ԍ�
	BYTE segmentLen = 1; //�Z�O�����g�̃Z�N�V������

	if( eit->table_id == 0x4E || eit->table_id == 0x4F ){
		sectionFlag = &sectionInfo->sectionPF;
	}else if( 0x50 <= eit->table_id && eit->table_id <= 0x57 ||	0x60 <= eit->table_id && eit->table_id <= 0x67){
		sectionFlag = &sectionInfo->sectionBasic;
		tableIndex = eit->table_id & 0x07;
		lastTableIndex = eit->last_table_id & 0x07;
		segmentLen = 8;
	}else if( 0x58 <= eit->table_id && eit->table_id <= 0x5F ||	0x68 <= eit->table_id && eit->table_id <= 0x6F){
		sectionFlag = &sectionInfo->sectionExt;
		tableIndex = eit->table_id & 0x07;
		lastTableIndex = eit->last_table_id & 0x07;
		segmentLen = 8;
	}

	if( sectionFlag != NULL ){
		//�o�[�W�����ԍ����ւ���Ă�����Clear
		if( eit->version_number != sectionFlag->version[tableIndex] ){
			if( (BYTE)~0 != sectionFlag->version[tableIndex] ){
				sectionFlag->Clear();
			}
			sectionFlag->version[tableIndex] = eit->version_number;
		}
		//���݂̃e�[�u���ԍ����T�u�e�[�u���̍ŏI�Ȃ�΍ŏI�ʂ��Z�N�V�����ԍ����L�^
		if( sectionFlag->last_section_number == (WORD)~0 && tableIndex == lastTableIndex ){
			sectionFlag->last_section_number = eit->last_section_number + (tableIndex * 256);
		}
		//���݂̃Z�N�V�����ʂ��ԍ� (�T�u�e�[�u���ԍ�*256)+�Z�N�V�����ԍ�
		WORD sectionNo = sectionIndex + (tableIndex * 256);

		//���擾�̃Z�N�V�����H
		if( (sectionFlag->sectionFlag[sectionNo/64] & (1ULL<<(sectionNo%64))) == 0 ){
			//�Z�O�����g�̍ŏI�Z�N�V�����ł���Ό��݂̃Z�N�V�����ԍ��ȍ~�𖄂߂�
			if( eit->section_number == eit->segment_last_section_number ){
				for( WORD i=sectionNo%64; i<=((sectionNo%64) | (segmentLen - 1)); i++ ){
					sectionFlag->sectionFlag[sectionNo/64] |= 1ULL<<i;
				}
			//���݂̃Z�N�V�����ԍ��ɊY������bit�݂̂𗧂Ă�
			}else{
				sectionFlag->sectionFlag[sectionNo/64] |= 1ULL<<sectionNo%64;
			}
		}else{
			alreadyGot = TRUE;
		}
	}

	if( alreadyGot || eit->failure ){
		UnLock();
		return TRUE;
	}

	ULONGLONG key = _Create64Key(eit->original_network_id, eit->transport_stream_id, eit->service_id);

	//�T�[�r�X��map���擾
	map<ULONGLONG, SERVICE_EVENT_INFO*>::iterator itr;
	SERVICE_EVENT_INFO* serviceInfo = NULL;

	itr = serviceEventMap.find(key);
	if( itr == serviceEventMap.end() ){
		serviceInfo = new SERVICE_EVENT_INFO;
		serviceEventMap.insert(pair<ULONGLONG, SERVICE_EVENT_INFO*>(key, serviceInfo));
	}else{
		serviceInfo = itr->second;
	}

	//�C�x���g���ƂɍX�V�K�v������
	for( size_t i=0; i<eit->eventInfoList.size(); i++ ){
		CEITTable::EVENT_INFO_DATA* eitEventInfo = eit->eventInfoList[i];
		map<WORD, EVENT_INFO*>::iterator itrEvent;
		EVENT_INFO* eventInfo = NULL;

		if( eitEventInfo->running_status == 1 || eitEventInfo->running_status == 3 ){
			//����s���܂��͒�~��
			_OutputDebugString(L"������s���܂��͒�~���C�x���g ONID:0x%04x TSID:0x%04x SID:0x%04x EventID:0x%04x %04d/%02d/%02d %02d:%02d",
				eit->original_network_id,  eit->transport_stream_id, eit->service_id, eitEventInfo->event_id,
				eitEventInfo->start_time.wYear, eitEventInfo->start_time.wMonth, eitEventInfo->start_time.wDay, eitEventInfo->start_time.wHour, eitEventInfo->start_time.wMinute
				);
			continue;
		}

		itrEvent = serviceInfo->eventMap.find(eitEventInfo->event_id);
		if( itrEvent == serviceInfo->eventMap.end() ){
			eventInfo = new EVENT_INFO;
			eventInfo->ONID = eit->original_network_id;
			eventInfo->TSID = eit->transport_stream_id;
			eventInfo->SID = eit->service_id;
			eventInfo->event_id = eitEventInfo->event_id;
			eventInfo->StartTimeFlag = eitEventInfo->StartTimeFlag;
			eventInfo->start_time = eitEventInfo->start_time;
			eventInfo->DurationFlag = eitEventInfo->DurationFlag;
			eventInfo->durationSec = eitEventInfo->durationHH*60*60 +
				eitEventInfo->durationMM*60 +
				eitEventInfo->durationSS;
			eventInfo->freeCAFlag = eitEventInfo->free_CA_mode;
			serviceInfo->eventMap.insert(pair<WORD, EVENT_INFO*>(eventInfo->event_id, eventInfo));
		}else{
			eventInfo = itrEvent->second;
		}
		if( eit->table_id == 0x4E || eit->table_id == 0x4F ){
			//p/f�Ȃ̂Ŏ��ԍX�V
			eventInfo->StartTimeFlag = eitEventInfo->StartTimeFlag;
			eventInfo->start_time = eitEventInfo->start_time;
			eventInfo->DurationFlag = eitEventInfo->DurationFlag;
			eventInfo->durationSec = eitEventInfo->durationHH*60*60 +
				eitEventInfo->durationMM*60 +
				eitEventInfo->durationSS;

			if( eit->section_number == 0 ){
				serviceInfo->nowEvent = eventInfo;
				eventInfo->pfFlag = TRUE;
			}else if( eit->section_number == 1 ){
				serviceInfo->nextEvent = eventInfo;
				eventInfo->pfFlag = TRUE;
			}
		}else if( 0x50 <= eit->table_id && eit->table_id <= 0x5F ){
			if( serviceInfo->nowEvent != NULL && serviceInfo->nextEvent != NULL ){
				if( serviceInfo->nowEvent->event_id != eitEventInfo->event_id &&
					serviceInfo->nextEvent->event_id != eitEventInfo->event_id &&
					eventInfo->pfFlag == FALSE){
					//���X�g���[����p/f����Ȃ��Ȃ玞�ԍX�V
					eventInfo->StartTimeFlag = eitEventInfo->StartTimeFlag;
					eventInfo->start_time = eitEventInfo->start_time;
					eventInfo->DurationFlag = eitEventInfo->DurationFlag;
					eventInfo->durationSec = eitEventInfo->durationHH*60*60 +
						eitEventInfo->durationMM*60 +
						eitEventInfo->durationSS;
				}
			}
		}

		//ExtendedEvent�͕�������̂�1�x�����`�F�b�N����
		BOOL checkExtFlag = FALSE;
		BOOL checkAudioFlag = FALSE;
		for( size_t j=0; j<eitEventInfo->descriptorList.size(); j++ ){
			if( eitEventInfo->descriptorList[j]->shortEvent != NULL ){
				AddShortEvent( eit, eventInfo, eitEventInfo->descriptorList[j]->shortEvent );
			}else if( eitEventInfo->descriptorList[j]->extendedEvent != NULL && checkExtFlag == FALSE){
				AddExtEvent(eit, eventInfo, &eitEventInfo->descriptorList );
				checkExtFlag = TRUE;
			}else if( eitEventInfo->descriptorList[j]->content != NULL ){
				AddContent( eit, eventInfo, eitEventInfo->descriptorList[j]->content );
			}else if( eitEventInfo->descriptorList[j]->component != NULL ){
				AddComponent( eit, eventInfo, eitEventInfo->descriptorList[j]->component );
			}else if( eitEventInfo->descriptorList[j]->audioComponent != NULL && checkAudioFlag == FALSE ){
				AddAudioComponent( eit, eventInfo, &eitEventInfo->descriptorList );
				checkAudioFlag = TRUE;
			}else if( eitEventInfo->descriptorList[j]->eventGroup != NULL ){
				if( eitEventInfo->descriptorList[j]->eventGroup->group_type == 0x01 ){
					AddEventGroup( eit, eventInfo, eitEventInfo->descriptorList[j]->eventGroup );
				}else if( eitEventInfo->descriptorList[j]->eventGroup->group_type == 0x02 ||
					eitEventInfo->descriptorList[j]->eventGroup->group_type == 0x04){
					AddEventRelay( eit, eventInfo, eitEventInfo->descriptorList[j]->eventGroup );
				}
			}
		}
	}

	UnLock();

	return TRUE;
}

BOOL CEpgDBUtil::AddEIT_SD(WORD PID, CEITTable_SD* eit)
{
	if( eit == NULL ){
		return FALSE;
	}
	if( Lock() == FALSE ) return FALSE;

	if( eit->original_network_id == 0x0003 ){
		BOOL ret = AddSDEventMap(eit);
		UnLock();
		return ret;
	}

	ULONGLONG key = _Create64Key(eit->original_network_id, eit->transport_stream_id, eit->service_id);

	//�T�[�r�X��map���擾
	map<ULONGLONG, SERVICE_EVENT_INFO*>::iterator itr;
	SERVICE_EVENT_INFO* serviceInfo = NULL;

	itr = serviceEventMap.find(key);
	if( itr == serviceEventMap.end() ){
		serviceInfo = new SERVICE_EVENT_INFO;
		serviceEventMap.insert(pair<ULONGLONG, SERVICE_EVENT_INFO*>(key, serviceInfo));
	}else{
		serviceInfo = itr->second;
	}

	//�C�x���g���ƂɍX�V�K�v������
	for( size_t i=0; i<eit->eventInfoList.size(); i++ ){
		CEITTable_SD::EVENT_INFO_DATA* eitEventInfo = eit->eventInfoList[i];
		map<WORD, EVENT_INFO*>::iterator itrEvent;
		EVENT_INFO* eventInfo = NULL;

		if( eitEventInfo->running_status != 0 && eitEventInfo->running_status != 2 && eitEventInfo->running_status != 4 ){
			//����s���܂��͒�~��
			_OutputDebugString(L"������s���܂��͒�~���C�x���g ONID:0x%04x TSID:0x%04x SID:0x%04x EventID:0x%04x %04d/%02d/%02d %02d:%02d\r\n",
				eit->original_network_id,  eit->transport_stream_id, eit->service_id, eitEventInfo->event_id,
				eitEventInfo->start_time.wYear, eitEventInfo->start_time.wMonth, eitEventInfo->start_time.wDay, eitEventInfo->start_time.wHour, eitEventInfo->start_time.wMinute
				);
			continue;
		}

		itrEvent = serviceInfo->eventMap.find(eitEventInfo->event_id);
		if( itrEvent == serviceInfo->eventMap.end() ){
			eventInfo = new EVENT_INFO;
			eventInfo->ONID = eit->original_network_id;
			eventInfo->TSID = eit->transport_stream_id;
			eventInfo->SID = eit->service_id;
			eventInfo->event_id = eitEventInfo->event_id;
			eventInfo->StartTimeFlag = eitEventInfo->StartTimeFlag;
			eventInfo->start_time = eitEventInfo->start_time;
			eventInfo->DurationFlag = eitEventInfo->DurationFlag;
			eventInfo->durationSec = eitEventInfo->durationHH*60*60 +
				eitEventInfo->durationMM*60 +
				eitEventInfo->durationSS;
			eventInfo->freeCAFlag = eitEventInfo->free_CA_mode;
			serviceInfo->eventMap.insert(pair<WORD, EVENT_INFO*>(eventInfo->event_id, eventInfo));
		}else{
			eventInfo = itrEvent->second;
		}
		if( 0xA0 <= eit->table_id && eit->table_id <= 0xAF ){
			if( serviceInfo->nowEvent != NULL && serviceInfo->nextEvent != NULL ){
				if( serviceInfo->nowEvent->event_id != eitEventInfo->event_id &&
					serviceInfo->nextEvent->event_id != eitEventInfo->event_id &&
					eventInfo->pfFlag == FALSE){
					//���X�g���[����p/f����Ȃ��Ȃ玞�ԍX�V
					eventInfo->StartTimeFlag = eitEventInfo->StartTimeFlag;
					eventInfo->start_time = eitEventInfo->start_time;
					eventInfo->DurationFlag = eitEventInfo->DurationFlag;
					eventInfo->durationSec = eitEventInfo->durationHH*60*60 +
						eitEventInfo->durationMM*60 +
						eitEventInfo->durationSS;
				}
			}
		}

		//ExtendedEvent�͕�������̂�1�x�����`�F�b�N����
		BOOL checkExtFlag = FALSE;
		BOOL checkAudioFlag = FALSE;
		for( size_t j=0; j<eitEventInfo->descriptorList.size(); j++ ){
			if( eitEventInfo->descriptorList[j]->shortEvent != NULL ){
				AddShortEvent_SD( eit, eventInfo, eitEventInfo->descriptorList[j]->shortEvent );
			}else if( eitEventInfo->descriptorList[j]->extendedEvent != NULL && checkExtFlag == FALSE){
				AddExtEvent_SD(eit, eventInfo, &eitEventInfo->descriptorList );
				checkExtFlag = TRUE;
			}else if( eitEventInfo->descriptorList[j]->content != NULL ){
				AddContent_SD( eit, eventInfo, eitEventInfo->descriptorList[j]->content );
			}else if( eitEventInfo->descriptorList[j]->component != NULL ){
				AddComponent_SD( eit, eventInfo, eitEventInfo->descriptorList[j]->component );
			}else if( eitEventInfo->descriptorList[j]->audioComponent != NULL && checkAudioFlag == FALSE ){
				AddAudioComponent_SD( eit, eventInfo, &eitEventInfo->descriptorList );
				checkAudioFlag = TRUE;
			//}else if( eitEventInfo->descriptorList[j]->eventGroup != NULL ){
			//	if( eitEventInfo->descriptorList[j]->eventGroup->group_type == 0x01 ){
			//		AddEventGroup( eit, eventInfo, eitEventInfo->descriptorList[j]->eventGroup );
			//	}else if( eitEventInfo->descriptorList[j]->eventGroup->group_type == 0x02 ||
			//		eitEventInfo->descriptorList[j]->eventGroup->group_type == 0x04){
			//		AddEventRelay( eit, eventInfo, eitEventInfo->descriptorList[j]->eventGroup );
			//	}
			}
		}
	}
	
	//�Z�N�V�����X�e�[�^�X
	//map<ULONGLONG, SECTION_STATUS_INFO*>::iterator itrSec;
	//SECTION_STATUS_INFO* sectionInfo = NULL;
	//itrSec = this->sectionMap.find(key);
	//if( itrSec == this->sectionMap.end() ){
	//	sectionInfo = new SECTION_STATUS_INFO;
	//	this->sectionMap.insert(pair<ULONGLONG, SECTION_STATUS_INFO*>(key, sectionInfo));
	//}else{
	//	sectionInfo = itrSec->second;
	//}

	//if( PID == 0x0027 ){
	//	//L-EIT
	//	sectionInfo->HEITFlag = FALSE;
	//	sectionInfo->last_table_idBasic = eit->last_table_id;
	//	sectionInfo->last_section_numberBasic = eit->last_section_number;

	//	DWORD sectionNo = eit->section_number;
	//	map<WORD, SECTION_FLAG_INFO>::iterator itrFlag;
	//	itrFlag = sectionInfo->sectionBasicMap.find(eit->table_id);
	//	if( itrFlag == sectionInfo->sectionBasicMap.end() ){
	//		DWORD maxFlag = 0;
	//		for( DWORD i=0; i<=eit->last_section_number; i++ ){
	//			maxFlag |= 1<<i;
	//		}
	//		SECTION_FLAG_INFO item;
	//		item.maxFlag = maxFlag;
	//		item.sectionFlag = 1<<sectionNo;
	//		sectionInfo->sectionBasicMap.insert(pair<WORD, SECTION_FLAG_INFO>(eit->table_id, item));
	//	}else{
	//		itrFlag->second.sectionFlag |= 1<<sectionNo;
	//	}

	//}else{
	//	//H-EIT
	//	sectionInfo->HEITFlag = TRUE;
	//	if( eit->section_number == eit->segment_last_section_number ){
	//		if( 0x50 <= eit->table_id && eit->table_id <= 0x57 ||
	//			0x60 <= eit->table_id && eit->table_id <= 0x67){
	//			sectionInfo->last_table_idBasic = eit->last_table_id;
	//			sectionInfo->last_section_numberBasic = eit->last_section_number;

	//			DWORD sectionNo = eit->section_number >> 3;
	//			map<WORD, SECTION_FLAG_INFO>::iterator itrFlag;
	//			itrFlag = sectionInfo->sectionBasicMap.find(eit->table_id);
	//			if( itrFlag == sectionInfo->sectionBasicMap.end() ){
	//				DWORD maxFlag = 0;
	//				for( DWORD i=0; i<=((DWORD)eit->last_section_number)>>3; i++ ){
	//					maxFlag |= 1<<i;
	//				}
	//				SECTION_FLAG_INFO item;
	//				item.maxFlag = maxFlag;
	//				item.sectionFlag = 1<<sectionNo;
	//				sectionInfo->sectionBasicMap.insert(pair<WORD, SECTION_FLAG_INFO>(eit->table_id, item));
	//			}else{
	//				itrFlag->second.sectionFlag |= (DWORD)1<<sectionNo;
	//			}
	//		}
	//		if( 0x58 <= eit->table_id && eit->table_id <= 0x5F ||
	//			0x68 <= eit->table_id && eit->table_id <= 0x6F){
	//			sectionInfo->last_table_idExt = eit->last_table_id;
	//			sectionInfo->last_section_numberExt = eit->last_section_number;

	//			DWORD sectionNo = eit->section_number >> 3;
	//			map<WORD, SECTION_FLAG_INFO>::iterator itrFlag;
	//			itrFlag = sectionInfo->sectionExtMap.find(eit->table_id);
	//			if( itrFlag == sectionInfo->sectionExtMap.end() ){
	//				DWORD maxFlag = 0;
	//				for( DWORD i=0; i<=((DWORD)eit->last_section_number)>>3; i++ ){
	//					maxFlag |= 1<<i;
	//				}
	//				SECTION_FLAG_INFO item;
	//				item.maxFlag = maxFlag;
	//				item.sectionFlag = 1<<sectionNo;
	//				sectionInfo->sectionExtMap.insert(pair<WORD, SECTION_FLAG_INFO>(eit->table_id, item));
	//			}else{
	//				itrFlag->second.sectionFlag |= (DWORD)1<<sectionNo;
	//			}
	//		}
	//	}
	//	if( eit->table_id == 0x4E && eit->section_number == 0){
	//		//���݂̔ԑg�̂͂��Ȃ̂ŁA�����܂ł̃Z�N�V�����͂��łɕ����ς�
	//		if(eit->eventInfoList.size() > 0){
	//			if( eit->eventInfoList[0]->StartTimeFlag == TRUE ){
	//				WORD sectionNo = 0;
	//				if( eit->eventInfoList[0]->DurationFlag == FALSE ){
	//					sectionNo = eit->eventInfoList[0]->start_time.wHour / 3;
	//				}else{
	//					SYSTEMTIME endTime;
	//					int DureSec = eit->eventInfoList[0]->durationHH*60*60 + eit->eventInfoList[0]->durationMM*60 + eit->eventInfoList[0]->durationSS;
	//					GetSumTime(eit->eventInfoList[0]->start_time, DureSec, &endTime);
	//					if( eit->eventInfoList[0]->start_time.wDay != endTime.wDay ){
	//						//���t�ς���Ă�̂ō����̕��͑S���I����Ă�͂�
	//						sectionNo = 7;
	//					}else{
	//						sectionNo = endTime.wHour / 3;
	//					}
	//				}
	//				DWORD flag = 0;
	//				for( WORD i=0; i<=sectionNo; i++ ){
	//					flag |= 1<<i;
	//				}
	//				if(	this->sectionNowFlag != flag ){
	//					this->sectionNowFlag = flag;
	//				}
	//			}
	//		}
	//	}
	//}
	UnLock();

	return TRUE;
}

BOOL CEpgDBUtil::AddShortEvent(CEITTable* eit, EVENT_INFO* eventInfo, CShortEventDesc* shortEvent)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->shortInfo == NULL ){
		eventInfo->shortInfo = new SHORT_EVENT_INFO;
		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate(eit, eventInfo->shortInfo->tableID, eventInfo->shortInfo->version);
	}
	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->shortInfo->tableID = eit->table_id;
		eventInfo->shortInfo->version = eit->version_number;

		CARIB8CharDecode arib;
		string event_name = "";
		string text_char = "";
		arib.PSISI((const BYTE*)shortEvent->event_name_char, shortEvent->event_name_length, &event_name);
		arib.PSISI((const BYTE*)shortEvent->text_char, shortEvent->text_length, &text_char);

		AtoW(event_name, eventInfo->shortInfo->event_name);
		AtoW(text_char, eventInfo->shortInfo->text_char);
	}

	return updateFlag;
}

BOOL CEpgDBUtil::AddShortEvent_SD(CEITTable_SD* eit, EVENT_INFO* eventInfo, CShortEventDesc* shortEvent)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->shortInfo == NULL ){
		eventInfo->shortInfo = new SHORT_EVENT_INFO;
		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate_SD(eit, eventInfo->shortInfo->tableID, eventInfo->shortInfo->version);
	}
	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->shortInfo->tableID = eit->table_id;
		eventInfo->shortInfo->version = eit->version_number;

		CARIB8CharDecode arib;
		string event_name = "";
		string text_char = "";
		arib.PSISI((const BYTE*)shortEvent->event_name_char, shortEvent->event_name_length, &event_name);
		arib.PSISI((const BYTE*)shortEvent->text_char, shortEvent->text_length, &text_char);

		AtoW(event_name, eventInfo->shortInfo->event_name);
		AtoW(text_char, eventInfo->shortInfo->text_char);
	}

	return updateFlag;
}

BOOL CEpgDBUtil::AddExtEvent(CEITTable* eit, EVENT_INFO* eventInfo, vector<DESCRIPTOR_DATA*>* descriptorList)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->extInfo == NULL ){
		eventInfo->extInfo = new EXTENDED_EVENT_INFO;

		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate(eit, eventInfo->extInfo->tableID, eventInfo->extInfo->version);
	}

	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->extInfo->tableID = eit->table_id;
		eventInfo->extInfo->version = eit->version_number;

		CARIB8CharDecode arib;
		string extendText = "";
		string itemDescBuff = "";
		string itemBuff = "";
		//text_length��0�ŉ^�p�����
//		string textBuff = "";

		for( size_t i=0; i<descriptorList->size(); i++ ){
			if( (*descriptorList)[i]->extendedEvent != NULL ){
				CExtendedEventDesc* extEvent = (*descriptorList)[i]->extendedEvent;
				for( size_t j=0; j<extEvent->itemList.size(); j++ ){
					CExtendedEventDesc::ITEM_DATA* item = extEvent->itemList[j];
					if( item->item_description_length > 0 ){
						//if( textBuff.size() > 0 ){
						//	string buff = "";
						//	arib.PSISI((const BYTE*)textBuff.c_str(), textBuff.length(), &buff);
						//	buff += "\r\n";
						//	extendText += buff;
						//	textBuff = "";
						//}
						if( itemBuff.size() > 0 ){
							string buff = "";
							arib.PSISI((const BYTE*)itemBuff.c_str(), (DWORD)itemBuff.length(), &buff);
							buff += "\r\n";
							extendText += buff;
							itemBuff = "";
						}

						itemDescBuff += item->item_description_char;
					}
					if( item->item_length > 0 ){
						//if( textBuff.size() > 0 ){
						//	string buff = "";
						//	arib.PSISI((const BYTE*)textBuff.c_str(), textBuff.length(), &buff);
						//	buff += "\r\n";
						//	extendText += buff;
						//	textBuff = "";
						//}
						if( itemDescBuff.size() > 0 ){
							string buff = "";
							arib.PSISI((const BYTE*)itemDescBuff.c_str(), (DWORD)itemDescBuff.length(), &buff);
							buff += "\r\n";
							extendText += buff;
							itemDescBuff = "";
						}

						itemBuff += item->item_char;
					}
				}
				//if( extEvent->text_length > 0 ){
				//	if( itemDescBuff.size() > 0 ){
				//		string buff = "";
				//		arib.PSISI((const BYTE*)itemDescBuff.c_str(), itemDescBuff.length(), &buff);
				//		buff += "\r\n";
				//		extendText += buff;
				//		itemDescBuff = "";
				//	}
				//	if( itemBuff.size() > 0 ){
				//		string buff = "";
				//		arib.PSISI((const BYTE*)itemBuff.c_str(), itemBuff.length(), &buff);
				//		buff += "\r\n";
				//		extendText += buff;
				//		itemBuff = "";
				//	}

				//	textBuff += extEvent->text_char;
				//}
			}
		}

		if( itemDescBuff.size() > 0 ){
			string buff = "";
			arib.PSISI((const BYTE*)itemDescBuff.c_str(), (DWORD)itemDescBuff.length(), &buff);
			buff += "\r\n";
			extendText += buff;
			itemDescBuff = "";
		}
		if( itemBuff.size() > 0 ){
			string buff = "";
			arib.PSISI((const BYTE*)itemBuff.c_str(), (DWORD)itemBuff.length(), &buff);
			buff += "\r\n";
			extendText += buff;
			itemBuff = "";
		}
		//if( textBuff.size() > 0 ){
		//	string buff = "";
		//	arib.PSISI((const BYTE*)textBuff.c_str(), textBuff.length(), &buff);
		//	buff += "\r\n";
		//	extendText += buff;
		//	textBuff = "";
		//}

		AtoW(extendText, eventInfo->extInfo->text_char);
	}

	return updateFlag;
}

BOOL CEpgDBUtil::AddExtEvent_SD(CEITTable_SD* eit, EVENT_INFO* eventInfo, vector<DESCRIPTOR_DATA*>* descriptorList)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->extInfo == NULL ){
		eventInfo->extInfo = new EXTENDED_EVENT_INFO;

		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate_SD(eit, eventInfo->extInfo->tableID, eventInfo->extInfo->version);
	}

	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->extInfo->tableID = eit->table_id;
		eventInfo->extInfo->version = eit->version_number;

		CARIB8CharDecode arib;
		string extendText = "";
		string itemDescBuff = "";
		string itemBuff = "";
		//text_length��0�ŉ^�p�����
//		string textBuff = "";

		for( size_t i=0; i<descriptorList->size(); i++ ){
			if( (*descriptorList)[i]->extendedEvent != NULL ){
				CExtendedEventDesc* extEvent = (*descriptorList)[i]->extendedEvent;
				for( size_t j=0; j<extEvent->itemList.size(); j++ ){
					CExtendedEventDesc::ITEM_DATA* item = extEvent->itemList[j];
					if( item->item_description_length > 0 ){
						//if( textBuff.size() > 0 ){
						//	string buff = "";
						//	arib.PSISI((const BYTE*)textBuff.c_str(), textBuff.length(), &buff);
						//	buff += "\r\n";
						//	extendText += buff;
						//	textBuff = "";
						//}
						if( itemBuff.size() > 0 ){
							string buff = "";
							arib.PSISI((const BYTE*)itemBuff.c_str(), (DWORD)itemBuff.length(), &buff);
							buff += "\r\n";
							extendText += buff;
							itemBuff = "";
						}

						itemDescBuff += item->item_description_char;
					}
					if( item->item_length > 0 ){
						//if( textBuff.size() > 0 ){
						//	string buff = "";
						//	arib.PSISI((const BYTE*)textBuff.c_str(), textBuff.length(), &buff);
						//	buff += "\r\n";
						//	extendText += buff;
						//	textBuff = "";
						//}
						if( itemDescBuff.size() > 0 ){
							string buff = "";
							arib.PSISI((const BYTE*)itemDescBuff.c_str(), (DWORD)itemDescBuff.length(), &buff);
							buff += "\r\n";
							extendText += buff;
							itemDescBuff = "";
						}

						itemBuff += item->item_char;
					}
				}
				//if( extEvent->text_length > 0 ){
				//	if( itemDescBuff.size() > 0 ){
				//		string buff = "";
				//		arib.PSISI((const BYTE*)itemDescBuff.c_str(), itemDescBuff.length(), &buff);
				//		buff += "\r\n";
				//		extendText += buff;
				//		itemDescBuff = "";
				//	}
				//	if( itemBuff.size() > 0 ){
				//		string buff = "";
				//		arib.PSISI((const BYTE*)itemBuff.c_str(), itemBuff.length(), &buff);
				//		buff += "\r\n";
				//		extendText += buff;
				//		itemBuff = "";
				//	}

				//	textBuff += extEvent->text_char;
				//}
			}
		}

		if( itemDescBuff.size() > 0 ){
			string buff = "";
			arib.PSISI((const BYTE*)itemDescBuff.c_str(), (DWORD)itemDescBuff.length(), &buff);
			buff += "\r\n";
			extendText += buff;
			itemDescBuff = "";
		}
		if( itemBuff.size() > 0 ){
			string buff = "";
			arib.PSISI((const BYTE*)itemBuff.c_str(), (DWORD)itemBuff.length(), &buff);
			buff += "\r\n";
			extendText += buff;
			itemBuff = "";
		}
		//if( textBuff.size() > 0 ){
		//	string buff = "";
		//	arib.PSISI((const BYTE*)textBuff.c_str(), textBuff.length(), &buff);
		//	buff += "\r\n";
		//	extendText += buff;
		//	textBuff = "";
		//}

		AtoW(extendText, eventInfo->extInfo->text_char);
	}

	return updateFlag;
}

BOOL CEpgDBUtil::AddContent(CEITTable* eit, EVENT_INFO* eventInfo, CContentDesc* content)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->contentInfo == NULL ){
		eventInfo->contentInfo = new CONTEN_INFO;
		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate(eit, eventInfo->contentInfo->tableID, eventInfo->contentInfo->version);
	}
	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->contentInfo->tableID = eit->table_id;
		eventInfo->contentInfo->version = eit->version_number;

		eventInfo->contentInfo->nibbleList = content->nibbleList;
	}

	return updateFlag;
}

BOOL CEpgDBUtil::AddContent_SD(CEITTable_SD* eit, EVENT_INFO* eventInfo, CContentDesc* content)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->contentInfo == NULL ){
		eventInfo->contentInfo = new CONTEN_INFO;
		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate_SD(eit, eventInfo->contentInfo->tableID, eventInfo->contentInfo->version);
	}
	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->contentInfo->tableID = eit->table_id;
		eventInfo->contentInfo->version = eit->version_number;

		eventInfo->contentInfo->nibbleList = content->nibbleList;
	}

	return updateFlag;
}

BOOL CEpgDBUtil::AddComponent(CEITTable* eit, EVENT_INFO* eventInfo, CComponentDesc* component)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->componentInfo == NULL ){
		eventInfo->componentInfo = new COMPONENT_INFO;
		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate(eit, eventInfo->componentInfo->tableID, eventInfo->componentInfo->version);
	}
	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->componentInfo->tableID = eit->table_id;
		eventInfo->componentInfo->version = eit->version_number;

		eventInfo->componentInfo->stream_content = component->stream_content;
		eventInfo->componentInfo->component_type = component->component_type;
		eventInfo->componentInfo->component_tag = component->component_tag;

		CARIB8CharDecode arib;
		string text_char = "";
		if( component->text_charLength > 0 ){
			arib.PSISI((const BYTE*)component->text_char, component->text_charLength, &text_char);

			AtoW(text_char, eventInfo->componentInfo->text_char);
		}

	}

	return updateFlag;
}

BOOL CEpgDBUtil::AddComponent_SD(CEITTable_SD* eit, EVENT_INFO* eventInfo, CComponentDesc* component)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->componentInfo == NULL ){
		eventInfo->componentInfo = new COMPONENT_INFO;
		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate_SD(eit, eventInfo->componentInfo->tableID, eventInfo->componentInfo->version);
	}
	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->componentInfo->tableID = eit->table_id;
		eventInfo->componentInfo->version = eit->version_number;

		eventInfo->componentInfo->stream_content = component->stream_content;
		eventInfo->componentInfo->component_type = component->component_type;
		eventInfo->componentInfo->component_tag = component->component_tag;

		CARIB8CharDecode arib;
		string text_char = "";
		if( component->text_charLength > 0 ){
			arib.PSISI((const BYTE*)component->text_char, component->text_charLength, &text_char);

			AtoW(text_char, eventInfo->componentInfo->text_char);
		}

	}

	return updateFlag;
}

BOOL CEpgDBUtil::AddAudioComponent(CEITTable* eit, EVENT_INFO* eventInfo, vector<DESCRIPTOR_DATA*>* descriptorList)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->audioInfo == NULL ){
		eventInfo->audioInfo = new AUDIO_COMPONENT_INFO;
		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate(eit, eventInfo->audioInfo->tableID, eventInfo->audioInfo->version);
	}
	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->audioInfo->tableID = eit->table_id;
		eventInfo->audioInfo->version = eit->version_number;
		eventInfo->audioInfo->componentList.clear();

		for( size_t i=0; i<descriptorList->size(); i++ ){
			if( (*descriptorList)[i]->audioComponent != NULL ){
				CAudioComponentDesc* audioComponent = (*descriptorList)[i]->audioComponent;
				AUDIO_COMPONENT_INFO_DATA item;

				item.stream_content = audioComponent->stream_content;
				item.component_type = audioComponent->component_type;
				item.component_tag = audioComponent->component_tag;

				item.stream_type = audioComponent->stream_type;
				item.simulcast_group_tag = audioComponent->simulcast_group_tag;
				item.ES_multi_lingual_flag = audioComponent->ES_multi_lingual_flag;
				item.main_component_flag = audioComponent->main_component_flag;
				item.quality_indicator = audioComponent->quality_indicator;
				item.sampling_rate = audioComponent->sampling_rate;


				CARIB8CharDecode arib;
				string text_char = "";
				if( audioComponent->text_charLength > 0 ){
					arib.PSISI((const BYTE*)audioComponent->text_char, audioComponent->text_charLength, &text_char);

					AtoW(text_char, item.text_char);
				}

				eventInfo->audioInfo->componentList.push_back(item);

			}
		}
	}

	return updateFlag;
}

BOOL CEpgDBUtil::AddAudioComponent_SD(CEITTable_SD* eit, EVENT_INFO* eventInfo, vector<DESCRIPTOR_DATA*>* descriptorList)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->audioInfo == NULL ){
		eventInfo->audioInfo = new AUDIO_COMPONENT_INFO;
		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate_SD(eit, eventInfo->audioInfo->tableID, eventInfo->audioInfo->version);
	}
	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->audioInfo->tableID = eit->table_id;
		eventInfo->audioInfo->version = eit->version_number;
		eventInfo->audioInfo->componentList.clear();

		for( size_t i=0; i<descriptorList->size(); i++ ){
			if( (*descriptorList)[i]->audioComponent != NULL ){
				CAudioComponentDesc* audioComponent = (*descriptorList)[i]->audioComponent;
				AUDIO_COMPONENT_INFO_DATA item;

				item.stream_content = audioComponent->stream_content;
				item.component_type = audioComponent->component_type;
				item.component_tag = audioComponent->component_tag;

				item.stream_type = audioComponent->stream_type;
				item.simulcast_group_tag = audioComponent->simulcast_group_tag;
				item.ES_multi_lingual_flag = audioComponent->ES_multi_lingual_flag;
				item.main_component_flag = audioComponent->main_component_flag;
				item.quality_indicator = audioComponent->quality_indicator;
				item.sampling_rate = audioComponent->sampling_rate;


				CARIB8CharDecode arib;
				string text_char = "";
				if( audioComponent->text_charLength > 0 ){
					arib.PSISI((const BYTE*)audioComponent->text_char, audioComponent->text_charLength, &text_char);

					AtoW(text_char, item.text_char);
				}

				eventInfo->audioInfo->componentList.push_back(item);

			}
		}
	}

	return updateFlag;
}

BOOL CEpgDBUtil::AddEventGroup(CEITTable* eit, EVENT_INFO* eventInfo, CEventGroupDesc* eventGroup)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->eventGroupInfo == NULL ){
		eventInfo->eventGroupInfo = new EVENTGROUP_INFO;
		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate(eit, eventInfo->eventGroupInfo->tableID, eventInfo->eventGroupInfo->version);
	}
	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->eventGroupInfo->tableID = eit->table_id;
		eventInfo->eventGroupInfo->version = eit->version_number;
		eventInfo->eventGroupInfo->eventData2List.clear();

		eventInfo->eventGroupInfo->group_type = eventGroup->group_type;
		eventInfo->eventGroupInfo->event_count = eventGroup->event_count;
		for( size_t i=0; i<eventGroup->eventDataList.size(); i++ ){
			CEventGroupDesc::EVENT_DATA2 item;
			item.event_id = eventGroup->eventDataList[i].event_id;
			item.service_id = eventGroup->eventDataList[i].service_id;
			item.original_network_id = eit->original_network_id;
			item.transport_stream_id = eit->transport_stream_id;

			eventInfo->eventGroupInfo->eventData2List.push_back(item);
		}
	}

	return updateFlag;
}

BOOL CEpgDBUtil::AddEventRelay(CEITTable* eit, EVENT_INFO* eventInfo, CEventGroupDesc* eventGroup)
{
	BOOL updateFlag = FALSE;
	if( eventInfo->eventRelayInfo == NULL ){
		eventInfo->eventRelayInfo = new EVENTGROUP_INFO;
		updateFlag = TRUE;
	}else{
		updateFlag = CheckUpdate(eit, eventInfo->eventRelayInfo->tableID, eventInfo->eventRelayInfo->version);
	}
	if( updateFlag == TRUE ){
		//�X�V�K�v
		eventInfo->eventRelayInfo->tableID = eit->table_id;
		eventInfo->eventRelayInfo->version = eit->version_number;
		eventInfo->eventRelayInfo->eventData2List.clear();

		eventInfo->eventRelayInfo->group_type = eventGroup->group_type;
		eventInfo->eventRelayInfo->event_count = eventGroup->event_count;
		if( eventGroup->group_type == 0x02 ){
			for( size_t i=0; i<eventGroup->eventDataList.size(); i++ ){
				CEventGroupDesc::EVENT_DATA2 item;
				item.event_id = eventGroup->eventDataList[i].event_id;
				item.service_id = eventGroup->eventDataList[i].service_id;
				item.original_network_id = eit->original_network_id;
				item.transport_stream_id = eit->transport_stream_id;

				eventInfo->eventRelayInfo->eventData2List.push_back(item);
			}
		}else{
			eventInfo->eventRelayInfo->eventData2List = eventGroup->eventData2List;
		}

	}

	return updateFlag;
}

BOOL CEpgDBUtil::CheckUpdate(CEITTable* eit, BYTE tableID, BYTE version)
{
	BOOL changeFlag = FALSE;
	if( eit->table_id == 0x4E ){
		if( tableID == 0x4E ){
			if( version != eit->version_number ){
				//�o�[�W�����ς�����̂ōX�V
				changeFlag = TRUE;
			}else{
				//�o�[�W�����ς���Ă��Ȃ��̂ōX�V�̕K�v�Ȃ�
			}
		}else{
			//[p/f]���ŐV�̂͂��Ȃ̂ōX�V
			changeFlag = TRUE;
		}
	}else if( eit->table_id == 0x4F ){
		if( tableID == 0x4F ){
			if( version != eit->version_number ){
				//�o�[�W�����ς�����̂ōX�V
				changeFlag = TRUE;
			}else{
				//�o�[�W�����ς���Ă��Ȃ��̂ōX�V�̕K�v�Ȃ�
			}
		}else if( 0x60 <= tableID && tableID <= 0x6F ){
			//[p/f]�̕����V�����͂��Ȃ̂ōX�V
			changeFlag = TRUE;
		}else{
			//���X�g���[�����Ȃ̂ōX�V���Ȃ�
		}
	}else if( 0x50 <= eit->table_id && eit->table_id <= 0x5F ){
		if( 0x50 <= tableID && tableID <= 0x5F ){
			if( tableID == eit->table_id ){
				if( version != eit->version_number ){
					//�o�[�W�����ς�����̂ōX�V
					changeFlag = TRUE;
				}else{
					//�o�[�W�����ς���Ă��Ȃ��̂ōX�V�̕K�v�Ȃ�
				}
			}else{
				//�e�[�u�����ς�����̂ōX�V
				changeFlag = TRUE;
			}
		}else if( 0x60 <= tableID && tableID <= 0x6F ||
			tableID == 0x4F ){
				//���X�g���[�����Ȃ̂ōX�V���Ă���
				changeFlag = TRUE;
		}else{
			//[p/f]���ŐV�̂͂��Ȃ̂ōX�V���Ȃ�
		}
	}else if( 0x60 <= eit->table_id && eit->table_id <= 0x6F ){
		if( 0x60 <= tableID && tableID <= 0x6F ){
			if( tableID == eit->table_id ){
				if( version != eit->version_number ){
					//�o�[�W�����ς�����̂ōX�V
					changeFlag = TRUE;
				}else{
					//�o�[�W�����ς���Ă��Ȃ��̂ōX�V�̕K�v�Ȃ�
				}
			}else{
				//�e�[�u�����ς�����̂ōX�V
				changeFlag = TRUE;
			}
		}else{
			//[p/f]�����X�g���[���Ȃ̂ōX�V�̕K�v�Ȃ�
		}
	}
	return changeFlag;
}

BOOL CEpgDBUtil::CheckUpdate_SD(CEITTable_SD* eit, BYTE tableID, BYTE version)
{
	BOOL changeFlag = FALSE;
	if( 0xA0 <= eit->table_id && eit->table_id <= 0xAF ){
		if( 0xA0 <= tableID && tableID <= 0xAF ){
			if( tableID == eit->table_id ){
				if( version != eit->version_number ){
					//�o�[�W�����ς�����̂ōX�V
					changeFlag = TRUE;
				}else{
					//�o�[�W�����ς���Ă��Ȃ��̂ōX�V�̕K�v�Ȃ�
				}
			}else{
				//�e�[�u�����ς�����̂ōX�V
				changeFlag = TRUE;
			}
		}else{
			//[p/f]���ŐV�̂͂��Ȃ̂ōX�V���Ȃ�
		}
	}
	return changeFlag;
}

void CEpgDBUtil::ClearSectionStatus()
{
	if( Lock() == FALSE ) return ;

	map<ULONGLONG, SECTION_STATUS_INFO*>::iterator itr;
	for( itr = this->sectionMap.begin(); itr != this->sectionMap.end(); itr++ ){
		itr->second->sectionPF.Clear();
		itr->second->sectionBasic.Clear();
		itr->second->sectionExt.Clear();
	}

	UnLock();
}

//EPG�擾���e�̐ݒ�
//�����F
// settings		[IN]�ݒ�l
void CEpgDBUtil::SetEpgGetSettings( WORD settings )
{
	epgGetSettings = settings;
}

BOOL CheckSectionAll(const SECTION_FLAG_INFO * section, WORD now, WORD count, WORD * getCount, WORD * totalCount)
{
	BOOL result = TRUE;
	WORD last;
	if( count != 0 ){
		last = now + count - 1;
	}else{
		last = section->last_section_number;
		if( last >= 2048 ){
			last = 2047;
		}
	}
	for( WORD i=now; i<=last; i++ ){
		if( totalCount != NULL ){
			totalCount[i/64]++;
		}
		if( (section->sectionFlag[i/64] & (1ULL<<(i%64))) == 0 ){
			result = FALSE;
		}else{
			if( getCount != NULL ){
				getCount[i/64]++;
			}
		}
	}
	return result;
}

void _DebugSectinInfo(WORD startPos, WORD endPos, WORD * getCount, WORD * totalCount)
{
#ifdef _DEBUG
	for( WORD i=startPos; i<=endPos; i++ ){
		_OutputDebugString(L"    %02d:get[%d] / total[%d]\r\n", i, getCount[i], totalCount[i]);
	}
#endif
}

//PerfecTV�T�[�r�X��EIT(�X�P�W���[��)
//�e�[�u���ԍ���3bit���Œ�œ��t�ɑΉ�����
//    0: 1���`4��
//    1: 5���`8��
//    2: 9���`12��
//    3: 13���`16��
//    4: 17���`20��
//    5: 21���`24��
//    6: 25���`28��
//    7: 26���`31��
//��{����7�����A�g������3�����񋟂���A���̒ǉ���3���ԒP�ʂōs����c�悤�ȋC������
BOOL CheckSectionDay(const SECTION_FLAG_INFO * section, SYSTEMTIME time, WORD count, WORD * getCount, WORD * totalCount)
{
	BOOL result = TRUE;
	for( int i=0; i<count/8; i++ ){
		WORD pos = (time.wHour / 3) * 8;
		for( int j=pos; j<pos+8; j++ ){
			if( totalCount != NULL ){
				totalCount[time.wDay-1]++;
			}
			if( (section->sectionFlag[time.wDay-1] & (1ULL<<j)) == 0 ){
				result = FALSE;
			}else{
				if( getCount != NULL ){
					getCount[time.wDay-1]++;
				}
			}
		}
		GetSumTime(time, 3*60*60, &time);
	}
	return result;
}

void _DebugSectinInfoDay(SYSTEMTIME time, WORD days, WORD * getCount, WORD * totalCount)
{
#ifdef _DEBUG
	for( WORD i=0; i<days; i++ ){
		_OutputDebugString(L"    %02d:get[%d] / total[%d]\r\n", time.wDay-1, getCount[time.wDay-1], totalCount[time.wDay-1]);
		GetSumTime(time, 24*60*60, &time);
	}
#endif
}

EPG_SECTION_STATUS CEpgDBUtil::GetSectionStatus(BOOL l_eitFlag)
{
	if( Lock() == FALSE ) return EpgNoData;

	EPG_SECTION_STATUS status = EpgNoData;
	if( this->sectionMap.size() == 0 ){
		UnLock();
		return status;
	}

	BOOL basicFlag = TRUE;
	BOOL extFlag = TRUE;
	BOOL pfFlag = TRUE;
	BOOL otherBasicFlag = TRUE;
	BOOL otherExtFlag = TRUE;
	BOOL otherPfFlag = TRUE;

	WORD now = (tdtTime.wHour / 3) * 8;

	WORD basicTotalCount[32] = {0,};
	WORD basicGetCount[32] = {0,};
	WORD extTotalCount[32] = {0,};
	WORD extGetCount[32] = {0,};
	WORD pfTotalCount[32] = {0,};
	WORD pfGetCount[32] = {0,};
	WORD otherBasicTotalCount[32] = {0,};
	WORD otherBasicGetCount[32] = {0,};
	WORD otherExtTotalCount[32] = {0,};
	WORD otherExtGetCount[32] = {0,};
	WORD otherPfTotalCount[32] = {0,};
	WORD otherPfGetCount[32] = {0,};

	BOOL serviceAudio = !!(epgGetSettings & EpgGetServiceAudio);
	BOOL serviceData = !!(epgGetSettings & EpgGetServiceData);
	BOOL serviceOther = !!(epgGetSettings & EpgGetServiceOther);

	BOOL serviceTemporary = !!(epgGetSettings & EpgGetServiceTemporary);

	BOOL lengthShort = !!(epgGetSettings & EpgGetLengthShort);

	BOOL otherInfoExt = !!(epgGetSettings & EpgGetOtherInfoExt);
	BOOL otherInfoBasic = !!(epgGetSettings & EpgGetOtherInfoBasic) || otherInfoExt;
	BOOL otherInfoPf = !!(epgGetSettings & EpgGetOtherInfoPf) || otherInfoBasic;

	BOOL infoExt = !!(epgGetSettings & EpgGetInfoExt) || otherInfoExt;
	BOOL infoBasic = !!(epgGetSettings & EpgGetInfoBasic) || otherInfoBasic || infoExt;
	BOOL infoPf = !!(epgGetSettings & EpgGetInfoPf) || otherInfoPf || infoBasic;

	WORD epgSections = 0;
	if( lengthShort == TRUE ){
		epgSections = 128;
	}

	map<ULONGLONG, SECTION_STATUS_INFO*>::iterator itr;
	for( itr = this->sectionMap.begin(); itr != this->sectionMap.end(); itr++ ){

		//�T�[�r�X���X�g����Ȃ�w��T�[�r�X�̂ݑΏ�
		map<ULONGLONG, BYTE>::iterator itrType;
		itrType = this->serviceList.find(itr->first);
		if( itrType != this->serviceList.end() ){
			if( itrType->second != 0x01											//�f��
					&& (itrType->second != 0x02 || serviceAudio == FALSE)		//����
					&& (itrType->second != 0x80 || serviceOther == FALSE)		//
					&& itrType->second != 0x81									//�v�����f��
					&& (itrType->second != 0xA1 || serviceTemporary == FALSE)	//�Վ��f��
					&& (itrType->second != 0xA2 || serviceTemporary == FALSE || serviceAudio == FALSE)	//�Վ�����
					&& (itrType->second != 0xA3 || serviceTemporary == FALSE || serviceData == FALSE)	//�Վ��f�[�^
					&& (itrType->second != 0xA4	|| serviceOther == FALSE)		//ES
					&& itrType->second != 0xA5									//�v�����f��
					&& (itrType->second != 0xA6 || serviceAudio == FALSE)		//�v��������
					&& (itrType->second != 0xA7	|| serviceData == FALSE)		//�v�����f�[�^
					&& (itrType->second != 0xC0 || serviceData == FALSE)		//�f�[�^
					){
				continue;
			}
		}

		//p/f
		if( itr->second->otherStreamFlag == 0 ){
			//���X�g���[��
			if( infoPf == TRUE ){
				//p/f EIT_present_following_flag�ɏ]��
				if( itr->second->EIT_present_following_flag == 1 ){
					if( CheckSectionAll(&itr->second->sectionPF, 0, 0, pfGetCount, pfTotalCount) == FALSE ){
						pfFlag = FALSE;
					}
				}
			}
		}else{
			//���X�g���[��
			if( otherInfoPf == TRUE ){
				//p/f EIT_present_following_flag�ɏ]��
				if( itr->second->EIT_present_following_flag == 1 ){
					if( CheckSectionAll(&itr->second->sectionPF, 0, 0, otherPfGetCount, otherPfTotalCount) == FALSE ){
						otherPfFlag = FALSE;
					}
				}
			}
		}

		if( this->ONID == 1 ){
			//PerfecTV�T�[�r�X �S�Ď��X�g���[����{���^�g�����Ƃ��Ĉ���
			if( itr->second->EIT_schedule_flag == 1 ){
				if( infoBasic == TRUE ){
					//��{���
					if( this->TSID == 0x0017 ){
						//�v����ch
						if( CheckSectionDay(&itr->second->sectionBasic, tdtTime, (7*64)+8, basicGetCount, basicTotalCount) == FALSE ){
							basicFlag = FALSE;
						}
					}else{
						//�v����CH�X�g���[���ȊO�ł͊�{���6H���̂�
						if( CheckSectionDay(&itr->second->sectionBasic, tdtTime, 16+8, basicGetCount, basicTotalCount) == FALSE ){
							basicFlag = FALSE;
						}
					}
				}

				if( infoExt == TRUE ){
					//�g�����
					if( this->TSID == 0x0017 ){
						//�v����ch
						if( CheckSectionDay(&itr->second->sectionExt, tdtTime, (3*64)+8, extGetCount, extTotalCount) == FALSE ){
							extFlag = FALSE;
						}
					}else{
						//�v����ch�̃X�g���[���ȊO�ł͊g�����Ȃ�
					}
				}
			}
		}else if( this->ONID == 3 ){
			//SKY�T�[�r�X �S�Ď��X�g���[����{���Ƃ��Ĉ���
			if( itr->second->EIT_present_following_flag == 1 ){
				if( CheckSectionAll(&itr->second->sectionBasic, 0, 0, basicGetCount, basicTotalCount) == FALSE ){
					basicFlag = extFlag = otherBasicFlag = otherExtFlag = FALSE;
				}
			}
		}else{
			//�n�f�W�EBS�ECS�ESPHD ���X�g���[����{���^�g�����A���X�g���[����{���^�g�������ʂɈ���
			if( itr->second->EIT_schedule_flag == 1 ){
				if( itr->second->otherStreamFlag == 0 ){
					//���X�g���[��
					if( infoBasic == TRUE ){
						//��{���
						if( CheckSectionAll(&itr->second->sectionBasic, now, epgSections, basicGetCount, basicTotalCount) == FALSE ){
							basicFlag = FALSE;
						}
					}

					if( infoExt == TRUE ){
						//�g�����
						if( itr->second->sectionExt.last_section_number != (WORD)~0 ){
							if( CheckSectionAll(&itr->second->sectionExt, now, epgSections, extGetCount, extTotalCount) == FALSE ){
								extFlag = FALSE;
							}
						}
					}
				}else{
					//���X�g���[��
					if( otherInfoBasic == TRUE ){
						//��{���
						if( CheckSectionAll(&itr->second->sectionBasic, now, epgSections, otherBasicGetCount, otherBasicTotalCount) == FALSE ){
							otherBasicFlag = FALSE;
						}
					}

					if( otherInfoExt == TRUE ){
						//�g�����
						if( itr->second->sectionExt.last_section_number != (WORD)~0 ){
							if( CheckSectionAll(&itr->second->sectionExt, now, epgSections, otherExtGetCount, otherExtTotalCount) == FALSE ){
								otherExtFlag = FALSE;
							}
						}
					}
				}
			}
		}

#ifndef _DEBUG
		if ( pfFlag == FALSE && basicFlag == FALSE && extFlag == FALSE && otherPfFlag == FALSE && otherBasicFlag == FALSE && otherExtFlag == FALSE ){
			break;
		}
	}
#endif

#ifdef _DEBUG
	_OutputDebugString(L"Audio:%d Data:%d Other:%d Temporary:%d\r\n", serviceAudio, serviceData, serviceOther, serviceTemporary);
	_OutputDebugString(L"Short:%d\r\n", lengthShort);
	_OutputDebugString(L"p/f:%d basic:%d ext:%d\r\n", infoPf, infoBasic, infoExt);
	_OutputDebugString(L"o-p/f:%d o-basic:%d o-ext:%d\r\n", otherInfoPf, otherInfoBasic, otherInfoExt);

	_OutputDebugString(L"p/f\r\n");
	_DebugSectinInfo(0, 7, pfGetCount, pfTotalCount);
	if( this->ONID == 1 ){
		_OutputDebugString(L"basic\r\n");
		_DebugSectinInfoDay(tdtTime, 8, basicGetCount, basicTotalCount);
		_OutputDebugString(L"ext\r\n");
		_DebugSectinInfoDay(tdtTime, 8, extGetCount, extTotalCount);
	}else if( this->ONID == 3 ){
		_OutputDebugString(L"basic & ext\r\n");
		_DebugSectinInfo(0, 8, basicGetCount, basicTotalCount);
	}else{
		_OutputDebugString(L"basic\r\n");
		_DebugSectinInfo(0, 7, basicGetCount, basicTotalCount);
		_OutputDebugString(L"ext\r\n");
		_DebugSectinInfo(0, 7, extGetCount, extTotalCount);
	}
	_OutputDebugString(L"o-p/f\r\n");
	_DebugSectinInfo(0, 7, otherPfGetCount, otherPfTotalCount);
	if( this->ONID == 1 ){
		_OutputDebugString(L"o-basic\r\n");
		_DebugSectinInfoDay(tdtTime, 8, otherBasicGetCount, otherBasicTotalCount);
		_OutputDebugString(L"o-ext\r\n");
		_DebugSectinInfoDay(tdtTime, 8, otherExtGetCount, otherExtTotalCount);
	}else if( this->ONID == 3 ){
		_OutputDebugString(L"o-basic & o-ext\r\n");
		_DebugSectinInfo(0, 8, otherBasicGetCount, otherBasicTotalCount);
	}else{
		_OutputDebugString(L"o-basic\r\n");
		_DebugSectinInfo(0, 7, otherBasicGetCount, otherBasicTotalCount);
		_OutputDebugString(L"o-ext\r\n");
		_DebugSectinInfo(0, 7, otherExtGetCount, otherExtTotalCount);
	}
#endif

	if( l_eitFlag == TRUE ){
		if( pfFlag == TRUE ){
			OutputDebugString(L"EpgLEITAll\r\n");
			status = EpgLEITAll;
		}else{
			OutputDebugString(L"EpgNeedData\r\n");
			status = EpgNeedData;
		}
	}else{
		if( pfFlag == TRUE && basicFlag == TRUE && extFlag == TRUE 
				&& otherPfFlag == TRUE && otherBasicFlag == TRUE && otherExtFlag == TRUE ){
			OutputDebugString(L"EpgHEITAll\r\n");
			status = EpgHEITAll;
		}else if( basicFlag == TRUE && otherBasicFlag == TRUE ){
			OutputDebugString(L"EpgBasicAll\r\n");
			status = EpgBasicAll;
		//}else if( extFlag == TRUE && otherExtFlag == TRUE ){
//			OutputDebugString(L"EpgExtendAll\r\n");
		//	status = EpgExtendAll;
		}else{
			OutputDebugString(L"EpgNeedData\r\n");
			status = EpgNeedData;
		}
	}
	UnLock();
	return status;
}

BOOL CEpgDBUtil::AddServiceList(CNITTable* nit)
{
	if( nit == NULL ){
		return FALSE;
	}
	if( Lock() == FALSE ) return FALSE;

	wstring network_nameW = L"";

	for( size_t i=0; i<nit->descriptorList.size(); i++ ){
		if( nit->descriptorList[i]->networkName != NULL ){
			CNetworkNameDesc* networkName = nit->descriptorList[i]->networkName;
			if( networkName->char_nameLength > 0 ){
				CARIB8CharDecode arib;
				string network_name = "";
				arib.PSISI((const BYTE*)networkName->char_name, networkName->char_nameLength, &network_name);
				AtoW(network_name, network_nameW);
			}
		}
	}

	for( size_t i=0; i<nit->TSInfoList.size(); i++ ){
		CNITTable::TS_INFO_DATA* tsInfo = nit->TSInfoList[i];
		//�T�[�r�X���X�V�p
		DWORD key = ((DWORD)tsInfo->original_network_id) <<16 | tsInfo->transport_stream_id;
		map<DWORD, DB_TS_INFO*>::iterator itrFind;
		itrFind = this->serviceInfoList.find(key);
		DB_TS_INFO* info;
		if( itrFind == this->serviceInfoList.end() ){
			info = new DB_TS_INFO;
			info->original_network_id = tsInfo->original_network_id;
			info->transport_stream_id = tsInfo->transport_stream_id;
			this->serviceInfoList.insert(pair<DWORD, DB_TS_INFO*>(key, info));
		}else{
			info = itrFind->second;
		}
		info->network_name = network_nameW;

		for( size_t j=0; j<tsInfo->descriptorList.size(); j++ ){
			DESCRIPTOR_DATA* desc = tsInfo->descriptorList[j];
			if( desc->serviceList != NULL ){
				for( size_t k=0; k<desc->serviceList->serviceList.size(); k++ ){
					CServiceListDesc::SERVICE_LIST_DATA serviceData = desc->serviceList->serviceList[k];
					ULONGLONG key = _Create64Key(tsInfo->original_network_id, tsInfo->transport_stream_id, serviceData.service_id);
					map<ULONGLONG, BYTE>::iterator itrService;
					itrService = this->serviceList.find(key);
					if( itrService == this->serviceList.end() ){
						this->serviceList.insert(pair<ULONGLONG, BYTE>(key, serviceData.service_type));
					}
				}
			}
			if( desc->TSInfo != NULL ){
				//ts_name��remote_control_key_id
				if( desc->TSInfo->length_of_ts_name > 0 ){
					CARIB8CharDecode arib;
					string ts_name = "";
					arib.PSISI((const BYTE*)desc->TSInfo->ts_name_char, desc->TSInfo->length_of_ts_name, &ts_name);
					AtoW(ts_name, info->ts_name);
				}
				info->remote_control_key_id = desc->TSInfo->remote_control_key_id;
			}
			if( desc->partialReception != NULL ){
				//������M�t���O
				map<WORD,DB_SERVICE_INFO*>::iterator itrService;
				for( size_t k=0; k<desc->partialReception->service_idList.size(); k++ ){
					itrService = info->serviceList.find(desc->partialReception->service_idList[k]);

					DB_SERVICE_INFO* item;
					if( itrService == info->serviceList.end() ){
						item = new DB_SERVICE_INFO;
						item->original_network_id = tsInfo->original_network_id;
						item->transport_stream_id = tsInfo->transport_stream_id;
						item->service_id = desc->partialReception->service_idList[k];
						info->serviceList.insert(pair<WORD,DB_SERVICE_INFO*>(item->service_id, item));
					}else{
						item = itrService->second;
					}
					item->partialReceptionFlag = 1;
				}
			}
		}
	}

	UnLock();
	return TRUE;
}

BOOL CEpgDBUtil::AddServiceList(WORD TSID, CSITTable* sit)
{
	if( Lock() == FALSE ) return FALSE;

	WORD ONID = 0xFFFF;
	for( size_t i=0; i<sit->descriptorList.size(); i++ ){
		if( sit->descriptorList[i]->networkIdentification != NULL ){
			ONID = sit->descriptorList[i]->networkIdentification->network_id;
		}
	}
	if(ONID == 0xFFFF){
		UnLock();
		return FALSE;
	}

	DWORD key = ((DWORD)ONID)<<16 | TSID;
	map<DWORD, DB_TS_INFO*>::iterator itrTS;
	itrTS = this->serviceInfoList.find(key);
	DB_TS_INFO* info;
	if( itrTS == this->serviceInfoList.end() ){
		info = new DB_TS_INFO;
		info->original_network_id = ONID;
		info->transport_stream_id = TSID;
		this->serviceInfoList.insert(pair<DWORD, DB_TS_INFO*>(key, info));
	}else{
		info = itrTS->second;
	}

	for(size_t i=0; i<sit->serviceLoopList.size(); i++ ){
		DB_SERVICE_INFO* item = new DB_SERVICE_INFO;
		item->original_network_id = ONID;
		item->transport_stream_id = TSID;
		item->service_id = sit->serviceLoopList[i]->service_id;

		for( size_t j=0; j<sit->serviceLoopList[i]->descriptorList.size(); j++ ){
			if( sit->serviceLoopList[i]->descriptorList[j]->service != NULL ){
				CServiceDesc* service = sit->serviceLoopList[i]->descriptorList[j]->service;
				CARIB8CharDecode arib;
				string service_provider_name = "";
				string service_name = "";
				if( service->service_provider_name_length > 0 ){
					arib.PSISI((const BYTE*)service->char_service_provider_name, service->service_provider_name_length, &service_provider_name);
				}
				if( service->service_name_length > 0 ){
					arib.PSISI((const BYTE*)service->char_service_name, service->service_name_length, &service_name);
				}
				AtoW(service_provider_name, item->service_provider_name);
				AtoW(service_name, item->service_name);

				item->service_type = service->service_type;
			}
		}
		if( info->serviceList[item->service_id] != NULL ){
			SAFE_DELETE(info->serviceList[item->service_id]);
		}
		info->serviceList[item->service_id] = item;
	}

	UnLock();
	return TRUE;
}

BOOL CEpgDBUtil::AddSDT(CSDTTable* sdt)
{
	if( Lock() == FALSE ) return FALSE;

	DWORD key = ((DWORD)sdt->original_network_id)<<16 | sdt->transport_stream_id;
	map<DWORD, DB_TS_INFO*>::iterator itrTS;
	itrTS = this->serviceInfoList.find(key);
	DB_TS_INFO* info;
	if( itrTS == this->serviceInfoList.end() ){
		info = new DB_TS_INFO;
		info->original_network_id = sdt->original_network_id;
		info->transport_stream_id = sdt->transport_stream_id;
		this->serviceInfoList.insert(pair<DWORD, DB_TS_INFO*>(key, info));
	}else{
		info = itrTS->second;
	}

	for(size_t i=0; i<sdt->serviceInfoList.size(); i++ ){
		map<WORD,DB_SERVICE_INFO*>::iterator itrS;
		itrS = info->serviceList.find(sdt->serviceInfoList[i]->service_id);
		DB_SERVICE_INFO* item;
		if( itrS == info->serviceList.end()){
			item = new DB_SERVICE_INFO;
			item->original_network_id = sdt->original_network_id;
			item->transport_stream_id = sdt->transport_stream_id;
			item->service_id = sdt->serviceInfoList[i]->service_id;
			info->serviceList.insert(pair<WORD,DB_SERVICE_INFO*>(item->service_id, item));
		}else{
			item = itrS->second;
		}

		for( size_t j=0; j<sdt->serviceInfoList[i]->descriptorList.size(); j++ ){
			if( sdt->serviceInfoList[i]->descriptorList[j]->service != NULL ){
				CServiceDesc* service = sdt->serviceInfoList[i]->descriptorList[j]->service;
				CARIB8CharDecode arib;
				string service_provider_name = "";
				string service_name = "";
				if( service->service_provider_name_length > 0 ){
					arib.PSISI((const BYTE*)service->char_service_provider_name, service->service_provider_name_length, &service_provider_name);
					AtoW(service_provider_name, item->service_provider_name);
				}
				if( service->service_name_length > 0 ){
					arib.PSISI((const BYTE*)service->char_service_name, service->service_name_length, &service_name);
					AtoW(service_name, item->service_name);
				}
				item->service_type = service->service_type;
			}
		}
	}

	if( sdt->table_id == 0x42 ){
		if( this->ONID != sdt->original_network_id ){
			this->ONID = sdt->original_network_id;
			ClearEventSD();
		}
		if( this->TSID != sdt->transport_stream_id ){
			this->TSID = sdt->transport_stream_id;
		}
	}

	//�Z�N�V�����}�b�v
	for(size_t i=0; i<sdt->serviceInfoList.size(); i++ ){
		ULONGLONG key;
		//SKY�T�[�r�X��TSID������key
		if( sdt->original_network_id == 3 ){
			key = _Create64Key(sdt->original_network_id, 0, sdt->serviceInfoList[i]->service_id);
		}else{
			key = _Create64Key(sdt->original_network_id, sdt->transport_stream_id, sdt->serviceInfoList[i]->service_id);
		}
		map<ULONGLONG, SECTION_STATUS_INFO*>::iterator itrSec;
		SECTION_STATUS_INFO* sectionInfo = NULL;
		itrSec = this->sectionMap.find(key);
		if( itrSec == this->sectionMap.end() ){
			sectionInfo = new SECTION_STATUS_INFO;
			this->sectionMap.insert(pair<ULONGLONG, SECTION_STATUS_INFO*>(key, sectionInfo));
		}else{
			sectionInfo = itrSec->second;
		}

		sectionInfo->EIT_present_following_flag = sdt->serviceInfoList[i]->EIT_present_following_flag;
		sectionInfo->EIT_schedule_flag = sdt->serviceInfoList[i]->EIT_schedule_flag;
		if( sdt->table_id == 0x46 ){
			sectionInfo->otherStreamFlag =1;
		}
	}

	UnLock();
	return TRUE;
}

BOOL CEpgDBUtil::SetTDTTime(SYSTEMTIME* time)
{
	if( Lock() == FALSE ) return FALSE;

	if( time == NULL ){
		return FALSE;
	}

	memcpy(&this->tdtTime, time, sizeof(SYSTEMTIME));

	UnLock();
	return TRUE;
}

//�w��T�[�r�X�̑SEPG�����擾����
//�߂�l�F
// �G���[�R�[�h
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// epgInfoListSize			[OUT]epgInfoList�̌�
// epgInfoList				[OUT]EPG���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDBUtil::GetEpgInfoList(
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	DWORD* epgInfoListSize,
	EPG_EVENT_INFO** epgInfoList
	)
{
	if( epgInfoListSize == NULL || epgInfoList == NULL ){
		return ERR_INVALID_ARG;
	}
	if( Lock() == FALSE ) return ERR_FALSE;

	SAFE_DELETE_ARRAY(this->epgInfoList);
	this->epgInfoListSize = 0;

	ULONGLONG key = _Create64Key(originalNetworkID, transportStreamID, serviceID);

	map<ULONGLONG, SERVICE_EVENT_INFO*>::iterator itr;
	itr = serviceEventMap.find(key);
	if( itr == serviceEventMap.end() ){
		UnLock();
		return ERR_NOT_FIND;
	}

	this->epgInfoListSize = (DWORD)itr->second->eventMap.size();
	if( this->epgInfoListSize == 0 ){
		UnLock();
		return ERR_NOT_FIND;
	}
	this->epgInfoList = new EPG_EVENT_INFO[this->epgInfoListSize];

	map<WORD, EVENT_INFO*>::iterator itrEvt;
	DWORD count = 0;
	for( itrEvt = itr->second->eventMap.begin(); itrEvt != itr->second->eventMap.end(); itrEvt++ ){
		CopyEpgInfo(this->epgInfoList+count, itrEvt->second);
		count++;
	}

	*epgInfoListSize = this->epgInfoListSize;
	*epgInfoList = this->epgInfoList;

	UnLock();
	return NO_ERR;
}

void CEpgDBUtil::CopyEpgInfo(EPG_EVENT_INFO* destInfo, EVENT_INFO* srcInfo)
{
	destInfo->event_id = srcInfo->event_id;
	destInfo->StartTimeFlag = srcInfo->StartTimeFlag;
	destInfo->start_time = srcInfo->start_time;
	destInfo->DurationFlag = srcInfo->DurationFlag;
	destInfo->durationSec = srcInfo->durationSec;
	destInfo->freeCAFlag = srcInfo->freeCAFlag;

	if( srcInfo->shortInfo != NULL ){
		EPG_SHORT_EVENT_INFO* item = new EPG_SHORT_EVENT_INFO;
		destInfo->shortInfo = item;

		item->event_nameLength = (WORD)srcInfo->shortInfo->event_name.size();
		item->event_name = new WCHAR[item->event_nameLength+1];
		ZeroMemory(item->event_name, sizeof(WCHAR)*(item->event_nameLength+1));
		if( item->event_nameLength > 0 ){
			wcscpy_s(item->event_name, item->event_nameLength+1, srcInfo->shortInfo->event_name.c_str());
		}

		item->text_charLength = (WORD)srcInfo->shortInfo->text_char.size();
		item->text_char = new WCHAR[item->text_charLength+1];
		ZeroMemory(item->text_char, sizeof(WCHAR)*(item->text_charLength+1));
		if( item->text_charLength > 0 ){
			wcscpy_s(item->text_char, item->text_charLength+1, srcInfo->shortInfo->text_char.c_str());
		}
	}

	if( srcInfo->extInfo != NULL ){
		EPG_EXTENDED_EVENT_INFO* item = new EPG_EXTENDED_EVENT_INFO;
		destInfo->extInfo = item;

		item->text_charLength = (WORD)srcInfo->extInfo->text_char.size();
		item->text_char = new WCHAR[item->text_charLength+1];
		ZeroMemory(item->text_char, sizeof(WCHAR)*(item->text_charLength+1));
		if( item->text_charLength > 0 ){
			wcscpy_s(item->text_char, item->text_charLength+1, srcInfo->extInfo->text_char.c_str());
		}
	}

	if( srcInfo->contentInfo != NULL ){
		EPG_CONTEN_INFO* item = new EPG_CONTEN_INFO;
		destInfo->contentInfo = item;

		item->listSize = (WORD)srcInfo->contentInfo->nibbleList.size();
		if( item->listSize > 0 ){
			item->nibbleList = new EPG_CONTENT[item->listSize];
			for( WORD i=0; i<item->listSize; i++ ){
				item->nibbleList[i].content_nibble_level_1 = srcInfo->contentInfo->nibbleList[i].content_nibble_level_1;
				item->nibbleList[i].content_nibble_level_2 = srcInfo->contentInfo->nibbleList[i].content_nibble_level_2;
				item->nibbleList[i].user_nibble_1 = srcInfo->contentInfo->nibbleList[i].user_nibble_1;
				item->nibbleList[i].user_nibble_2 = srcInfo->contentInfo->nibbleList[i].user_nibble_2;
			}
		}
	}

	if( srcInfo->componentInfo != NULL ){
		EPG_COMPONENT_INFO* item = new EPG_COMPONENT_INFO;
		destInfo->componentInfo = item;

		item->stream_content = srcInfo->componentInfo->stream_content;
		item->component_type = srcInfo->componentInfo->component_type;
		item->component_tag = srcInfo->componentInfo->component_tag;

		item->text_charLength = (WORD)srcInfo->componentInfo->text_char.size();
		item->text_char = new WCHAR[item->text_charLength+1];
		ZeroMemory(item->text_char, sizeof(WCHAR)*(item->text_charLength+1));
		if( item->text_charLength > 0 ){
			wcscpy_s(item->text_char, item->text_charLength+1, srcInfo->componentInfo->text_char.c_str());
		}
	}

	if( srcInfo->audioInfo != NULL ){
		EPG_AUDIO_COMPONENT_INFO* item = new EPG_AUDIO_COMPONENT_INFO;
		destInfo->audioInfo = item;
		item->listSize = (WORD)srcInfo->audioInfo->componentList.size();
		if( item->listSize > 0 ){
			item->audioList = new EPG_AUDIO_COMPONENT_INFO_DATA[item->listSize];
			for( WORD i=0; i<item->listSize; i++ ){
				item->audioList[i].stream_content = srcInfo->audioInfo->componentList[i].stream_content;
				item->audioList[i].component_type = srcInfo->audioInfo->componentList[i].component_type;
				item->audioList[i].component_tag = srcInfo->audioInfo->componentList[i].component_tag;
				item->audioList[i].stream_type = srcInfo->audioInfo->componentList[i].stream_type;
				item->audioList[i].simulcast_group_tag = srcInfo->audioInfo->componentList[i].simulcast_group_tag;
				item->audioList[i].ES_multi_lingual_flag = srcInfo->audioInfo->componentList[i].ES_multi_lingual_flag;
				item->audioList[i].main_component_flag = srcInfo->audioInfo->componentList[i].main_component_flag;
				item->audioList[i].quality_indicator = srcInfo->audioInfo->componentList[i].quality_indicator;
				item->audioList[i].sampling_rate = srcInfo->audioInfo->componentList[i].sampling_rate;

				item->audioList[i].text_charLength = (WORD)srcInfo->audioInfo->componentList[i].text_char.size();
				item->audioList[i].text_char = new WCHAR[item->audioList[i].text_charLength+1];
				ZeroMemory(item->audioList[i].text_char, sizeof(WCHAR)*(item->audioList[i].text_charLength+1));
				if( item->audioList[i].text_charLength > 0 ){
					wcscpy_s(item->audioList[i].text_char, item->audioList[i].text_charLength+1, srcInfo->audioInfo->componentList[i].text_char.c_str());
				}
			}
		}
	}

	if( srcInfo->eventGroupInfo != NULL ){
		EPG_EVENTGROUP_INFO* item = new EPG_EVENTGROUP_INFO;
		destInfo->eventGroupInfo = item;

		item->group_type = srcInfo->eventGroupInfo->group_type;
		item->event_count = srcInfo->eventGroupInfo->event_count;

		if( item->event_count > 0 ){
			item->eventDataList = new EPG_EVENT_DATA[item->event_count];
			for( BYTE i=0; i<item->event_count; i++ ){
				item->eventDataList[i].original_network_id = srcInfo->eventGroupInfo->eventData2List[i].original_network_id;
				item->eventDataList[i].transport_stream_id = srcInfo->eventGroupInfo->eventData2List[i].transport_stream_id;
				item->eventDataList[i].service_id = srcInfo->eventGroupInfo->eventData2List[i].service_id;
				item->eventDataList[i].event_id = srcInfo->eventGroupInfo->eventData2List[i].event_id;
			}
		}
	}

	if( srcInfo->eventRelayInfo != NULL ){
		EPG_EVENTGROUP_INFO* item = new EPG_EVENTGROUP_INFO;
		destInfo->eventRelayInfo = item;

		item->group_type = srcInfo->eventRelayInfo->group_type;
		//���`�����l���̂Ƃ�event_count�͂O�ɂȂ��Ă���
		//item->event_count = srcInfo->eventGroupInfo->event_count;
		item->event_count = (BYTE)srcInfo->eventRelayInfo->eventData2List.size();

		if( item->event_count > 0 ){
			item->eventDataList = new EPG_EVENT_DATA[item->event_count];
			for( BYTE i=0; i<item->event_count; i++ ){
				item->eventDataList[i].original_network_id = srcInfo->eventRelayInfo->eventData2List[i].original_network_id;
				item->eventDataList[i].transport_stream_id = srcInfo->eventRelayInfo->eventData2List[i].transport_stream_id;
				item->eventDataList[i].service_id = srcInfo->eventRelayInfo->eventData2List[i].service_id;
				item->eventDataList[i].event_id = srcInfo->eventRelayInfo->eventData2List[i].event_id;
			}
		}
	}

}

//�~�ς��ꂽEPG���̂���T�[�r�X�ꗗ���擾����
//SERVICE_EXT_INFO�̏��͂Ȃ��ꍇ������
//�߂�l�F
// �G���[�R�[�h
//�����F
// serviceListSize			[OUT]serviceList�̌�
// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDBUtil::GetServiceListEpgDB(
	DWORD* serviceListSize,
	SERVICE_INFO** serviceList
	)
{
	SAFE_DELETE_ARRAY(this->serviceDBList);
	this->serviceDBListSize = 0;

	if( serviceListSize == NULL || serviceList == NULL ){
		return ERR_INVALID_ARG;
	}

	this->serviceDBListSize = (DWORD)this->serviceEventMap.size();
	this->serviceDBList = new SERVICE_INFO[this->serviceDBListSize];

	DWORD count = 0;
	map<ULONGLONG, SERVICE_EVENT_INFO*>::iterator itr;
	for(itr = this->serviceEventMap.begin(); itr != this->serviceEventMap.end(); itr++ ){
		this->serviceDBList[count].original_network_id = (WORD)(itr->first>>32);
		this->serviceDBList[count].transport_stream_id = (WORD)((itr->first&0xFFFF0000)>>16);
		this->serviceDBList[count].service_id = (WORD)(itr->first&0xFFFF);

		DWORD infoKey = ((DWORD)this->serviceDBList[count].original_network_id) << 16 | this->serviceDBList[count].transport_stream_id;
		map<DWORD, DB_TS_INFO*>::iterator itrInfo;
		itrInfo = this->serviceInfoList.find(infoKey);
		if( itrInfo != this->serviceInfoList.end() ){
			map<WORD,DB_SERVICE_INFO*>::iterator itrService;
			itrService = itrInfo->second->serviceList.find(this->serviceDBList[count].service_id);
			if( itrService != itrInfo->second->serviceList.end() ){
				DB_TS_INFO* info = itrInfo->second;
				DB_SERVICE_INFO* item = itrService->second;
				this->serviceDBList[count].extInfo = new SERVICE_EXT_INFO;

				this->serviceDBList[count].extInfo->service_type = item->service_type;
				this->serviceDBList[count].extInfo->partialReceptionFlag = item->partialReceptionFlag;
				if( info->remote_control_key_id != 0 ){
					if( 0x7880 <= item->original_network_id && item->original_network_id <= 0x7FEB ){
						if( (item->service_id & 0x0187) == 0x0000 ){
							this->serviceDBList[count].extInfo->remote_control_key_id = info->remote_control_key_id;
						}
						this->serviceDBList[count].extInfo->direct_tuning_number = 
								(((item->service_id & 0x0180) >> 7) * 200) +	//�T�[�r�X��� * 200 +
								(info->remote_control_key_id * 10) +			//�����R���L�[ID * 10 +
								((item->service_id & 0x0007) + 1);				//�T�[�r�X�ԍ� + 1
					}else{
						this->serviceDBList[count].extInfo->remote_control_key_id = info->remote_control_key_id;
					}
				}

				if( item->service_provider_name.size() > 0 ){
					this->serviceDBList[count].extInfo->service_provider_name = new WCHAR[item->service_provider_name.size()+1];
					wcscpy_s(this->serviceDBList[count].extInfo->service_provider_name, item->service_provider_name.size()+1, item->service_provider_name.c_str());
				}
				if( item->service_name.size() > 0 ){
					this->serviceDBList[count].extInfo->service_name = new WCHAR[item->service_name.size()+1];
					wcscpy_s(this->serviceDBList[count].extInfo->service_name, item->service_name.size()+1, item->service_name.c_str());
				}
				if( info->network_name.size() > 0 ){
					this->serviceDBList[count].extInfo->network_name = new WCHAR[info->network_name.size()+1];
					wcscpy_s(this->serviceDBList[count].extInfo->network_name, info->network_name.size()+1, info->network_name.c_str());
				}
				if( info->ts_name.size() > 0 ){
					this->serviceDBList[count].extInfo->ts_name = new WCHAR[info->ts_name.size()+1];
					wcscpy_s(this->serviceDBList[count].extInfo->ts_name, info->ts_name.size()+1, info->ts_name.c_str());
				}
			}
		}

		count++;
	}

	*serviceListSize = this->serviceDBListSize;
	*serviceList = this->serviceDBList;

	return NO_ERR;
}

//�w��T�[�r�X�̌���or����EPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// nextFlag					[IN]TRUE�i���̔ԑg�j�AFALSE�i���݂̔ԑg�j
// nowTime					[IN]���݂̎���
// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDBUtil::GetEpgInfo(
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	BOOL nextFlag,
	SYSTEMTIME nowTime,
	EPG_EVENT_INFO** epgInfo
	)
{
	if( epgInfo == NULL ){
		return ERR_INVALID_ARG;
	}
	if( Lock() == FALSE ) return ERR_FALSE;

	SAFE_DELETE(this->epgInfo);

	ULONGLONG key = _Create64Key(originalNetworkID, transportStreamID, serviceID);

	map<ULONGLONG, SERVICE_EVENT_INFO*>::iterator itr;
	itr = serviceEventMap.find(key);
	if( itr == serviceEventMap.end() ){
		UnLock();
		return ERR_NOT_FIND;
	}

	__int64 nowTime64 = ConvertI64Time(nowTime);

	//�܂�p/f�̊m�F
	if( itr->second->nowEvent != NULL && nextFlag == FALSE ){
		if( itr->second->nowEvent->StartTimeFlag == TRUE && itr->second->nowEvent->DurationFlag == TRUE){
			__int64 startTime = ConvertI64Time(itr->second->nowEvent->start_time);
			__int64 endTime = GetSumTime(itr->second->nowEvent->start_time, itr->second->nowEvent->durationSec);
			if( startTime <= nowTime64 && nowTime64 < endTime ){
				//���ԓ��ɂ���̂Ő������͂�
				this->epgInfo = new EPG_EVENT_INFO;
				CopyEpgInfo(this->epgInfo, itr->second->nowEvent);
				goto Err_End;
			}
		}else{
			//�ǂ���������Ȃ̂ŐM����
			this->epgInfo = new EPG_EVENT_INFO;
			CopyEpgInfo(this->epgInfo, itr->second->nowEvent);
			goto Err_End;
		}
	}else if( itr->second->nextEvent != NULL && nextFlag == TRUE ){
		if( itr->second->nextEvent->StartTimeFlag == TRUE && itr->second->nextEvent->DurationFlag == TRUE){
			__int64 startTime = ConvertI64Time(itr->second->nextEvent->start_time);
			//__int64 endTime = GetSumTime(itr->second->nextEvent->start_time, itr->second->nextEvent->durationSec);
			if( nowTime64 <= startTime ){
				//�J�n���Ԑ�ɂ���̂Ő������͂�
				this->epgInfo = new EPG_EVENT_INFO;
				CopyEpgInfo(this->epgInfo, itr->second->nextEvent);
				goto Err_End;
			}
		}else{
			//�ǂ���������Ȃ̂ŐM����
			this->epgInfo = new EPG_EVENT_INFO;
			CopyEpgInfo(this->epgInfo, itr->second->nextEvent);
			goto Err_End;
		}
	}
	/*
	//p/f�Ŋm�F�ł��Ȃ������̂�DB�̎��ԓI�ɂ������̒T��
	if( nextFlag == FALSE ){
		//����
		map<WORD, EVENT_INFO*>::iterator itrEvt;
		for( itrEvt = itr->second->eventMap.begin(); itrEvt != itr->second->eventMap.end(); itrEvt++ ){
			if( itrEvt->second->StartTimeFlag == TRUE && itrEvt->second->DurationFlag == TRUE ){
				__int64 startTime = ConvertI64Time(itrEvt->second->start_time);
				__int64 endTime = GetSumTime(itrEvt->second->start_time, itrEvt->second->durationSec);
				if( startTime <= nowTime64 && nowTime64 < endTime ){
					//���ԓ��ɂ���̂Ő������͂�
					this->epgInfo = new EPG_EVENT_INFO;
					CopyEpgInfo(this->epgInfo, itrEvt->second);
					goto Err_End;
				}
			}
		}
	}else{
		//p/f�ɂȂ��̂Ŏ��ԓI�Ɏ��ɂȂ�ԑg�T��

		map<__int64, EVENT_INFO*> timeSort;
		map<WORD, EVENT_INFO*>::iterator itrEvt;
		for( itrEvt = itr->second->eventMap.begin(); itrEvt != itr->second->eventMap.end(); itrEvt++ ){
			if( itrEvt->second->StartTimeFlag == TRUE ){
				__int64 startTime = ConvertI64Time(itrEvt->second->start_time);
				//���̌�����K�v����̂Ŏ��ԂŃ\�[�g
				timeSort.insert(pair<__int64, EVENT_INFO*>(startTime, itrEvt->second));
			}
		}
		map<__int64, EVENT_INFO*>::iterator itrSort;
		for( itrSort = timeSort.begin(); itrSort != timeSort.end(); itrSort++ ){
			if( nowTime64 < itrSort->first ){
				//���݂��J�n���ԑ����̂Ŏ��ɂȂ�͂�
				this->epgInfo = new EPG_EVENT_INFO;
				CopyEpgInfo(this->epgInfo, itrSort->second);
				goto Err_End;
			}
		}
	}
	*/
Err_End:
	if( this->epgInfo == NULL ){
		UnLock();
		return ERR_NOT_FIND;
	}
	*epgInfo = this->epgInfo;

	UnLock();
	return NO_ERR;
}

//�w��C�x���g��EPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// EventID					[IN]�擾�Ώۂ�EventID
// pfOnlyFlag				[IN]p/f����̂݌������邩�ǂ���
// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDBUtil::SearchEpgInfo(
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	WORD eventID,
	BYTE pfOnlyFlag,
	EPG_EVENT_INFO** epgInfo
	)
{
	if( epgInfo == NULL ){
		return ERR_INVALID_ARG;
	}
	if( Lock() == FALSE ) return ERR_FALSE;

	SAFE_DELETE(this->searchEpgInfo);

	ULONGLONG key = _Create64Key(originalNetworkID, transportStreamID, serviceID);

	map<ULONGLONG, SERVICE_EVENT_INFO*>::iterator itr;
	itr = serviceEventMap.find(key);
	if( itr == serviceEventMap.end() ){
		UnLock();
		return ERR_NOT_FIND;
	}

	if( pfOnlyFlag == 0 ){
		map<WORD, EVENT_INFO*>::iterator itrEvent;
		itrEvent = itr->second->eventMap.find(eventID);
		if( itrEvent != itr->second->eventMap.end() ){
			this->searchEpgInfo = new EPG_EVENT_INFO;
			CopyEpgInfo(this->searchEpgInfo, itrEvent->second);
			goto Err_End;
		}
	}else{
		if( itr->second->nowEvent != NULL ){
			if( itr->second->nowEvent->event_id == eventID ){
				this->searchEpgInfo = new EPG_EVENT_INFO;
				CopyEpgInfo(this->searchEpgInfo, itr->second->nowEvent);
				goto Err_End;
			}
		}
		if( itr->second->nextEvent != NULL ){
			if( itr->second->nextEvent->event_id == eventID ){
				this->searchEpgInfo = new EPG_EVENT_INFO;
				CopyEpgInfo(this->searchEpgInfo, itr->second->nextEvent);
				goto Err_End;
			}
		}
	}

Err_End:
	if( this->searchEpgInfo == NULL ){
		UnLock();
		return ERR_NOT_FIND;
	}
	*epgInfo = this->searchEpgInfo;

	UnLock();
	return NO_ERR;
}

BOOL CEpgDBUtil::AddSDEventMap(CEITTable_SD* eit)
{
	if( eit == NULL ){
		return FALSE;
	}

	ULONGLONG key = _Create64Key(eit->original_network_id, 0, eit->service_id);

	BOOL alreadyGot = FALSE;

	//�Z�N�V�����X�e�[�^�X
	map<ULONGLONG, SECTION_STATUS_INFO*>::iterator itrSec;
	SECTION_STATUS_INFO* sectionInfo = NULL;
	itrSec = this->sectionMapSD.find(key);
	if( itrSec == this->sectionMapSD.end() ){
		sectionInfo = new SECTION_STATUS_INFO;
		this->sectionMapSD.insert(pair<ULONGLONG, SECTION_STATUS_INFO*>(key, sectionInfo));
	}else{
		sectionInfo = itrSec->second;
	}

	SECTION_FLAG_INFO* sectionFlag = NULL;
	if( eit->table_id == 0xA4 ){
		sectionFlag = &sectionInfo->sectionBasic;
	}else{
		sectionFlag = &sectionInfo->sectionExt;
	}

	//�o�[�W�����ԍ����ւ���Ă�����Clear
	if( eit->version_number != sectionFlag->version[0] ){
		if( (BYTE)~0 != sectionFlag->version[0] ){
			sectionFlag->Clear();
		}
		sectionFlag->version[0] = eit->version_number;
	}
	//�ŏI�Z�N�V�����ԍ�
	if( sectionFlag->last_section_number == (WORD)~0 ){
		sectionFlag->last_section_number = eit->last_section_number;
	}
	//���擾�̃Z�N�V�����H
	if( (sectionFlag->sectionFlag[eit->section_number/64] & (1ULL<<(eit->section_number%64))) == 0 ){
		//���݂̃Z�N�V�����ԍ��ɊY������bit�𗧂Ă�
		sectionFlag->sectionFlag[eit->section_number/64] |= 1ULL<<eit->section_number%64;
	}else{
		alreadyGot = TRUE;
	}
	
	if( alreadyGot ){
		return TRUE;
	}

	//�T�[�r�X��map���擾
	map<ULONGLONG, SERVICE_EVENT_INFO*>::iterator itr;
	SERVICE_EVENT_INFO* serviceInfo = NULL;

	itr = serviceEventMapSD.find(key);
	if( itr == serviceEventMapSD.end() ){
		serviceInfo = new SERVICE_EVENT_INFO;
		serviceEventMapSD.insert(pair<ULONGLONG, SERVICE_EVENT_INFO*>(key, serviceInfo));
	}else{
		serviceInfo = itr->second;
	}

	//�C�x���g���ƂɍX�V�K�v������
	for( size_t i=0; i<eit->eventInfoList.size(); i++ ){
		CEITTable_SD::EVENT_INFO_DATA* eitEventInfo = eit->eventInfoList[i];
		map<WORD, EVENT_INFO*>::iterator itrEvent;
		EVENT_INFO* eventInfo = NULL;

		if( eitEventInfo->running_status != 0 && eitEventInfo->running_status != 2 && eitEventInfo->running_status != 4 ){
			//����s���܂��͒�~��
			_OutputDebugString(L"������s���܂��͒�~���C�x���g ONID:0x%04x TSID:0x%04x SID:0x%04x EventID:0x%04x %04d/%02d/%02d %02d:%02d\r\n",
				eit->original_network_id,  eit->transport_stream_id, eit->service_id, eitEventInfo->event_id,
				eitEventInfo->start_time.wYear, eitEventInfo->start_time.wMonth, eitEventInfo->start_time.wDay, eitEventInfo->start_time.wHour, eitEventInfo->start_time.wMinute
				);
			continue;
		}

		itrEvent = serviceInfo->eventMap.find(eitEventInfo->event_id);
		if( itrEvent == serviceInfo->eventMap.end() ){
			eventInfo = new EVENT_INFO;
			eventInfo->ONID = eit->original_network_id;
			eventInfo->TSID = eit->transport_stream_id;
			eventInfo->SID = eit->service_id;
			eventInfo->event_id = eitEventInfo->event_id;
			eventInfo->StartTimeFlag = eitEventInfo->StartTimeFlag;
			eventInfo->start_time = eitEventInfo->start_time;
			eventInfo->DurationFlag = eitEventInfo->DurationFlag;
			eventInfo->durationSec = eitEventInfo->durationHH*60*60 +
				eitEventInfo->durationMM*60 +
				eitEventInfo->durationSS;
			eventInfo->freeCAFlag = eitEventInfo->free_CA_mode;
			serviceInfo->eventMap.insert(pair<WORD, EVENT_INFO*>(eventInfo->event_id, eventInfo));
		}else{
			eventInfo = itrEvent->second;

			eventInfo->StartTimeFlag = eitEventInfo->StartTimeFlag;
			eventInfo->start_time = eitEventInfo->start_time;
			eventInfo->DurationFlag = eitEventInfo->DurationFlag;
			eventInfo->durationSec = eitEventInfo->durationHH*60*60 +
				eitEventInfo->durationMM*60 +
				eitEventInfo->durationSS;
		}

		//ExtendedEvent�͕�������̂�1�x�����`�F�b�N����
		BOOL checkExtFlag = FALSE;
		BOOL checkAudioFlag = FALSE;
		for( size_t j=0; j<eitEventInfo->descriptorList.size(); j++ ){
			if( eitEventInfo->descriptorList[j]->shortEvent != NULL ){
				AddShortEvent_SD( eit, eventInfo, eitEventInfo->descriptorList[j]->shortEvent );
			}else if( eitEventInfo->descriptorList[j]->extendedEvent != NULL && checkExtFlag == FALSE){
				AddExtEvent_SD(eit, eventInfo, &eitEventInfo->descriptorList );
				checkExtFlag = TRUE;
			}else if( eitEventInfo->descriptorList[j]->content != NULL ){
				AddContent_SD( eit, eventInfo, eitEventInfo->descriptorList[j]->content );
			}else if( eitEventInfo->descriptorList[j]->component != NULL ){
				AddComponent_SD( eit, eventInfo, eitEventInfo->descriptorList[j]->component );
			}else if( eitEventInfo->descriptorList[j]->audioComponent != NULL && checkAudioFlag == FALSE ){
				AddAudioComponent_SD( eit, eventInfo, &eitEventInfo->descriptorList );
				checkAudioFlag = TRUE;
			}
		}
	}

	return TRUE;
}

BOOL CEpgDBUtil::AddEIT_SD2(WORD PID, CEITTable_SD2* eit)
{
	if( eit == NULL ){
		return FALSE;
	}
	if( Lock() == FALSE ) return FALSE;

	bool alreadyGot = FALSE;

	//�Z�N�V�����X�e�[�^�X
	map<ULONGLONG, SECTION_STATUS_INFO*>::iterator itrSec;
	SECTION_STATUS_INFO* sectionInfo = NULL;
	ULONGLONG key0 = _Create64Key(eit->original_network_id, 0, eit->service_id);
	itrSec = this->sectionMap.find(key0);
	if( itrSec == this->sectionMap.end() ){
		sectionInfo = new SECTION_STATUS_INFO;
		this->sectionMap.insert(pair<ULONGLONG, SECTION_STATUS_INFO*>(key0, sectionInfo));
	}else{
		sectionInfo = itrSec->second;
	}
	//�o�[�W�����ԍ����ւ���Ă�����Clear
	if( (BYTE)~0 != sectionInfo->sectionBasic.version[0] && sectionInfo->sectionBasic.version[0] != eit->version_number ){
		sectionInfo->sectionBasic.Clear();
	}
	//�擾�ς̃Z�N�V����?
	if ( (sectionInfo->sectionBasic.sectionFlag[eit->section_number/64] & (1ULL<<(eit->section_number%64))) != 0 ){
		alreadyGot = TRUE;
	}
	
	if( alreadyGot ){
		UnLock();
		return NO_ERR;
	}

	ULONGLONG key = _Create64Key(eit->original_network_id, 0, eit->service_id2);

	//A4/A7�Z�N�V�����X�e�[�^�X�̊m�F
	BOOL existInfo = FALSE;
	map<ULONGLONG, SECTION_STATUS_INFO*>::iterator itrSec2;
	SECTION_STATUS_INFO* sectionInfo2 = NULL;
	itrSec2 = this->sectionMapSD.find(key);
	if( itrSec2 != this->sectionMapSD.end() ){
		sectionInfo2 = itrSec2->second;
		if( CheckSectionAll(&sectionInfo2->sectionBasic, 0, 0, NULL, NULL) == TRUE && CheckSectionAll(&sectionInfo2->sectionExt, 0, 0, NULL, NULL) == TRUE ){
			existInfo = TRUE;
		}
	}

	if( !existInfo ){
		UnLock();
		return NO_ERR;
	}

	//�T�[�r�X��map���擾
	map<ULONGLONG, SERVICE_EVENT_INFO*>::iterator itr;
	SERVICE_EVENT_INFO* serviceInfo = NULL;

	itr = serviceEventMapSD.find(key);
	if( itr != serviceEventMapSD.end() ){
		serviceInfo = itr->second;
		BOOL foundAll = TRUE;
		for( size_t i=0; i<eit->eventMapList.size(); i++ ){
			for( size_t j=0; j<eit->eventMapList[i]->eventList.size(); j++ ){
				map<WORD, EVENT_INFO*>::iterator itrEvent;
				itrEvent = serviceInfo->eventMap.find(eit->eventMapList[i]->eventList[j].a4table_eventID);
				if( itrEvent != serviceInfo->eventMap.end() ){
					ULONGLONG key2 = _Create64Key(itrEvent->second->ONID, itrEvent->second->TSID, eit->service_id);
					map<ULONGLONG, SERVICE_EVENT_INFO*>::iterator itrMainDB;
					SERVICE_EVENT_INFO* mainServiceInfo = NULL;
					itrMainDB = serviceEventMap.find(key2);
					if( itrMainDB == serviceEventMap.end() ){
						mainServiceInfo = new SERVICE_EVENT_INFO;
						serviceEventMap.insert(pair<ULONGLONG, SERVICE_EVENT_INFO*>(key2, mainServiceInfo));
					}else{
						mainServiceInfo = itrMainDB->second;
					}
					EVENT_INFO* eventInfo = NULL;
					map<WORD, EVENT_INFO*>::iterator itrMainEvent;
					itrMainEvent = mainServiceInfo->eventMap.find(eit->eventMapList[i]->eventList[j].event_id);
					if( itrMainEvent == mainServiceInfo->eventMap.end()){
						eventInfo = new EVENT_INFO;
						mainServiceInfo->eventMap.insert(pair<WORD, EVENT_INFO*>(eit->eventMapList[i]->eventList[j].event_id, eventInfo));
					}else{
						eventInfo = itrMainEvent->second;
					}

					eventInfo->ONID = itrEvent->second->ONID;
					eventInfo->TSID = itrEvent->second->TSID;
					eventInfo->SID = eit->service_id;
					eventInfo->event_id = eit->eventMapList[i]->eventList[j].event_id;
					eventInfo->StartTimeFlag = itrEvent->second->StartTimeFlag;
					eventInfo->start_time = eit->eventMapList[i]->start_day;
					eventInfo->start_time.wHour = eit->eventMapList[i]->eventList[j].hour;
					eventInfo->start_time.wMinute = eit->eventMapList[i]->eventList[j].minute;
					eventInfo->DurationFlag = itrEvent->second->DurationFlag;
					if( eit->eventMapList[i]->eventList[j].duration == 0 ){
						eventInfo->durationSec = itrEvent->second->durationSec;
					}else{
						eventInfo->durationSec = eit->eventMapList[i]->eventList[j].duration;
					}
					eventInfo->freeCAFlag = itrEvent->second->freeCAFlag;

					if(itrEvent->second->shortInfo != NULL && eventInfo->shortInfo == NULL){
						eventInfo->shortInfo = new SHORT_EVENT_INFO;
						*eventInfo->shortInfo = *itrEvent->second->shortInfo;
					}
					if(itrEvent->second->extInfo != NULL && eventInfo->extInfo == NULL ){
						eventInfo->extInfo = new EXTENDED_EVENT_INFO;
						*eventInfo->extInfo = *itrEvent->second->extInfo;
					}
					if(itrEvent->second->contentInfo != NULL && eventInfo->contentInfo == NULL ){
						eventInfo->contentInfo = new CONTEN_INFO;
						*eventInfo->contentInfo = *itrEvent->second->contentInfo;
					}
					if(itrEvent->second->componentInfo != NULL && eventInfo->componentInfo == NULL ){
						eventInfo->componentInfo = new COMPONENT_INFO;
						*eventInfo->componentInfo = *itrEvent->second->componentInfo;
					}
					if(itrEvent->second->audioInfo != NULL && eventInfo->audioInfo == NULL ){
						eventInfo->audioInfo = new AUDIO_COMPONENT_INFO;
						*eventInfo->audioInfo = *itrEvent->second->audioInfo;
					}

				}else{
					foundAll = FALSE;
				}
			}
		}

		//�S�ēo�^�ł�����Z�N�V�����X�e�[�^�X�X�V
		if( foundAll == TRUE ){
			//�o�[�W�����ԍ�
			if( (WORD)~0 == sectionInfo->sectionBasic.version[0] ){
				sectionInfo->sectionBasic.version[0] = eit->version_number;
			}
			//�ŏI�Z�N�V�����ԍ�
			if( (WORD)~0 == sectionInfo->sectionBasic.last_section_number ){
				sectionInfo->sectionBasic.last_section_number = eit->last_section_number;
			}
			//���݂̃Z�N�V�����ԍ��ɊY������bit�𗧂Ă�
			sectionInfo->sectionBasic.sectionFlag[eit->section_number/64] |= 1ULL<<(eit->section_number%64);
		}
	}

	UnLock();
	return NO_ERR;
}

void CEpgDBUtil::ClearEvent()
{
	map<ULONGLONG, SERVICE_EVENT_INFO*>::iterator itr;
	for( itr = this->serviceEventMap.begin(); itr != this->serviceEventMap.end(); itr++ ){
		SAFE_DELETE(itr->second);
	}
	this->serviceEventMap.clear();
	ClearSectionMap();
}

void CEpgDBUtil::ClearSectionMap()
{
	map<ULONGLONG, SECTION_STATUS_INFO*>::iterator itrSec;
	for( itrSec = this->sectionMap.begin(); itrSec != this->sectionMap.end(); itrSec++ ){
		SAFE_DELETE(itrSec->second);
	}
	this->sectionMap.clear();
}

void CEpgDBUtil::ClearEventSD()
{
	map<ULONGLONG, SERVICE_EVENT_INFO*>::iterator itr;
	for( itr = this->serviceEventMapSD.begin(); itr != this->serviceEventMapSD.end(); itr++ ){
		SAFE_DELETE(itr->second);
	}
	this->serviceEventMapSD.clear();

	map<ULONGLONG, SECTION_STATUS_INFO*>::iterator itrSec;
	for( itrSec = this->sectionMapSD.begin(); itrSec != this->sectionMapSD.end(); itrSec++ ){
		SAFE_DELETE(itrSec->second);
	}
	this->sectionMapSD.clear();
}
