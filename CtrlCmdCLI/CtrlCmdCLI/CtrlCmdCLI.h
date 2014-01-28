// CtrlCmdCLI.h

#pragma once

#include "CtrlCmdCLIDef.h"
#include "../../Common/SendCtrlCmd.h"
#include "../../Common/twitterUtil.h"

using namespace System;
using namespace System::Collections::Generic;

namespace CtrlCmdCLI {
	public ref class CtrlCmdUtil
	{
		// TODO: ���̃N���X�́A���[�U�[�̃��\�b�h�������ɒǉ����Ă��������B
	public:
		CtrlCmdUtil(void);
		~CtrlCmdUtil(void);
		!CtrlCmdUtil(void);

		void GetContentKindList(
			Dictionary<UInt16, Def::ContentKind^>^% list
			);
		void GetComponentKindList(
			Dictionary<UInt16, Def::ComponentKind^>^% list
			);

		/// <summary>
		/// �R�}���h���M���@�̐ݒ�
		/// </summary>
        /// <param name="tcpFlag">[IN]true�FTCP/IP���[�h�Afalse�F���O�t���p�C�v���[�h</param>
		void SetSendMode(
			bool tcpFlag
			);

		/// <summary>
		/// ���O�t���p�C�v���[�h���̐ڑ����ݒ�
		/// </summary>
        /// <param name="eventName">[IN]�r������pEvent�̖��O</param>
        /// <param name="pipeName">[IN]�ڑ��p�C�v�̖��O</param>
		void SetPipeSetting(
			String^ eventName,
			String^ pipeName
			);

		/// <summary>
		/// TCP/IP���[�h���̐ڑ����ݒ�
		/// </summary>
        /// <param name="ip">[IN]�ڑ���IP</param>
        /// <param name="port">[IN]�ڑ���|�[�g</param>
		void SetNWSetting(
			String^ ip,
			UInt32 port
			);

		/// <summary>
		/// �ڑ��������̃^�C���A�E�g�ݒ�
		/// </summary>
        /// <param name="timeOut">[IN]�^�C���A�E�g�l�i�P�ʁFms�j</param>
		void SetConnectTimeOut(
			UInt32 timeOut
			);

		/// <summary>
		/// Program.txt��ǉ��ōēǂݍ��݂���
		/// </summary>
		UInt32 SendAddloadReserve();

		/// <summary>
		/// EPG�f�[�^���ēǂݍ��݂���
		/// </summary>
		UInt32 SendReloadEpg();

		/// <summary>
		/// �ݒ�����ēǂݍ��݂���
		/// </summary>
		UInt32 SendReloadSetting();

		/// <summary>
		/// EpgTimerSrv.exe���I������
		/// </summary>
		UInt32 SendClose();

		/// <summary>
		/// EpgTimerSrv.exe�̃p�C�v�ڑ�GUI�Ƃ��ăv���Z�X��o�^����
		/// </summary>
        /// <param name="processID">[IN]�v���Z�XID</param>
		UInt32 SendRegistGUI(
			UInt32 processID
			);

		/// <summary>
		/// EpgTimerSrv.exe�̃p�C�v�ڑ�GUI�o�^����������
		/// </summary>
        /// <param name="processID">[IN]�v���Z�XID</param>
		UInt32 SendUnRegistGUI(
			UInt32 processID
			);
		
		/// <summary>
		/// EpgTimerSrv.exe��TCP�ڑ�GUI�Ƃ��ăv���Z�X��o�^����
		/// </summary>
        /// <param name="port">[IN]�|�[�g</param>
		UInt32 SendRegistTCP(
			UInt32 port
			);

		/// <summary>
		/// EpgTimerSrv.exe��TCP�ڑ�GUI�o�^����������
		/// </summary>
        /// <param name="port">[IN]�|�[�g</param>
		UInt32 SendUnRegistTCP(
			UInt32 port
			);

		/// <summary>
		/// �\��ꗗ���擾����
		/// </summary>
        /// <param name="val">[OUT]�\��ꗗ</param>
		UInt32 SendEnumReserve(
			List<Def::ReserveData^>^% val
			);

		/// <summary>
		/// �\������擾����
		/// </summary>
        /// <param name="reserveID">[IN]�擾������̗\��ID</param>
        /// <param name="val">[OUT]�\����</param>
		UInt32 SendGetReserve(
			UInt32 reserveID,
			Def::ReserveData^% val
			);

		/// <summary>
		/// �\���ǉ�����
		/// </summary>
        /// <param name="val">[IN]�ǉ�����\��ꗗ</param>
		UInt32 SendAddReserve(
			List<Def::ReserveData^>^ val
			);

		/// <summary>
		/// �\����폜����
		/// </summary>
        /// <param name="val">[IN]�폜����\��ID�ꗗ</param>
		UInt32 SendDelReserve(
			List<UInt32>^ val
			);

		/// <summary>
		/// �\���ύX����
		/// </summary>
        /// <param name="val">[IN]�ύX����\��ꗗ</param>
		UInt32 SendChgReserve(
			List<Def::ReserveData^>^ val
			);

		/// <summary>
		/// �`���[�i�[���Ƃ̗\��ꗗ���擾����
		/// </summary>
        /// <param name="val">[OUT]�\����</param>
		UInt32 SendEnumTunerReserve(
			List<Def::TunerReserveInfo^>^% val
			);

		/// <summary>
		/// �^��ςݏ��ꗗ���擾����
		/// </summary>
        /// <param name="val">[OUT]�^��ςݏ��ꗗ</param>
		UInt32 SendEnumRecInfo(
			List<Def::RecFileInfo^>^% val
			);

		/// <summary>
		/// �^��ςݏ����폜����
		/// </summary>
        /// <param name="val">[IN]�폜����ID�ꗗ</param>
		UInt32 SendDelRecInfo(
			List<UInt32>^ val
			);

		/// <summary>
		/// �^��ςݏ��̃v���e�N�g��ύX����
		/// </summary>
        /// <param name="val">[IN]�^��ςݏ��ꗗ</param>
		UInt32 SendChgProtectRecInfo(
			List<Def::RecFileInfo^>^ val
			);

		/// <summary>
		/// �T�[�r�X�ꗗ���擾����
		/// </summary>
        /// <param name="val">[OUT]�T�[�r�X�ꗗ</param>
		UInt32 SendEnumService(
			List<Def::EpgServiceInfo^>^% val
			);

		/// <summary>
		/// �T�[�r�X�w��Ŕԑg�����ꗗ���擾����
		/// </summary>
        /// <param name="service">[IN]ONID����32 | TSID����16 | SID�Ƃ����T�[�r�XID</param>
        /// <param name="val">[OUT]�ԑg���ꗗ</param>
		UInt32 SendEnumPgInfo(
			UInt64 service,
			List<Def::EpgEventInfo^>^% val
			);

		/// <summary>
		/// �w��C�x���g�̔ԑg�����擾����
		/// </summary>
        /// <param name="pgID">[IN]ONID����48 | TSID����32 | SID����16 | EventID�Ƃ���ID</param>
        /// <param name="val">[OUT]�ԑg���</param>
		UInt32 SendGetPgInfo(
			UInt64 pgID,
			Def::EpgEventInfo^% val
			);

		/// <summary>
		/// �w��C�x���g�̔ԑg�����擾����
		/// </summary>
        /// <param name="key">[IN]�����L�[�i�����w�莞�͂܂Ƃ߂Č������ʂ��Ԃ�j</param>
        /// <param name="val">[OUT]�ԑg���ꗗ</param>
		UInt32 SendSearchPg(
			List<Def::EpgSearchKeyInfo^>^ key,
			List<Def::EpgEventInfo^>^% val
			);

		/// <summary>
		/// �X�^���o�C�A�x�~�A�V���b�g�_�E�����s���Ă������̊m�F
		/// </summary>
		UInt32 SendChkSuspend(
			);

		/// <summary>
		/// �X�^���o�C�A�x�~�A�V���b�g�_�E���Ɉڍs����
		/// </summary>
        /// <param name="val">[IN]1:�X�^���o�C 2:�x�~ 3:�V���b�g�_�E�� | 0x0100:���A��ċN��</param>
		UInt32 SendSuspend(
			UInt16 val
			);

		/// <summary>
		/// PC�ċN�����s��
		/// </summary>
		UInt32 SendReboot(
			);

		/// <summary>
		/// 10�b���EPG�f�[�^�̎擾���s��
		/// </summary>
		UInt32 SendEpgCapNow(
			);

		/// <summary>
		/// �����\��o�^�����ꗗ���擾����
		/// </summary>
        /// <param name="val">[OUT]�����ꗗ</param>
		UInt32 SendEnumEpgAutoAdd(
			List<Def::EpgAutoAddData^>^% val
			);

		/// <summary>
		/// �����\��o�^������ǉ�����
		/// </summary>
        /// <param name="val">[IN]�����ꗗ</param>
		UInt32 SendAddEpgAutoAdd(
			List<Def::EpgAutoAddData^>^ val
			);

		/// <summary>
		/// �����\��o�^�������폜����
		/// </summary>
        /// <param name="val">[IN]ID�ꗗ</param>
		UInt32 SendDelEpgAutoAdd(
			List<UInt32>^ val
			);

		/// <summary>
		/// �����\��o�^������ύX����
		/// </summary>
        /// <param name="val">[IN]�����ꗗ</param>
		UInt32 SendChgEpgAutoAdd(
			List<Def::EpgAutoAddData^>^ val
			);

		/// <summary>
		/// �����\��o�^�����ꗗ���擾����
		/// </summary>
        /// <param name="val">[OUT]�����ꗗ</param>
		UInt32 SendEnumManualAdd(
			List<Def::ManualAutoAddData^>^% val
			);

		/// <summary>
		/// �����\��o�^������ǉ�����
		/// </summary>
        /// <param name="val">[IN]�����ꗗ</param>
		UInt32 SendAddManualAdd(
			List<Def::ManualAutoAddData^>^ val
			);

		/// <summary>
		/// �����\��o�^�������폜����
		/// </summary>
        /// <param name="val">[IN]ID�ꗗ</param>
		UInt32 SendDelManualAdd(
			List<UInt32>^ val
			);

		/// <summary>
		/// �����\��o�^������ύX����
		/// </summary>
        /// <param name="val">[IN]�����ꗗ</param>
		UInt32 SendChgManualAdd(
			List<Def::ManualAutoAddData^>^ val
			);

		/// <summary>
		/// �w��t�@�C����]������
		/// </summary>
        /// <param name="val">[IN]�t�@�C����</param>
        /// <param name="resVal">[OUT]�o�C�i��</param>
		UInt32 SendFileCopy(
			String^ val,
			[Runtime::InteropServices::Out]array<byte>^% resVal
			);

		/// <summary>
		/// �ԑg���ꗗ���擾����
		/// </summary>
        /// <param name="val">[OUT]�ԑg���ꗗ</param>
		UInt32 SendEnumPgAll(
			List<Def::EpgServiceEventInfo^>^% val
			);

		/// <summary>
		/// PlugIn�t�@�C���̈ꗗ���擾����
		/// </summary>
        /// <param name="val">[IN]1:ReName�A2:Write</param>
        /// <param name="resVal">[OUT]�t�@�C���ꗗ</param>
		UInt32 SendEnumPlugIn(
			UInt16 val,
			List<String^>^% resVal
			);

		/// <summary>
		/// TVTest�̃`�����l���؂�ւ��p�̏����擾����
		/// </summary>
        /// <param name="val">[IN]ONID����32 | TSID����16 | SID�Ƃ����T�[�r�XID</param>
        /// <param name="resVal">[OUT]�`�����l�����</param>
		UInt32 SendGetChgChTVTest(
			UInt64 val,
			Def::TvTestChChgInfo^% resVal
			);

		/// <summary>
		/// �l�b�g���[�N���[�h��EpgDataCap_Bon�̃`�����l����؂�ւ�
		/// </summary>
        /// <param name="val">[IN]�`�����l�����</param>
		UInt32 SendNwTVSetCh(
			Def::SetChInfo^ val
			);

		/// <summary>
		/// �l�b�g���[�N���[�h�ŋN������EpgDataCap_Bon���I��
		/// </summary>
		UInt32 SendNwTVClose(
			);

		/// <summary>
		/// �l�b�g���[�N���[�h�ŋN������Ƃ��̃��[�h
		/// </summary>
        /// <param name="val">[IN]���[�h�i1:UDP 2:TCP 3:UDP+TCP�j</param>
		UInt32 SendNwTVMode(
			UInt32 val
			);

		/// <summary>
		/// �X�g���[���z�M�p�t�@�C�����J��
		/// </summary>
        /// <param name="val">[IN]�J���t�@�C���̃T�[�o�[���t�@�C���p�X</param>
        /// <param name="resVal">[OUT]����pCtrlID</param>
		UInt32 SendNwPlayOpen(
			String^ val,
			UInt32% resVal
			);

		/// <summary>
		/// �X�g���[���z�M�p�t�@�C�����^�C���V�t�g���[�h�ŊJ��
		/// </summary>
        /// <param name="val">[IN]�\��ID</param>
        /// <param name="resVal">[OUT]����pCtrlID</param>
		UInt32 SendNwTimeShiftOpen(
			UInt32 val,
			Def::NWPlayTimeShiftInfo^% resVal
			);

		/// <summary>
		/// �^��t�@�C���̃l�b�g���[�N�p�X���擾
		/// </summary>
        /// <param name="path">[IN]�t�@�C���p�X</param>
        /// <param name="resVal">[OUT]�l�b�g���[�N�p�X</param>
		UInt32 SendGetRecFileNetworkPath(
			String^ path,
			String^% resVal
			);

//View�iTVTest�j�p

		/// <summary>
		/// BonDriver�̐؂�ւ�
		/// </summary>
        /// <param name="val">[IN]BonDriver�t�@�C����</param>
		UInt32 SendViewSetBonDrivere(
			String^ val
			);

		/// <summary>
		/// �g�p����BonDriver�̃t�@�C�������擾
		/// </summary>
        /// <param name="val">[OUT]BonDriver�t�@�C����</param>
		UInt32 SendViewGetBonDrivere(
			String^% val
			);

		/// <summary>
		/// �`�����l���؂�ւ�
		/// </summary>
        /// <param name="val">[IN]�`�����l�����</param>
		UInt32 SendViewSetCh(
			Def::SetChInfo^ val
			);

		/// <summary>
		/// �A�v���P�[�V�����̏I��
		/// </summary>
		UInt32 SendViewAppClose(
			);

		/// <summary>
		/// �X�g���[�~���O�z�M����ID�̐ݒ�
		/// </summary>
        /// <param name="val">[IN]�X�g���[�~���O�z�M������</param>
		UInt32 SendViewSetStreamingInfo(
			Def::TVTestStreamingInfo^ val
			);

	private:
		CSendCtrlCmd* sendCmd;

	private:
		void CopyData(DateTime src, SYSTEMTIME* dest);
		DateTime ConvertTime(SYSTEMTIME* src);
		void CopyData(Def::ReserveData^ src, RESERVE_DATA* dest);
		void CopyData(RESERVE_DATA* src, Def::ReserveData^% dest);
		void CopyData(Def::RecSettingData^ src, REC_SETTING_DATA* dest);
		void CopyData(REC_SETTING_DATA* src, Def::RecSettingData^% dest);

		void CopyData(Def::EpgShortEventInfo^ src, EPGDB_SHORT_EVENT_INFO* dest);
		void CopyData(EPGDB_SHORT_EVENT_INFO* src, Def::EpgShortEventInfo^% dest);
		void CopyData(Def::EpgExtendedEventInfo^ src, EPGDB_EXTENDED_EVENT_INFO* dest);
		void CopyData(EPGDB_EXTENDED_EVENT_INFO* src, Def::EpgExtendedEventInfo^% dest);
		void CopyData(Def::EpgContentData^ src, EPGDB_CONTENT_DATA* dest);
		void CopyData(EPGDB_CONTENT_DATA* src, Def::EpgContentData^% dest);
		void CopyData(Def::EpgContentInfo^ src, EPGDB_CONTEN_INFO* dest);
		void CopyData(EPGDB_CONTEN_INFO* src, Def::EpgContentInfo^% dest);
		void CopyData(Def::EpgComponentInfo^ src, EPGDB_COMPONENT_INFO* dest);
		void CopyData(EPGDB_COMPONENT_INFO* src, Def::EpgComponentInfo^% dest);
		void CopyData(Def::EpgAudioComponentInfo^ src, EPGDB_AUDIO_COMPONENT_INFO* dest);
		void CopyData(EPGDB_AUDIO_COMPONENT_INFO* src, Def::EpgAudioComponentInfo^% dest);
		void CopyData(Def::EpgAudioComponentInfoData^ src, EPGDB_AUDIO_COMPONENT_INFO_DATA* dest);
		void CopyData(EPGDB_AUDIO_COMPONENT_INFO_DATA* src, Def::EpgAudioComponentInfoData^% dest);
		void CopyData(Def::EpgEventData^ src, EPGDB_EVENT_DATA* dest);
		void CopyData(EPGDB_EVENT_DATA* src, Def::EpgEventData^% dest);
		void CopyData(Def::EpgEventGroupInfo^ src, EPGDB_EVENTGROUP_INFO* dest);
		void CopyData(EPGDB_EVENTGROUP_INFO* src, Def::EpgEventGroupInfo^% dest);
		void CopyData(Def::EpgEventInfo^ src, EPGDB_EVENT_INFO* dest);
		void CopyData(EPGDB_EVENT_INFO* src, Def::EpgEventInfo^% dest);

		void CopyData(Def::EpgServiceInfo^ src, EPGDB_SERVICE_INFO* dest);
		void CopyData(EPGDB_SERVICE_INFO* src, Def::EpgServiceInfo^% dest);
		void CopyData(Def::EpgSearchDateInfo^ src, EPGDB_SEARCH_DATE_INFO* dest);
		void CopyData(EPGDB_SEARCH_DATE_INFO* src, Def::EpgSearchDateInfo^% dest);
		void CopyData(Def::EpgSearchKeyInfo^ src, EPGDB_SEARCH_KEY_INFO* dest);
		void CopyData(EPGDB_SEARCH_KEY_INFO* src, Def::EpgSearchKeyInfo^% dest);

		void CopyData(Def::RecFileInfo^ src, REC_FILE_INFO* dest);
		void CopyData(REC_FILE_INFO* src, Def::RecFileInfo^% dest);

		void CopyData(Def::EpgAutoAddData^ src, EPG_AUTO_ADD_DATA* dest);
		void CopyData(EPG_AUTO_ADD_DATA* src, Def::EpgAutoAddData^% dest);

		void CopyData(Def::ManualAutoAddData^ src, MANUAL_AUTO_ADD_DATA* dest);
		void CopyData(MANUAL_AUTO_ADD_DATA* src, Def::ManualAutoAddData^% dest);

		void CopyData(Def::TunerReserveInfo^ src, TUNER_RESERVE_INFO* dest);
		void CopyData(TUNER_RESERVE_INFO* src, Def::TunerReserveInfo^% dest);

		void CopyData(Def::EpgServiceEventInfo^ src, EPGDB_SERVICE_EVENT_INFO* dest);
		void CopyData(EPGDB_SERVICE_EVENT_INFO* src, Def::EpgServiceEventInfo^% dest);

		void CopyData(Def::SetChInfo^ src, SET_CH_INFO* dest);
		void CopyData(SET_CH_INFO* src, Def::SetChInfo^% dest);

		void CopyData(Def::TvTestChChgInfo^ src, TVTEST_CH_CHG_INFO* dest);
		void CopyData(TVTEST_CH_CHG_INFO* src, Def::TvTestChChgInfo^% dest);

		void CopyData(Def::TVTestStreamingInfo^ src, TVTEST_STREAMING_INFO* dest);
		void CopyData(TVTEST_STREAMING_INFO* src, Def::TVTestStreamingInfo^% dest);

		void CopyData(Def::NWPlayTimeShiftInfo^ src, NWPLAY_TIMESHIFT_INFO* dest);
		void CopyData(NWPLAY_TIMESHIFT_INFO* src, Def::NWPlayTimeShiftInfo^% dest);

	};

