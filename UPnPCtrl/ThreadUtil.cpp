#include "stdafx.h"

#include "MPInclude.h"

#include "ThreadUtil.h"

typedef struct _MP_LOCK_HANDLE_ITEM{
#ifdef _WIN32
	HANDLE lockEvent;
#else
//Linux�n
	pthread_mutex_t	mutex;
#endif
}MP_LOCK_HANDLE_ITEM;

typedef struct _MP_THREAD_HANDLE_ITEM{
#ifdef _WIN32
	HANDLE thread;
#else
//Linux�n
	pthread_t thread;
#endif
}MP_THREAD_HANDLE_ITEM;


//�r������p�̃n���h�����쐬����
//Windows�ł�CreateEvent
//Linux�n��pthread_mutex_init
//�߂�l�F�n���h���A�G���[��NULL
MP_LOCK_HANDLE MP_LOCK_CreateHandle(
	void
)
{
	MP_LOCK_HANDLE_ITEM* handle = NULL;
	handle = (MP_LOCK_HANDLE_ITEM*)malloc(sizeof(MP_LOCK_HANDLE_ITEM));
	if(handle != NULL ){
#ifdef _WIN32
		handle->lockEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
#else
		pthread_mutex_init(&handle->mutex,NULL);
#endif
	}

	return (MP_LOCK_HANDLE)handle;
}

//�n���h���̉��
//�����handle��NULL��
//Windows�ł�CloseHandle
//Linux�n��pthread_mutex_destroy
void MP_LOCK_CloseHandle(
	MP_LOCK_HANDLE* handle				//[IN]MP_LOCK_CreateHandle�̖߂�l
)
{
	MP_LOCK_HANDLE_ITEM* info = NULL;
	if( handle == NULL ){
		return;
	}
	info = (MP_LOCK_HANDLE_ITEM*)*handle;
	if( info != NULL ){
#ifdef _WIN32
		CloseHandle(info->lockEvent);
#else
		pthread_mutex_destroy(&info->mutex);
#endif
		free(info);
	}
	*handle = NULL;
}

//�r������p��Lock����
//Windows�ł�WaitForSingleObject
//Linux�n��pthread_mutex_lock
//�߂�l�F0�ȏ�Ő����A0�����ŃG���[
int MP_LOCK_Lock(
	MP_LOCK_HANDLE handle				//[IN]MP_LOCK_CreateHandle�̖߂�l
)
{
	MP_LOCK_HANDLE_ITEM* info = NULL;
	if( handle == NULL ){
		return -1;
	}
	info = (MP_LOCK_HANDLE_ITEM*)handle;
#ifdef _WIN32
	WaitForSingleObject(info->lockEvent, INFINITE);
#else
	pthread_mutex_lock(&info->mutex);
#endif
	return 0;
}

//�r������p��UnLock����
//Windows�ł�SetEvent
//Linux�n��pthread_mutex_unlock
//�߂�l�F0�ȏ�Ő����A0�����ŃG���[
int MP_LOCK_UnLock(
	MP_LOCK_HANDLE handle				//[IN]MP_LOCK_CreateHandle�̖߂�l
)
{
	MP_LOCK_HANDLE_ITEM* info = NULL;
	if( handle == NULL ){
		return -1;
	}
	info = (MP_LOCK_HANDLE_ITEM*)handle;
#ifdef _WIN32
	SetEvent(info->lockEvent);
#else
	pthread_mutex_unlock(&info->mutex);
#endif
	return 0;
}

//�ʃX���b�h�Ŋ֐������s����
//Windows�ł�_beginthreadex
//Linux�n��pthread_create
//�߂�l�F�n���h���A�G���[��NULL
MP_THREAD_HANDLE MP_THREAD_BegineThread(
	MP_THREAD_FUNC func,				//[IN]�V�K�X���b�h�̎��s���N�����郋�[�`��
	void* param							//[IN]�V�K�X���b�h�ɓn��������
)
{
	MP_THREAD_HANDLE_ITEM* handle = NULL;
	if( func == NULL ){
		return NULL;
	}

	handle = (MP_THREAD_HANDLE_ITEM*)malloc(sizeof(MP_THREAD_HANDLE_ITEM));
	if( handle != NULL ){
#ifdef _WIN32
		handle->thread = (HANDLE)_beginthreadex(NULL, 0, func, param, CREATE_SUSPENDED, NULL);
		if( handle->thread != 0 ){
			ResumeThread(handle->thread);
		}else{
			free(handle);
			handle = NULL;
		}
#else
		//Linux�n
		if( pthread_create(&handle->thread, 0, func, param) != 0 ){
			free(handle);
			handle = NULL;
		}
#endif
	}

	return (MP_THREAD_HANDLE)handle;
}

//�X���b�h�̏I����҂��J������������i�^�C���A�E�g�͂Ȃ��j
//�L�����Z�������͂��炩���ߍs���Ă�������
//Windows�ł�WaitForSingleObject
//Linux�n��pthread_join
//�I����handle��NULL��
void MP_THREAD_WaitEndThread(
	MP_THREAD_HANDLE* handle			//[IN]MP_THREAD_BegineThread�̖߂�l
)
{
	MP_THREAD_HANDLE_ITEM* info = NULL;
	if( handle == NULL ){
		return ;
	}
	info = (MP_THREAD_HANDLE_ITEM*)*handle;
	if( info != NULL ){
#ifdef _WIN32
		WaitForSingleObject(info->thread, INFINITE);
		CloseHandle(info->thread);
#else
		pthread_join(info->thread);
#endif
		free(info);
	}
	*handle = NULL;
}

//Sleep���s��
//Windows�ł�Sleep
//Linux�n��nanosleep
void MP_Sleep(
	int msec			//[IN]msec�P��
)
{
#ifdef _WIN32
	Sleep(msec);
#else
	struct timespec req;
	req.tv_sec = msec/1000;
	req.tvnsec = (msec%1000)*1000*1000;
	nanosleep(&req, NULL);
#endif
}

