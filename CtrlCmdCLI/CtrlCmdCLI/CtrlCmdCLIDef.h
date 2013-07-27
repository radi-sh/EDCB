#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace CtrlCmdCLI{
namespace Def {
		/// <summary>
	/// �^��ݒ���
	/// </summary>
	public ref class RecFileSetInfo{
	public:
		/// <summary>
		/// �^��t�H���_�p�X
		/// </summary>
		property String^ RecFolder;

		/// <summary>
		/// �o��PlugIn
		/// </summary>
		property String^ WritePlugIn;

		/// <summary>
		/// �t�@�C�����ϊ�PlugIn
		/// </summary>
		property String^ RecNamePlugIn;

		/// <summary>
		/// �t�@�C�����ʑΉ� �^��J�n�������ɓ����Ŏg�p�B�\����Ƃ��Ă͕K�v�Ȃ�
		/// </summary>
		property String^ RecFileName;
	public:
		RecFileSetInfo(void){
			RecFolder = gcnew String("");
			WritePlugIn = gcnew String("");
			RecNamePlugIn = gcnew String("");
			RecFileName = gcnew String("");
		};
	};


	/// <summary>
	/// �^��ݒ���
	/// </summary>
	public ref class RecSettingData{
	public:
		/// <summary>
		/// �^�惂�[�h
		/// </summary>
		property Byte RecMode;
		
		/// <summary>
		/// �D��x
		/// </summary>
		property Byte Priority;

		/// <summary>
		/// �Ǐ]���[�h
		/// </summary>
		property Byte TuijyuuFlag;

		/// <summary>
		/// �w��T�[�r�X���[�h
		/// </summary>
		property UInt32 ServiceMode;

		/// <summary>
		/// �҂�����i�H�j�^�惂�[�h
		/// </summary>
		property Byte PittariFlag;

		/// <summary>
		/// �^�����s�p�b�`�t�@�C���p�X
		/// </summary>
		property String^ BatFilePath;

		/// <summary>
		/// �^��t�H���_�p�X
		/// </summary>
		property List<RecFileSetInfo^>^ RecFolderList;

		/// <summary>
		/// �x�~�^�X�^���o�C���[�h
		/// </summary>
		property Byte SuspendMode;

		/// <summary>
		/// �^���ċN�����邩�ǂ���
		/// </summary>
		property Byte RebootFlag;

		/// <summary>
		/// �}�[�W���ݒ肪�L�����ǂ���
		/// </summary>
		property Byte UseMargineFlag;

		/// <summary>
		/// �J�n�}�[�W��
		/// </summary>
		property Int32 StartMargine;

		/// <summary>
		/// �I���}�[�W��
		/// </summary>
		property Int32 EndMargine;

		/// <summary>
		/// �㑱����T�[�r�X���A����t�@�C���Ř^��
		/// </summary>
		property Byte ContinueRecFlag;

		/// <summary>
		/// ����CH�ɕ�����M�T�[�r�X������ꍇ�A�����^�悷�邩�ǂ���
		/// </summary>
		property Byte PartialRecFlag;

		/// <summary>
		/// �����I�Ɏg�pTuner���Œ�
		/// </summary>
		property UInt32 TunerID;

		/// <summary>
		/// ������M�T�[�r�X�^��̃t�H���_
		/// </summary>
		property List<RecFileSetInfo^>^ PartialRecFolder;

	public:
		RecSettingData(void){
			RecMode = 0;
			Priority = 1;
			TuijyuuFlag = 1;
			ServiceMode  = 0;
			PittariFlag = 0;
			BatFilePath = gcnew String("");
			RecFolderList = gcnew List<RecFileSetInfo^>();
			SuspendMode = 0;
			RebootFlag = 0;
			UseMargineFlag = 0;
			StartMargine = 0;
			EndMargine = 0;
			ContinueRecFlag = 0;
			PartialRecFlag = 0;
			TunerID = 0;
			PartialRecFolder = gcnew List<RecFileSetInfo^>();
		};
	};