	public ref class TwitterPlugInUtil
	{
		// TODO: ���̃N���X�́A���[�U�[�̃��\�b�h�������ɒǉ����Ă��������B
	public:
		TwitterPlugInUtil(void);
		~TwitterPlugInUtil(void);
		!TwitterPlugInUtil(void);

		UInt32 Initialize();
		UInt32 UnInitialize();

		bool GetIEProxyConfig(
			Def::CurrentUserIEProxyConfig^% proxyConfig
			);

		bool GetProxyAutoDetect(
			Def::ProxyConfig^% proxyConfig
			);

		bool GetProxyAutoScript(
			String^ scriptURL,
			Def::ProxyConfig^% proxyConfig
			);

		void SetProxy(
			bool useProxy,
			Def::UseProxyInfo^ proxyInfo
			);

		bool GetAuthorizationUrl(
			String^% url
			);

		bool SetAuthorizationPWD(
			String^ password
			);

		bool SendTweet(
			bool asyncMode,
			String^ text
			);

	private:
		CTwitterUtil* util;

	private:
		void CopyData(Def::CurrentUserIEProxyConfig^ src, CURRENT_USER_IE_PROXY_CONFIG* dest);
		void CopyData(CURRENT_USER_IE_PROXY_CONFIG* src, Def::CurrentUserIEProxyConfig^% dest);
		void CopyData(Def::ProxyConfig^ src, PROXY_CONFIG* dest);
		void CopyData(PROXY_CONFIG* src, Def::ProxyConfig^% dest);
		void CopyData(Def::UseProxyInfo^ src, USE_PROXY_INFO* dest);
		void CopyData(USE_PROXY_INFO* src, Def::UseProxyInfo^% dest);
	};
}
