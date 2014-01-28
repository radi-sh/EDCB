// EpgTimerSrv.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <stdio.h>
#include "EpgTimerSrv.h"
#include "EpgTimerSrvDef.h"
#include "EpgTimerSrvMain.h"
#include "ServiceUtil.h"

#include "../../Common/CommonDef.h"
#include "../../Common/Util.h"
#include "../../Common/SendCtrlCmd.h"

HANDLE g_hMutex;
SERVICE_STATUS_HANDLE g_hStatusHandle;
CEpgTimerSrvMain* g_pMain;
DWORD g_dwCurrentSSState = SERVICE_STOPPED;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	if( _tcslen(lpCmdLine) > 0 ){
		if( lpCmdLine[0] == '-' || lpCmdLine[0] == '/' ){
			if( _tcsicmp( _T("install"), lpCmdLine+1 ) == 0 ){
				WCHAR strExePath[512] = L"";
				GetModuleFileName(NULL, strExePath, 512);
				if( InstallService(strExePath, SERVICE_NAME,SERVICE_NAME) == FALSE ){
					printf("�C���X�g�[���Ɏ��s���܂����BVista�ȍ~�̏ꍇ�͊Ǘ��Ҍ������K�v�ł��B");
				}
				return 0;
			}else if( _tcsicmp( _T("remove"), lpCmdLine+1 ) == 0 ){
				if( RemoveService(SERVICE_NAME) == FALSE ){
					printf("�A���C���X�g�[���Ɏ��s���܂����BVista�ȍ~�̏ꍇ�͊Ǘ��Ҍ������K�v�ł��B");
				}
				return 0;
			}
		}
	}


	if( IsInstallService(SERVICE_NAME) == FALSE ){
		//���ʂ�exe�Ƃ��ċN�����s��
		g_hMutex = _CreateMutex(TRUE, EPG_TIMER_BON_SRV_MUTEX);
		int err = GetLastError();
		if( g_hMutex != NULL ){
			if( err != ERROR_ALREADY_EXISTS ) {
				//�N��
				StartMain(FALSE);
			}else{
				// �N������Ă���̂ŗ\��ǉ��̊m�F
				CSendCtrlCmd cmd;
				cmd.SetConnectTimeOut(1000);
				cmd.SendAddloadReserve();
			}
			::ReleaseMutex(g_hMutex);
			::CloseHandle(g_hMutex);
		}else{
			// �N������Ă���̂ŗ\��ǉ��̊m�F
			CSendCtrlCmd cmd;
			cmd.SetConnectTimeOut(1000);
			cmd.SendAddloadReserve();
		}
	}else{
		//�T�[�r�X�Ƃ��ăC���X�g�[���ς�
		if( IsStopService(SERVICE_NAME) == FALSE ){
			g_hMutex = _CreateMutex(TRUE, EPG_TIMER_BON_SRV_MUTEX);
			int err = GetLastError();
			if( g_hMutex != NULL && err != ERROR_ALREADY_EXISTS ) {
				//�N��
				SERVICE_TABLE_ENTRY dispatchTable[] = {
					{ SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)service_main},
					{ NULL, NULL}
				};
				if( StartServiceCtrlDispatcher(dispatchTable) == FALSE ){
					OutputDebugString(_T("StartServiceCtrlDispatcher failed"));
				}
			}else{
				// �N������Ă���̂ŗ\��ǉ��̊m�F
				CSendCtrlCmd cmd;
				cmd.SetConnectTimeOut(1000);
				cmd.SendAddloadReserve();
			}
		}else{
			//Stop��ԂȂ̂ŋN������
			StartServiceCtrl(SERVICE_NAME);
		}
	}

	return 0;
}

void WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv)
{
	g_hStatusHandle = RegisterServiceCtrlHandlerEx( SERVICE_NAME, (LPHANDLER_FUNCTION_EX)service_ctrl, NULL);

	if (g_hStatusHandle == NULL){
		goto cleanup;
	}
	
	SendStatusScm(SERVICE_START_PENDING, 0, 1);

	SendStatusScm(SERVICE_RUNNING, 0, 0);
	StartMain(TRUE);

cleanup:
	SendStatusScm(SERVICE_STOPPED, 0, 0);

   return;
}

DWORD WINAPI service_ctrl(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
	switch (dwControl){
		case SERVICE_CONTROL_STOP:
		case SERVICE_CONTROL_SHUTDOWN:
			SendStatusScm(SERVICE_STOP_PENDING, 0, 1);
			StopMain();
			return NO_ERROR;
		case SERVICE_CONTROL_POWEREVENT:
			OutputDebugString(_T("SERVICE_CONTROL_POWEREVENT"));
			if ( dwEventType == PBT_APMQUERYSUSPEND ){
				OutputDebugString(_T("PBT_APMQUERYSUSPEND"));
				if( g_pMain != NULL && g_pMain->IsSuspending() == FALSE ){
					if( g_pMain->ChkSuspend() == FALSE ){
						OutputDebugString(_T("BROADCAST_QUERY_DENY"));
						return BROADCAST_QUERY_DENY;
					}
				}
			}else if( dwEventType == PBT_APMRESUMESUSPEND ){
				OutputDebugString(_T("PBT_APMRESUMESUSPEND"));
			}
			OutputDebugString(_T("break"));
			break;
		default:
			break;
	}
	SendStatusScm(g_dwCurrentSSState, 0, 0);
	return NO_ERROR;
}

BOOL SendStatusScm(int iState, int iExitcode, int iProgress)
{
	SERVICE_STATUS ss;

	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState = iState;
	ss.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_POWEREVENT;
	ss.dwWin32ExitCode = iExitcode;
	ss.dwServiceSpecificExitCode = 0;
	ss.dwCheckPoint = iProgress;
	ss.dwWaitHint = 10000;

	g_dwCurrentSSState = iState;

	return SetServiceStatus(g_hStatusHandle, &ss);
}

void StartMain(BOOL bService)
{
	//���C���X���b�h�ɑ΂���COM�̏�����
	CoInitialize(NULL);

	g_pMain = new CEpgTimerSrvMain;
	g_pMain->StartMain(bService);
	delete g_pMain;
	g_pMain = NULL;

	CoUninitialize();
}

void StopMain()
{
	if( g_pMain != NULL ){
		g_pMain->StopMain();
	}
}