	/// <summary>
	/// �\����
	/// </summary>
	public ref class ReserveData{
	public:
		/// <summary>
		/// �ԑg��
		/// </summary>
		property String^ Title;

		/// <summary>
		/// �^��J�n����
		/// </summary>
		property DateTime StartTime;

		/// <summary>
		/// �^�摍����
		/// </summary>
		property UInt32 DurationSecond;

		/// <summary>
		/// �T�[�r�X��
		/// </summary>
		property String^ StationName;

		/// <summary>
		/// OriginalNetworkID
		/// </summary>
		property UInt16 OriginalNetworkID;

		/// <summary>
		/// TransportStreamID
		/// </summary>
		property UInt16 TransportStreamID;

		/// <summary>
		/// ServiceID
		/// </summary>
		property UInt16 ServiceID;

		/// <summary>
		/// EventID
		/// </summary>
		property UInt16 EventID;

		/// <summary>
		/// �R�����g
		/// </summary>
		property String^ Comment;

		/// <summary>
		/// �\��ID
		/// </summary>
		property UInt32 ReserveID;

		/// <summary>
		/// �\��ҋ@�������H
		/// </summary>
		property Byte RecWaitFlag;

		/// <summary>
		/// ���Ԃ��� 1:���Ԃ��ă`���[�i�[����Ȃ��\�񂠂� 2:�`���[�i�[����Ȃ��ė\��ł��Ȃ�
		/// </summary>
		property Byte OverlapMode;

		/// <summary>
		/// �^��t�@�C���p�X
		/// </summary>
		property String^ RecFilePath;

		/// <summary>
		/// �\�񎞂̊J�n����
		/// </summary>
		property DateTime StartTimeEpg;

		/// <summary>
		/// �^��ݒ�
		/// </summary>
		property RecSettingData^ RecSetting;

		/// <summary>
		/// �\��ǉ����
		/// </summary>
		property UInt32 ReserveStatus;

		/// <summary>
		/// �^��\��t�@�C����
		/// </summary>
		property List<String^>^ RecFileNameList;

		/// <summary>
		/// �����p
		/// </summary>
		property UInt32 param1;
	public:
		ReserveData(void){
			Title = gcnew String("");
//			StartTime = gcnew DateTime();
			DurationSecond = 0;
			StationName = gcnew String("");
			OriginalNetworkID = 0xFFFF;
			TransportStreamID = 0xFFFF;
			ServiceID = 0xFFFF;
			EventID = 0xFFFF;
			Comment = gcnew String("");
			ReserveID = 0;
			RecWaitFlag = 0;
			OverlapMode = 0;
			RecFilePath = gcnew String("");
//			StartTimeEpg = gcnew DateTime();
			RecSetting = gcnew RecSettingData();
			ReserveStatus = 0;
			RecFileNameList = gcnew List<String^>();
			param1 = 0;
		};
	};

	/// <summary>
	/// �^��ςݏ��
	/// </summary>
	public ref class RecFileInfo{
	public:
		/// <summary>
		/// ����ID
		/// </summary>
		property UInt32 ID;

		/// <summary>
		/// �^��t�@�C���p�X
		/// </summary>
		property String^ RecFilePath;

		/// <summary>
		/// �ԑg��
		/// </summary>
		property String^ Title;

		/// <summary>
		/// �^��J�n����
		/// </summary>
		property DateTime StartTime;

		/// <summary>
		/// �^�摍����
		/// </summary>
		property UInt32 DurationSecond;

		/// <summary>
		/// �T�[�r�X��
		/// </summary>
		property String^ ServiceName;

		/// <summary>
		/// OriginalNetworkID
		/// </summary>
		property UInt16 OriginalNetworkID;

		/// <summary>
		/// TransportStreamID
		/// </summary>
		property UInt16 TransportStreamID;

		/// <summary>
		/// ServiceID
		/// </summary>
		property UInt16 ServiceID;

		/// <summary>
		/// EventID
		/// </summary>
		property UInt16 EventID;

		/// <summary>
		/// �h���b�v��
		/// </summary>
		property UInt64 Drops;

		/// <summary>
		/// �X�N�����u����
		/// </summary>
		property UInt64 Scrambles;

		/// <summary>
		/// �^�挋�ʂ̃X�e�[�^�X
		/// </summary>
		property UInt32 RecStatus;

		/// <summary>
		/// �\�񎞂̊J�n����
		/// </summary>
		property DateTime StartTimeEpg;

		/// <summary>
		/// �R�����g
		/// </summary>
		property String^ Comment;

		/// <summary>
		/// .program.txt�t�@�C���̓��e
		/// </summary>
		property String^ ProgramInfo;

		/// <summary>
		/// .err�t�@�C���̓��e
		/// </summary>
		property String^ ErrInfo;

		/// <summary>
		/// �v���e�N�g�t���O
		/// </summary>
		property bool ProtectFlag;

	public:
		RecFileInfo(void){
			ID = 0;
			RecFilePath = gcnew String("");
			Title = gcnew String("");
			DurationSecond = 0;
			ServiceName = gcnew String("");
			OriginalNetworkID = 0xFFFF;
			TransportStreamID = 0xFFFF;
			ServiceID = 0xFFFF;
			EventID = 0xFFFF;
			Drops = 0;
			Scrambles = 0;
			RecStatus = 0;
			Comment = gcnew String("");
			ProgramInfo = gcnew String("");
			ErrInfo = gcnew String("");
			ProtectFlag = false;
		};
	};

