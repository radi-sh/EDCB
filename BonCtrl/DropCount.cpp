#include "StdAfx.h"
#include "DropCount.h"


CDropCount::CDropCount(void)
{
	this->drop = 0;
	this->dropMp2Stream = 0;
	this->scramble = 0;
	this->lastLogTime = 0;

	this->lastLogDrop = 0;
	this->lastLogScramble = 0;

	this->signalLv = 0;
	this->bonFile = L"";
}


CDropCount::~CDropCount(void)
{
}

void CDropCount::AddData(BYTE* data, DWORD size)
{
	if( data == NULL || size == 0 ){
		return ;
	}
	for( DWORD i=0; i<size; i+=188 ){
		CTSPacketUtil packet;
		if( packet.Set188TS(data+i, 188) == TRUE ){
			map<WORD, DROP_INFO>::iterator itr;
			itr = this->infoMap.find(packet.PID);
			if( itr == this->infoMap.end() ){
				DROP_INFO item;
				item.PID = packet.PID;
				this->infoMap.insert(pair<WORD, DROP_INFO>(item.PID, item));
				CheckCounter(&packet, &item);
			}
			else{
				CheckCounter(&packet, &(itr->second));
			}
		}
	}
}

void CDropCount::Clear()
{
	this->infoMap.clear();
	this->drop = 0;
	this->dropMp2Stream = 0;
	this->scramble = 0;
	this->log.clear();
	this->lastLogTime = 0;

	this->lastLogDrop = 0;
	this->lastLogScramble = 0;
	this->signalLv = 0;
}

void CDropCount::SetSignal(float level)
{
	this->signalLv = level;
}

void CDropCount::SetBonDriver(wstring bonDriver)
{
	this->bonFile = bonDriver;
}

void CDropCount::GetCount(ULONGLONG* drop, ULONGLONG* scramble)
{
	if( drop != NULL ){
		*drop = this->dropMp2Stream;
	}
	if( scramble != NULL ){
		*scramble = this->scramble;
	}
}

ULONGLONG CDropCount::GetDropCount()
{
	return this->dropMp2Stream;
}

ULONGLONG CDropCount::GetScrambleCount()
{
	return this->scramble;
}

void CDropCount::CheckCounter(CTSPacketUtil* tsPacket, DROP_INFO* info)
{
	if( tsPacket == NULL || info == NULL ){
		return;
	}

	if( tsPacket->transport_scrambling_control != 0 ){
		//スクランブルカウント
		info->scramble++;
		this->scramble++;
	}
	
	//ドロップチェック
	BOOL duplicateFlag = FALSE;
	BYTE dropCount = 0;
	if (info->total == 0) {
		//最初のパケットなのでドロップ判定しない
		goto CHK_END;
	}

	if (tsPacket->PID == 0x1FFF){
		//NULL
		goto CHK_END;
	}

	if (tsPacket->adaptation_field_control == 0x00 || tsPacket->adaptation_field_control == 0x02){
		//ペイロードが存在しない場合は判定しない
		goto CHK_END;
	}

	if (/* tsPacket->adaptation_field_control == 0x02 ||*/ tsPacket->adaptation_field_control == 0x03) {
		//アダプションフィールド有
		if (tsPacket->discontinuity_indicator == 1) {
			//不連続を示しているので判定不要
			goto CHK_END;
		}
	}

	if (tsPacket->continuity_counter == info->lastCounter) {
		duplicateFlag = TRUE;
		if (info->duplicateFlag == FALSE){
			//duplicate packetは1回のみ連続と判定
			goto CHK_END;
		}else{
			//2回目以上のduplicateは不連続
			dropCount = 1;
			goto CHK_END;
		}
	}
	BYTE right = (info->lastCounter + 1) & 0xf;
	if (tsPacket->continuity_counter != right) {
		//カウンターが飛んだので不連続
		/*
		dropCount = (tsPacket->continuity_counter - right) & 0xf;
		info->total += dropCount - 1;
		*/
		dropCount = 1;
		goto CHK_END;
	}

CHK_END:
	info->total++;
	info->drop += dropCount;
	this->drop += dropCount;
	if (info->isMp2Stream) {
		this->dropMp2Stream += dropCount;
	}
	info->lastCounter = tsPacket->continuity_counter;
	info->duplicateFlag = duplicateFlag;
	if( this->lastLogTime + 5 < GetTimeCount() ){
		if( this->lastLogDrop != this->drop ||
			this->lastLogScramble != this->scramble
			){
				wstring log;
				SYSTEMTIME now;
				GetLocalTime(&now);
				Format(log, L"%04d/%02d/%02d %02d:%02d:%02d Drop:%I64d Scramble:%I64d Signal: %.02f",
					now.wYear,
					now.wMonth,
					now.wDay,
					now.wHour,
					now.wMinute,
					now.wSecond,
					this->drop,
					this->scramble,
					this->signalLv
					);
				this->log.push_back(log);
				this->lastLogDrop = this->drop;
				this->lastLogScramble = this->scramble;
		}
		this->lastLogTime = GetTimeCount();
	}
}

