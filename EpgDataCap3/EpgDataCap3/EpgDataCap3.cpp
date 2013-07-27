// EpgDataCap3.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"

#include "EpgDataCap3Main.h"
#include "../../Common/ErrDef.h"

map<DWORD, CEpgDataCap3Main*> g_List;
DWORD g_nextID = 1;

DWORD GetNextID()
{
	DWORD nextID = 0xFFFFFFFF;

	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(g_nextID);
	if( itr == g_List.end() ){
		nextID = g_nextID;
		g_nextID++;
		if( g_nextID == 0 || g_nextID == 0xFFFFFFFF){
			g_nextID = 1;
		}
	}else{
		for( DWORD i=1; i<0xFFFFFFFF; i++ ){
			itr = g_List.find(g_nextID);
			if( itr == g_List.end() ){
				nextID = g_nextID;
				g_nextID++;
				if( g_nextID == 0 || g_nextID == 0xFFFFFFFF){
					g_nextID = 1;
				}
				break;
			}else{
				g_nextID++;
			}
			if( g_nextID == 0 || g_nextID == 0xFFFFFFFF){
				g_nextID = 1;
			}
		}
	}

	return nextID;
}

//DLL�̏�����
//�߂�l�F
// �G���[�R�[�h
//�����F
// asyncMode		[IN]TRUE:�񓯊����[�h�AFALSE:�������[�h
// id				[OUT]����ID
DWORD WINAPI InitializeEP(
	BOOL asyncFlag,
	DWORD* id
	)
{
	if( id == NULL ){
		return ERR_INVALID_ARG;
	}

	CEpgDataCap3Main* main = new CEpgDataCap3Main;
	DWORD err = main->Initialize(asyncFlag);
	if( err == NO_ERR ){
		*id = GetNextID();
		g_List.insert(pair<DWORD, CEpgDataCap3Main*>(*id, main));
	}
	return err;
}

//DLL�̊J��
//�߂�l�F
// �G���[�R�[�h
//�����F
// id		[IN]����ID InitializeEP�̖߂�l
DWORD WINAPI UnInitializeEP(
	DWORD id
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return ERR_NOT_INIT;
	}

	DWORD err = itr->second->UnInitialize();

	SAFE_DELETE(itr->second);

	g_List.erase(itr);

	return err;
}

//��͑Ώۂ�TS�p�P�b�g�P��ǂݍ��܂���
//�߂�l�F
// �G���[�R�[�h
// id		[IN]����ID InitializeEP�̖߂�l
// data		[IN]TS�p�P�b�g�P��
// size		[IN]data�̃T�C�Y�i188�A192������ɂȂ�͂��j
DWORD WINAPI AddTSPacketEP(
	DWORD id,
	BYTE* data,
	DWORD size
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return ERR_NOT_INIT;
	}

	return itr->second->AddTSPacket(data, size);
}

//��̓f�[�^�̌��݂̃X�g���[���h�c���擾����
//�߂�l�F
// �G���[�R�[�h
// id						[IN]����ID
// originalNetworkID		[OUT]���݂�originalNetworkID
// transportStreamID		[OUT]���݂�transportStreamID
DWORD WINAPI GetTSIDEP(
	DWORD id,
	WORD* originalNetworkID,
	WORD* transportStreamID
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return ERR_NOT_INIT;
	}

	return itr->second->GetTSID(originalNetworkID, transportStreamID);
}

//���X�g���[���̃T�[�r�X�ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// id						[IN]����ID
// serviceListSize			[OUT]serviceList�̌�
// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD WINAPI GetServiceListActualEP(
	DWORD id,
	DWORD* serviceListSize,
	SERVICE_INFO** serviceList
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return ERR_NOT_INIT;
	}

	return itr->second->GetServiceListActual(serviceListSize, serviceList);
}

//�X�g���[���̕ύX��ʒm����
//�����F
// id						[IN]����ID
void WINAPI SetStreamChangeEventEP(
	DWORD id
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return;
	}

	itr->second->SetStreamChangeEvent();
}

//�~�ς��ꂽEPG���̂���T�[�r�X�ꗗ���擾����
//SERVICE_EXT_INFO�̏��͂Ȃ��ꍇ������
//�߂�l�F
// �G���[�R�[�h
//�����F
// id						[IN]����ID
// serviceListSize			[OUT]serviceList�̌�
// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD WINAPI GetServiceListEpgDBEP(
	DWORD id,
	DWORD* serviceListSize,
	SERVICE_INFO** serviceList
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return ERR_NOT_INIT;
	}

	return itr->second->GetServiceListEpgDB(serviceListSize, serviceList);
}

//�w��T�[�r�X�̑SEPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// id						[IN]����ID
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// epgInfoListSize			[OUT]epgInfoList�̌�
// epgInfoList				[OUT]EPG���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD WINAPI GetEpgInfoListEP(
	DWORD id,
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	DWORD* epgInfoListSize,
	EPG_EVENT_INFO** epgInfoList
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return ERR_NOT_INIT;
	}

	return itr->second->GetEpgInfoList(originalNetworkID, transportStreamID, serviceID, epgInfoListSize, epgInfoList);
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
DWORD WINAPI GetEpgInfoEP(
	DWORD id,
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	BOOL nextFlag,
	EPG_EVENT_INFO** epgInfo
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return ERR_NOT_INIT;
	}

	return itr->second->GetEpgInfo(originalNetworkID, transportStreamID, serviceID, nextFlag, epgInfo);
}

//�w��C�x���g��EPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// id						[IN]����ID
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// eventID					[IN]�擾�Ώۂ�EventID
// pfOnlyFlag				[IN]p/f����̂݌������邩�ǂ���
// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
DWORD WINAPI SearchEpgInfoEP(
	DWORD id,
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	WORD eventID,
	BYTE pfOnlyFlag,
	EPG_EVENT_INFO** epgInfo
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return ERR_NOT_INIT;
	}

	return itr->second->SearchEpgInfo(originalNetworkID, transportStreamID, serviceID, eventID, pfOnlyFlag, epgInfo);
}

//EPG�f�[�^�̒~�Ϗ�Ԃ����Z�b�g����
//�����F
// id						[IN]����ID
void WINAPI ClearSectionStatusEP(
	DWORD id
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return ;
	}

	itr->second->ClearSectionStatus();
}

//EPG�擾���e�̐ݒ�
//�����F
// settings		[IN]�ݒ�l
void WINAPI SetEpgGetSettingsEP(
	DWORD id,
	WORD settings
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return;
	}

	itr->second->SetEpgGetSettings(settings);
}

//EPG�f�[�^�̒~�Ϗ�Ԃ��擾����
//�߂�l�F
// �X�e�[�^�X
//�����F
// id						[IN]����ID
// l_eitFlag				[IN]L-EIT�̃X�e�[�^�X���擾
EPG_SECTION_STATUS WINAPI GetSectionStatusEP(
	DWORD id,
	BOOL l_eitFlag
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return EpgNoData;
	}

	return itr->second->GetSectionStatus(l_eitFlag);
}

//PC���v�����Ƃ����X�g���[�����ԂƂ̍����擾����
//�߂�l�F
// ���̕b��
//�����F
// id						[IN]����ID
int WINAPI GetTimeDelayEP(
	DWORD id
	)
{
	map<DWORD, CEpgDataCap3Main*>::iterator itr;
	itr = g_List.find(id);
	if( itr == g_List.end() ){
		return EpgNoData;
	}

	return itr->second->GetTimeDelay();
}