	/// <summary>
	/// EPG��{���
	/// </summary>
	public ref class EpgShortEventInfo{
	public:
		/// <summary>
		/// �C�x���g��
		/// </summary>
		property String^ event_name;

		/// <summary>
		/// ���
		/// </summary>
		property String^ text_char;
	public:
		EpgShortEventInfo(void){
			event_name = gcnew String("");
			text_char = gcnew String("");
		};
	};

	/// <summary>
	/// EPG�g�����
	/// </summary>
	public ref class EpgExtendedEventInfo{
	public:
		/// <summary>
		/// �ڍ׏��
		/// </summary>
		property String^ text_char;
	public:
		EpgExtendedEventInfo(void){
			text_char = gcnew String("");
		};
	};

	/// <summary>
	/// EPG�W�������f�[�^
	/// </summary>
	public ref class EpgContentData{
	public:
		/// <summary>
		/// �W������1
		/// </summary>
		property Byte content_nibble_level_1;
		/// <summary>
		/// �W������2
		/// </summary>
		property Byte content_nibble_level_2;
		/// <summary>
		/// ���[�U�W������
		/// </summary>
		property Byte user_nibble_1;
		/// <summary>
		/// ���[�U�W������
		/// </summary>
		property Byte user_nibble_2;
	public:
		EpgContentData(void){
			content_nibble_level_1 = 0;
			content_nibble_level_2 = 0;
			user_nibble_1 = 0;
			user_nibble_2 = 0;
		};
	};

	/// <summary>
	/// EPG�W���������
	/// </summary>
	public ref class EpgContentInfo{
	public:
		/// <summary>
		/// </summary>
		property List<EpgContentData^>^ nibbleList;
	public:
		EpgContentInfo(void){
			nibbleList = gcnew List<EpgContentData^>();
		};
	};

	/// <summary>
	/// EPG�f�����
	/// </summary>
	public ref class EpgComponentInfo{
	public:
		/// <summary>
		/// �R���|�[�l���g���e
		/// </summary>
		property Byte stream_content;
		/// <summary>
		/// �R���|�[�l���g���
		/// </summary>
		property Byte component_type;
		/// <summary>
		/// �R���|�[�l���g�^�O
		/// </summary>
		property Byte component_tag;
		/// <summary>
		/// �R���|�[�l���g�L�q
		/// </summary>
		property String^ text_char;
	public:
		EpgComponentInfo(void){
			stream_content = 0;
			component_type = 0;
			component_tag = 0;
			text_char = gcnew String("");
		};
	};

