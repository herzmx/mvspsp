/******************************************************************************

	psp.c

	PSPメイン

******************************************************************************/

#include "psp.h"


#ifdef KERNEL_MODE
PSP_MODULE_INFO(PBPNAME_STR, 0x1000, VERSION_MAJOR, VERSION_MINOR);
PSP_MAIN_THREAD_ATTR(0);
#else
PSP_MODULE_INFO(PBPNAME_STR, 0, VERSION_MAJOR, VERSION_MINOR);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
#endif


/******************************************************************************
	グローバル変数
******************************************************************************/

volatile int Loop;
volatile int Sleep;
char launchDir[MAX_PATH];
int psp_cpuclock;


/******************************************************************************
	グローバル関数
******************************************************************************/

/*------------------------------------------------------
	CPUクロック設定
------------------------------------------------------*/

void set_cpu_clock(int value)
{
	switch (value)
	{
	case PSPCLOCK_266: scePowerSetClockFrequency(266, 266, 133); break;
	case PSPCLOCK_333: scePowerSetClockFrequency(333, 333, 166); break;
	default: scePowerSetClockFrequency(222, 222, 111); break;
	}
}


/******************************************************************************
	ローカル関数
******************************************************************************/

/*--------------------------------------------------------
	Exit Callback
--------------------------------------------------------*/

#ifndef KERNEL_MODE
static SceKernelCallbackFunction ExitCallback(int arg1, int arg2, void *arg)
{
	Loop = LOOP_EXIT;
	return 0;
}
#endif

/*--------------------------------------------------------
	Power Callback
--------------------------------------------------------*/

static SceKernelCallbackFunction PowerCallback(int unknown, int pwrflags, void *arg)
{
	int cbid;

	if (pwrflags & PSP_POWER_CB_POWER_SWITCH)
	{
		Sleep = 1;
	}
	else if (pwrflags & PSP_POWER_CB_RESUME_COMPLETE)
	{
		Sleep = 0;
	}

	cbid = sceKernelCreateCallback("Power Callback", (void *)PowerCallback, NULL);

	scePowerRegisterCallback(0, cbid);

	return 0;
}

/*--------------------------------------------------------
	コールバックスレッド作成
--------------------------------------------------------*/

static int CallbackThread(SceSize args, void *argp)
{
	int cbid;

#ifndef KERNEL_MODE
	cbid = sceKernelCreateCallback("Exit Callback", (void *)ExitCallback, NULL);
	sceKernelRegisterExitCallback(cbid);
#endif

	cbid = sceKernelCreateCallback("Power Callback", (void *)PowerCallback, NULL);
	scePowerRegisterCallback(0, cbid);

	sceKernelSleepThreadCB();

	return 0;
}


/*--------------------------------------------------------
	コールバックスレッド設定
--------------------------------------------------------*/

static int SetupCallbacks(void)
{
	SceUID thread_id = 0;

	thread_id = sceKernelCreateThread("Update Thread", CallbackThread, 0x12, 0xFA0, 0, NULL);
	if (thread_id >= 0)
	{
		sceKernelStartThread(thread_id, 0, 0);
	}

	Loop  = LOOP_EXIT;
	Sleep = 0;

	return thread_id;
}


/*--------------------------------------------------------
	main()
--------------------------------------------------------*/

#ifdef KERNEL_MODE

static volatile int home_active;
volatile UINT32 home_button;

static int home_button_thread(SceSize args, void *argp)
{
	SceCtrlData paddata;

	home_active = 1;

	while (home_active)
	{
		sceCtrlPeekBufferPositive(&paddata, 1);
		home_button = paddata.Buttons & PSP_CTRL_HOME;
		sceKernelDelayThread(200);
	}

	sceKernelExitThread(0);

	return 0;
}

static int user_main(SceSize args, void *argp)
{
	SetupCallbacks();

	set_cpu_clock(PSPCLOCK_222);

	pad_init();
	video_set_mode(32);
	video_init();
	file_browser();
	video_exit(1);

	sceKernelExitThread(0);

	return 0;
}

int main(int argc, char *argv[])
{
	SceUID main_thread;
	SceUID home_thread;
	char *p;

	memset(launchDir, 0, sizeof(launchDir));
	strncpy(launchDir, argv[0], MAX_PATH - 1);
	if ((p = strrchr(launchDir, '/')) != NULL)
	{
		*(p + 1) = '\0';
	}

#ifdef ADHOC
	pspSdkLoadAdhocModules();
#endif

	home_thread = sceKernelCreateThread("Home Button Thread",
								home_button_thread,
								0x11,
								0x200,
								0,
								NULL);

	main_thread = sceKernelCreateThread("User Mode Thread",
								user_main,
								0x11,
								256 * 1024,
								PSP_THREAD_ATTR_USER,
								NULL);

	sceKernelStartThread(home_thread, 0, 0);

	sceKernelStartThread(main_thread, 0, 0);
	sceKernelWaitThreadEnd(main_thread, NULL);

	home_active = 0;
	sceKernelWaitThreadEnd(home_thread, NULL);

	sceKernelExitGame();

	return 0;
}

#else

int main(int argc, char *argv[])
{
	char *p;

	SetupCallbacks();

	memset(launchDir, 0, sizeof(launchDir));
	strncpy(launchDir, argv[0], MAX_PATH - 1);
	if ((p = strrchr(launchDir, '/')) != NULL)
	{
		*(p + 1) = '\0';
	}

	set_cpu_clock(PSPCLOCK_222);

	pad_init();
	video_set_mode(32);
	video_init();
	file_browser();
	video_exit(1);

	sceKernelExitGame();

	return 0;
}

#endif /* KERNEL_MODE */
