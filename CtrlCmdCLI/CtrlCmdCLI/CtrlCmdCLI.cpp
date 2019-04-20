// ����� ���C�� DLL �t�@�C���ł��B

#include "stdafx.h"

#include "CtrlCmdCLI.h"
#include "../../Common/ErrDef.h"

#include <vcclr.h>
#include <string>

using namespace CtrlCmdCLI;

CtrlCmdUtil::CtrlCmdUtil(void)
{
	this->sendCmd = new CSendCtrlCmd;
}

CtrlCmdUtil::~CtrlCmdUtil(void)
{
	if( this->sendCmd != NULL ){
		delete this->sendCmd;
		this->sendCmd = NULL;
	}
}

CtrlCmdUtil::!CtrlCmdUtil(void)
{
	if( this->sendCmd != NULL ){
		delete this->sendCmd;
		this->sendCmd = NULL;
	}
}

void CtrlCmdUtil::GetContentKindList(
	Dictionary<UInt16, Def::ContentKind^>^% list
	)
{
	list->Add(0x00FF, gcnew Def::ContentKind("�j���[�X�^��", "", 0x00, 0xFF));
    list->Add(0x0000, gcnew Def::ContentKind("�j���[�X�^��", "�莞�E����", 0x00, 0x00));
    list->Add(0x0001, gcnew Def::ContentKind("�j���[�X�^��", "�V�C", 0x00, 0x01));
    list->Add(0x0002, gcnew Def::ContentKind("�j���[�X�^��", "���W�E�h�L�������g", 0x00, 0x02));
    list->Add(0x0003, gcnew Def::ContentKind("�j���[�X�^��", "�����E����", 0x00, 0x03));
    list->Add(0x0004, gcnew Def::ContentKind("�j���[�X�^��", "�o�ρE�s��", 0x00, 0x04));
    list->Add(0x0005, gcnew Def::ContentKind("�j���[�X�^��", "�C�O�E����", 0x00, 0x05));
    list->Add(0x0006, gcnew Def::ContentKind("�j���[�X�^��", "���", 0x00, 0x06));
    list->Add(0x0007, gcnew Def::ContentKind("�j���[�X�^��", "���_�E��k", 0x00, 0x07));
    list->Add(0x0008, gcnew Def::ContentKind("�j���[�X�^��", "�񓹓���", 0x00, 0x08));
    list->Add(0x0009, gcnew Def::ContentKind("�j���[�X�^��", "���[�J���E�n��", 0x00, 0x09));
    list->Add(0x000A, gcnew Def::ContentKind("�j���[�X�^��", "���", 0x00, 0x0A));
    list->Add(0x000F, gcnew Def::ContentKind("�j���[�X�^��", "���̑�", 0x00, 0x0F));

    list->Add(0x01FF, gcnew Def::ContentKind("�X�|�[�c", "", 0x01, 0xFF));
    list->Add(0x0100, gcnew Def::ContentKind("�X�|�[�c", "�X�|�[�c�j���[�X", 0x01, 0x00));
    list->Add(0x0101, gcnew Def::ContentKind("�X�|�[�c", "�싅", 0x01, 0x01));
    list->Add(0x0102, gcnew Def::ContentKind("�X�|�[�c", "�T�b�J�[", 0x01, 0x02));
    list->Add(0x0103, gcnew Def::ContentKind("�X�|�[�c", "�S���t", 0x01, 0x03));
    list->Add(0x0104, gcnew Def::ContentKind("�X�|�[�c", "���̑��̋��Z", 0x01, 0x04));
    list->Add(0x0105, gcnew Def::ContentKind("�X�|�[�c", "���o�E�i���Z", 0x01, 0x05));
    list->Add(0x0106, gcnew Def::ContentKind("�X�|�[�c", "�I�����s�b�N�E���ۑ��", 0x01, 0x06));
    list->Add(0x0107, gcnew Def::ContentKind("�X�|�[�c", "�}���\���E����E���j", 0x01, 0x07));
    list->Add(0x0108, gcnew Def::ContentKind("�X�|�[�c", "���[�^�[�X�|�[�c", 0x01, 0x08));
    list->Add(0x0108, gcnew Def::ContentKind("�X�|�[�c", "�}�����E�E�B���^�[�X�|�[�c", 0x01, 0x09));
    list->Add(0x0109, gcnew Def::ContentKind("�X�|�[�c", "���n�E���c���Z", 0x01, 0x0A));
    list->Add(0x010F, gcnew Def::ContentKind("�X�|�[�c", "���̑�", 0x01, 0x0F));

    list->Add(0x02FF, gcnew Def::ContentKind("���^���C�h�V���[", "", 0x02, 0xFF));
    list->Add(0x0200, gcnew Def::ContentKind("���^���C�h�V���[", "�|�\�E���C�h�V���[", 0x02, 0x00));
    list->Add(0x0201, gcnew Def::ContentKind("���^���C�h�V���[", "�t�@�b�V����", 0x02, 0x01));
    list->Add(0x0202, gcnew Def::ContentKind("���^���C�h�V���[", "��炵�E�Z�܂�", 0x02, 0x02));
    list->Add(0x0203, gcnew Def::ContentKind("���^���C�h�V���[", "���N�E���", 0x02, 0x03));
    list->Add(0x0204, gcnew Def::ContentKind("���^���C�h�V���[", "�V���b�s���O�E�ʔ�", 0x02, 0x04));
    list->Add(0x0205, gcnew Def::ContentKind("���^���C�h�V���[", "�O�����E����", 0x02, 0x05));
    list->Add(0x0206, gcnew Def::ContentKind("���^���C�h�V���[", "�C�x���g", 0x02, 0x06));
    list->Add(0x0207, gcnew Def::ContentKind("���^���C�h�V���[", "�ԑg�Љ�E���m�点", 0x02, 0x07));
    list->Add(0x020F, gcnew Def::ContentKind("���^���C�h�V���[", "���̑�", 0x02, 0x0F));
                
    list->Add(0x03FF, gcnew Def::ContentKind("�h���}", "", 0x03, 0xFF));
    list->Add(0x0300, gcnew Def::ContentKind("�h���}", "�����h���}", 0x03, 0x00));
    list->Add(0x0301, gcnew Def::ContentKind("�h���}", "�C�O�h���}", 0x03, 0x01));
    list->Add(0x0302, gcnew Def::ContentKind("�h���}", "���㌀", 0x03, 0x02));
    list->Add(0x030F, gcnew Def::ContentKind("�h���}", "���̑�", 0x03, 0x0F));

    list->Add(0x04FF, gcnew Def::ContentKind("���y", "", 0x04, 0xFF));
    list->Add(0x0400, gcnew Def::ContentKind("���y", "�������b�N�E�|�b�v�X", 0x04, 0x00));
    list->Add(0x0401, gcnew Def::ContentKind("���y", "�C�O���b�N�E�|�b�v�X", 0x04, 0x01));
    list->Add(0x0402, gcnew Def::ContentKind("���y", "�N���V�b�N�E�I�y��", 0x04, 0x02));
    list->Add(0x0403, gcnew Def::ContentKind("���y", "�W���Y�E�t���[�W����", 0x04, 0x03));
    list->Add(0x0404, gcnew Def::ContentKind("���y", "�̗w�ȁE����", 0x04, 0x04));
    list->Add(0x0405, gcnew Def::ContentKind("���y", "���C�u�E�R���T�[�g", 0x04, 0x05));
    list->Add(0x0406, gcnew Def::ContentKind("���y", "�����L���O�E���N�G�X�g", 0x04, 0x06));
    list->Add(0x0407, gcnew Def::ContentKind("���y", "�J���I�P�E�̂ǎ���", 0x04, 0x07));
    list->Add(0x0408, gcnew Def::ContentKind("���y", "���w�E�M�y", 0x04, 0x08));
    list->Add(0x0409, gcnew Def::ContentKind("���y", "���w�E�L�b�Y", 0x04, 0x09));
    list->Add(0x040A, gcnew Def::ContentKind("���y", "�������y�E���[���h�~���[�W�b�N", 0x04, 0x0A));
    list->Add(0x040F, gcnew Def::ContentKind("���y", "���̑�", 0x04, 0x0F));

    list->Add(0x05FF, gcnew Def::ContentKind("�o���G�e�B", "", 0x05, 0xFF));
    list->Add(0x0500, gcnew Def::ContentKind("�o���G�e�B", "�N�C�Y", 0x05, 0x00));
    list->Add(0x0501, gcnew Def::ContentKind("�o���G�e�B", "�Q�[��", 0x05, 0x01));
    list->Add(0x0502, gcnew Def::ContentKind("�o���G�e�B", "�g�[�N�o���G�e�B", 0x05, 0x02));
    list->Add(0x0503, gcnew Def::ContentKind("�o���G�e�B", "���΂��E�R���f�B", 0x05, 0x03));
    list->Add(0x0504, gcnew Def::ContentKind("�o���G�e�B", "���y�o���G�e�B", 0x05, 0x04));
    list->Add(0x0505, gcnew Def::ContentKind("�o���G�e�B", "���o���G�e�B", 0x05, 0x05));
    list->Add(0x0506, gcnew Def::ContentKind("�o���G�e�B", "�����o���G�e�B", 0x05, 0x06));
    list->Add(0x050F, gcnew Def::ContentKind("�o���G�e�B", "���̑�", 0x05, 0x0F));

    list->Add(0x06FF, gcnew Def::ContentKind("�f��", "", 0x06, 0xFF));
    list->Add(0x0600, gcnew Def::ContentKind("�f��", "�m��", 0x06, 0x00));
    list->Add(0x0601, gcnew Def::ContentKind("�f��", "�M��", 0x06, 0x01));
    list->Add(0x0602, gcnew Def::ContentKind("�f��", "�A�j��", 0x06, 0x02));
    list->Add(0x060F, gcnew Def::ContentKind("�f��", "���̑�", 0x06, 0x0F));

    list->Add(0x07FF, gcnew Def::ContentKind("�A�j���^���B", "", 0x07, 0xFF));
    list->Add(0x0700, gcnew Def::ContentKind("�A�j���^���B", "�����A�j��", 0x07, 0x00));
    list->Add(0x0701, gcnew Def::ContentKind("�A�j���^���B", "�C�O�A�j��", 0x07, 0x01));
    list->Add(0x0702, gcnew Def::ContentKind("�A�j���^���B", "���B", 0x07, 0x02));
    list->Add(0x070F, gcnew Def::ContentKind("�A�j���^���B", "���̑�", 0x07, 0x0F));

    list->Add(0x08FF, gcnew Def::ContentKind("�h�L�������^���[�^���{", "", 0x08, 0xFF));
    list->Add(0x0800, gcnew Def::ContentKind("�h�L�������^���[�^���{", "�Љ�E����", 0x08, 0x00));
    list->Add(0x0801, gcnew Def::ContentKind("�h�L�������^���[�^���{", "���j�E�I�s", 0x08, 0x01));
    list->Add(0x0802, gcnew Def::ContentKind("�h�L�������^���[�^���{", "���R�E�����E��", 0x08, 0x02));
    list->Add(0x0803, gcnew Def::ContentKind("�h�L�������^���[�^���{", "�F���E�Ȋw�E��w", 0x08, 0x03));
    list->Add(0x0804, gcnew Def::ContentKind("�h�L�������^���[�^���{", "�J���`���[�E�`������", 0x08, 0x04));
    list->Add(0x0805, gcnew Def::ContentKind("�h�L�������^���[�^���{", "���w�E���|", 0x08, 0x05));
    list->Add(0x0806, gcnew Def::ContentKind("�h�L�������^���[�^���{", "�X�|�[�c", 0x08, 0x06));
    list->Add(0x0807, gcnew Def::ContentKind("�h�L�������^���[�^���{", "�h�L�������^���[�S��", 0x08, 0x07));
    list->Add(0x0808, gcnew Def::ContentKind("�h�L�������^���[�^���{", "�C���^�r���[�E���_", 0x08, 0x08));
    list->Add(0x080F, gcnew Def::ContentKind("�h�L�������^���[�^���{", "���̑�", 0x08, 0x0F));

    list->Add(0x09FF, gcnew Def::ContentKind("����^����", "", 0x09, 0xFF));
    list->Add(0x0900, gcnew Def::ContentKind("����^����", "���㌀�E�V��", 0x09, 0x00));
    list->Add(0x0901, gcnew Def::ContentKind("����^����", "�~���[�W�J��", 0x09, 0x01));
    list->Add(0x0902, gcnew Def::ContentKind("����^����", "�_���X�E�o���G", 0x09, 0x02));
    list->Add(0x0903, gcnew Def::ContentKind("����^����", "����E���|", 0x09, 0x03));
    list->Add(0x0904, gcnew Def::ContentKind("����^����", "�̕���E�ÓT", 0x09, 0x04));
    list->Add(0x090F, gcnew Def::ContentKind("����^����", "���̑�", 0x09, 0x0F));

    list->Add(0x0AFF, gcnew Def::ContentKind("��^����", "", 0x0A, 0xFF));
    list->Add(0x0A00, gcnew Def::ContentKind("��^����", "���E�ނ�E�A�E�g�h�A", 0x0A, 0x00));
    list->Add(0x0A01, gcnew Def::ContentKind("��^����", "���|�E�y�b�g�E��|", 0x0A, 0x01));
    list->Add(0x0A02, gcnew Def::ContentKind("��^����", "���y�E���p�E�H�|", 0x0A, 0x02));
    list->Add(0x0A03, gcnew Def::ContentKind("��^����", "�͌�E����", 0x0A, 0x03));
    list->Add(0x0A04, gcnew Def::ContentKind("��^����", "�����E�p�`���R", 0x0A, 0x04));
    list->Add(0x0A05, gcnew Def::ContentKind("��^����", "�ԁE�I�[�g�o�C", 0x0A, 0x05));
    list->Add(0x0A06, gcnew Def::ContentKind("��^����", "�R���s���[�^�E�s�u�Q�[��", 0x0A, 0x06));
    list->Add(0x0A07, gcnew Def::ContentKind("��^����", "��b�E��w", 0x0A, 0x07));
    list->Add(0x0A08, gcnew Def::ContentKind("��^����", "�c���E���w��", 0x0A, 0x08));
    list->Add(0x0A09, gcnew Def::ContentKind("��^����", "���w���E���Z��", 0x0A, 0x09));
    list->Add(0x0A0A, gcnew Def::ContentKind("��^����", "��w���E��", 0x0A, 0x0A));
    list->Add(0x0A0B, gcnew Def::ContentKind("��^����", "���U����E���i", 0x0A, 0x0B));
    list->Add(0x0A0C, gcnew Def::ContentKind("��^����", "������", 0x0A, 0x0C));
    list->Add(0x0A0F, gcnew Def::ContentKind("��^����", "���̑�", 0x0A, 0x0F));

    list->Add(0x0BFF, gcnew Def::ContentKind("����", "", 0x0B, 0xFF));
    list->Add(0x0B00, gcnew Def::ContentKind("����", "�����", 0x0B, 0x00));
    list->Add(0x0B01, gcnew Def::ContentKind("����", "��Q��", 0x0B, 0x01));
    list->Add(0x0B02, gcnew Def::ContentKind("����", "�Љ��", 0x0B, 0x02));
    list->Add(0x0B03, gcnew Def::ContentKind("����", "�{�����e�B�A", 0x0B, 0x03));
    list->Add(0x0B04, gcnew Def::ContentKind("����", "��b", 0x0B, 0x04));
    list->Add(0x0B05, gcnew Def::ContentKind("����", "�����i�����j", 0x0B, 0x05));
    list->Add(0x0B06, gcnew Def::ContentKind("����", "�������", 0x0B, 0x06));
    list->Add(0x0B0F, gcnew Def::ContentKind("����", "���̑�", 0x0B, 0x0F));

    list->Add(0x0FFF, gcnew Def::ContentKind("���̑�", "", 0x0F, 0xFF));
    list->Add(0xFFFF, gcnew Def::ContentKind("�Ȃ�", "", 0xFF, 0xFF));

}
void CtrlCmdUtil::GetComponentKindList(
	Dictionary<UInt16, Def::ComponentKind^>^% list
	)
{
    list->Add(0x0101, gcnew Def::ComponentKind(0x01, 0x01, "480i(525i)�A�A�X�y�N�g��4:3"));
    list->Add(0x0102, gcnew Def::ComponentKind(0x01, 0x02, "480i(525i)�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x0103, gcnew Def::ComponentKind(0x01, 0x03, "480i(525i)�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x0104, gcnew Def::ComponentKind(0x01, 0x04, "480i(525i)�A�A�X�y�N�g�� > 16:9"));
    list->Add(0x0191, gcnew Def::ComponentKind(0x01, 0x91, "2160p�A�A�X�y�N�g��4:3"));
    list->Add(0x0192, gcnew Def::ComponentKind(0x01, 0x92, "2160p�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x0193, gcnew Def::ComponentKind(0x01, 0x93, "2160p�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x0194, gcnew Def::ComponentKind(0x01, 0x94, "2160p�A�A�X�y�N�g�� > 16:9"));
    list->Add(0x01A1, gcnew Def::ComponentKind(0x01, 0xA1, "480p(525p)�A�A�X�y�N�g��4:3"));
    list->Add(0x01A2, gcnew Def::ComponentKind(0x01, 0xA2, "480p(525p)�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x01A3, gcnew Def::ComponentKind(0x01, 0xA3, "480p(525p)�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x01A4, gcnew Def::ComponentKind(0x01, 0xA4, "480p(525p)�A�A�X�y�N�g�� > 16:9"));
    list->Add(0x01B1, gcnew Def::ComponentKind(0x01, 0xB1, "1080i(1125i)�A�A�X�y�N�g��4:3"));
    list->Add(0x01B2, gcnew Def::ComponentKind(0x01, 0xB2, "1080i(1125i)�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x01B3, gcnew Def::ComponentKind(0x01, 0xB3, "1080i(1125i)�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x01B4, gcnew Def::ComponentKind(0x01, 0xB4, "1080i(1125i)�A�A�X�y�N�g�� > 16:9"));
    list->Add(0x01C1, gcnew Def::ComponentKind(0x01, 0xC1, "720p(750p)�A�A�X�y�N�g��4:3"));
    list->Add(0x01C2, gcnew Def::ComponentKind(0x01, 0xC2, "720p(750p)�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x01C3, gcnew Def::ComponentKind(0x01, 0xC3, "720p(750p)�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x01C4, gcnew Def::ComponentKind(0x01, 0xC4, "720p(750p)�A�A�X�y�N�g�� > 16:9"));
    list->Add(0x01D1, gcnew Def::ComponentKind(0x01, 0xD1, "240p �A�X�y�N�g��4:3"));
    list->Add(0x01D2, gcnew Def::ComponentKind(0x01, 0xD2, "240p �A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x01D3, gcnew Def::ComponentKind(0x01, 0xD3, "240p �A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x01D4, gcnew Def::ComponentKind(0x01, 0xD4, "240p �A�X�y�N�g�� > 16:9"));
    list->Add(0x01E1, gcnew Def::ComponentKind(0x01, 0xE1, "1080p(1125p)�A�A�X�y�N�g��4:3"));
    list->Add(0x01E2, gcnew Def::ComponentKind(0x01, 0xE2, "1080p(1125p)�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x01E3, gcnew Def::ComponentKind(0x01, 0xE3, "1080p(1125p)�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x01E4, gcnew Def::ComponentKind(0x01, 0xE4, "1080p(1125p)�A�A�X�y�N�g�� > 16:9"));
    list->Add(0x0201, gcnew Def::ComponentKind(0x02, 0x01, "1/0���[�h�i�V���O�����m�j"));
    list->Add(0x0202, gcnew Def::ComponentKind(0x02, 0x02, "1/0�{1/0���[�h�i�f���A�����m�j"));
    list->Add(0x0203, gcnew Def::ComponentKind(0x02, 0x03, "2/0���[�h�i�X�e���I�j"));
    list->Add(0x0204, gcnew Def::ComponentKind(0x02, 0x04, "2/1���[�h"));
    list->Add(0x0205, gcnew Def::ComponentKind(0x02, 0x05, "3/0���[�h"));
    list->Add(0x0206, gcnew Def::ComponentKind(0x02, 0x06, "2/2���[�h"));
    list->Add(0x0207, gcnew Def::ComponentKind(0x02, 0x07, "3/1���[�h"));
    list->Add(0x0208, gcnew Def::ComponentKind(0x02, 0x08, "3/2���[�h"));
    list->Add(0x0209, gcnew Def::ComponentKind(0x02, 0x09, "3/2�{LFE���[�h�i3/2.1���[�h�j"));
    list->Add(0x020A, gcnew Def::ComponentKind(0x02, 0x0A, "3/3.1���[�h"));
    list->Add(0x020B, gcnew Def::ComponentKind(0x02, 0x0B, "2/0/0-2/0/2-0.1���[�h"));
    list->Add(0x020C, gcnew Def::ComponentKind(0x02, 0x0C, "5/2.1���[�h"));
    list->Add(0x020D, gcnew Def::ComponentKind(0x02, 0x0D, "3/2/2.1���[�h"));
    list->Add(0x020E, gcnew Def::ComponentKind(0x02, 0x0E, "2/0/0-3/0/2-0.1���[�h"));
    list->Add(0x020F, gcnew Def::ComponentKind(0x02, 0x0F, "0/2/0-3/0/2-0.1���[�h"));
    list->Add(0x0210, gcnew Def::ComponentKind(0x02, 0x10, "2/0/0-3/2/3-0.2���[�h"));
    list->Add(0x0211, gcnew Def::ComponentKind(0x02, 0x11, "3/3/3-5/2/3-3/0/0.2���[�h"));
    list->Add(0x0240, gcnew Def::ComponentKind(0x02, 0x40, "���o��Q�җp�������"));
    list->Add(0x0241, gcnew Def::ComponentKind(0x02, 0x41, "���o��Q�җp����"));
    list->Add(0x0501, gcnew Def::ComponentKind(0x05, 0x01, "H.264|MPEG-4 AVC�A480i(525i)�A�A�X�y�N�g��4:3"));
    list->Add(0x0502, gcnew Def::ComponentKind(0x05, 0x02, "H.264|MPEG-4 AVC�A480i(525i)�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x0503, gcnew Def::ComponentKind(0x05, 0x03, "H.264|MPEG-4 AVC�A480i(525i)�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x0504, gcnew Def::ComponentKind(0x05, 0x04, "H.264|MPEG-4 AVC�A480i(525i)�A�A�X�y�N�g�� > 16:9"));
    list->Add(0x0591, gcnew Def::ComponentKind(0x05, 0x91, "H.264|MPEG-4 AVC�A2160p�A�A�X�y�N�g��4:3"));
    list->Add(0x0592, gcnew Def::ComponentKind(0x05, 0x92, "H.264|MPEG-4 AVC�A2160p�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x0593, gcnew Def::ComponentKind(0x05, 0x93, "H.264|MPEG-4 AVC�A2160p�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x0594, gcnew Def::ComponentKind(0x05, 0x94, "H.264|MPEG-4 AVC�A2160p�A�A�X�y�N�g�� > 16:9"));
    list->Add(0x05A1, gcnew Def::ComponentKind(0x05, 0xA1, "H.264|MPEG-4 AVC�A480p(525p)�A�A�X�y�N�g��4:3"));
    list->Add(0x05A2, gcnew Def::ComponentKind(0x05, 0xA2, "H.264|MPEG-4 AVC�A480p(525p)�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x05A3, gcnew Def::ComponentKind(0x05, 0xA3, "H.264|MPEG-4 AVC�A480p(525p)�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x05A4, gcnew Def::ComponentKind(0x05, 0xA4, "H.264|MPEG-4 AVC�A480p(525p)�A�A�X�y�N�g�� > 16:9"));
    list->Add(0x05B1, gcnew Def::ComponentKind(0x05, 0xB1, "H.264|MPEG-4 AVC�A1080i(1125i)�A�A�X�y�N�g��4:3"));
    list->Add(0x05B2, gcnew Def::ComponentKind(0x05, 0xB2, "H.264|MPEG-4 AVC�A1080i(1125i)�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x05B3, gcnew Def::ComponentKind(0x05, 0xB3, "H.264|MPEG-4 AVC�A1080i(1125i)�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x05B4, gcnew Def::ComponentKind(0x05, 0xB4, "H.264|MPEG-4 AVC�A1080i(1125i)�A�A�X�y�N�g�� > 16:9"));
    list->Add(0x05C1, gcnew Def::ComponentKind(0x05, 0xC1, "H.264|MPEG-4 AVC�A720p(750p)�A�A�X�y�N�g��4:3"));
    list->Add(0x05C2, gcnew Def::ComponentKind(0x05, 0xC2, "H.264|MPEG-4 AVC�A720p(750p)�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x05C3, gcnew Def::ComponentKind(0x05, 0xC3, "H.264|MPEG-4 AVC�A720p(750p)�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x05C4, gcnew Def::ComponentKind(0x05, 0xC4, "H.264|MPEG-4 AVC�A720p(750p)�A�A�X�y�N�g�� > 16:9"));
    list->Add(0x05D1, gcnew Def::ComponentKind(0x05, 0xD1, "H.264|MPEG-4 AVC�A240p �A�X�y�N�g��4:3"));
    list->Add(0x05D2, gcnew Def::ComponentKind(0x05, 0xD2, "H.264|MPEG-4 AVC�A240p �A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x05D3, gcnew Def::ComponentKind(0x05, 0xD3, "H.264|MPEG-4 AVC�A240p �A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x05D4, gcnew Def::ComponentKind(0x05, 0xD4, "H.264|MPEG-4 AVC�A240p �A�X�y�N�g�� > 16:9"));
    list->Add(0x05E1, gcnew Def::ComponentKind(0x05, 0xE1, "H.264|MPEG-4 AVC�A1080p(1125p)�A�A�X�y�N�g��4:3"));
    list->Add(0x05E2, gcnew Def::ComponentKind(0x05, 0xE2, "H.264|MPEG-4 AVC�A1080p(1125p)�A�A�X�y�N�g��16:9 �p���x�N�g������"));
    list->Add(0x05E3, gcnew Def::ComponentKind(0x05, 0xE3, "H.264|MPEG-4 AVC�A1080p(1125p)�A�A�X�y�N�g��16:9 �p���x�N�g���Ȃ�"));
    list->Add(0x05E4, gcnew Def::ComponentKind(0x05, 0xE4, "H.264|MPEG-4 AVC�A1080p(1125p)�A�A�X�y�N�g�� > 16:9"));

}


void CtrlCmdUtil::CopyData(DateTime src, SYSTEMTIME* dest)
{
	__int64 i64Time = src.ToFileTime();
	FILETIME fTime;
	fTime.dwHighDateTime = (DWORD)(i64Time>>32);
	fTime.dwLowDateTime = (DWORD)(i64Time&0x00000000FFFFFFFF);
	if( i64Time <= 0 ){
		ZeroMemory(dest, sizeof(SYSTEMTIME));
	}else{
		FileTimeToSystemTime(&fTime,dest);
	}
}

DateTime CtrlCmdUtil::ConvertTime(SYSTEMTIME* src)
{
	FILETIME fTime;
	SystemTimeToFileTime( src, &fTime );
	__int64 i64Time = 0;
	i64Time = ((__int64)fTime.dwHighDateTime)<<32 | fTime.dwLowDateTime;
	if( i64Time <= 0 ){
		return DateTime::MinValue;
	}
	return DateTime::FromFileTimeUtc(i64Time);
}
/*
DateTime CtrlCmdUtil::CopyData(SYSTEMTIME* src, DateTime*& dest)
{
	FILETIME fTime;
	SystemTimeToFileTime( src, &fTime );
	__int64 i64Time = 0;
	i64Time = ((__int64)fTime.dwHighDateTime)<<32 | fTime.dwLowDateTime;

	dest = &DateTime::FromFileTimeUtc(i64Time);
	return DateTime::FromFileTimeUtc(i64Time);
}
*/
void CtrlCmdUtil::CopyData(Def::ReserveData^ src, RESERVE_DATA* dest)
{
	pin_ptr<const wchar_t> titlePin = PtrToStringChars(src->Title);
	pin_ptr<const wchar_t> stationNamePin = PtrToStringChars(src->StationName);
	pin_ptr<const wchar_t> commentPin = PtrToStringChars(src->Comment);
	pin_ptr<const wchar_t> recFilePathPin = PtrToStringChars(src->RecFilePath);

	dest->title = titlePin;
	CopyData(src->StartTime, &dest->startTime);
	dest->durationSecond = src->DurationSecond;
	dest->stationName = stationNamePin;
	dest->originalNetworkID = src->OriginalNetworkID;
	dest->transportStreamID = src->TransportStreamID;
	dest->serviceID = src->ServiceID;
	dest->eventID = src->EventID;
	dest->comment = commentPin;
	dest->reserveID = src->ReserveID;
	dest->recWaitFlag = src->RecWaitFlag;
	dest->overlapMode = src->OverlapMode;
	dest->recFilePath = recFilePathPin;
	CopyData(src->StartTimeEpg, &dest->startTimeEpg);
	CopyData(src->RecSetting, &dest->recSetting);
	dest->reserveStatus = src->ReserveStatus;

	for( int i=0; i<src->RecFileNameList->Count; i++){
		pin_ptr<const wchar_t> namePin = PtrToStringChars(src->RecFileNameList[i]);

		dest->recFileNameList.push_back(namePin);
	}
	dest->param1 = src->param1;
}

void CtrlCmdUtil::CopyData(RESERVE_DATA* src, Def::ReserveData^% dest)
{
	dest->Title = gcnew String(src->title.c_str());
	dest->StartTime = ConvertTime( &src->startTime );
//	CopyData(&src->startTime, dest->StartTime);
	dest->DurationSecond = src->durationSecond;
	dest->StationName = gcnew String(src->stationName.c_str());
	dest->OriginalNetworkID = src->originalNetworkID;
	dest->TransportStreamID = src->transportStreamID;
	dest->ServiceID = src->serviceID;
	dest->EventID = src->eventID;
	dest->Comment = gcnew String(src->comment.c_str());
	dest->ReserveID = src->reserveID;
	dest->RecWaitFlag = src->recWaitFlag;
	dest->OverlapMode = src->overlapMode;
	dest->RecFilePath = gcnew String(src->recFilePath.c_str());
	dest->StartTimeEpg = ConvertTime( &src->startTimeEpg );
//	CopyData(&src->startTimeEpg, dest->StartTimeEpg);
	CopyData(&src->recSetting, dest->RecSetting);
	dest->ReserveStatus = src->reserveStatus;

	for( size_t i=0; i<src->recFileNameList.size(); i++){
		dest->RecFileNameList->Add( gcnew String(src->recFileNameList[i].c_str()) );
	}
	dest->param1 = src->param1;

}

void CtrlCmdUtil::CopyData(Def::RecSettingData^ src, REC_SETTING_DATA* dest)
{
	pin_ptr<const wchar_t> batFilePathPin = PtrToStringChars(src->BatFilePath);
	for( int i=0; i<src->RecFolderList->Count; i++){
		REC_FILE_SET_INFO item;
		pin_ptr<const wchar_t> recFolderPathPin = PtrToStringChars(src->RecFolderList[i]->RecFolder);
		pin_ptr<const wchar_t> writePlugInPin = PtrToStringChars(src->RecFolderList[i]->WritePlugIn);
		pin_ptr<const wchar_t> recNamePlugInPin = PtrToStringChars(src->RecFolderList[i]->RecNamePlugIn);
		pin_ptr<const wchar_t> recFileNamePin = PtrToStringChars(src->RecFolderList[i]->RecFileName);
		item.recFolder = recFolderPathPin;
		item.writePlugIn = writePlugInPin;
		item.recNamePlugIn = recNamePlugInPin;
		item.recFileName = recFileNamePin;

		dest->recFolderList.push_back(item);
	}
	for( int i=0; i<src->PartialRecFolder->Count; i++){
		REC_FILE_SET_INFO item;
		pin_ptr<const wchar_t> recFolderPathPin = PtrToStringChars(src->PartialRecFolder[i]->RecFolder);
		pin_ptr<const wchar_t> writePlugInPin = PtrToStringChars(src->PartialRecFolder[i]->WritePlugIn);
		pin_ptr<const wchar_t> recNamePlugInPin = PtrToStringChars(src->PartialRecFolder[i]->RecNamePlugIn);
		pin_ptr<const wchar_t> recFileNamePin = PtrToStringChars(src->PartialRecFolder[i]->RecFileName);
		item.recFolder = recFolderPathPin;
		item.writePlugIn = writePlugInPin;
		item.recNamePlugIn = recNamePlugInPin;
		item.recFileName = recFileNamePin;

		dest->partialRecFolder.push_back(item);
	}

	dest->recMode = src->RecMode;
	dest->priority = src->Priority;
	dest->tuijyuuFlag = src->TuijyuuFlag;
	dest->serviceMode = src->ServiceMode;
	dest->pittariFlag = src->PittariFlag;
	dest->batFilePath = batFilePathPin;
	dest->suspendMode = src->SuspendMode;
	dest->rebootFlag = src->RebootFlag;
	dest->useMargineFlag = src->UseMargineFlag;
	dest->startMargine = src->StartMargine;
	dest->endMargine = src->EndMargine;
	dest->continueRecFlag = src->ContinueRecFlag;
	dest->partialRecFlag = src->PartialRecFlag;
	dest->tunerID = src->TunerID;

}

void CtrlCmdUtil::CopyData(REC_SETTING_DATA* src, Def::RecSettingData^% dest)
{
	dest->RecMode = src->recMode;
	dest->Priority = src->priority;
	dest->TuijyuuFlag = src->tuijyuuFlag;
	dest->ServiceMode = src->serviceMode;
	dest->PittariFlag = src->pittariFlag;
	dest->BatFilePath = gcnew String(src->batFilePath.c_str());

	for( size_t i=0; i<src->recFolderList.size(); i++){
		Def::RecFileSetInfo^ item = gcnew Def::RecFileSetInfo();
		item->RecFolder = gcnew String(src->recFolderList[i].recFolder.c_str());
		item->WritePlugIn = gcnew String(src->recFolderList[i].writePlugIn.c_str());
		item->RecNamePlugIn = gcnew String(src->recFolderList[i].recNamePlugIn.c_str());
		item->RecFileName = gcnew String(src->recFolderList[i].recFileName.c_str());
		dest->RecFolderList->Add( item );
	}
	for( size_t i=0; i<src->partialRecFolder.size(); i++){
		Def::RecFileSetInfo^ item = gcnew Def::RecFileSetInfo();
		item->RecFolder = gcnew String(src->partialRecFolder[i].recFolder.c_str());
		item->WritePlugIn = gcnew String(src->partialRecFolder[i].writePlugIn.c_str());
		item->RecNamePlugIn = gcnew String(src->partialRecFolder[i].recNamePlugIn.c_str());
		item->RecFileName = gcnew String(src->partialRecFolder[i].recFileName.c_str());
		dest->PartialRecFolder->Add( item );
	}
	dest->SuspendMode = src->suspendMode;
	dest->RebootFlag = src->rebootFlag;
	dest->UseMargineFlag = src->useMargineFlag;
	dest->StartMargine = src->startMargine;
	dest->EndMargine = src->endMargine;
	dest->ContinueRecFlag = src->continueRecFlag;
	dest->PartialRecFlag = src->partialRecFlag;
	dest->TunerID = src->tunerID;
}

void CtrlCmdUtil::CopyData(Def::EpgShortEventInfo^ src, EPGDB_SHORT_EVENT_INFO* dest)
{
	pin_ptr<const wchar_t> event_namePin = PtrToStringChars(src->event_name);
	pin_ptr<const wchar_t> text_charPin = PtrToStringChars(src->text_char);

	dest->event_name = event_namePin;
	dest->text_char = text_charPin;
}

void CtrlCmdUtil::CopyData(EPGDB_SHORT_EVENT_INFO* src, Def::EpgShortEventInfo^% dest)
{
	dest->event_name = gcnew String(src->event_name.c_str());
	dest->text_char = gcnew String(src->text_char.c_str());

}

void CtrlCmdUtil::CopyData(Def::EpgExtendedEventInfo^ src, EPGDB_EXTENDED_EVENT_INFO* dest)
{
	pin_ptr<const wchar_t> text_charPin = PtrToStringChars(src->text_char);
	dest->text_char = text_charPin;
}

void CtrlCmdUtil::CopyData(EPGDB_EXTENDED_EVENT_INFO* src, Def::EpgExtendedEventInfo^% dest)
{
	dest->text_char = gcnew String(src->text_char.c_str());
}

void CtrlCmdUtil::CopyData(Def::EpgContentData^ src, EPGDB_CONTENT_DATA* dest)
{
	dest->content_nibble_level_1 = src->content_nibble_level_1;
	dest->content_nibble_level_2 = src->content_nibble_level_2;
	dest->user_nibble_1 = src->user_nibble_1;
	dest->user_nibble_2 = src->user_nibble_2;
}

void CtrlCmdUtil::CopyData(EPGDB_CONTENT_DATA* src, Def::EpgContentData^% dest)
{
	dest->content_nibble_level_1 = src->content_nibble_level_1;
	dest->content_nibble_level_2 = src->content_nibble_level_2;
	dest->user_nibble_1 = src->user_nibble_1;
	dest->user_nibble_2 = src->user_nibble_2;
}

void CtrlCmdUtil::CopyData(Def::EpgContentInfo^ src, EPGDB_CONTEN_INFO* dest)
{
	for( int i=0; i<src->nibbleList->Count; i++ ){
		EPGDB_CONTENT_DATA item;
		CopyData(src->nibbleList[i], &item);
		dest->nibbleList.push_back(item);
	}
}

void CtrlCmdUtil::CopyData(EPGDB_CONTEN_INFO* src, Def::EpgContentInfo^% dest)
{
	for( size_t i=0; i<src->nibbleList.size(); i++ ){
		Def::EpgContentData^ item = gcnew Def::EpgContentData();
		CopyData(&src->nibbleList[i], item);
		dest->nibbleList->Add(item);
	}
}

void CtrlCmdUtil::CopyData(Def::EpgComponentInfo^ src, EPGDB_COMPONENT_INFO* dest)
{
	pin_ptr<const wchar_t> text_charPin = PtrToStringChars(src->text_char);

	dest->stream_content = src->stream_content;
	dest->component_type = src->component_type;
	dest->component_tag = src->component_tag;
	dest->text_char = text_charPin;
}

void CtrlCmdUtil::CopyData(EPGDB_COMPONENT_INFO* src, Def::EpgComponentInfo^% dest)
{
	dest->stream_content = src->stream_content;
	dest->component_type = src->component_type;
	dest->component_tag = src->component_tag;
	dest->text_char = gcnew String(src->text_char.c_str());
}

void CtrlCmdUtil::CopyData(Def::EpgAudioComponentInfoData^ src, EPGDB_AUDIO_COMPONENT_INFO_DATA* dest)
{
	pin_ptr<const wchar_t> text_charPin = PtrToStringChars(src->text_char);

	dest->stream_content = src->stream_content;
	dest->component_type = src->component_type;
	dest->component_tag = src->component_tag;
	dest->stream_type = src->stream_type;
	dest->simulcast_group_tag = src->simulcast_group_tag;
	dest->ES_multi_lingual_flag = src->ES_multi_lingual_flag;
	dest->main_component_flag = src->main_component_flag;
	dest->quality_indicator = src->quality_indicator;
	dest->sampling_rate = src->sampling_rate;
	dest->text_char = text_charPin;
}

void CtrlCmdUtil::CopyData(EPGDB_AUDIO_COMPONENT_INFO_DATA* src, Def::EpgAudioComponentInfoData^% dest)
{
	dest->stream_content = src->stream_content;
	dest->component_type = src->component_type;
	dest->component_tag = src->component_tag;
	dest->stream_type = src->stream_type;
	dest->simulcast_group_tag = src->simulcast_group_tag;
	dest->ES_multi_lingual_flag = src->ES_multi_lingual_flag;
	dest->main_component_flag = src->main_component_flag;
	dest->quality_indicator = src->quality_indicator;
	dest->sampling_rate = src->sampling_rate;
	dest->text_char = gcnew String(src->text_char.c_str());
}

void CtrlCmdUtil::CopyData(Def::EpgAudioComponentInfo^ src, EPGDB_AUDIO_COMPONENT_INFO* dest)
{
	for( int i=0; i<src->componentList->Count; i++ ){
		EPGDB_AUDIO_COMPONENT_INFO_DATA item;
		CopyData(src->componentList[i], &item);
		dest->componentList.push_back(item);
	}
}

void CtrlCmdUtil::CopyData(EPGDB_AUDIO_COMPONENT_INFO* src, Def::EpgAudioComponentInfo^% dest)
{
	for( size_t i=0; i<src->componentList.size(); i++ ){
		Def::EpgAudioComponentInfoData^ item = gcnew Def::EpgAudioComponentInfoData();
		CopyData(&src->componentList[i], item);
		dest->componentList->Add(item);
	}
}

void CtrlCmdUtil::CopyData(Def::EpgEventData^ src, EPGDB_EVENT_DATA* dest)
{
	dest->original_network_id = src->original_network_id;
	dest->transport_stream_id = src->transport_stream_id;
	dest->service_id = src->service_id;
	dest->event_id = src->event_id;
}

void CtrlCmdUtil::CopyData(EPGDB_EVENT_DATA* src, Def::EpgEventData^% dest)
{
	dest->original_network_id = src->original_network_id;
	dest->transport_stream_id = src->transport_stream_id;
	dest->service_id = src->service_id;
	dest->event_id = src->event_id;
}

void CtrlCmdUtil::CopyData(Def::EpgEventGroupInfo^ src, EPGDB_EVENTGROUP_INFO* dest)
{
	dest->group_type = src->group_type;
	for( int i=0; i<src->eventDataList->Count; i++ ){
		EPGDB_EVENT_DATA item;
		CopyData(src->eventDataList[i], &item);
		dest->eventDataList.push_back(item);
	}
}

void CtrlCmdUtil::CopyData(EPGDB_EVENTGROUP_INFO* src, Def::EpgEventGroupInfo^% dest)
{
	dest->group_type = src->group_type;
	for( size_t i=0; i<src->eventDataList.size(); i++ ){
		Def::EpgEventData^ item = gcnew Def::EpgEventData();
		CopyData(&src->eventDataList[i], item);
		dest->eventDataList->Add(item);
	}
}

void CtrlCmdUtil::CopyData(Def::EpgEventInfo^ src, EPGDB_EVENT_INFO* dest)
{
	dest->original_network_id = src->original_network_id;
	dest->transport_stream_id = src->transport_stream_id;
	dest->service_id = src->service_id;
	dest->event_id = src->event_id;
	dest->StartTimeFlag = src->StartTimeFlag;
//	CopyData(src->start_time, &dest->start_time);
	dest->DurationFlag = src->DurationFlag;
	dest->durationSec = src->durationSec;
	dest->freeCAFlag = src->FreeCAFlag;

	if( src->ShortInfo != nullptr ){
		dest->shortInfo = new EPGDB_SHORT_EVENT_INFO;
		CopyData(src->ShortInfo, dest->shortInfo);
	}else{
		dest->shortInfo = NULL;
	}
	if( src->ExtInfo != nullptr ){
		dest->extInfo = new EPGDB_EXTENDED_EVENT_INFO;
		CopyData(src->ExtInfo, dest->extInfo);
	}else{
		dest->extInfo = NULL;
	}
	if( src->ContentInfo != nullptr ){
		dest->contentInfo = new EPGDB_CONTEN_INFO;
		CopyData(src->ContentInfo, dest->contentInfo);
	}else{
		dest->contentInfo = NULL;
	}
	if( src->ComponentInfo != nullptr ){
		dest->componentInfo = new EPGDB_COMPONENT_INFO;
		CopyData(src->ComponentInfo, dest->componentInfo);
	}else{
		dest->componentInfo = NULL;
	}
	if( src->AudioInfo != nullptr ){
		dest->audioInfo = new EPGDB_AUDIO_COMPONENT_INFO;
		CopyData(src->AudioInfo, dest->audioInfo);
	}else{
		dest->audioInfo = NULL;
	}
	if( src->EventGroupInfo != nullptr ){
		dest->eventGroupInfo = new EPGDB_EVENTGROUP_INFO;
		CopyData(src->EventGroupInfo, dest->eventGroupInfo);
	}else{
		dest->eventGroupInfo = NULL;
	}
	if( src->EventRelayInfo != nullptr ){
		dest->eventRelayInfo = new EPGDB_EVENTGROUP_INFO;
		CopyData(src->EventRelayInfo, dest->eventRelayInfo);
	}else{
		dest->eventRelayInfo = NULL;
	}

}

void CtrlCmdUtil::CopyData(EPGDB_EVENT_INFO* src, Def::EpgEventInfo^% dest)
{
	dest->original_network_id = src->original_network_id;
	dest->transport_stream_id = src->transport_stream_id;
	dest->service_id = src->service_id;
	dest->event_id = src->event_id;
	dest->StartTimeFlag = src->StartTimeFlag;
	dest->start_time = ConvertTime( &src->start_time );
//	CopyData(&src->start_time, dest->start_time);
	dest->DurationFlag = src->DurationFlag;
	dest->durationSec = src->durationSec;
	dest->FreeCAFlag = src->freeCAFlag;

	if( src->shortInfo != NULL ){
		dest->ShortInfo = gcnew Def::EpgShortEventInfo();
		CopyData(src->shortInfo, dest->ShortInfo);
	}else{
		dest->ShortInfo = nullptr;
	}
	if( src->extInfo != NULL ){
		dest->ExtInfo = gcnew Def::EpgExtendedEventInfo();
		CopyData(src->extInfo, dest->ExtInfo);
	}else{
		dest->ExtInfo = nullptr;
	}
	if( src->contentInfo != NULL ){
		dest->ContentInfo = gcnew Def::EpgContentInfo();
		CopyData(src->contentInfo, dest->ContentInfo);
	}else{
		dest->ContentInfo = nullptr;
	}
	if( src->componentInfo != NULL ){
		dest->ComponentInfo = gcnew Def::EpgComponentInfo();
		CopyData(src->componentInfo, dest->ComponentInfo);
	}else{
		dest->ComponentInfo = nullptr;
	}
	if( src->audioInfo != NULL ){
		dest->AudioInfo = gcnew Def::EpgAudioComponentInfo();
		CopyData(src->audioInfo, dest->AudioInfo);
	}else{
		dest->AudioInfo = nullptr;
	}
	if( src->eventGroupInfo != NULL ){
		dest->EventGroupInfo = gcnew Def::EpgEventGroupInfo();
		CopyData(src->eventGroupInfo, dest->EventGroupInfo);
	}else{
		dest->EventGroupInfo = nullptr;
	}
	if( src->eventRelayInfo != NULL ){
		dest->EventRelayInfo = gcnew Def::EpgEventGroupInfo();
		CopyData(src->eventRelayInfo, dest->EventRelayInfo);
	}else{
		dest->EventRelayInfo = nullptr;
	}

}

void CtrlCmdUtil::CopyData(Def::EpgServiceInfo^ src, EPGDB_SERVICE_INFO* dest)
{
	pin_ptr<const wchar_t> service_provider_namePin = PtrToStringChars(src->service_provider_name);
	pin_ptr<const wchar_t> service_namePin = PtrToStringChars(src->service_name);
	pin_ptr<const wchar_t> network_namePin = PtrToStringChars(src->network_name);
	pin_ptr<const wchar_t> ts_namePin = PtrToStringChars(src->ts_name);

	dest->ONID = src->ONID;
	dest->TSID = src->TSID;
	dest->SID = src->SID;
	dest->service_type = src->service_type;
	dest->partialReceptionFlag = src->partialReceptionFlag;
	dest->service_provider_name = service_provider_namePin;
	dest->service_name = service_namePin;
	dest->network_name = network_namePin;
	dest->ts_name = ts_namePin;
	dest->remote_control_key_id = src->remote_control_key_id;
	dest->direct_tuning_number = src->direct_tuning_number;
}

void CtrlCmdUtil::CopyData(EPGDB_SERVICE_INFO* src, Def::EpgServiceInfo^% dest)
{
	dest->ONID = src->ONID;
	dest->TSID = src->TSID;
	dest->SID = src->SID;
	dest->service_type = src->service_type;
	dest->partialReceptionFlag = src->partialReceptionFlag;
	dest->service_provider_name = gcnew String(src->service_provider_name.c_str());
	dest->service_name = gcnew String(src->service_name.c_str());
	dest->network_name = gcnew String(src->network_name.c_str());
	dest->ts_name = gcnew String(src->ts_name.c_str());
	dest->remote_control_key_id = src->remote_control_key_id;
	dest->direct_tuning_number = src->direct_tuning_number;
}

void CtrlCmdUtil::CopyData(Def::EpgSearchDateInfo^ src, EPGDB_SEARCH_DATE_INFO* dest)
{
	dest->startDayOfWeek = src->startDayOfWeek;
	dest->startHour = src->startHour;
	dest->startMin = src->startMin;
	dest->endDayOfWeek = src->endDayOfWeek;
	dest->endHour = src->endHour;
	dest->endMin = src->endMin;
}

void CtrlCmdUtil::CopyData(EPGDB_SEARCH_DATE_INFO* src, Def::EpgSearchDateInfo^% dest)
{
	dest->startDayOfWeek = src->startDayOfWeek;
	dest->startHour = src->startHour;
	dest->startMin = src->startMin;
	dest->endDayOfWeek = src->endDayOfWeek;
	dest->endHour = src->endHour;
	dest->endMin = src->endMin;
}

void CtrlCmdUtil::CopyData(Def::EpgSearchKeyInfo^ src, EPGDB_SEARCH_KEY_INFO* dest)
{
	pin_ptr<const wchar_t> andKeyPin = PtrToStringChars(src->andKey);
	pin_ptr<const wchar_t> notKeyPin = PtrToStringChars(src->notKey);

	dest->andKey = andKeyPin;
	dest->notKey = notKeyPin;
	dest->titleOnlyFlag = src->titleOnlyFlag;
	dest->regExpFlag = src->regExpFlag;

	for( int i=0; i<src->contentList->Count; i++ ){
		EPGDB_CONTENT_DATA item;
		CopyData(src->contentList[i], &item);
		dest->contentList.push_back(item);
	}

	for( int i=0; i<src->dateList->Count; i++ ){
		EPGDB_SEARCH_DATE_INFO item;
		CopyData(src->dateList[i], &item);
		dest->dateList.push_back(item);
	}

	for( int i=0; i<src->serviceList->Count; i++ ){
		dest->serviceList.push_back(src->serviceList[i]);
	}

	for( int i=0; i<src->videoList->Count; i++ ){
		dest->videoList.push_back(src->videoList[i]);
	}

	for( int i=0; i<src->audioList->Count; i++ ){
		dest->audioList.push_back(src->audioList[i]);
	}
	dest->aimaiFlag = src->aimaiFlag;
	dest->notContetFlag = src->notContetFlag;
	dest->notDateFlag = src->notDateFlag;
	dest->freeCAFlag = src->freeCAFlag;
	dest->chkRecEnd = src->chkRecEnd;
	dest->chkRecDay = src->chkRecDay;
}

void CtrlCmdUtil::CopyData(EPGDB_SEARCH_KEY_INFO* src, Def::EpgSearchKeyInfo^% dest)
{
	dest->andKey = gcnew String(src->andKey.c_str());
	dest->notKey = gcnew String(src->notKey.c_str());
	dest->titleOnlyFlag = src->titleOnlyFlag;
	dest->regExpFlag = src->regExpFlag;

	for( size_t i=0; i<src->contentList.size(); i++ ){
		Def::EpgContentData^ item = gcnew Def::EpgContentData();
		CopyData(&src->contentList[i], item);
		dest->contentList->Add(item);
	}

	for( size_t i=0; i<src->dateList.size(); i++ ){
		Def::EpgSearchDateInfo^ item = gcnew Def::EpgSearchDateInfo();
		CopyData(&src->dateList[i], item);
		dest->dateList->Add(item);
	}

	for( size_t i=0; i<src->serviceList.size(); i++ ){
		dest->serviceList->Add(src->serviceList[i]);
	}

	for( size_t i=0; i<src->videoList.size(); i++ ){
		dest->videoList->Add(src->videoList[i]);
	}

	for( size_t i=0; i<src->audioList.size(); i++ ){
		dest->audioList->Add(src->audioList[i]);
	}

	dest->aimaiFlag = src->aimaiFlag;
	dest->notContetFlag = src->notContetFlag;
	dest->notDateFlag = src->notDateFlag;
	dest->freeCAFlag = src->freeCAFlag;
	dest->chkRecEnd = src->chkRecEnd;
	dest->chkRecDay = src->chkRecDay;
}

void CtrlCmdUtil::CopyData(Def::RecFileInfo^ src, REC_FILE_INFO* dest)
{
	pin_ptr<const wchar_t> recFilePathPin = PtrToStringChars(src->RecFilePath);
	pin_ptr<const wchar_t> titlePin = PtrToStringChars(src->Title);
	pin_ptr<const wchar_t> serviceNamePin = PtrToStringChars(src->ServiceName);
	pin_ptr<const wchar_t> commentPin = PtrToStringChars(src->Comment);
	pin_ptr<const wchar_t> programInfoPin = PtrToStringChars(src->ProgramInfo);
	pin_ptr<const wchar_t> errInfoPin = PtrToStringChars(src->ErrInfo);

	dest->id = src->ID;
	dest->recFilePath = recFilePathPin;
	dest->title = titlePin;
	CopyData(src->StartTime, &dest->startTime);
	dest->durationSecond = src->DurationSecond;
	dest->serviceName = serviceNamePin;
	dest->originalNetworkID = src->OriginalNetworkID;
	dest->transportStreamID = src->TransportStreamID;
	dest->serviceID = src->ServiceID;
	dest->eventID = src->EventID;
	dest->drops = src->Drops;
	dest->scrambles = src->Scrambles;
	dest->recStatus = src->RecStatus;
	CopyData(src->StartTimeEpg, &dest->startTimeEpg);
	dest->comment = commentPin;
	dest->programInfo = programInfoPin;
	dest->errInfo = errInfoPin;
	dest->protectFlag = src->ProtectFlag;
}

void CtrlCmdUtil::CopyData(REC_FILE_INFO* src, Def::RecFileInfo^% dest)
{
	dest->ID = src->id;
	dest->RecFilePath = gcnew String(src->recFilePath.c_str());
	dest->Title = gcnew String(src->title.c_str());
	dest->StartTime = ConvertTime( &src->startTime );
	dest->DurationSecond = src->durationSecond;
	dest->ServiceName = gcnew String(src->serviceName.c_str());
	dest->OriginalNetworkID = src->originalNetworkID;
	dest->TransportStreamID = src->transportStreamID;
	dest->ServiceID = src->serviceID;
	dest->EventID = src->eventID;
	dest->Drops = src->drops;
	dest->Scrambles = src->scrambles;
	dest->RecStatus = src->recStatus;
	dest->StartTimeEpg = ConvertTime( &src->startTimeEpg );
	dest->Comment = gcnew String(src->comment.c_str());
	dest->ProgramInfo = gcnew String(src->programInfo.c_str());
	dest->ErrInfo = gcnew String(src->errInfo.c_str());
	if( src->protectFlag == 1 ){
		dest->ProtectFlag = true;
	}else{
		dest->ProtectFlag = false;
	}
}

void CtrlCmdUtil::CopyData(Def::EpgAutoAddData^ src, EPG_AUTO_ADD_DATA* dest)
{
	dest->dataID = src->dataID;
	CopyData(src->searchInfo, &dest->searchInfo);
	CopyData(src->recSetting, &dest->recSetting);
	dest->addCount = src->addCount;
}

void CtrlCmdUtil::CopyData(EPG_AUTO_ADD_DATA* src, Def::EpgAutoAddData^% dest)
{
	dest->dataID = src->dataID;
	CopyData(&src->searchInfo, dest->searchInfo);
	CopyData(&src->recSetting, dest->recSetting);
	dest->addCount = src->addCount;
	
}

void CtrlCmdUtil::CopyData(Def::ManualAutoAddData^ src, MANUAL_AUTO_ADD_DATA* dest)
{
	pin_ptr<const wchar_t> titlePin = PtrToStringChars(src->title);
	pin_ptr<const wchar_t> stationNamePin = PtrToStringChars(src->stationName);

	dest->dataID = src->dataID;
	dest->dayOfWeekFlag = src->dayOfWeekFlag;
	dest->startTime = src->startTime;
	dest->durationSecond = src->durationSecond;
	dest->title = titlePin;
	dest->stationName = stationNamePin;
	dest->originalNetworkID = src->originalNetworkID;
	dest->transportStreamID = src->transportStreamID;
	dest->serviceID = src->serviceID;
	CopyData(src->recSetting, &dest->recSetting);
}

void CtrlCmdUtil::CopyData(MANUAL_AUTO_ADD_DATA* src, Def::ManualAutoAddData^% dest)
{
	dest->dataID = src->dataID;
	dest->dayOfWeekFlag = src->dayOfWeekFlag;
	dest->startTime = src->startTime;
	dest->durationSecond = src->durationSecond;
	dest->title = gcnew String(src->title.c_str());
	dest->stationName = gcnew String(src->stationName.c_str());
	dest->originalNetworkID = src->originalNetworkID;
	dest->transportStreamID = src->transportStreamID;
	dest->serviceID = src->serviceID;
	CopyData(&src->recSetting, dest->recSetting);
}

void CtrlCmdUtil::CopyData(Def::TunerReserveInfo^ src, TUNER_RESERVE_INFO* dest)
{
	pin_ptr<const wchar_t> tunerNamePin = PtrToStringChars(src->tunerName);

	dest->tunerID = src->tunerID;
	dest->tunerName = tunerNamePin;
	for( int i=0; i<src->reserveList->Count; i++ ){
		dest->reserveList.push_back(src->reserveList[i]);
	}
}

void CtrlCmdUtil::CopyData(TUNER_RESERVE_INFO* src, Def::TunerReserveInfo^% dest)
{
	dest->tunerID = src->tunerID;
	dest->tunerName = gcnew String(src->tunerName.c_str());
	for( size_t i=0; i<src->reserveList.size(); i++ ){
		dest->reserveList->Add(src->reserveList[i]);
	}
}

void CtrlCmdUtil::CopyData(Def::EpgServiceEventInfo^ src, EPGDB_SERVICE_EVENT_INFO* dest)
{
	CopyData(src->serviceInfo, &dest->serviceInfo);
	for( int i=0; i<src->eventList->Count; i++ ){
		EPGDB_EVENT_INFO* item = new EPGDB_EVENT_INFO;
		CopyData(src->eventList[i], item);
		dest->eventList.push_back(item);
	}
}

void CtrlCmdUtil::CopyData(EPGDB_SERVICE_EVENT_INFO* src, Def::EpgServiceEventInfo^% dest)
{
	CopyData(&src->serviceInfo, dest->serviceInfo);
	for( size_t i=0; i<src->eventList.size(); i++ ){
		Def::EpgEventInfo^ item = gcnew Def::EpgEventInfo();
		CopyData(src->eventList[i], item);
		dest->eventList->Add(item);
	}
}

void CtrlCmdUtil::CopyData(Def::SetChInfo^ src, SET_CH_INFO* dest)
{
	dest->useSID = src->useSID;
	dest->ONID = src->ONID;
	dest->TSID = src->TSID;
	dest->SID = src->SID;
	dest->useBonCh = src->useBonCh;
	dest->space = src->space;
	dest->ch = src->ch;
}

void CtrlCmdUtil::CopyData(SET_CH_INFO* src, Def::SetChInfo^% dest)
{
	dest->useSID = src->useSID;
	dest->ONID = src->ONID;
	dest->TSID = src->TSID;
	dest->SID = src->SID;
	dest->useBonCh = src->useBonCh;
	dest->space = src->space;
	dest->ch = src->ch;
}

void CtrlCmdUtil::CopyData(Def::TvTestChChgInfo^ src, TVTEST_CH_CHG_INFO* dest)
{
	pin_ptr<const wchar_t> bonDriverPin = PtrToStringChars(src->bonDriver);

	dest->bonDriver = bonDriverPin;
	CopyData(src->chInfo, &dest->chInfo);
}

void CtrlCmdUtil::CopyData(TVTEST_CH_CHG_INFO* src, Def::TvTestChChgInfo^% dest)
{
	dest->bonDriver = gcnew String(src->bonDriver.c_str());
	CopyData(&src->chInfo, dest->chInfo);
}

void CtrlCmdUtil::CopyData(Def::TVTestStreamingInfo^ src, TVTEST_STREAMING_INFO* dest)
{
	pin_ptr<const wchar_t> filePathPin = PtrToStringChars(src->filePath);

	dest->filePath = filePathPin;

	dest->enableMode = src->enableMode;
	dest->ctrlID = src->ctrlID;
	dest->serverIP = src->serverIP;
	dest->serverPort = src->serverPort;
	dest->udpSend = src->udpSend;
	dest->tcpSend = src->tcpSend;
	dest->timeShiftMode = src->timeShiftMode;
}

void CtrlCmdUtil::CopyData(TVTEST_STREAMING_INFO* src, Def::TVTestStreamingInfo^% dest)
{
	dest->filePath = gcnew String(src->filePath.c_str());
	dest->enableMode = src->enableMode;
	dest->ctrlID = src->ctrlID;
	dest->serverIP = src->serverIP;
	dest->serverPort = src->serverPort;
	dest->udpSend = src->udpSend;
	dest->tcpSend = src->tcpSend;
	dest->timeShiftMode = src->timeShiftMode;
}

void CtrlCmdUtil::CopyData(Def::NWPlayTimeShiftInfo^ src, NWPLAY_TIMESHIFT_INFO* dest)
{
	pin_ptr<const wchar_t> filePathPin = PtrToStringChars(src->filePath);

	dest->filePath = filePathPin;
	dest->ctrlID = src->ctrlID;
}

void CtrlCmdUtil::CopyData(NWPLAY_TIMESHIFT_INFO* src, Def::NWPlayTimeShiftInfo^% dest)
{
	dest->filePath = gcnew String(src->filePath.c_str());
	dest->ctrlID = src->ctrlID;
}

/// <summary>
/// �R�}���h���M���@�̐ݒ�
/// </summary>
/// <param name="tcpFlag">[IN]true�FTCP/IP���[�h�Afalse�F���O�t���p�C�v���[�h</param>
void CtrlCmdUtil::SetSendMode(
	bool tcpFlag
	)
{
	BOOL flag = TRUE;
	if( tcpFlag == false ){
		flag = FALSE;
	}
	this->sendCmd->SetSendMode(flag);
}

/// <summary>
/// ���O�t���p�C�v���[�h���̐ڑ����ݒ�
/// </summary>
/// <param name="eventName">[IN]�r������pEvent�̖��O</param>
/// <param name="pipeName">[IN]�ڑ��p�C�v�̖��O</param>
void CtrlCmdUtil::SetPipeSetting(
	String^ eventName,
	String^ pipeName
	)
{
	pin_ptr<const wchar_t> eventNamePin = PtrToStringChars(eventName);
	std::wstring _eventName(eventNamePin);
	pin_ptr<const wchar_t> pipeNamePin = PtrToStringChars(pipeName);
	std::wstring _pipeName(pipeNamePin);

	this->sendCmd->SetPipeSetting(_eventName, _pipeName);
}

/// <summary>
/// TCP/IP���[�h���̐ڑ����ݒ�
/// </summary>
/// <param name="ip">[IN]�ڑ���IP</param>
/// <param name="port">[IN]�ڑ���|�[�g</param>
void CtrlCmdUtil::SetNWSetting(
	String^ ip,
	UInt32 port
	)
{
	pin_ptr<const wchar_t> ipPin = PtrToStringChars(ip);
	std::wstring _ip(ipPin);

	this->sendCmd->SetNWSetting(_ip, port);
}

/// <summary>
/// �ڑ��������̃^�C���A�E�g�ݒ�
/// </summary>
/// <param name="timeOut">[IN]�^�C���A�E�g�l�i�P�ʁFms�j</param>
void CtrlCmdUtil::SetConnectTimeOut(
	UInt32 timeOut
	)
{
	this->sendCmd->SetConnectTimeOut(timeOut);
}

/// <summary>
/// Program.txt��ǉ��ōēǂݍ��݂���
/// </summary>
UInt32 CtrlCmdUtil::SendAddloadReserve()
{
	return this->sendCmd->SendAddloadReserve();
}

/// <summary>
/// EPG�f�[�^���ēǂݍ��݂���
/// </summary>
UInt32 CtrlCmdUtil::SendReloadEpg()
{
	return this->sendCmd->SendReloadEpg();
}

/// <summary>
/// �ݒ�����ēǂݍ��݂���
/// </summary>
UInt32 CtrlCmdUtil::SendReloadSetting()
{
	return this->sendCmd->SendReloadSetting();
}

/// <summary>
/// EpgTimerSrv.exe���I������
/// </summary>
UInt32 CtrlCmdUtil::SendClose()
{
	return this->sendCmd->SendClose();
}

/// <summary>
/// EpgTimerSrv.exe�̃p�C�v�ڑ�GUI�Ƃ��ăv���Z�X��o�^����
/// </summary>
/// <param name="processID">[IN]�v���Z�XID</param>
UInt32 CtrlCmdUtil::SendRegistGUI(
	UInt32 processID
	)
{
	return this->sendCmd->SendRegistGUI(processID);
}

/// <summary>
/// EpgTimerSrv.exe�̃p�C�v�ڑ�GUI�o�^����������
/// </summary>
/// <param name="processID">[IN]�v���Z�XID</param>
UInt32 CtrlCmdUtil::SendUnRegistGUI(
	UInt32 processID
	)
{
	return this->sendCmd->SendUnRegistGUI(processID);
}

/// <summary>
/// EpgTimerSrv.exe��TCP�ڑ�GUI�Ƃ��ăv���Z�X��o�^����
/// </summary>
/// <param name="port">[IN]�|�[�g</param>
UInt32 CtrlCmdUtil::SendRegistTCP(
	UInt32 port
	)
{
	return this->sendCmd->SendRegistTCP(port);
}

/// <summary>
/// EpgTimerSrv.exe��TCP�ڑ�GUI�o�^����������
/// </summary>
/// <param name="port">[IN]�|�[�g</param>
UInt32 CtrlCmdUtil::SendUnRegistTCP(
	UInt32 port
	)
{
	return this->sendCmd->SendUnRegistTCP(port);
}

/// <summary>
/// �\��ꗗ���擾����
/// </summary>
/// <param name="val">[OUT]�\��ꗗ</param>
UInt32 CtrlCmdUtil::SendEnumReserve(
	List<Def::ReserveData^>^% val
	)
{
	vector<RESERVE_DATA> list;
	DWORD ret = this->sendCmd->SendEnumReserve2(&list);
	for( size_t i=0; i<list.size(); i++ ){
		Def::ReserveData^ item = gcnew Def::ReserveData();
		CopyData(&list[i], item);
		val->Add(item);
	}

	return ret;
}

/// <summary>
/// �\������擾����
/// </summary>
/// <param name="reserveID">[IN]�擾������̗\��ID</param>
/// <param name="val">[OUT]�\����</param>
UInt32 CtrlCmdUtil::SendGetReserve(
	UInt32 reserveID,
	Def::ReserveData^% val
	)
{
	RESERVE_DATA info;
	DWORD ret = this->sendCmd->SendGetReserve2(reserveID, &info);
	if( ret == CMD_SUCCESS ){
		CopyData(&info, val);
	}
	return ret;
}

/// <summary>
/// �\���ǉ�����
/// </summary>
/// <param name="val">[IN]�ǉ�����\��ꗗ</param>
UInt32 CtrlCmdUtil::SendAddReserve(
	List<Def::ReserveData^>^ val
	)
{
	vector<RESERVE_DATA> list;
	for( int i=0; i<val->Count; i++ ){
		RESERVE_DATA item;
		CopyData(val[i], &item);
		list.push_back(item);
	}
	DWORD ret = this->sendCmd->SendAddReserve2(&list);

	return ret;
}

/// <summary>
/// �\����폜����
/// </summary>
/// <param name="val">[IN]�폜����\��ID�ꗗ</param>
UInt32 CtrlCmdUtil::SendDelReserve(
	List<UInt32>^ val
	)
{
	vector<DWORD> list;
	for( int i=0; i<val->Count; i++ ){
		list.push_back(val[i]);
	}
	DWORD ret = this->sendCmd->SendDelReserve(&list);

	return ret;
}

/// <summary>
/// �\���ύX����
/// </summary>
/// <param name="val">[IN]�ύX����\��ꗗ</param>
UInt32 CtrlCmdUtil::SendChgReserve(
	List<Def::ReserveData^>^ val
	)
{
	vector<RESERVE_DATA> list;
	for( int i=0; i<val->Count; i++ ){
		RESERVE_DATA item;
		CopyData(val[i], &item);
		list.push_back(item);
	}
	DWORD ret = this->sendCmd->SendChgReserve2(&list);

	return ret;
}


/// <summary>
/// �`���[�i�[���Ƃ̗\��ꗗ���擾����
/// </summary>
/// <param name="val">[OUT]�\����</param>
UInt32 CtrlCmdUtil::SendEnumTunerReserve(
	List<Def::TunerReserveInfo^>^% val
	)
{
	vector<TUNER_RESERVE_INFO> list;
	DWORD ret = this->sendCmd->SendEnumTunerReserve(&list);
	if( ret == CMD_SUCCESS ){
		for( size_t i=0; i<list.size(); i++ ){
			Def::TunerReserveInfo^ item = gcnew Def::TunerReserveInfo();
			CopyData(&list[i], item);
			val->Add(item);
		}
	}

	return ret;
}

/// <summary>
/// �^��ςݏ��ꗗ���擾����
/// </summary>
/// <param name="val">[OUT]�^��ςݏ��ꗗ</param>
UInt32 CtrlCmdUtil::SendEnumRecInfo(
	List<Def::RecFileInfo^>^% val
	)
{
	vector<REC_FILE_INFO> list;
	DWORD ret = this->sendCmd->SendEnumRecInfo2(&list);
	if( ret == CMD_SUCCESS ){
		for( size_t i=0; i<list.size(); i++ ){
			Def::RecFileInfo^ item = gcnew Def::RecFileInfo();
			CopyData(&list[i], item);
			val->Add(item);
		}
	}

	return ret;
}

/// <summary>
/// �^��ςݏ����폜����
/// </summary>
/// <param name="val">[IN]�폜����ID�ꗗ</param>
UInt32 CtrlCmdUtil::SendDelRecInfo(
	List<UInt32>^ val
	)
{
	vector<DWORD> list;
	for( int i=0; i<val->Count; i++ ){
		list.push_back(val[i]);
	}
	DWORD ret = this->sendCmd->SendDelRecInfo(&list);

	return ret;
}

/// <summary>
/// �\���ύX����
/// </summary>
/// <param name="val">[IN]�ύX����\��ꗗ</param>
UInt32 CtrlCmdUtil::SendChgProtectRecInfo(
	List<Def::RecFileInfo^>^ val
	)
{
	vector<REC_FILE_INFO> list;
	for( int i=0; i<val->Count; i++ ){
		REC_FILE_INFO item;
		CopyData(val[i], &item);
		list.push_back(item);
	}
	DWORD ret = this->sendCmd->SendChgProtectRecInfo2(&list);

	return ret;
}

/// <summary>
/// �T�[�r�X�ꗗ���擾����
/// </summary>
/// <param name="val">[OUT]�T�[�r�X�ꗗ</param>
UInt32 CtrlCmdUtil::SendEnumService(
	List<Def::EpgServiceInfo^>^% val
	)
{
	vector<EPGDB_SERVICE_INFO> list;
	DWORD ret = this->sendCmd->SendEnumService(&list);
	if( ret == CMD_SUCCESS ){
		for( size_t i=0; i<list.size(); i++ ){
			Def::EpgServiceInfo^ item = gcnew Def::EpgServiceInfo();
			CopyData(&list[i], item);
			val->Add(item);
		}
	}

	return ret;
}

/// <summary>
/// �T�[�r�X�w��Ŕԑg�����ꗗ���擾����
/// </summary>
/// <param name="service">[IN]ONID����32 | TSID����16 | SID�Ƃ����T�[�r�XID</param>
/// <param name="val">[OUT]�ԑg���ꗗ</param>
UInt32 CtrlCmdUtil::SendEnumPgInfo(
	UInt64 service,
	List<Def::EpgEventInfo^>^% val
	)
{
	vector<EPGDB_EVENT_INFO*> list;
	DWORD ret = this->sendCmd->SendEnumPgInfo(service, &list);
	if( ret == CMD_SUCCESS ){
		for( size_t i=0; i<list.size(); i++ ){
			Def::EpgEventInfo^ item = gcnew Def::EpgEventInfo();
			CopyData(list[i], item);
			val->Add(item);
			SAFE_DELETE(list[i]);
		}
	}

	return ret;
}

/// <summary>
/// �w��C�x���g�̔ԑg�����擾����
/// </summary>
/// <param name="pgID">[IN]ONID����48 | TSID����32 | SID����16 | EventID�Ƃ���ID</param>
/// <param name="val">[OUT]�ԑg���</param>
UInt32 CtrlCmdUtil::SendGetPgInfo(
	UInt64 pgID,
	Def::EpgEventInfo^% val
	)
{
	EPGDB_EVENT_INFO info;
	DWORD ret = this->sendCmd->SendGetPgInfo(pgID, &info);
	if( ret == CMD_SUCCESS ){
		CopyData(&info, val);
	}

	return ret;
}

/// <summary>
/// �w��C�x���g�̔ԑg�����擾����
/// </summary>
/// <param name="key">[IN]�����L�[�i�����w�莞�͂܂Ƃ߂Č������ʂ��Ԃ�j</param>
/// <param name="val">[OUT]�ԑg���ꗗ</param>
UInt32 CtrlCmdUtil::SendSearchPg(
	List<Def::EpgSearchKeyInfo^>^ key,
	List<Def::EpgEventInfo^>^% val
	)
{
	vector<EPGDB_SEARCH_KEY_INFO> keyList;
	vector<EPGDB_EVENT_INFO*> list;

	for( int i=0; i<key->Count; i++ ){
		EPGDB_SEARCH_KEY_INFO item;
		CopyData(key[i], &item);
		keyList.push_back(item);
	}

	DWORD ret = this->sendCmd->SendSearchPg(&keyList, &list);
	if( ret == CMD_SUCCESS ){
		for( size_t i=0; i<list.size(); i++ ){
			Def::EpgEventInfo^ item = gcnew Def::EpgEventInfo();
			CopyData(list[i], item);
			val->Add(item);
			SAFE_DELETE(list[i]);
		}
	}

	return ret;
}

/// <summary>
/// �X�^���o�C�A�x�~�A�V���b�g�_�E�����s���Ă������̊m�F
/// </summary>
UInt32 CtrlCmdUtil::SendChkSuspend(
	)
{
	return this->sendCmd->SendChkSuspend();
}

/// <summary>
/// �X�^���o�C�A�x�~�A�V���b�g�_�E���Ɉڍs����
/// </summary>
/// <param name="val">[IN]1:�X�^���o�C 2:�x�~ 3:�V���b�g�_�E�� | 0x0100:���A��ċN��</param>
UInt32 CtrlCmdUtil::SendSuspend(
	UInt16 val
	)
{
	return this->sendCmd->SendSuspend(val);
}

/// <summary>
/// PC�ċN�����s��
/// </summary>
UInt32 CtrlCmdUtil::SendReboot(
	)
{
	return this->sendCmd->SendReboot();
}

/// <summary>
/// 10�b���EPG�f�[�^�̎擾���s��
/// </summary>
UInt32 CtrlCmdUtil::SendEpgCapNow(
	)
{
	return this->sendCmd->SendEpgCapNow();
}

/// <summary>
/// �����\��o�^�����ꗗ���擾����
/// </summary>
/// <param name="val">[OUT]�����ꗗ</param>
UInt32 CtrlCmdUtil::SendEnumEpgAutoAdd(
	List<Def::EpgAutoAddData^>^% val
	)
{
	vector<EPG_AUTO_ADD_DATA> list;
	DWORD ret = this->sendCmd->SendEnumEpgAutoAdd2(&list);
	for( size_t i=0; i<list.size(); i++ ){
		Def::EpgAutoAddData^ item = gcnew Def::EpgAutoAddData();
		CopyData(&list[i], item);
		val->Add(item);
	}

	return ret;
}

/// <summary>
/// �\���ǉ�����
/// </summary>
/// <param name="val">[IN]�����ꗗ</param>
UInt32 CtrlCmdUtil::SendAddEpgAutoAdd(
	List<Def::EpgAutoAddData^>^ val
	)
{
	vector<EPG_AUTO_ADD_DATA> list;
	for( int i=0; i<val->Count; i++ ){
		EPG_AUTO_ADD_DATA item;
		CopyData(val[i], &item);
		list.push_back(item);
	}
	DWORD ret = this->sendCmd->SendAddEpgAutoAdd2(&list);

	return ret;
}

/// <summary>
/// �\����폜����
/// </summary>
/// <param name="val">[IN]ID�ꗗ</param>
UInt32 CtrlCmdUtil::SendDelEpgAutoAdd(
	List<UInt32>^ val
	)
{
	vector<DWORD> list;
	for( int i=0; i<val->Count; i++ ){
		list.push_back(val[i]);
	}
	DWORD ret = this->sendCmd->SendDelEpgAutoAdd(&list);

	return ret;
}

/// <summary>
/// �\���ύX����
/// </summary>
/// <param name="val">[IN]�����ꗗ</param>
UInt32 CtrlCmdUtil::SendChgEpgAutoAdd(
	List<Def::EpgAutoAddData^>^ val
	)
{
	vector<EPG_AUTO_ADD_DATA> list;
	for( int i=0; i<val->Count; i++ ){
		EPG_AUTO_ADD_DATA item;
		CopyData(val[i], &item);
		list.push_back(item);
	}
	DWORD ret = this->sendCmd->SendChgEpgAutoAdd2(&list);

	return ret;
}

/// <summary>
/// �����\��o�^�����ꗗ���擾����
/// </summary>
/// <param name="val">[OUT]�����ꗗ</param>
UInt32 CtrlCmdUtil::SendEnumManualAdd(
	List<Def::ManualAutoAddData^>^% val
	)
{
	vector<MANUAL_AUTO_ADD_DATA> list;
	DWORD ret = this->sendCmd->SendEnumManualAdd2(&list);
	for( size_t i=0; i<list.size(); i++ ){
		Def::ManualAutoAddData^ item = gcnew Def::ManualAutoAddData();
		CopyData(&list[i], item);
		val->Add(item);
	}

	return ret;
}

/// <summary>
/// �����\��o�^������ǉ�����
/// </summary>
/// <param name="val">[IN]�����ꗗ</param>
UInt32 CtrlCmdUtil::SendAddManualAdd(
	List<Def::ManualAutoAddData^>^ val
	)
{
	vector<MANUAL_AUTO_ADD_DATA> list;
	for( int i=0; i<val->Count; i++ ){
		MANUAL_AUTO_ADD_DATA item;
		CopyData(val[i], &item);
		list.push_back(item);
	}
	DWORD ret = this->sendCmd->SendAddManualAdd2(&list);

	return ret;
}

/// <summary>
/// �����\��o�^�������폜����
/// </summary>
/// <param name="val">[IN]ID�ꗗ</param>
UInt32 CtrlCmdUtil::SendDelManualAdd(
	List<UInt32>^ val
	)
{
	vector<DWORD> list;
	for( int i=0; i<val->Count; i++ ){
		list.push_back(val[i]);
	}
	DWORD ret = this->sendCmd->SendDelManualAdd(&list);

	return ret;
}

/// <summary>
/// �����\��o�^������ύX����
/// </summary>
/// <param name="val">[IN]�����ꗗ</param>
UInt32 CtrlCmdUtil::SendChgManualAdd(
	List<Def::ManualAutoAddData^>^ val
	)
{
	vector<MANUAL_AUTO_ADD_DATA> list;
	for( int i=0; i<val->Count; i++ ){
		MANUAL_AUTO_ADD_DATA item;
		CopyData(val[i], &item);
		list.push_back(item);
	}
	DWORD ret = this->sendCmd->SendChgManualAdd2(&list);

	return ret;
}

/// <summary>
/// �w��t�@�C����]������
/// </summary>
/// <param name="val">[IN]�t�@�C����</param>
/// <param name="resVal">[OUT]�o�C�i��</param>
UInt32 CtrlCmdUtil::SendFileCopy(
	String^ val,
	[Runtime::InteropServices::Out]cli::array<byte>^% resVal
	)
{
	pin_ptr<const wchar_t> valPin = PtrToStringChars(val);
	std::wstring _val(valPin);

	DWORD resValSize = 0;
	BYTE* _resVal = NULL;
	DWORD ret = this->sendCmd->SendFileCopy(_val, &_resVal, &resValSize);
	if( ret == CMD_SUCCESS ){
		resVal = gcnew cli::array<byte>(resValSize);
		System::Runtime::InteropServices::Marshal::Copy(IntPtr(_resVal), resVal, 0, resValSize);
	}

	return ret;
}

/// <summary>
/// �ԑg���ꗗ���擾����
/// </summary>
/// <param name="val">[OUT]�ԑg���ꗗ</param>
UInt32 CtrlCmdUtil::SendEnumPgAll(
	List<Def::EpgServiceEventInfo^>^% val
	)
{
	vector<EPGDB_SERVICE_EVENT_INFO*> _val;
	DWORD ret = this->sendCmd->SendEnumPgAll(&_val);
	if( ret == CMD_SUCCESS ){
		for( size_t i=0; i<_val.size(); i++ ){
			Def::EpgServiceEventInfo^ item = gcnew Def::EpgServiceEventInfo();
			CopyData(_val[i], item);
			val->Add(item);
			for( size_t j=0; j<_val[i]->eventList.size(); j++ ){
				SAFE_DELETE(_val[i]->eventList[j]);
			}
			SAFE_DELETE(_val[i]);
		}
	}
	return ret;
}


/// <summary>
/// PlugIn�t�@�C���̈ꗗ���擾����
/// </summary>
/// <param name="val">[IN]1:ReName�A2:Write</param>
/// <param name="resVal">[OUT]�t�@�C���ꗗ</param>
UInt32 CtrlCmdUtil::SendEnumPlugIn(
	UInt16 val,
	List<String^>^% resVal
	)
{
	vector<wstring> list;
	DWORD ret = this->sendCmd->SendEnumPlugIn(val, &list);
	if( ret == CMD_SUCCESS ){
		for( size_t i=0; i<list.size(); i++ ){
			resVal->Add(gcnew String(list[i].c_str()));
		}
	}

	return ret;
}

/// <summary>
/// TVTest�̃`�����l���؂�ւ��p�̏����擾����
/// </summary>
/// <param name="val">[IN]ONID����32 | TSID����16 | SID�Ƃ����T�[�r�XID</param>
/// <param name="resVal">[OUT]�`�����l�����</param>
UInt32 CtrlCmdUtil::SendGetChgChTVTest(
	UInt64 val,
	Def::TvTestChChgInfo^% resVal
	)
{
	TVTEST_CH_CHG_INFO _resVal;
	DWORD ret = this->sendCmd->SendGetChgChTVTest(val, &_resVal);
	if( ret == CMD_SUCCESS ){
		CopyData(&_resVal, resVal);
	}

	return ret;
}

/// <summary>
/// �l�b�g���[�N���[�h��EpgDataCap_Bon�̃`�����l����؂�ւ�
/// </summary>
/// <param name="val">[IN]�`�����l�����</param>
UInt32 CtrlCmdUtil::SendNwTVSetCh(
	Def::SetChInfo^ val
	)
{
	SET_CH_INFO chInfo;
	CopyData(val, &chInfo);
	DWORD ret = this->sendCmd->SendNwTVSetCh(&chInfo);

	return ret;
}

/// <summary>
/// �l�b�g���[�N���[�h�ŋN������EpgDataCap_Bon���I��
/// </summary>
UInt32 CtrlCmdUtil::SendNwTVClose(
	)
{
	DWORD ret = this->sendCmd->SendNwTVClose();

	return ret;
}

/// <summary>
/// �l�b�g���[�N���[�h�ŋN������Ƃ��̃��[�h
/// </summary>
/// <param name="val">[IN]���[�h�i1:UDP 2:TCP 3:UDP+TCP�j</param>
UInt32 CtrlCmdUtil::SendNwTVMode(
	UInt32 val
	)
{
	DWORD ret = this->sendCmd->SendNwTVMode(val);

	return ret;
}

/// <summary>
/// �^��t�@�C���̃l�b�g���[�N�p�X���擾
/// </summary>
/// <param name="path">[IN]�t�@�C���p�X</param>
/// <param name="resVal">[OUT]�l�b�g���[�N�p�X</param>
UInt32 CtrlCmdUtil::SendGetRecFileNetworkPath(
	String^ val,
	String^% resVal
	)
{
	pin_ptr<const wchar_t> valPin = PtrToStringChars(val);
	std::wstring _val(valPin);
	wstring _resVal = L"";
	DWORD ret = this->sendCmd->SendGetRecFileNetworkPath(_val, &_resVal);
	if( ret == CMD_SUCCESS ){
		resVal = gcnew String(_resVal.c_str());
	}

	return ret;
}

/// <summary>
/// BonDriver�̐؂�ւ�
/// </summary>
/// <param name="val">[IN]BonDriver�t�@�C����</param>
UInt32 CtrlCmdUtil::SendViewSetBonDrivere(
	String^ val
	)
{
	pin_ptr<const wchar_t> valPin = PtrToStringChars(val);
	std::wstring _val(valPin);

	DWORD ret = this->sendCmd->SendViewSetBonDrivere(valPin);

	return ret;
}

/// <summary>
/// �g�p����BonDriver�̃t�@�C�������擾
/// </summary>
/// <param name="val">[OUT]BonDriver�t�@�C����</param>
UInt32 CtrlCmdUtil::SendViewGetBonDrivere(
	String^% val
	)
{
	wstring resVal = L"";
	DWORD ret = this->sendCmd->SendViewGetBonDrivere(&resVal);
	if( ret == CMD_SUCCESS ){
		val = gcnew String(resVal.c_str());
	}

	return ret;
}

/// <summary>
/// �`�����l���؂�ւ�
/// </summary>
/// <param name="val">[IN]�`�����l�����</param>
UInt32 CtrlCmdUtil::SendViewSetCh(
	Def::SetChInfo^ val
	)
{
	SET_CH_INFO chInfo;
	CopyData(val, &chInfo);
	DWORD ret = this->sendCmd->SendViewSetCh(&chInfo);

	return ret;
}

/// <summary>
/// �A�v���P�[�V�����̏I��
/// </summary>
UInt32 CtrlCmdUtil::SendViewAppClose(
	)
{
	DWORD ret = this->sendCmd->SendViewAppClose();

	return ret;
}

/// <summary>
/// �X�g���[���z�M�p�t�@�C�����J��
/// </summary>
/// <param name="val">[IN]�J���t�@�C���̃T�[�o�[���t�@�C���p�X</param>
/// <param name="resVal">[OUT]����pCtrlID</param>
UInt32 CtrlCmdUtil::SendNwPlayOpen(
	String^ val,
	UInt32% resVal
	)
{
	pin_ptr<const wchar_t> valPin = PtrToStringChars(val);
	std::wstring _val(valPin);

	DWORD ctrlID = 0;
	DWORD ret = this->sendCmd->SendNwPlayOpen(valPin, &ctrlID);
	resVal = ctrlID;

	return ret;
}

/// <summary>
/// �X�g���[�~���O�z�M����ID�̐ݒ�
/// </summary>
/// <param name="val">[IN]�X�g���[�~���O�z�M������</param>
UInt32 CtrlCmdUtil::SendViewSetStreamingInfo(
	Def::TVTestStreamingInfo^ val
	)
{
	TVTEST_STREAMING_INFO info;
	CopyData(val, &info);
	DWORD ret = this->sendCmd->SendViewSetStreamingInfo(&info);

	return ret;
}

/// <summary>
/// �X�g���[���z�M�p�t�@�C�����^�C���V�t�g���[�h�ŊJ��
/// </summary>
/// <param name="val">[IN]�\��ID</param>
/// <param name="resVal">[OUT]����pCtrlID</param>
UInt32 CtrlCmdUtil::SendNwTimeShiftOpen(
	UInt32 val,
	Def::NWPlayTimeShiftInfo^% resVal
	)
{
	NWPLAY_TIMESHIFT_INFO _resVal;
	DWORD ret = this->sendCmd->SendNwTimeShiftOpen(val, &_resVal);
	if( ret == CMD_SUCCESS ){
		CopyData(&_resVal, resVal);
	}

	return ret;
}