	/// <summary>
	/// EPG�������f�[�^
	/// </summary>
	public ref class EpgAudioComponentInfoData{
	public:
		/// <summary>
		/// �R���|�[�l���g���e
		/// </summary>
		property Byte stream_content;
		/// <summary>
		/// �R���|�[�l���g���
		/// </summary>
		property Byte component_type;
		/// <summary>
		/// �R���|�[�l���g�^�O
		/// </summary>
		property Byte component_tag;
		/// <summary>
		/// �X�g���[���`�����
		/// </summary>
		property Byte stream_type;
		/// <summary>
		/// �T�C�}���L���X�g�O���[�v����
		/// </summary>
		property Byte simulcast_group_tag;
		/// <summary>
		/// ES ������t���O
		/// </summary>
		property Byte ES_multi_lingual_flag;
		/// <summary>
		/// ��R���|�[�l���g�t���O
		/// </summary>
		property Byte main_component_flag;
		/// <summary>
		/// �����\��
		/// </summary>
		property Byte quality_indicator;
		/// <summary>
		/// �T���v�����O���g��
		/// </summary>
		property Byte sampling_rate;
		/// <summary>
		/// �R���|�[�l���g�L�q
		/// </summary>
		property String^ text_char;
	public:
		EpgAudioComponentInfoData(void){
			stream_content = 0;
			component_type = 0;
			component_tag = 0;
			stream_type = 0;
			simulcast_group_tag = 0;
			ES_multi_lingual_flag = 0;
			main_component_flag = 0;
			quality_indicator = 0;
			sampling_rate = 0;
			text_char = gcnew String("");
		};
	};

	/// <summary>
	/// EPG�������
	/// </summary>
	public ref class EpgAudioComponentInfo{
	public:
		/// <summary>
		/// </summary>
		property List<EpgAudioComponentInfoData^>^ componentList;
	public:
		EpgAudioComponentInfo(void){
			componentList = gcnew List<EpgAudioComponentInfoData^>();
		};
	};

	/// <summary>
	/// EPG�C�x���g�f�[�^
	/// </summary>
	public ref class EpgEventData{
	public:
		/// <summary>
		/// OriginalNetworkID
		/// </summary>
		property UInt16 original_network_id;
		/// <summary>
		///	TransportStreamID
		/// </summary>
		property UInt16 transport_stream_id;
		/// <summary>
		/// ServiceID
		/// </summary>
		property UInt16 service_id;
		/// <summary>
		/// EventID
		/// </summary>
		property UInt16 event_id;
	public:
		EpgEventData(void){
			original_network_id = 0xFFFF;
			transport_stream_id = 0xFFFF;
			service_id = 0xFFFF;
			event_id = 0xFFFF;
		};
	};

	/// <summary>
	/// EPG�C�x���g�O���[�v���
	/// </summary>
	public ref class EpgEventGroupInfo{
	public:
		/// <summary>
		/// �O���[�v�^�C�v
		/// </summary>
		property Byte group_type;
		/// <summary>
		/// �C�x���g�f�[�^
		/// </summary>
		property List<EpgEventData^>^ eventDataList;
	public:
		EpgEventGroupInfo(void){
			group_type = 0;
			eventDataList = gcnew List<EpgEventData^>();
		};
	};

	/// <summary>
	/// EPG�C�x���g���
	/// </summary>
	public ref class EpgEventInfo{
	public:
		/// <summary>
		/// OriginalNetworkID
		/// </summary>
		property UInt16 original_network_id;
		/// <summary>
		///	TransportStreamID
		/// </summary>
		property UInt16 transport_stream_id;
		/// <summary>
		/// ServiceID
		/// </summary>
		property UInt16 service_id;
		/// <summary>
		/// EventID
		/// </summary>
		property UInt16 event_id;
		/// <summary>
		/// start_time���L�����ǂ���
		/// </summary>
		property Byte StartTimeFlag;
		/// <summary>
		/// �J�n����
		/// </summary>
		property DateTime start_time;
		/// <summary>
		/// durationSec���L�����ǂ���
		/// </summary>
		property Byte DurationFlag;
		/// <summary>
		/// �����ԁi�P�ʁF�b�j
		/// </summary>
		property UInt32 durationSec;

		/// <summary>
		/// ��{���
		/// </summary>
		property EpgShortEventInfo^ ShortInfo;
		/// <summary>
		/// �g�����
		/// </summary>
		property EpgExtendedEventInfo^ ExtInfo;
		/// <summary>
		/// �W���������
		/// </summary>
		property EpgContentInfo^ ContentInfo;
		/// <summary>
		/// �f�����
		/// </summary>
		property EpgComponentInfo^ ComponentInfo;
		/// <summary>
		/// �������
		/// </summary>
		property EpgAudioComponentInfo^ AudioInfo;
		/// <summary>
		/// �C�x���g�O���[�v���
		/// </summary>
		property EpgEventGroupInfo^ EventGroupInfo;
		/// <summary>
		/// �C�x���g�����[���
		/// </summary>
		property EpgEventGroupInfo^ EventRelayInfo;
		/// <summary>
		/// �m���X�N�����u�����ǂ���
		/// </summary>
		property Byte FreeCAFlag;

	public:
		EpgEventInfo(void){
			original_network_id = 0xFFFF;
			transport_stream_id = 0xFFFF;
			service_id = 0xFFFF;
			event_id = 0xFFFF;
			StartTimeFlag = 0;
//			start_time = gcnew DateTime();
			DurationFlag = 0;
			durationSec = 0;
			ShortInfo = nullptr;
			ExtInfo = nullptr;
			ContentInfo = nullptr;
			ComponentInfo = nullptr;
			AudioInfo = nullptr;
			EventGroupInfo = nullptr;
			EventRelayInfo = nullptr;
			FreeCAFlag = 0;
		};
	};

