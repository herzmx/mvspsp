/******************************************************************************

	sound.c

	PSP サウンドスレッド

******************************************************************************/

#include "psp.h"
#include "emumain.h"


/******************************************************************************
	ローカル変数
******************************************************************************/

static volatile int sound_active;
static int sound_handle;
static SceUID sound_thread;
static int sound_volume;
static int sound_enable;
static int sound_pause;
static INT16 ALIGN_PSPDATA sound_buffer[2][SOUND_BUFFER_SIZE];

static struct sound_t sound_info;


/******************************************************************************
	グローバル変数
******************************************************************************/

struct sound_t *sound = &sound_info;


/******************************************************************************
	ローカル関数
******************************************************************************/

/*--------------------------------------------------------
	サウンド更新スレッド
--------------------------------------------------------*/

static int sound_update_thread(SceSize args, void *argp)
{
	int flip = 0;

	while (sound_active)
	{
		if (Sleep)
		{
			do
			{
				sceKernelDelayThread(5000000);
			} while (Sleep);
		}

		if (sound_enable)
		{
			(*sound->update)(sound_buffer[flip]);
			sceAudioOutputPannedBlocking(sound_handle, sound_volume, sound_volume, (char *)sound_buffer[flip]);
		}
		else
		{
			sound_pause = 1;
			memset(sound_buffer[flip], 0, SOUND_BUFFER_SIZE);
			sceAudioOutputPannedBlocking(sound_handle, 0, 0, (char *)sound_buffer[flip]);
		}
		flip ^= 1;
	}
	return 0;
}


/******************************************************************************
	グローバル関数
******************************************************************************/

/*--------------------------------------------------------
	サウンド初期化
--------------------------------------------------------*/

void sound_thread_init(void)
{
	sound_active = 0;
	sound_thread = -1;
	sound_handle = -1;
	sound_volume = 0;
	sound_enable = 0;
}


/*--------------------------------------------------------
	サウンド終了
--------------------------------------------------------*/

void sound_thread_exit(void)
{
	sound_thread_stop();
}


/*--------------------------------------------------------
	サウンド有効/無効切り替え
--------------------------------------------------------*/

void sound_thread_enable(int enable)
{
	if (sound_active)
	{
		sound_enable = enable;

		if (sound_enable)
		{
			sound_pause = 0;
			sound_thread_set_volume();
		}
		else
		{
			sound_volume = 0;
			while (!sound_pause) sceKernelDelayThread(1);
		}
	}
}


/*--------------------------------------------------------
	サウンドのボリューム設定
--------------------------------------------------------*/

void sound_thread_set_volume(void)
{
	if (sound_active)
		sound_volume = PSP_AUDIO_VOLUME_MAX * (option_sound_volume * 10) / 100;
}


/*--------------------------------------------------------
	サウンドスレッド開始
--------------------------------------------------------*/

int sound_thread_start(void)
{
	sound_active = 0;
	sound_thread = -1;
	sound_handle = -1;
	sound_enable = 0;

	memset((void *)sound_buffer[0], 0, SOUND_BUFFER_SIZE);
	memset((void *)sound_buffer[1], 0, SOUND_BUFFER_SIZE);

	if (sound->stereo)
		sound_handle = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, SOUND_SAMPLES, PSP_AUDIO_FORMAT_STEREO);
	else
		sound_handle = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, SOUND_SAMPLES, PSP_AUDIO_FORMAT_MONO);
	if (sound_handle < 0)
	{
		fatalerror(TEXT(COULD_NOT_RESERVE_AUDIO_CHANNEL_FOR_SOUND));
		return 0;
	}

	sound_thread = sceKernelCreateThread("Sound thread", sound_update_thread, 0x08, sound->stack, 0, NULL);
	if (sound_thread < 0)
	{
		fatalerror(TEXT(COULD_NOT_START_SOUND_THREAD));
		sceAudioChRelease(sound_handle);
		sound_handle = -1;
		return 0;
	}

	sound_active = 1;
	sceKernelStartThread(sound_thread, 0, 0);

	sound_thread_set_volume();

	return 1;
}


/*--------------------------------------------------------
	サウンドスレッド停止
--------------------------------------------------------*/

void sound_thread_stop(void)
{
	if (sound_thread >= 0)
	{
		sound_volume = 0;
		sound_enable = 0;

		sound_active = 0;
		sceKernelWaitThreadEnd(sound_thread, NULL);

		sceKernelDeleteThread(sound_thread);
		sound_thread = -1;

		sceAudioChRelease(sound_handle);
		sound_handle = -1;
	}
}