void CDropCount::SaveLog(wstring filePath)
{
	HANDLE file = _CreateFile2( filePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( file != INVALID_HANDLE_VALUE ){
		DWORD write;

		string buff;
		for( size_t i=0; i<log.size(); i++ ){
			WtoA(log[i], buff);
			buff += "\r\n";
			WriteFile(file, buff.c_str(), (DWORD)buff.size(), &write, NULL );
		}

		buff = "\r\n";
		WriteFile(file, buff.c_str(), (DWORD)buff.size(), &write, NULL );

		map<WORD, DROP_INFO>::iterator itr;
		for( itr = this->infoMap.begin(); itr != this->infoMap.end(); itr++ ){
			if (itr->second.total) {
				string desc = "";
				map<WORD, string>::iterator itrPID;
				switch (itr->first){
				case 0x0000:
					desc = "PAT";
					break;
				case 0x0001:
					desc = "CAT";
					break;
				case 0x0010:
					desc = "NIT";
					break;
				case 0x0011:
					desc = "SDT/BAT";
					break;
				case 0x0012:
				case 0x0026:
				case 0x0027:
					desc = "EIT";
					break;
				case 0x0013:
					desc = "RST";
					break;
				case 0x0014:
					desc = "TDT/TOT";
					break;
				case 0x0017:
					desc = "DCT";
					break;
				case 0x001E:
					desc = "DIT";
					break;
				case 0x001F:
					desc = "SIT";
					break;
				case 0x0020:
					desc = "LIT";
					break;
				case 0x0021:
					desc = "ERT";
					break;
				case 0x0022:
					desc = "PCAT";
					break;
				case 0x0023:
				case 0x0028:
					desc = "SDTT";
					break;
				case 0x0024:
					desc = "BIT";
					break;
				case 0x0025:
					desc = "NBIT/LDT";
					break;
				case 0x0029:
					desc = "CDT";
					break;
				case 0x1FFF:
					desc = "NULL";
					break;
				default:
					desc = itr->second.pidName;
					break;
				}
				Format(buff, "PID: 0x%04X  Total:%9I64d  Drop:%9I64d  Scramble: %9I64d  %s%s\r\n",
					itr->first, itr->second.total, itr->second.drop, itr->second.scramble, desc.c_str(), itr->second.isMp2Stream ? "" : " (drop ignored)");
				WriteFile(file, buff.c_str(), (DWORD)buff.size(), &write, NULL);
			}
		}

		string strA;
		WtoA(bonFile, strA);
		Format(buff, "\r\n使用BonDriver : %s\r\n", strA.c_str());
		WriteFile(file, buff.c_str(), (DWORD)buff.size(), &write, NULL );

		CloseHandle(file);
	}
}

void CDropCount::SetPIDName(
	map<WORD, string>* pidName
	)
{
	map<WORD, string>::iterator itrIn;
	for(itrIn = pidName->begin(); itrIn != pidName->end(); itrIn++){
			map<WORD, DROP_INFO>::iterator itr;
			itr = this->infoMap.find(itrIn->first);
			DROP_INFO *item;
			if (itr == this->infoMap.end()){
				item = new DROP_INFO();
				item->PID = itrIn->first;
				this->infoMap.insert(pair<WORD, DROP_INFO>(item->PID, *item));
			} else {
				item = &(itr->second);
			}
			item->pidName = itrIn->second;
			if (itrIn->second != "ECM" && itrIn->second != "EMM") {
				item->isMp2Stream = TRUE;
			}
	}
}