	/// <summary>
	/// EPG�C�x���g���
	/// </summary>
	public ref class EpgServiceInfo{
	public:
		/// <summary>
		/// OriginalNetworkID
		/// </summary>
		property UInt16 ONID;
		/// <summary>
		///	TransportStreamID
		/// </summary>
		property UInt16 TSID;
		/// <summary>
		/// ServiceID
		/// </summary>
		property UInt16 SID;
		/// <summary>
		/// ServiceType
		/// </summary>
		property BYTE service_type;
		/// <summary>
		/// ������M�t���O
		/// </summary>
		property BYTE partialReceptionFlag;
		/// <summary>
		/// ServiceProviderName
		/// </summary>
		property String^ service_provider_name;
		/// <summary>
		/// ServiceName
		/// </summary>
		property String^ service_name;
		/// <summary>
		/// NetworkName
		/// </summary>
		property String^ network_name;
		/// <summary>
		/// TsName
		/// </summary>
		property String^ ts_name;
		/// <summary>
		/// �����R���L�[ID
		/// </summary>
		property BYTE remote_control_key_id;
		/// <summary>
		/// �_�C���N�g�I�ǔԍ�
		/// </summary>
		property WORD direct_tuning_number;

	public:
		EpgServiceInfo(void){
			ONID = 0xFFFF;
			TSID = 0xFFFF;
			SID = 0xFFFF;
			service_type = 0;
			partialReceptionFlag = 0;
			service_provider_name = gcnew String("");
			service_name = gcnew String("");
			network_name = gcnew String("");
			ts_name = gcnew String("");
			remote_control_key_id = 0;
			direct_tuning_number = 0;
		};
	};

	/// <summary>
	/// �����L�[���
	/// </summary>
	public ref class EpgSearchDateInfo{
	public:
		/// <summary>
		/// �J�n�j��
		/// </summary>
		property Byte startDayOfWeek;
		/// <summary>
		/// �J�n����
		/// </summary>
		property UInt16 startHour;
		/// <summary>
		/// �J�n��
		/// </summary>
		property UInt16 startMin;
		/// <summary>
		/// �I���j��
		/// </summary>
		property Byte endDayOfWeek;
		/// <summary>
		/// �I������
		/// </summary>
		property UInt16 endHour;
		/// <summary>
		/// �I����
		/// </summary>
		property UInt16 endMin;
	public:
		EpgSearchDateInfo(void){
			startDayOfWeek = 0;
			startHour = 0;
			startMin = 0;
			endDayOfWeek = 0;
			endHour = 0;
			endMin = 0;
		};
	};

