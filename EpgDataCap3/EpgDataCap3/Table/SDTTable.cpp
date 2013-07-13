#include "StdAfx.h"
#include "SDTTable.h"

#include "../../../Common/EpgTimerUtil.h"
#include "../Descriptor/Descriptor.h"
#include "../ARIB8CharDecode.h"

CSDTTable::CSDTTable(void)
{
}

CSDTTable::~CSDTTable(void)
{
	Clear();
}

void CSDTTable::Clear()
{
	for( size_t i=0 ;i<serviceInfoList.size(); i++ ){
		SAFE_DELETE(serviceInfoList[i]);
	}
	serviceInfoList.clear();
}

BOOL CSDTTable::Decode( BYTE* data, DWORD dataSize, DWORD* decodeReadSize )
{
	if( data == NULL ){
		return FALSE;
	}
	Clear();

	//////////////////////////////////////////////////////
	//�T�C�Y�̃`�F�b�N
	//�Œ��table_id��section_length+CRC�̃T�C�Y�͕K�{
	if( dataSize < 7 ){
		return FALSE;
	}
	//->�T�C�Y�̃`�F�b�N

	DWORD readSize = 0;
	//////////////////////////////////////////////////////
	//��͏���
	table_id = data[0];
	section_syntax_indicator = (data[1]&0x80)>>7;
	section_length = ((WORD)data[1]&0x0F)<<8 | data[2];
	readSize+=3;

	if( section_syntax_indicator != 1 ){
		//�Œ�l����������
		_OutputDebugString( L"++CSDTTable:: section_syntax err" );
		return FALSE;
	}
	if( table_id != 0x42 && table_id != 0x46 ){
		//table_id����������
		_OutputDebugString( L"++CSDTTable:: table_id err 0x%02X", table_id );
		return FALSE;
	}
	if( readSize+section_length > dataSize && section_length > 3){
		//�T�C�Y�ُ�
		_OutputDebugString( L"++CSDTTable:: size err %d > %d", readSize+section_length, dataSize );
		return FALSE;
	}
	//CRC�`�F�b�N
	crc32 = ((DWORD)data[3+section_length-4])<<24 |
		((DWORD)data[3+section_length-3])<<16 |
		((DWORD)data[3+section_length-2])<<8 |
		data[3+section_length-1];
	if( crc32 != _Crc32(3+section_length-4, data) ){
		_OutputDebugString( L"++CSDTTable:: CRC err" );
		return FALSE;
	}

	if( section_length > 4 ){
		transport_stream_id = ((WORD)data[readSize])<<8 | data[readSize+1];
		version_number = (data[readSize+2]&0x3E)>>1;
		current_next_indicator = data[readSize+2]&0x01;
		section_number = data[readSize+3];
		last_section_number = data[readSize+4];
		original_network_id = ((WORD)data[readSize+5])<<8 | data[readSize+6];
		readSize += 8;
		while( readSize < (DWORD)section_length+3-4 ){
			SERVICE_INFO_DATA* item = new SERVICE_INFO_DATA;
			//if( original_network_id == 0x0001 || original_network_id == 0x0003 || original_network_id == 0x000A ){
			//	item->service_id = ((WORD)data[readSize]&0x0F)<<8 | data[readSize+1];
			//}else{
				item->service_id = ((WORD)data[readSize])<<8 | data[readSize+1];
			//}
			item->EIT_user_defined_flags = (data[readSize+2]&0x1C)>>2;
			item->EIT_schedule_flag = (data[readSize+2]&0x02)>>1;
			item->EIT_present_following_flag = data[readSize+2]&0x01;
			item->running_status = (data[readSize+3]&0xE0)>>5;
			item->free_CA_mode = (data[readSize+3]&0x10)>>4;
			item->descriptors_loop_length = ((WORD)data[readSize+3]&0x0F)<<8 | data[readSize+4];
			readSize += 5;
			BOOL err = TRUE;
			if( readSize+item->descriptors_loop_length <= (DWORD)section_length+3-4 && item->descriptors_loop_length > 0){
				if( original_network_id == 0x0001 || original_network_id == 0x0003){
					if( SDDecode( data+readSize, item->descriptors_loop_length, &(item->descriptorList), NULL ) == TRUE ){
						err = FALSE;
					}
				}
				CDescriptor descriptor;
				if( descriptor.Decode( data+readSize, item->descriptors_loop_length, &(item->descriptorList), NULL ) == TRUE ){
					err = FALSE;
				}
			}

			readSize+=item->descriptors_loop_length;
			if( err == TRUE ){
				_OutputDebugString( L"++CSDTTable:: descriptor2 err" );
				SAFE_DELETE(item);
				return FALSE;
			}
			serviceInfoList.push_back(item);

		}
	}else{
		return FALSE;
	}
	//->��͏���

	if( decodeReadSize != NULL ){
		*decodeReadSize = 3+section_length;
	}

	return TRUE;
}

BOOL CSDTTable::SDDecode( BYTE* data, DWORD dataSize, vector<DESCRIPTOR_DATA*>* descriptorList, DWORD* decodeReadSize )
{
	BOOL ret = TRUE;
	if( data == NULL || dataSize == 0 || descriptorList == NULL ){
		return FALSE;
	}
	DWORD decodeSize = 0;

	DESCRIPTOR_DATA* item = new DESCRIPTOR_DATA;
	item->service = new CServiceDesc;

	while( decodeSize < dataSize ){
		BYTE* readPos = data+decodeSize;
		if( readPos[0] == 0x8A ){
			//�T�[�r�X�^�C�v
			item->service->service_type = readPos[2];
			//if( item->service->service_type == 0x81 ){
			//	item->service->service_type = 0xA1;
			//}
			decodeSize += readPos[1]+2;
		}else
		if( readPos[0] == 0x82 ){
			//�T�[�r�X��
			if( readPos[2] == 0x01 ){
				//���{��ŁH
				item->service->service_name_length = readPos[1]-1;
				item->service->char_service_name = new CHAR[item->service->service_name_length];
				memcpy(item->service->char_service_name, readPos+3, item->service->service_name_length);
			}
			decodeSize += readPos[1]+2;
		}else{
			decodeSize += readPos[1]+2;
		}
	}
	if( item->service->service_name_length == 0 ){
		SAFE_DELETE(item);
		ret = FALSE;
	}else{
		descriptorList->push_back(item);
	}

	if( decodeReadSize != NULL ){
		*decodeReadSize = dataSize;
	}
	return ret;
}

