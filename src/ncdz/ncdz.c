/******************************************************************************

	ncdz.c

	NEOGEO CDZエミュレーションコア

******************************************************************************/

#include "ncdz.h"


/******************************************************************************
	グローバル変数
******************************************************************************/

int neogeo_boot_bios;
int neogeo_region;


/******************************************************************************
	ローカル変数
******************************************************************************/

static int cpu_reset_flag;

#include "startup.c"


/******************************************************************************
	ローカル関数
******************************************************************************/

/*--------------------------------------------------------
	NEOGEO CDZエミュレーション初期化
--------------------------------------------------------*/

static int neogeo_init(void)
{
	video_set_mode(16);

	memset(memory_region_cpu1, 0, 0x200000);
	memset(memory_region_cpu2, 0, 0x10000);

	neogeo_driver_init();
	neogeo_video_init();
	input_reset();

	cpu_reset_flag = 0;

	swab(startup_bin, memory_region_cpu1 + 0x10f300, 0xd00);
	m68000_write_memory_32(0x120002, 0xffffffff);
	m68000_write_memory_32(0x11c808, 0xc0c760);	// load screen setup function
	m68000_write_memory_32(0x11c80c, 0xc0c814);	// load screen progress function
	m68000_write_memory_32(0x11c810, 0xc190e2);	// load screen default anime data

	if (!neogeo_boot_bios)
	{
		neogeo_driver_reset();
		neogeo_video_reset();

		if (!cdrom_process_ipl())
		{
			if (!fatal_error)
			{
				fatalerror(TEXT(ERROR_WHILE_PROCESSING_IPL_TXT));
			}
			return 0;
		}

		if (NGH_NUMBER(0x0085))
		{
			UINT16 *mem16 = (UINT16 *)memory_region_cpu1;

			mem16[0x132020 >> 1] = 0x4ef9;
			mem16[0x132022 >> 1] = 0x00c0;
			mem16[0x132024 >> 1] = 0xdb60;

			mem16[0x132026 >> 1] = 0x4ef9;
			mem16[0x132028 >> 1] = 0x00c0;
			mem16[0x13202a >> 1] = 0xdb6a;
		}
	}

	return 1;
}


/*--------------------------------------------------------
	NEOGEO CDZエミュレーションリセット
--------------------------------------------------------*/

static void neogeo_reset(void)
{
	video_set_mode(16);
	video_clear_screen();

	autoframeskip_reset();

	timer_reset();
	sound_reset();
	input_reset();

	neogeo_driver_reset();
	neogeo_video_reset();

	if (game_index != 99)
	{
		if (cpu_reset_flag == 0)
		{
			m68000_set_reg(M68K_PC, 0xc0a822);

			m68000_write_memory_32(0x108000 + 0x76ee, m68000_read_memory_32(0x68));	// IRQ vector
//			m68000_write_memory_32(0x108000 + 0x76f2, m68000_read_memory_32(0x64));	// VBLANK

			m68000_write_memory_8(0x108000 + 0x76d9, 0x01);
			m68000_write_memory_8(0x108000 + 0x76c3, 0x01);
			m68000_write_memory_8(0x108000 + 0x764b, 0xff);
			m68000_write_memory_8(0x108000 + 0x76f6, 0xff);
			m68000_write_memory_8(0x108000 + 0x7e85, 0x01);
		}
		else
		{
			m68000_set_reg(M68K_PC, 0x000122);

			m68000_write_memory_8(0x108000 + 0x7d80, 0x82);
			m68000_write_memory_8(0x108000 + 0x7daf, 0x01);
			m68000_write_memory_8(0x108000 + 0x7ee1, 0x0a);
			m68000_write_memory_8(0x108000 + 0x7675, 0x01);
			m68000_write_memory_8(0x108000 + 0x7ebf, 0x00);
			m68000_write_memory_32(0x108000 + 0x7db6, 0);
			m68000_write_memory_32(0x108000 + 0x7dba, 0);
		}

		cpu_reset_flag = 1;
	}

	m68000_write_memory_16(0xff011c, ~(neogeo_region << 8));
	m68000_write_memory_8(0x10fd83, neogeo_region);

	cdda_current_track = 0;

	Loop = LOOP_EXEC;
}


/*--------------------------------------------------------
	NEOGEO CDZエミュレーション終了
--------------------------------------------------------*/

static void neogeo_exit(void)
{
	video_set_mode(32);
	video_clear_screen();

	ui_popup_reset();

	msg_screen_init(WP_LOGO, ICON_SYSTEM, TEXT(EXIT_EMULATION2));

	msg_printf(TEXT(PLEASE_WAIT2));

#ifdef COMMAND_LIST
	free_commandlist();
#endif
	save_gamecfg(game_name);

	msg_printf(TEXT(DONE2));

	show_exit_screen();
	neogeo_boot_bios = 0;
}


/*--------------------------------------------------------
	NEOGEO CDZエミュレーション実行
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
				do
				{
					sceKernelDelayThread(5000000);
				} while (Sleep);

				autoframeskip_reset();
			}

			timer_update_cpu();

			neogeo_cdda_check();

			if (--watchdog_counter <= 0)
			{
				Loop = LOOP_RESET;
				ui_popup(TEXT(RESET_CAUSED_BY_WATCHDOG_COUNTER));
			}

			update_screen();

			if (neogeo_loadfinished)
			{
				neogeo_loadfinished = 0;
				video_enable_w(1);
				fix_disable_w(0);
				spr_disable_w(0);
			}

			update_inputport();
		}

		sound_mute(1);
		cdda_stop();
	}
}


/******************************************************************************
	グローバル関数
******************************************************************************/

/*--------------------------------------------------------
	NEOGEO CDZエミュレーション再起動
--------------------------------------------------------*/

void neogeo_restart(void)
{
	Loop = LOOP_RESTART;
}


/*--------------------------------------------------------
	NEOGEO CDZエミュレーション終了 (called by BIOS)
--------------------------------------------------------*/

void neogeo_bios_exit(void)
{
	Loop = LOOP_BROWSER;
}


/*--------------------------------------------------------
	NEOGEO CDZエミュレーション開始
--------------------------------------------------------*/

void neogeo_main(void)
{
	while (Loop >= LOOP_RESTART)
	{
		ui_popup_reset();

		fatal_error = 0;

		video_clear_screen();

		if (memory_init())
		{
			if (sound_init())
			{
				if (cdda_init())
				{
					cdrom_init();
					if (input_init())
					{
						Loop = LOOP_EXEC;

						if (neogeo_init())
						{
							neogeo_run();
						}
						neogeo_exit();
					}
					input_shutdown();
					cdrom_shutdown();
				}
				cdda_shutdown();
			}
			sound_exit();
		}
		memory_shutdown();
		show_fatal_error();
	}
	ui_popup_reset();
}