	/// <summary>
	/// �����L�[���
	/// </summary>
	public ref class EpgSearchKeyInfo{
	public:
		/// <summary>
		/// And�L�[���[�h
		/// </summary>
		property String^ andKey;
		/// <summary>
		/// Not�L�[���[�h
		/// </summary>
		property String^ notKey;
		/// <summary>
		/// �ԑg���̂݌����ΏۂƂ���
		/// </summary>
		property Byte titleOnlyFlag;
		/// <summary>
		/// ���K�\�����[�h�Ō������s��
		/// </summary>
		property Byte regExpFlag;
		/// <summary>
		/// �W�������i�荞��
		/// </summary>
		property List<EpgContentData^>^ contentList;
		/// <summary>
		/// ���ԍi�荞��
		/// </summary>
		property List<EpgSearchDateInfo^>^ dateList;
		/// <summary>
		/// �����Ώۂ̃T�[�r�X�ꗗ
		/// </summary>
		property List<Int64>^ serviceList;
		/// <summary>
		/// �f���i�荞��
		/// </summary>
		property List<UInt16>^ videoList;
		/// <summary>
		/// �����i�荞��
		/// </summary>
		property List<UInt16>^ audioList;
		/// <summary>
		/// �����܂��������s��
		/// </summary>
		property Byte aimaiFlag;
		/// <summary>
		/// �W�������i�荞��NOT����
		/// </summary>
		property Byte notContetFlag;
		/// <summary>
		/// �f���i�荞��NOT����
		/// </summary>
		property Byte notDateFlag;
		/// <summary>
		/// FreeCA���[�h
		/// </summary>
		property Byte freeCAFlag;
		/// <summary>
		/// �^��ς��̃`�F�b�N����
		/// </summary>
		property Byte chkRecEnd;
		/// <summary>
		/// �^��ς��̃`�F�b�N�Ώۊ���
		/// </summary>
		property UInt16 chkRecDay;
	public:
		EpgSearchKeyInfo(void){
			andKey = gcnew String("");
			notKey = gcnew String("");
			titleOnlyFlag = 0;
			regExpFlag = 0;
			contentList = gcnew List<EpgContentData^>();
			dateList = gcnew List<EpgSearchDateInfo^>();
			serviceList = gcnew List<Int64>();
			videoList = gcnew List<UInt16>();
			audioList = gcnew List<UInt16>();
			aimaiFlag = 0;
			notContetFlag = 0;
			notDateFlag = 0;
			freeCAFlag = 0;
			chkRecEnd = 0;
			chkRecDay = 6;
		};
	};

	/// <summary>
	/// �����\��o�^���
	/// </summary>
	public ref class EpgAutoAddData{
	public:
		/// <summary>
		/// ID
		/// </summary>
		property UInt32 dataID;
		/// <summary>
		/// �����L�[
		/// </summary>
		property EpgSearchKeyInfo^ searchInfo;
		/// <summary>
		/// �^��ݒ�
		/// </summary>
		property RecSettingData^ recSetting;
		/// <summary>
		/// �\��o�^��
		/// </summary>
		property UInt32 addCount;
	public:
		EpgAutoAddData(void){
			dataID = 0;
			searchInfo = gcnew EpgSearchKeyInfo();
			recSetting = gcnew RecSettingData();
			addCount = 0;
		};
	};

	/// <summary>
	/// �����\��o�^���
	/// </summary>
	public ref class ManualAutoAddData{
	public:
		/// <summary>
		/// ID
		/// </summary>
		property UInt32 dataID;
		/// <summary>
		/// �Ώۗj��
		/// </summary>
		property Byte dayOfWeekFlag;
		/// <summary>
		/// �^��J�n���ԁi00:00��0�Ƃ��ĕb�P�ʁj
		/// </summary>
		property UInt32 startTime;
		/// <summary>
		/// �^�摍����
		/// </summary>
		property UInt32 durationSecond;
		/// <summary>
		/// �ԑg��
		/// </summary>
		property String^ title;
		/// <summary>
		/// �T�[�r�X��
		/// </summary>
		property String^ stationName;
		/// <summary>
		/// ONID
		/// </summary>
		property UInt16 originalNetworkID;
		/// <summary>
		/// TSID
		/// </summary>
		property UInt16 transportStreamID;
		/// <summary>
		/// SID
		/// </summary>
		property UInt16 serviceID;
		/// <summary>
		/// �^��ݒ�
		/// </summary>
		property RecSettingData^ recSetting;
	public:
		ManualAutoAddData(void){
			dataID = 0;
			dayOfWeekFlag = 0;
			startTime = 0;
			durationSecond = 0;
			title = gcnew String("");
			stationName = gcnew String("");
			originalNetworkID = 0;
			transportStreamID = 0;
			serviceID = 0;

			recSetting = gcnew RecSettingData();
		};
	};

