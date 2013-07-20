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

	//�X�g���[�����̌��݂̎��ԏ����擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// time				[OUT]�X�g���[�����̌��݂̎���
	DWORD GetNowTime(
		SYSTEMTIME* time
		);

	//PC���v�����Ƃ����X�g���[�����ԂƂ̍����擾����
	//�߂�l�F
	// ���̕b��
	int GetTimeDelay(
		);

	//�X�g���[���̕ύX��ʒm����
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

	CEpgDBUtil* epgDBUtil;

	//PID���̃o�b�t�@�����O
	//�L�[ PID
	map<WORD, CTSBuffUtil*> buffUtilMap;

	CPATTable* patInfo;
	CCATTable* catInfo;
	map<WORD, CPMTTable*> pmtMap;
	NIT_SECTION_INFO* nitActualInfo;
	SDT_SECTION_INFO* sdtActualInfo;
	map<DWORD, SDT_SECTION_INFO*> sdtOtherMap;
	CTOTTable* totInfo;
	CTDTTable* tdtInfo;
	CBITTable* bitInfo;
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

	//���X�g���[���̃T�[�r�X�ꗗ��SIT����擾����
	//�߂�l�F
	// �G���[�R�[�h
	//�����F
	// serviceListSize			[OUT]serviceList�̌�
	// serviceList				[OUT]�T�[�r�X���̃��X�g�iDLL���Ŏ����I��delete����B���Ɏ擾���s���܂ŗL���j
	DWORD GetServiceListSIT(
		DWORD* serviceListSize,
		SERVICE_INFO** serviceList
		);

};
