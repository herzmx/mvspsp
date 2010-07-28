/******************************************************************************

	mvs.c

	MVSエミュレーションコア

******************************************************************************/

#include "mvs.h"


/******************************************************************************
	グローバル変数
******************************************************************************/

int neogeo_bios;
int neogeo_region;
int neogeo_save_sound_flag;


/******************************************************************************
	ローカル関数
******************************************************************************/

/*--------------------------------------------------------
	MVSエミュレーション初期化
--------------------------------------------------------*/

static int neogeo_init(void)
{
	SceUID fd;
	char path[MAX_PATH];

#ifdef ADHOC
	if (!adhoc_enable)
#endif
	{
		sprintf(path, "%smemcard/%s.bin", launchDir, game_name);
		if ((fd = sceIoOpen(path, PSP_O_RDONLY, 0777)) >= 0)
		{
			sceIoRead(fd, neogeo_memcard, 0x800);
			sceIoClose(fd);
		}

		sprintf(path, "%snvram/%s.nv", launchDir, game_name);
		if ((fd = sceIoOpen(path, PSP_O_RDONLY, 0777)) >= 0)
		{
			sceIoRead(fd, neogeo_sram16, 0x2000);
			sceIoClose(fd);
			swab(neogeo_sram16, neogeo_sram16, 0x2000);
		}
	}

	neogeo_driver_init();
	neogeo_video_init();

	msg_printf(TEXT(DONE2));

	msg_screen_clear();
	video_clear_screen();

	return 1;
}


/*--------------------------------------------------------
	MVSエミュレーションリセット
--------------------------------------------------------*/

static void neogeo_reset(void)
{
	video_set_mode(16);
	video_clear_screen();

	autoframeskip_reset();

	timer_reset();
	input_reset();
	sound_reset();

	neogeo_driver_reset();
	neogeo_video_reset();

	Loop = LOOP_EXEC;
}


/*--------------------------------------------------------
	MVSエミュレーション終了
--------------------------------------------------------*/

static void neogeo_exit(void)
{
	SceUID fd;
	char path[MAX_PATH];

	video_set_mode(32);
	video_clear_screen();

	ui_popup_reset(POPUP_MENU);

	video_clear_screen();
	msg_screen_init(WP_LOGO, ICON_SYSTEM, TEXT(EXIT_EMULATION2));

	msg_printf(TEXT(PLEASE_WAIT2));

#ifdef ADHOC
	if (!adhoc_enable)
#endif
	{
		sprintf(path, "%smemcard/%s.bin", launchDir, game_name);
		if ((fd = sceIoOpen(path, PSP_O_WRONLY|PSP_O_CREAT, 0777)) >= 0)
		{
			sceIoWrite(fd, neogeo_memcard, 0x800);
			sceIoClose(fd);
		}

		sprintf(path, "%snvram/%s.nv", launchDir, game_name);
		if ((fd = sceIoOpen(path, PSP_O_WRONLY|PSP_O_CREAT, 0777)) >= 0)
		{
			swab(neogeo_sram16, neogeo_sram16, 0x2000);
			sceIoWrite(fd, neogeo_sram16, 0x2000);
			sceIoClose(fd);
		}


#ifdef COMMAND_LIST
		free_commandlist();
#endif

		if (neogeo_save_sound_flag) option_sound_enable = 1;
		save_gamecfg(game_name);
	}

	msg_printf(TEXT(DONE2));

	show_exit_screen();
}


/*--------------------------------------------------------
	MVSエミュレーション実行
--------------------------------------------------------*/

static void neogeo_run(void)
{
	while (Loop >= LOOP_RESET)
	{
		neogeo_reset();

		while (Loop == LOOP_EXEC)
		{
			if (Sleep)
			{
				cache_sleep(1);

				do
				{
					sceKernelDelayThread(5000000);
				} while (Sleep);

				cache_sleep(0);
				autoframeskip_reset();
			}

			(*timer_update_cpu)();
			update_screen();
			update_inputport();
		}

		video_clear_screen();
		sound_mute(1);
	}
}


/******************************************************************************
	グローバル関数
******************************************************************************/

/*--------------------------------------------------------
	MVSエミュレーションメイン
--------------------------------------------------------*/

void neogeo_main(void)
{
	video_set_mode(32);

	Loop = LOOP_RESET;

	while (Loop >= LOOP_RESTART)
	{
		Loop = LOOP_EXEC;

		ui_popup_reset(POPUP_GAME);

		fatal_error = 0;

		video_clear_screen();

		if (memory_init())
		{
			if (sound_init())
			{
				input_init();

				if (neogeo_init())
				{
					neogeo_run();
				}
				neogeo_exit();

				input_shutdown();
			}
			sound_exit();
		}
		memory_shutdown();
		show_fatal_error();
	}
}