	/// <summary>
	/// �`���[�i�[�\����
	/// </summary>
	public ref class TunerReserveInfo{
	public:
		/// <summary>
		/// ID
		/// </summary>
		property UInt32 tunerID;
		/// <summary>
		/// �����L�[
		/// </summary>
		property String^ tunerName;
		/// <summary>
		/// �^��ݒ�
		/// </summary>
		property List<Int32>^ reserveList;
	public:
		TunerReserveInfo(void){
			tunerID = 0;
			tunerName = gcnew String("");
			reserveList = gcnew List<Int32>();
		};
	};

	/// <summary>
	/// �ԑg�ꗗ���
	/// </summary>
	public ref class EpgServiceEventInfo{
	public:
		/// <summary>
		/// �T�[�r�X���
		/// </summary>
		property EpgServiceInfo^ serviceInfo;
		/// <summary>
		/// �ԑg�ꗗ
		/// </summary>
		property List<EpgEventInfo^>^ eventList;
	public:
		EpgServiceEventInfo(void){
			serviceInfo = gcnew EpgServiceInfo();
			eventList = gcnew List<EpgEventInfo^>();
		};
	};

	/// <summary>
	/// �`�����l���ύX���
	/// </summary>
	public ref class SetChInfo{
	public:
		/// <summary>
		/// ONID��TSID��SID�̒l���g�p�ł��邩�ǂ���
		/// </summary>
		property UInt32 useSID;
		/// <summary>
		/// ONID
		/// </summary>
		property UInt16 ONID;
		/// <summary>
		/// TSID
		/// </summary>
		property UInt16 TSID;
		/// <summary>
		/// SID
		/// </summary>
		property UInt16 SID;
		/// <summary>
		/// Space��Ch�̒l���g�p�ł��邩�ǂ���
		/// </summary>
		property UInt32 useBonCh;
		/// <summary>
		/// space
		/// </summary>
		property UInt32 space;
		/// <summary>
		/// ch
		/// </summary>
		property UInt32 ch;
	public:
		SetChInfo(void){
		};
	};

	/// <summary>
	/// TVTest�̃`�����l���؂�ւ��p
	/// </summary>
	public ref class TvTestChChgInfo{
	public:
		/// <summary>
		/// �T�[�r�X���
		/// </summary>
		property String^ bonDriver;
		/// <summary>
		/// �ԑg�ꗗ
		/// </summary>
		property Def::SetChInfo^ chInfo;
	public:
		TvTestChChgInfo(void){
			bonDriver = gcnew String("");
			chInfo = gcnew Def::SetChInfo();
		};
	};

	/// <summary>
	/// �R���e���c���
	/// </summary>
	public ref class ContentKind{
	public:
		/// <summary>
		/// ���C���W����������
		/// </summary>
		property String^ ContentName;
		/// <summary>
		/// �T�u�W����������
		/// </summary>
		property String^ SubName;
		/// <summary>
		/// ���C�����ʎq
		/// </summary>
		property Byte Nibble1;
		/// <summary>
		/// �T�u���ʎq
		/// </summary>
		property Byte Nibble2;
	public:
		ContentKind(void){
			ContentName = gcnew String("");
			SubName = gcnew String("");
			Nibble1 = 0;
			Nibble2 = 0;
		};
		ContentKind(String^ contentName, String^ subName, Byte nibble1, Byte nibble2){
			ContentName = contentName;
			SubName = subName;
			Nibble1 = nibble1;
			Nibble2 = nibble2;
		};
	};

	/// <summary>
	/// �R���|�[�l���g���
	/// </summary>
	public ref class ComponentKind{
	public:
		/// <summary>
		/// ����
		/// </summary>
		property String^ ComponentName;
		/// <summary>
		/// �X�g���[�����ʎq
		/// </summary>
		property Byte StreamContent;
		/// <summary>
		/// �R���|�[�l���g���ʎq
		/// </summary>
		property Byte ComponentType;
	public:
		ComponentKind(void){
			ComponentName = gcnew String("");
			StreamContent = 0;
			ComponentType = 0;
		};
		ComponentKind(Byte streamContent, Byte componentType, String^ componentName){
			ComponentName = componentName;
			StreamContent = streamContent;
			ComponentType = componentType;
		};
	};

