#ifndef __EPG_DATA_CAP3_H__
#define __EPG_DATA_CAP3_H__

#include "../../Common/EpgDataCap3Def.h"

//DLL�̏�����
//�߂�l�F
// �G���[�R�[�h
//�����F
// asyncMode		[IN]TRUE:�񓯊����[�h�AFALSE:�������[�h
// id				[OUT]����ID
__declspec(dllexport)
DWORD WINAPI InitializeEP(
	BOOL asyncFlag,
	DWORD* id
	);

//DLL�̊J��
//�߂�l�F
// �G���[�R�[�h
//�����F
// id		[IN]����ID
__declspec(dllexport)
DWORD WINAPI UnInitializeEP(
	DWORD id
	);

//��͑Ώۂ�TS�p�P�b�g�P��ǂݍ��܂���
//�߂�l�F
// �G���[�R�[�h
//�����F
// id		[IN]����ID
// data		[IN]TS�p�P�b�g�P��
// size		[IN]data�̃T�C�Y�i188�A192������ɂȂ�͂��j
__declspec(dllexport)
DWORD WINAPI AddTSPacketEP(
	DWORD id,
	BYTE* data,
	DWORD size
	);

//��̓f�[�^�̌��݂̃X�g���[���h�c���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// id						[IN]����ID
// originalNetworkID		[OUT]���݂�originalNetworkID
// transportStreamID		[OUT]���݂�transportStreamID
__declspec(dllexport)
DWORD WINAPI GetTSIDEP(
	DWORD id,
	WORD* originalNetworkID,
	WORD* transportStreamID
	);

//���X�g���[���̃T�[�r�X�ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// id						[IN]����ID
// serviceListSize			[OUT]serviceList�̌�
// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
__declspec(dllexport)
DWORD WINAPI GetServiceListActualEP(
	DWORD id,
	DWORD* serviceListSize,
	SERVICE_INFO** serviceList
	);

//�X�g���[���̕ύX��ʒm����
//�����F
// id						[IN]����ID
__declspec(dllexport)
void WINAPI SetStreamChangeEventEP(
	DWORD id
	);

//�~�ς��ꂽEPG���̂���T�[�r�X�ꗗ���擾����
//SERVICE_EXT_INFO�̏��͂Ȃ��ꍇ������
//�߂�l�F
// �G���[�R�[�h
//�����F
// id						[IN]����ID
// serviceListSize			[OUT]serviceList�̌�
// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
__declspec(dllexport)
DWORD WINAPI GetServiceListEpgDBEP(
	DWORD id,
	DWORD* serviceListSize,
	SERVICE_INFO** serviceList
	);

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
__declspec(dllexport)
DWORD WINAPI GetEpgInfoListEP(
	DWORD id,
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	DWORD* epgInfoListSize,
	EPG_EVENT_INFO** epgInfoList
	);

//�w��T�[�r�X�̌���or����EPG�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
// serviceID				[IN]�擾�Ώۂ�ServiceID
// nextFlag					[IN]TRUE�i���̔ԑg�j�AFALSE�i���݂̔ԑg�j
// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
__declspec(dllexport)
DWORD WINAPI GetEpgInfoEP(
	DWORD id,
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	BOOL nextFlag,
	EPG_EVENT_INFO** epgInfo
	);

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
__declspec(dllexport)
DWORD WINAPI SearchEpgInfoEP(
	DWORD id,
	WORD originalNetworkID,
	WORD transportStreamID,
	WORD serviceID,
	WORD eventID,
	BYTE pfOnlyFlag,
	EPG_EVENT_INFO** epgInfo
	);

//EPG�f�[�^�̒~�Ϗ�Ԃ����Z�b�g����
//�����F
// id						[IN]����ID
__declspec(dllexport)
void WINAPI ClearSectionStatusEP(
	DWORD id
	);

//EPG�擾���e�̐ݒ�
//�����F
// id						[IN]����ID
// settings					[IN]�ݒ�l
__declspec(dllexport)
void WINAPI SetEpgGetSettingsEP(
	WORD settings
	);

//EPG�f�[�^�̒~�Ϗ�Ԃ��擾����
//�߂�l�F
// �X�e�[�^�X
//�����F
// id						[IN]����ID
// l_eitFlag				[IN]L-EIT�̃X�e�[�^�X���擾
__declspec(dllexport)
EPG_SECTION_STATUS WINAPI GetSectionStatusEP(
	DWORD id,
	BOOL l_eitFlag
	);

//PC���v�����Ƃ����X�g���[�����ԂƂ̍����擾����
//�߂�l�F
// ���̕b��
//�����F
// id						[IN]����ID
__declspec(dllexport)
int WINAPI GetTimeDelayEP(
	DWORD id
	);

#endif
