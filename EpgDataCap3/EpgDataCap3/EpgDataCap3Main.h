#pragma once

#include "DecodeUtil.h"
#include "EpgDBUtil.h"
#include "../../Common/ErrDef.h"

class CEpgDataCap3Main
{
public:
	CEpgDataCap3Main(void);
	~CEpgDataCap3Main(void);

	//DLL�̏�����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// asyncMode		[IN]TRUE:�񓯊����[�h�AFALSE:�������[�h
	DWORD Initialize(
		BOOL asyncFlag
		);

	//DLL�̊J��
	//�߂�l�F
	// �G���[�R�[�h
	DWORD UnInitialize(
		);

	//��͑Ώۂ�TS�p�P�b�g�P��ǂݍ��܂���
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// data		[IN]TS�p�P�b�g�P��
	// size		[IN]data�̃T�C�Y�i188�A192������ɂȂ�͂��j
	DWORD AddTSPacket(
		BYTE* data,
		DWORD size
		);

	//��̓f�[�^�̌��݂̃X�g���[���h�c���擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// originalNetworkID		[OUT]���݂�originalNetworkID
	// transportStreamID		[OUT]���݂�transportStreamID
	DWORD GetTSID(
		WORD* originalNetworkID,
		WORD* transportStreamID
		);

	//���X�g���[���̃T�[�r�X�ꗗ���擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// serviceListSize			[OUT]serviceList�̌�
	// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
	DWORD GetServiceListActual(
		DWORD* serviceListSize,
		SERVICE_INFO** serviceList
		);

	//�X�g���[���̕ύX��ʒm����
	void SetStreamChangeEvent();

	//�~�ς��ꂽEPG���̂���T�[�r�X�ꗗ���擾����
	//SERVICE_EXT_INFO�̏��͂Ȃ��ꍇ������
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// serviceListSize			[OUT]serviceList�̌�
	// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
	DWORD GetServiceListEpgDB(
		DWORD* serviceListSize,
		SERVICE_INFO** serviceList
		);

	//�w��T�[�r�X�̑SEPG�����擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
	// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
	// serviceID				[IN]�擾�Ώۂ�ServiceID
	// epgInfoListSize			[OUT]epgInfoList�̌�
	// epgInfoList				[OUT]EPG���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
	DWORD GetEpgInfoList(
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
	DWORD GetEpgInfo(
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
	// originalNetworkID		[IN]�擾�Ώۂ�originalNetworkID
	// transportStreamID		[IN]�擾�Ώۂ�transportStreamID
	// serviceID				[IN]�擾�Ώۂ�ServiceID
	// EventID					[IN]�擾�Ώۂ�EventID
	// pfOnlyFlag				[IN]p/f����̂݌������邩�ǂ���
	// epgInfo					[OUT]EPG���iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
	DWORD SearchEpgInfo(
		WORD originalNetworkID,
		WORD transportStreamID,
		WORD serviceID,
		WORD eventID,
		BYTE pfOnlyFlag,
		EPG_EVENT_INFO** epgInfo
		);

	//EPG�f�[�^�̒~�Ϗ�Ԃ����Z�b�g����
	void ClearSectionStatus();

	//EPG�擾���e�̐ݒ�
	//�����F
	// settings		[IN]�ݒ�l
	void SetEpgGetSettings(
		WORD settings
		);

	//EPG�f�[�^�̒~�Ϗ�Ԃ��擾����
	//�߂�l�F
	// �X�e�[�^�X
	//�����F
	// l_eitFlag		[IN]L-EIT�̃X�e�[�^�X���擾
	EPG_SECTION_STATUS GetSectionStatus(
		BOOL l_eitFlag
		);

	//PC���v�����Ƃ����X�g���[�����ԂƂ̍����擾����
	//�߂�l�F
	// ���̕b��
	int GetTimeDelay(
		);

protected:
	HANDLE lockEvent;

	CDecodeUtil decodeUtilClass;
	CEpgDBUtil epgDBUtilClass;

protected:
	//PublicAPI�r������p
	BOOL Lock(LPCWSTR log = NULL, DWORD timeOut = 60*1000);
	void UnLock(LPCWSTR log = NULL);
};