	/// <summary>
	/// �X�g���[�~���O�z�M����ID�̏��
	/// </summary>
	public ref class TVTestStreamingInfo{
	public:
		/// <summary>
		/// �L��/����
		/// </summary>
		property UInt32 enableMode;
		/// <summary>
		/// ����ID
		/// </summary>
		property UInt32 ctrlID;
		/// <summary>
		/// �T�[�o�[IP
		/// </summary>
		property UInt32 serverIP;
		/// <summary>
		/// �T�[�o�[�҂��󂯃|�[�g
		/// </summary>
		property UInt32 serverPort;
		/// <summary>
		/// �Đ��t�@�C���p�X
		/// </summary>
		property String^ filePath;
		/// <summary>
		/// UDP���M
		/// </summary>
		property UInt32 udpSend;
		/// <summary>
		/// TCP���M
		/// </summary>
		property UInt32 tcpSend;
		/// <summary>
		/// TCP���M
		/// </summary>
		property UInt32 timeShiftMode;
	public:
		TVTestStreamingInfo(void){
			filePath = gcnew String("");
		};
	};

	/// <summary>
	/// �X�g���[�~���O�z�M����ID�̏��
	/// </summary>
	public ref class NWPlayTimeShiftInfo{
	public:
		/// <summary>
		/// ����ID
		/// </summary>
		property UInt32 ctrlID;
		/// <summary>
		/// �Đ��t�@�C���p�X
		/// </summary>
		property String^ filePath;
	public:
		NWPlayTimeShiftInfo(void){
			filePath = gcnew String("");
		};
	};

	/// <summary>
	/// IE��Proxy�ݒ�
	/// </summary>
	public ref class CurrentUserIEProxyConfig{
	public:
		/// <summary>
		/// �ݒ�������I�Ɍ��o���邩�ǂ���
		/// </summary>
		property UInt32 autoDetect;
		/// <summary>
		/// �����\���X�N���v�g��URL
		/// </summary>
		property String^ autoConfigUrl;
		/// <summary>
		/// �T�[�o�[���i�A�h���X:�|�[�g�ԍ��j
		/// </summary>
		property String^ proxy;
		/// <summary>
		/// Proxy�̗�O�ݒ�
		/// </summary>
		property String^ proxyBypass;
	public:
		CurrentUserIEProxyConfig(void){
			autoConfigUrl = gcnew String("");
			proxy = gcnew String("");
			proxyBypass = gcnew String("");
		};
	};

	/// <summary>
	/// Proxy�̏��
	/// </summary>
	public ref class ProxyConfig{
	public:
		/// <summary>
		/// �T�[�o�[���i�A�h���X:�|�[�g�ԍ��j
		/// </summary>
		property String^ proxy;
		/// <summary>
		/// Proxy�̗�O�ݒ�
		/// </summary>
		property String^ proxyBypass;
	public:
		ProxyConfig(void){
			proxy = gcnew String("");
			proxyBypass = gcnew String("");
		};
	};

	/// <summary>
	/// �ڑ����Ɏg�p����Proxy�̏��
	/// </summary>
	public ref class UseProxyInfo{
	public:
		/// <summary>
		/// �T�[�o�[���i�A�h���X:�|�[�g�ԍ��j
		/// </summary>
		property String^ serverName;
		/// <summary>
		/// Proxy�̗�O�ݒ�
		/// </summary>
		property String^ proxyBypass;
		/// <summary>
		/// �F��Proxy���[�U�[��
		/// </summary>
		property String^ userName;
		/// <summary>
		/// �F��Proxy�p�X���[�h
		/// </summary>
		property String^ password;
	public:
		UseProxyInfo(void){
			serverName = gcnew String("");
			proxyBypass = gcnew String("");
			userName = gcnew String("");
			password = gcnew String("");
		};
	};

}
}
