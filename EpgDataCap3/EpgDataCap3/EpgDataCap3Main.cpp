#include "StdAfx.h"
#include "EpgDataCap3Main.h"


CEpgDataCap3Main::CEpgDataCap3Main(void)
{
	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL);

	decodeUtilClass.SetEpgDB(&(this->epgDBUtilClass));
}

CEpgDataCap3Main::~CEpgDataCap3Main(void)
{
	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}
}

BOOL CEpgDataCap3Main::Lock(LPCWSTR log, DWORD timeOut)
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

void CEpgDataCap3Main::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

//DLL�̏�����
//�߂�l�F
// �G���[�R�[�h
//�����F
// asyncMode		[IN]TRUE:�񓯊����[�h�AFALSE:�������[�h
DWORD CEpgDataCap3Main::Initialize(
	BOOL asyncFlag
	)
{
	return NO_ERR;
}

//DLL�̊J��
//�߂�l�F
// �G���[�R�[�h
DWORD CEpgDataCap3Main::UnInitialize(
	)
{
	return NO_ERR;
}

//��͑Ώۂ�TS�p�P�b�g�P��ǂݍ��܂���
//�߂�l�F
// �G���[�R�[�h
// data		[IN]TS�p�P�b�g�P��
// size		[IN]data�̃T�C�Y�i188�A192������ɂȂ�͂��j
DWORD CEpgDataCap3Main::AddTSPacket(
	BYTE* data,
	DWORD size
	)
{
	if( size < 188 ){
		return ERR_INVALID_ARG;
	}

	if( Lock() == FALSE ) return ERR_FALSE;

	DWORD stratPos = 0;
	if( size > 188 ){
		if( data[0] != 0x47 ){
			if( data[size-188] == 0x47 ){
				stratPos = size-188;
			}else{
				for( DWORD i=0; i<size-188; i++ ){
					if( data[i] == 0x47 ){
						break;
					}else{
						stratPos++;
					}
				}
			}
		}
	}
	DWORD err = this->decodeUtilClass.AddTSData(data+stratPos, 188);
	UnLock();
	return err;
}

//��̓f�[�^�̌��݂̃X�g���[���h�c���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[OUT]���݂�originalNetworkID
// transportStreamID		[OUT]���݂�transportStreamID
DWORD CEpgDataCap3Main::GetTSID(
	WORD* originalNetworkID,
	WORD* transportStreamID
	)
{
	if( Lock() == FALSE ) return ERR_FALSE;
	DWORD err = this->decodeUtilClass.GetTSID(originalNetworkID, transportStreamID);
	UnLock();
	return err;
}

//���X�g���[���̃T�[�r�X�ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// serviceListSize			[OUT]serviceList�̌�
// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDataCap3Main::GetServiceListActual(
	DWORD* serviceListSize,
	SERVICE_INFO** serviceList
	)
{
	if( Lock() == FALSE ) return ERR_FALSE;
	DWORD err = this->decodeUtilClass.GetServiceListActual(serviceListSize, serviceList);
	UnLock();
	return err;
}

//�X�g���[���̕ύX��ʒm����
void CEpgDataCap3Main::SetStreamChangeEvent()
{
	if( Lock() == FALSE ) return ;
	this->decodeUtilClass.SetStreamChangeEvent();
	UnLock();
	return ;
}

//�~�ς��ꂽEPG���̂���T�[�r�X�ꗗ���擾����
//SERVICE_EXT_INFO�̏��͂Ȃ��ꍇ������
//�߂�l�F
// �G���[�R�[�h
//�����F
// serviceListSize			[OUT]serviceList�̌�
// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDataCap3Main::GetServiceListEpgDB(
	DWORD* serviceListSize,
	SERVICE_INFO** serviceList
	)
{
	if( Lock() == FALSE ) return ERR_FALSE;
	DWORD err = this->epgDBUtilClass.GetServiceListEpgDB(serviceListSize, serviceList);
	UnLock();
	return err;
}

//�w��T�[�r�X�̑SEPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// epgInfoListSize			[OUT]epgInfoList�̌�
// epgInfoList				[OUT]EPG���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDataCap3Main::GetEpgInfoList(
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	DWORD* epgInfoListSize,
	EPG_EVENT_INFO** epgInfoList
	)
{
	if( Lock() == FALSE ) return ERR_FALSE;
	DWORD err = this->epgDBUtilClass.GetEpgInfoList(originalNetworkID, transportStreamID, serviceID, epgInfoListSize, epgInfoList);
	UnLock();
	return err;
}

//�w��T�[�r�X�̌���or����EPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// nextFlag					[IN]TRUE�i���̔ԑg�j�AFALSE�i���݂̔ԑg�j
// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD CEpgDataCap3Main::GetEpgInfo(
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	BOOL nextFlag,
	EPG_EVENT_INFO** epgInfo
	)
{
	if( Lock() == FALSE ) return ERR_FALSE;
	SYSTEMTIME nowTime;
	if( this->decodeUtilClass.GetNowTime(&nowTime) != NO_ERR ){
		GetLocalTime(&nowTime);
	}
	DWORD err = this->epgDBUtilClass.GetEpgInfo(originalNetworkID, transportStreamID, serviceID, nextFlag, nowTime, epgInfo);
	UnLock();
	return err;
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
DWORD CEpgDataCap3Main::SearchEpgInfo(
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	WORD eventID,
	BYTE pfOnlyFlag,
	EPG_EVENT_INFO** epgInfo
	)
{
	if( Lock() == FALSE ) return ERR_FALSE;

	DWORD err = this->epgDBUtilClass.SearchEpgInfo(originalNetworkID, transportStreamID, serviceID, eventID, pfOnlyFlag, epgInfo);
	UnLock();
	return err;
}

//EPG�f�[�^�̒~�Ϗ�Ԃ����Z�b�g����
void CEpgDataCap3Main::ClearSectionStatus()
{
	if( Lock() == FALSE ) return ;
	this->epgDBUtilClass.ClearSectionStatus();
	UnLock();
	return ;
}

//EPG�擾���e�̐ݒ�
//�����F
// settings		[IN]�ݒ�l
void CEpgDataCap3Main::SetEpgGetSettings(WORD settings)
{
	if( Lock() == FALSE ) return;
	this->epgDBUtilClass.SetEpgGetSettings(settings);
	UnLock();
	return;
}

//EPG�f�[�^�̒~�Ϗ�Ԃ��擾����
//�߂�l�F
// �X�e�[�^�X
//�����F
// l_eitFlag		[IN]L-EIT�̃X�e�[�^�X���擾
EPG_SECTION_STATUS CEpgDataCap3Main::GetSectionStatus(BOOL l_eitFlag)
{
	if( Lock() == FALSE ) return EpgNoData;
	EPG_SECTION_STATUS status = this->epgDBUtilClass.GetSectionStatus(l_eitFlag);
	UnLock();
	return status;
}

//PC���v�����Ƃ����X�g���[�����ԂƂ̍����擾����
//�߂�l�F
// ���̕b��
int CEpgDataCap3Main::GetTimeDelay(
	)
{
	if( Lock() == FALSE ) return 0;
	int delay = this->decodeUtilClass.GetTimeDelay();
	UnLock();
	return delay;
}
