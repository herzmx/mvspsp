/******************************************************************************

	cps2.c

	CPS2エミュレーションコア

******************************************************************************/

#include "cps2.h"


/******************************************************************************
	ローカル関数
******************************************************************************/

/*--------------------------------------------------------
	CPS2エミュレーション初期化
--------------------------------------------------------*/

static int cps2_init(void)
{
	if (!cps2_driver_init())
		return 0;

	msg_printf(TEXT(DONE2));

	msg_screen_clear();
	video_clear_screen();

	return cps2_video_init();
}


/*--------------------------------------------------------
	CPS2エミュレーションリセット
--------------------------------------------------------*/

static void cps2_reset(void)
{
	video_set_mode(16);
	video_clear_screen();

	Loop = LOOP_EXEC;

	autoframeskip_reset();

	cps2_driver_reset();
	cps2_video_reset();

	timer_reset();
	input_reset();
	sound_reset();
}


/*--------------------------------------------------------
	CPSエミュレーション終了
--------------------------------------------------------*/

static void cps2_exit(void)
{
	video_set_mode(32);
	video_clear_screen();

	ui_popup_reset(POPUP_MENU);

	video_clear_screen();
	msg_screen_init(WP_LOGO, ICON_SYSTEM, TEXT(EXIT_EMULATION2));

	msg_printf(TEXT(PLEASE_WAIT2));

	cps2_video_exit();
	cps2_driver_exit();

#ifdef ADHOC
	if (!adhoc_enable)
#endif
	{
#ifdef COMMAND_LIST
		free_commandlist();
#endif
		save_gamecfg(game_name);
	}

	msg_printf(TEXT(DONE2));

	show_exit_screen();
}


/*--------------------------------------------------------
	CPSエミュレーション実行
--------------------------------------------------------*/

static void cps2_run(void)
{
	while (Loop >= LOOP_RESET)
	{
		cps2_reset();

		while (Loop == LOOP_EXEC)
		{
			if (Sleep)
			{
#if USE_CACHE
				cache_sleep(1);
#endif

				do
				{
					sceKernelDelayThread(5000000);
				} while (Sleep);

#if USE_CACHE
				cache_sleep(0);
#endif
				autoframeskip_reset();
			}

			timer_update_cpu();
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
	CPS2エミュレーションメイン
--------------------------------------------------------*/

void cps2_main(void)
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

				if (cps2_init())
				{
					cps2_run();
				}
				cps2_exit();

				input_shutdown();
			}
			sound_exit();
		}
		memory_shutdown();
		show_fatal_error();
	}
}
