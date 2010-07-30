/******************************************************************************

	ui_menu.c

	PSP メニュー

******************************************************************************/

#include "emumain.h"

#define MENU_BLANK	{ LF, }
#define MENU_RETURN	{ RETURN_TO_MAIN_MENU, }
#define MENU_END	{ EOM, }


/*------------------------------------------------------
	エミュレーションリセット
------------------------------------------------------*/

static int menu_reset(void)
{
	if (messagebox(MB_RESETEMULATION))
	{
		video_clear_screen();
		video_flip_screen(1);
		Loop = LOOP_RESET;
		Sleep = 0;
		return 1;
	}
	return 0;
}


/*------------------------------------------------------
	エミュレーション再起動
------------------------------------------------------*/

static int menu_restart(void)
{
	if (messagebox(MB_RESTARTEMULATION))
	{
		video_clear_screen();
		video_flip_screen(1);
		Loop = LOOP_RESTART;
		Sleep = 0;
	}
	return 1;
}


/*------------------------------------------------------
	ファイルブラウザに戻る
------------------------------------------------------*/

static int menu_browser(void)
{
	Loop = LOOP_BROWSER;
	Sleep = 0;
	return 1;
}


/*------------------------------------------------------
	エミュレータ終了
------------------------------------------------------*/

static int menu_exit(void)
{
	if (messagebox(MB_EXITEMULATION))
	{
		Loop = LOOP_EXIT;
		Sleep = 0;
		return 1;
	}
	return 0;
}


/*------------------------------------------------------
	コマンドリスト表示
------------------------------------------------------*/

#ifdef COMMAND_LIST
static int menu_cmdlist(void)
{
	commandlist(0);

	if (Loop == LOOP_EXIT) return 1;
	return 0;
}
#endif


/*------------------------------------------------------
	ゲーム設定メニュー
------------------------------------------------------*/

#define GAMECFG_MAX_ITEMS	32

typedef struct {
	const char *label;
	int *value;
	int enable;
	int flag;
	int old_value;
	int value_max;
	const char *values_label[12];
} gamecfg_t;

typedef struct {
	int label;
	int *value;
	int flag;
	int value_max;
	int values_label[12];
} gamecfg2_t;


#define CFG_CONTINUE	0
#define CFG_RESTART		1
#define CFG_RESET		2

#define INCLUDE_GAMECFG_STRUCT

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
#include "menu/cps.c"
#elif (EMU_SYSTEM == MVS)
#include "menu/mvs.c"
#elif (EMU_SYSTEM == NCDZ)
#include "menu/ncdz.c"
#endif

#undef INCLUDE_GAMECFG_STRUCT

static int menu_gamecfg(void)
{
	int sel = 0, rows = 13, top = 0;
	int i, arrowl, arrowr, prev_sel, update = 1;
	gamecfg_t gamecfg[GAMECFG_MAX_ITEMS];
	gamecfg2_t *gamecfg2;
	int gamecfg_num;

	for (i = 0; i < GAMECFG_MAX_ITEMS; i++)
		gamecfg[i].enable = 1;

#define INCLUDE_GAMECFG_MENU

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
#include "menu/cps.c"
#elif (EMU_SYSTEM == MVS)
#include "menu/mvs.c"
#elif (EMU_SYSTEM == NCDZ)
#include "menu/ncdz.c"
#endif

#undef INCLUDE_GAMECFG_MENU

	i = 0;
	while (gamecfg2[i].label)
	{
		int j;

		gamecfg[i].label     = TEXT(gamecfg2[i].label);
		gamecfg[i].value     = gamecfg2[i].value;
		gamecfg[i].flag      = gamecfg2[i].flag;
		gamecfg[i].value_max = gamecfg2[i].value_max;

		for (j = 0; j <= gamecfg2[i].value_max; j++)
			gamecfg[i].values_label[j] = TEXT(gamecfg2[i].values_label[j]);

		if (gamecfg[i].value)
		{
			if (*gamecfg[i].value < 0)
				*gamecfg[i].value = 0;
			if (*gamecfg[i].value > gamecfg[i].value_max)
				*gamecfg[i].value = gamecfg[i].value_max;

			gamecfg[i].old_value = *gamecfg[i].value;
		}
		i++;
	}
	gamecfg_num = i;

	pad_wait_clear();
	load_background(WP_GAMECFG);
	ui_popup_reset();

	do
	{
		if (update)
		{
			show_background();

			arrowl = 0;
			arrowr = 0;
			if (gamecfg[sel].value)
			{
				int cur = *gamecfg[sel].value;

				if (cur > 0) arrowl = 1;
				if (cur < gamecfg[sel].value_max) arrowr = 1;
			}

			small_icon_shadow(8, 3, UI_COLOR(UI_PAL_TITLE), ICON_CONFIG);
			uifont_print_shadow(36, 5, UI_COLOR(UI_PAL_TITLE), TEXT(GAME_CONFIGURATION_MENU));
			draw_scrollbar(470, 26, 479, 270, rows, gamecfg_num, sel);

			for (i = 0; i < rows; i++)
			{
				if (top + i >= gamecfg_num) break;

				if (gamecfg[top + i].label[0] == '\n')
					continue;

				if (gamecfg[top + i].enable)
				{
					if ((top + i) == sel)
					{
						uifont_print_shadow(16, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), gamecfg[top + i].label);
						if (arrowl)
						{
							uifont_print_shadow(180, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), FONT_LEFTTRIANGLE);
						}
					}
					else
						uifont_print(16, 40 + i * 17, UI_COLOR(UI_PAL_NORMAL), gamecfg[top + i].label);
				}
				else
				{
					if ((top + i) == sel)
						uifont_print_shadow(16, 40 + i * 17, COLOR_GRAY, gamecfg[top + i].label);
					else
						uifont_print(16, 40 + i * 17, COLOR_DARKGRAY, gamecfg[top + i].label);
				}

				if (gamecfg[top + i].value)
				{
					int val = *gamecfg[top + i].value;

					if (gamecfg[top + i].enable)
					{
						if ((top + i) == sel)
						{
							uifont_print_shadow(200, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), gamecfg[top + i].values_label[val]);
							if (arrowr)
							{
								int width = uifont_get_string_width(gamecfg[top + i].values_label[val]);
								uifont_print_shadow(204 + width, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
							}
						}
						else
							uifont_print(200, 40 + i * 17, UI_COLOR(UI_PAL_NORMAL), gamecfg[top + i].values_label[val]);
					}
					else
					{
						if ((top + i) == sel)
							uifont_print_shadow(200, 40 + i * 17, COLOR_GRAY, gamecfg[top + i].values_label[val]);
						else
							uifont_print(200, 40 + i * 17, COLOR_DARKGRAY, gamecfg[top + i].values_label[val]);
					}
				}
			}

			update  = draw_battery_status(1);
			//update |= draw_volume_status(1);
			update |= ui_show_popup(1);
			video_flip_screen(1);
		}
		else
		{
			update  = draw_battery_status(0);
			//update |= draw_volume_status(0);
			update |= ui_show_popup(0);
			video_wait_vsync();
		}

		prev_sel = sel;

		if (pad_pressed(PSP_CTRL_UP))
		{
			sel--;
			if (sel < 0) sel = gamecfg_num - 1;
			if (gamecfg[sel].label[0] == '\n') sel--;
		}
		else if (pad_pressed(PSP_CTRL_DOWN))
		{
			sel++;
			if (sel > gamecfg_num - 1) sel = 0;
			if (gamecfg[sel].label[0] == '\n') sel++;
		}
		else if (pad_pressed(PSP_CTRL_LEFT))
		{
			if (gamecfg[sel].value && gamecfg[sel].enable)
			{
				int cur = *gamecfg[sel].value;

				if (cur > 0)
				{
					*gamecfg[sel].value = cur - 1;
					update = 1;
				}
			}
		}
		else if (pad_pressed(PSP_CTRL_RIGHT))
		{
			if (gamecfg[sel].value && gamecfg[sel].enable)
			{
				int cur = *gamecfg[sel].value;

				if (cur < gamecfg[sel].value_max)
				{
					*gamecfg[sel].value = cur + 1;
					update = 1;
				}
			}
		}
		else if (pad_pressed(PSP_CTRL_CIRCLE))
		{
			if (sel == gamecfg_num - 1)
				break;
		}
		else if (pad_pressed(PSP_CTRL_RTRIGGER))
		{
			help(HELP_GAMECONFIG);
			update = 1;
		}

		if (top > gamecfg_num - rows) top = gamecfg_num - rows;
		if (top < 0) top = 0;
		if (sel >= gamecfg_num) sel = 0;
		if (sel < 0) sel = gamecfg_num - 1;
		if (sel >= top + rows) top = sel - rows + 1;
		if (sel < top) top = sel;

		if (prev_sel != sel) update = 1;

		pad_update();

		if (Loop == LOOP_EXIT) break;

	} while (!pad_pressed(PSP_CTRL_CROSS));

	for (i = 0; i < gamecfg_num; i++)
	{
		if (gamecfg[i].value && gamecfg[i].flag == CFG_RESTART)
		{
			if (gamecfg[i].old_value != *gamecfg[i].value)
			{
				menu_restart();
				return 1;
			}
		}
	}

	return 0;
}


/*------------------------------------------------------
	ボタン設定メニュー
------------------------------------------------------*/

#define KEYCFG_MAX_ITEMS	40

static const int button_value[13] =
{
	0,
	PSP_CTRL_UP,
	PSP_CTRL_DOWN,
	PSP_CTRL_LEFT,
	PSP_CTRL_RIGHT,
	PSP_CTRL_CIRCLE,
	PSP_CTRL_CROSS,
	PSP_CTRL_SQUARE,
	PSP_CTRL_TRIANGLE,
	PSP_CTRL_LTRIGGER,

	PSP_CTRL_RTRIGGER,
	PSP_CTRL_START,
	PSP_CTRL_SELECT
};

typedef struct {
	const char *label;
	int type;
	int value;
	int code;
} keycfg_t;

typedef struct {
	int label;
	int type;
	int code;
} keycfg2_t;

#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
enum
{
	NEOGEO_PAD = 0,
	NEOGEO_MVS,
	NEOGEO_PLAY = 2,
	USER_DEFINE
};
#endif

#define KEYCFG_COMMENT 0
#define KEYCFG_BUTTON  1
#define KEYCFG_NUMBER  2
#define KEYCFG_ANALOG  3
#define KEYCFG_LAYOUT  4

#define INCLUDE_KEYCFG_STRUCT

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
#include "menu/cps.c"
#elif (EMU_SYSTEM == MVS)
#include "menu/mvs.c"
#elif (EMU_SYSTEM == NCDZ)
#include "menu/ncdz.c"
#endif

#undef INCLUDE_KEYCFG_STRUCT

static int menu_keycfg(void)
{
	int sel = 0, prev_sel, rows = 13, top = 0;
	int i, j, arrowl, arrowr, update = 1;
	keycfg_t keycfg[KEYCFG_MAX_ITEMS];
	keycfg2_t *keycfg2;
	int keycfg_num;
	const char *button_name[13];
	const char *sensitivity[3];
#if (EMU_SYSTEM == CPS2)
	const char *progear_p2[2];
#elif (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
	const char *layout[4];
#endif

#define INCLUDE_KEYCFG_MENU

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
#include "menu/cps.c"
#elif (EMU_SYSTEM == MVS)
#include "menu/mvs.c"
#elif (EMU_SYSTEM == NCDZ)
#include "menu/ncdz.c"
#endif

#undef INCLUDE_KEYCFG_MENU

	for (i = 0; i < 13; i++)
		button_name[i] = TEXT(NOT_USE + i);

	sensitivity[0] = TEXT(SENSITIVITY_LOW);
	sensitivity[1] = TEXT(SENSITIVITY_NORMAL);
	sensitivity[2] = TEXT(SENSITIVITY_HIGH);

#if (EMU_SYSTEM == CPS2)
	progear_p2[0] = TEXT(PLAYER1_START);
	progear_p2[1] = TEXT(PLAYER2_START);
#elif (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
	layout[0] = TEXT(LAYOUT_TYPE1);
	layout[1] = TEXT(LAYOUT_TYPE2);
	layout[2] = TEXT(LAYOUT_TYPE3);
	layout[3] = TEXT(LAYOUT_USER);
#endif

	i = 0;
	while (keycfg2[i].label)
	{
		keycfg[i].label = TEXT(keycfg2[i].label);
		keycfg[i].type  = keycfg2[i].type;
		keycfg[i].value = 0;
		keycfg[i].code  = keycfg2[i].code;

		switch (keycfg[i].type)
		{
		case KEYCFG_BUTTON:
			{
				int code = input_map[keycfg[i].code];

				if (code)
				{
					for (j = 0; j < 13; j++)
					{
						if (code == button_value[j])
						{
							keycfg[i].value = j;
							break;
						}
					}
				}
			}
			break;

		case KEYCFG_NUMBER:
			keycfg[i].value = af_interval;
			break;

		case KEYCFG_ANALOG:
			keycfg[i].value = analog_sensitivity;
			break;
		}
		i++;
	}
	keycfg_num = i;

#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
	if (keycfg[6].value == 6 && keycfg[7].value == 5 && keycfg[8].value == 7 && keycfg[9].value == 8)
	{
		keycfg[0].value = NEOGEO_PAD;
	}
	else if (keycfg[6].value == 7 && keycfg[7].value == 8 && keycfg[8].value == 6 && keycfg[9].value == 5)
	{
		keycfg[0].value = NEOGEO_MVS;
	}
	else
	{
		keycfg[0].value = USER_DEFINE;
	}
#endif

	pad_wait_clear();
	load_background(WP_KEYCFG);
	ui_popup_reset();

	do
	{
		if (update)
		{
			show_background();

			small_icon_shadow(8, 3, UI_COLOR(UI_PAL_TITLE), ICON_KEYCONFIG);
			uifont_print_shadow(36, 5, UI_COLOR(UI_PAL_TITLE), TEXT(KEY_CONFIGURATION_MENU));
			draw_scrollbar(470, 26, 479, 270, rows, keycfg_num, sel);

			arrowl = 0;
			arrowr = 0;
			if (keycfg[sel].type)
			{
				if (keycfg[sel].value > 0) arrowl = 1;
				if (keycfg[sel].type == KEYCFG_BUTTON && keycfg[sel].value < 12) arrowr = 1;
				if (keycfg[sel].type == KEYCFG_NUMBER && keycfg[sel].value < 10) arrowr = 1;
				if (keycfg[sel].type == KEYCFG_ANALOG && keycfg[sel].value < 2) arrowr = 1;
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
				if (keycfg[sel].type == KEYCFG_LAYOUT && keycfg[sel].value < 1) arrowr = 1;
#endif
			}

			for (i = 0; i < rows; i++)
			{
				int value = keycfg[top + i].value;
				char temp[16];
				const char *name;
				const char *label = keycfg[top + i].label;

				if ((top + i) >= keycfg_num) break;

				if (label[0] == '\n')
					continue;

				switch (keycfg[top + i].type)
				{
				case KEYCFG_BUTTON:
#if (EMU_SYSTEM == CPS2)
					if (!strcmp(driver->name, "progear"))
						if (top + i == 10) label = progear_p2[option_controller ^ 1];
#endif
					name = button_name[value];
					break;

				case KEYCFG_NUMBER:
					if (value == 0)
						sprintf(temp, TEXT(xFRAME), value + 1);
					else
						sprintf(temp, TEXT(xFRAMES), value + 1);
					name = (const char *)temp;
					break;

				case KEYCFG_ANALOG:
					name = sensitivity[value];
					break;

#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
				case KEYCFG_LAYOUT:
					name = layout[value];
					break;
#endif

				default:
					temp[0] = '\0';
					name = temp;
					break;
				}

				if ((top + i) == sel)
				{
					uifont_print_shadow(16, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), label);
					uifont_print_shadow(190, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), name);

					if (arrowl)
					{
						uifont_print_shadow(170, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), FONT_LEFTTRIANGLE);
					}
					if (arrowr)
					{
						int width = uifont_get_string_width(name);
						uifont_print_shadow(194 + width, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
					}
				}
				else
				{
					uifont_print(16, 40 + i * 17, UI_COLOR(UI_PAL_NORMAL), label);
					uifont_print(190, 40 + i * 17, UI_COLOR(UI_PAL_NORMAL), name);
				}
			}

			update  = draw_battery_status(1);
			//update |= draw_volume_status(1);
			update |= ui_show_popup(1);
			video_flip_screen(1);
		}
		else
		{
			update  = draw_battery_status(0);
			//update |= draw_volume_status(0);
			update |= ui_show_popup(0);
			video_wait_vsync();
		}

		prev_sel = sel;

		if (pad_pressed(PSP_CTRL_UP))
		{
			sel--;
			if (sel < 0) sel = keycfg_num - 1;
			if (keycfg[sel].label[0] == '\n') sel--;
		}
		else if (pad_pressed(PSP_CTRL_DOWN))
		{
			sel++;
			if (sel > keycfg_num - 1) sel = 0;
			if (keycfg[sel].label[0] == '\n') sel++;
		}
		else if (pad_pressed(PSP_CTRL_LEFT))
		{
			if (keycfg[sel].type)
			{
				if (keycfg[sel].value > 0)
				{
					keycfg[sel].value--;
					update = 1;
				}
			}
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
			if (keycfg[sel].type == KEYCFG_LAYOUT && update)
			{
				switch (keycfg[sel].value)
				{
				case NEOGEO_PAD:
					keycfg[6].value = 6;
					keycfg[7].value = 5;
					keycfg[8].value = 7;
					keycfg[9].value = 8;
					break;
					
				case NEOGEO_PLAY:
					keycfg[6].value = 7;
					keycfg[7].value = 6;
					keycfg[8].value = 8;
					keycfg[9].value = 5;
					break;
				}
					
				case NEOGEO_MVS:
					keycfg[6].value = 7;
					keycfg[7].value = 8;
					keycfg[8].value = 6;
					keycfg[9].value = 5;
					break;
				}
			}
#endif
		}
		else if (pad_pressed(PSP_CTRL_RIGHT))
		{
			if (keycfg[sel].type == KEYCFG_BUTTON)
			{
				if (keycfg[sel].value < 12)
				{
					keycfg[sel].value++;
					update = 1;
				}
			}
			else if (keycfg[sel].type == KEYCFG_NUMBER)
			{
				if (keycfg[sel].value < 10)
				{
					keycfg[sel].value++;
					update = 1;
				}
			}
			else if (keycfg[sel].type == KEYCFG_ANALOG)
			{
				if (keycfg[sel].value < 2)
				{
					keycfg[sel].value++;
					update = 1;
				}
			}
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
			else if (keycfg[sel].type == KEYCFG_LAYOUT)
			{
				if (keycfg[sel].value == 0)
				{
					keycfg[sel].value++;
					update = 1;

					keycfg[6].value = 7;
					keycfg[7].value = 8;
					keycfg[8].value = 6;
					keycfg[9].value = 5;
				}
			}
#endif
		}
		else if (pad_pressed(PSP_CTRL_CIRCLE))
		{
			if (sel == keycfg_num - 1)
				break;
		}
		else if (pad_pressed(PSP_CTRL_RTRIGGER))
		{
			help(HELP_KEYCONFIG);
			update = 1;
		}

		if (top > keycfg_num - rows) top = keycfg_num - rows;
		if (top < 0) top = 0;
		if (sel >= keycfg_num) sel = 0;
		if (sel < 0) sel = keycfg_num - 1;
		if (sel >= top + rows) top = sel - rows + 1;
		if (sel < top) top = sel;

		if (prev_sel != sel) update = 1;

#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		if (update)
		{
			if (keycfg[6].value == 6 && keycfg[7].value == 5 && keycfg[8].value == 7 && keycfg[9].value == 8)
			{
				keycfg[0].value = NEOGEO_PAD;
			}
			else if (keycfg[6].value == 7 && keycfg[7].value == 8 && keycfg[8].value == 6 && keycfg[9].value == 5)
			{
				keycfg[0].value = NEOGEO_MVS;
			}
			else
			{
				keycfg[0].value = USER_DEFINE;
			}
		}
#endif

		pad_update();

		if (Loop == LOOP_EXIT) break;

	} while (!pad_pressed(PSP_CTRL_CROSS));

	for (i = 0; i < keycfg_num; i++)
	{
		if (keycfg[i].type == KEYCFG_BUTTON)
		{
			input_map[keycfg[i].code] = button_value[keycfg[i].value];
		}
		else if (keycfg[i].type == KEYCFG_NUMBER)
		{
			af_interval = keycfg[i].value;
		}
		else if (keycfg[i].type == KEYCFG_ANALOG)
		{
			analog_sensitivity = keycfg[i].value;
		}
	}

	return 0;
}


/*------------------------------------------------------
	ディップスイッチ設定メニュー
------------------------------------------------------*/

#if (EMU_SYSTEM != CPS2 && EMU_SYSTEM != NCDZ)

static int menu_dipswitch(void)
{
	int sel = 0, rows = 13, top = 0, sx = 240;
	int i, arrowl, arrowr, prev_sel, update = 1;
	dipswitch_t *dipswitch;
	int dipswitch_num = 0;

#define INCLUDE_LOAD_DIPSWITCH

#if (EMU_SYSTEM == CPS1)
#include "menu/cps.c"
#elif (EMU_SYSTEM == MVS)
#include "menu/mvs.c"
#endif

#undef INCLUDE_LOAD_DIPSWITCH

	while (dipswitch[dipswitch_num].label[0])
		dipswitch_num++;

	pad_wait_clear();
	load_background(WP_DIPSW);
	ui_popup_reset();

	do
	{
		if (update)
		{
			show_background();

			arrowl = 0;
			arrowr = 0;
			if (dipswitch[sel].mask)
			{
				int cur = dipswitch[sel].value;

				if (cur > 0) arrowl = 1;
				if (cur < dipswitch[sel].value_max) arrowr = 1;
			}

			small_icon_shadow(8, 3, UI_COLOR(UI_PAL_TITLE), ICON_DIPSWITCH);
			uifont_print_shadow(36, 5, UI_COLOR(UI_PAL_TITLE), TEXT(DIP_SWITCH_SETTINGS_MENU));
			draw_scrollbar(470, 26, 479, 270, rows, dipswitch_num, sel);

			for (i = 0; i < rows; i++)
			{
				if ((top + i) >= dipswitch_num) break;

				if (dipswitch[top + i].label[0] == '\n')
					continue;

				if (top + i == sel)
				{
					if (!dipswitch[top + i].enable)
					{
						uifont_print_shadow(16, 40 + i * 17, COLOR_GRAY, dipswitch[top + i].label);
					}
					else
					{
						uifont_print_shadow(16, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), dipswitch[top + i].label);
						if (arrowl)
							uifont_print_shadow(sx - 20, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), FONT_LEFTTRIANGLE);
					}
				}
				else
				{
					if (!dipswitch[top + i].enable)
						uifont_print(16, 40 + i * 17, COLOR_DARKGRAY, dipswitch[top + i].label);
					else
						uifont_print(16, 40 + i * 17, UI_COLOR(UI_PAL_NORMAL), dipswitch[top + i].label);
				}

				if (dipswitch[top + i].mask)
				{
					int val = dipswitch[top + i].value;

					if (top + i == sel)
					{
						if (!dipswitch[top + i].enable)
						{
							uifont_print_shadow(sx, 40 + i * 17, COLOR_GRAY, dipswitch[top + i].values_label[val]);
						}
						else
						{
							uifont_print_shadow(sx, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), dipswitch[top + i].values_label[val]);
							if (arrowr)
							{
								int width = uifont_get_string_width(dipswitch[top + i].values_label[val]);
								uifont_print_shadow(sx + 4 + width, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
							}
						}
					}
					else
					{
						if (!dipswitch[top + i].enable)
							uifont_print(sx, 40 + i * 17, COLOR_DARKGRAY, dipswitch[top + i].values_label[val]);
						else
							uifont_print(sx, 40 + i * 17, UI_COLOR(UI_PAL_NORMAL), dipswitch[top + i].values_label[val]);
					}
				}
			}

			update  = draw_battery_status(1);
			//update |= draw_volume_status(1);
			update |= ui_show_popup(1);
			video_flip_screen(1);
		}
		else
		{
			update  = draw_battery_status(0);
			//update |= draw_volume_status(0);
			update |= ui_show_popup(0);
			video_wait_vsync();
		}

		prev_sel = sel;

		if (pad_pressed(PSP_CTRL_UP))
		{
			sel--;
			if (sel < 0) sel = dipswitch_num - 1;
			if (dipswitch[sel].label[0] == '\n') sel--;
		}
		else if (pad_pressed(PSP_CTRL_DOWN))
		{
			sel++;
			if (sel > dipswitch_num - 1) sel = 0;
			if (dipswitch[sel].label[0] == '\n') sel++;
		}
		else if (pad_pressed(PSP_CTRL_LEFT))
		{
			if (dipswitch[sel].mask)
			{
				int cur = dipswitch[sel].value;

				if (cur > 0)
				{
					dipswitch[sel].value = cur - 1;
					update = 1;
				}
			}
		}
		else if (pad_pressed(PSP_CTRL_RIGHT))
		{
			if (dipswitch[sel].mask)
			{
				int cur = dipswitch[sel].value;

				if (cur < dipswitch[sel].value_max)
				{
					dipswitch[sel].value = cur + 1;
					update = 1;
				}
			}
		}
		else if (pad_pressed(PSP_CTRL_CIRCLE))
		{
			if (sel == dipswitch_num - 1)
				break;
		}
		else if (pad_pressed(PSP_CTRL_RTRIGGER))
		{
			help(HELP_DIPSWITCH);
			update = 1;
		}

		if (top > dipswitch_num - rows) top = dipswitch_num - rows;
		if (top < 0) top = 0;
		if (sel >= dipswitch_num) sel = 0;
		if (sel < 0) sel = dipswitch_num - 1;
		if (sel >= top + rows) top = sel - rows + 1;
		if (sel < top) top = sel;

		if (prev_sel != sel) update = 1;

		pad_update();

		if (Loop == LOOP_EXIT) break;

	} while (!pad_pressed(PSP_CTRL_CROSS));

#define INCLUDE_SAVE_DIPSWITCH

#if (EMU_SYSTEM == CPS1)
#include "menu/cps.c"
#elif (EMU_SYSTEM == MVS)
#include "menu/mvs.c"
#endif

#undef INCLUDE_SAVE_DIPSWITCH

	return 0;
}

#endif


/*------------------------------------------------------
	セーブ/ロードステート
------------------------------------------------------*/

#ifdef SAVE_STATE

#define STATE_FUNC_SAVE		0
#define STATE_FUNC_LOAD		1
#define STATE_FUNC_DEL		2


static UINT8 slot[10];
static int state_func;
static int state_sel;

static void state_draw_thumbnail(void)
{
	void *tex = UI_TEXTURE;

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
	if (machine_screen_type)
	{
		RECT clip1 = { 0, 0, 112, 152 };
		RECT clip2 = { 317, 34, 317+112, 34+152 };
		video_draw_texture(GU_PSM_5551, VRAM_FMT, tex, draw_frame, &clip1, &clip2);
	}
	else
#endif
	{
		RECT clip1 = { 0, 0, 152, 112 };
		RECT clip2 = { 298, 52, 298+152, 52+112 };
		video_draw_texture(GU_PSM_5551, VRAM_FMT, tex, draw_frame, &clip1, &clip2);
	}
}

static void state_refresh_screen(int reload_thumbnail)
{
	int i, x;
	char name[16], state[32], buf[64];

	if (reload_thumbnail)
	{
		find_state_file(slot);

		if (!slot[state_sel])
		{
			state_clear_thumbnail();
			state_func = STATE_FUNC_SAVE;
		}
		else
		{
			state_load_thumbnail(state_sel);
			if (state_version == current_state_version)
				state_func = STATE_FUNC_LOAD;
			else
			{
				state_func = STATE_FUNC_SAVE;
				strcpy(date_str, "----/--/--");
				strcpy(time_str, "--:--:--");
			}
		}
	}

	show_background();
	if (machine_screen_type)
		draw_box_shadow(318, 33, 429, 184);
	else
		draw_box_shadow(298, 52, 449, 163);
	state_draw_thumbnail();

	sprintf(buf, "%s %s", FONT_LTRIGGER, TEXT(MAIN_MENU));
	i = uifont_get_string_width(buf);
	uifont_print_shadow(8, 5, UI_COLOR(UI_PAL_NORMAL), buf);

	sprintf(buf, "%s %s", TEXT(SAVE_LOAD_STATE), FONT_RTRIGGER);
	uifont_print_shadow(i + 16, 5, UI_COLOR(UI_PAL_NORMAL), "|");
	uifont_print_shadow(i + 24, 5, UI_COLOR(UI_PAL_TITLE), buf);

	if (ui_text_get_language() == LANG_JAPANESE)
		x = 12;
	else
		x = 0;

	for (i = 0; i < 10; i++)
	{
		sprintf(name, TEXT(SLOTx), i);
		if (slot[i])
			sprintf(state, "%s.sv%d", game_name, i);
		else
			strcpy(state, TEXT(EMPTY));

		if (i == state_sel)
		{
			small_icon_light(12, 38 + i * 22, UI_COLOR(UI_PAL_SELECT), ICON_MEMSTICK);
			uifont_print_shadow(40, 40 + i * 22, UI_COLOR(UI_PAL_SELECT), name);
			uifont_print_shadow(92 + x, 40 + i * 22, UI_COLOR(UI_PAL_SELECT), state);

			if (slot[state_sel])
			{
				if (state_func == STATE_FUNC_LOAD)
				{
					uifont_print_shadow(200, 40 + i * 22, UI_COLOR(UI_PAL_SELECT), FONT_LEFTTRIANGLE);
					uifont_print_shadow(222, 40 + i * 22, UI_COLOR(UI_PAL_SELECT), TEXT(LOAD));
					uifont_print_shadow(262, 40 + i * 22, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
				}
				else if (state_func == STATE_FUNC_DEL)
				{
					uifont_print_shadow(200, 40 + i * 22, UI_COLOR(UI_PAL_SELECT), FONT_LEFTTRIANGLE);
					uifont_print_shadow(222, 40 + i * 22, UI_COLOR(UI_PAL_SELECT), TEXT(DELETE));
				}
				else
				{
					uifont_print_shadow(222, 40 + i * 22, UI_COLOR(UI_PAL_SELECT), TEXT(SAVE));
					uifont_print_shadow(262, 40 + i * 22, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
				}
			}
			else
			{
				uifont_print_shadow(222, 40 + i * 22, UI_COLOR(UI_PAL_SELECT), TEXT(SAVE));
			}

			hline_gradation(40, 278, 56 + i * 22, UI_COLOR(UI_PAL_NORMAL), UI_COLOR(UI_PAL_SELECT), 14);
			hline_gradation(41, 279, 57 + i * 22, COLOR_BLACK, COLOR_BLACK, 8);
		}
		else
		{
			small_icon(12, 38 + i * 22, UI_COLOR(UI_PAL_NORMAL), ICON_MEMSTICK);
			uifont_print(40, 40 + i * 22, UI_COLOR(UI_PAL_NORMAL), name);
			uifont_print(92 + x, 40 + i * 22, UI_COLOR(UI_PAL_NORMAL), state);
		}
	}

	if (state_version == current_state_version || !slot[state_sel])
	{
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		if (machine_screen_type)
		{
			uifont_print(300, 198, UI_COLOR(UI_PAL_NORMAL), TEXT(PLAY_DATE));
			uifont_print(378, 198, UI_COLOR(UI_PAL_NORMAL), date_str);
			uifont_print(300, 218, UI_COLOR(UI_PAL_NORMAL), TEXT(SAVE_TIME));
			uifont_print(378, 218, UI_COLOR(UI_PAL_NORMAL), time_str);
			uifont_print(300, 238, UI_COLOR(UI_PAL_NORMAL), TEXT(STATE_VERSION));
			uifont_print(378, 238, UI_COLOR(UI_PAL_NORMAL), stver_str);
		}
		else
#endif
		{
			uifont_print(290, 190, UI_COLOR(UI_PAL_NORMAL), TEXT(PLAY_DATE));
			uifont_print(368, 190, UI_COLOR(UI_PAL_NORMAL), date_str);
			uifont_print(290, 210, UI_COLOR(UI_PAL_NORMAL), TEXT(SAVE_TIME));
			uifont_print(368, 210, UI_COLOR(UI_PAL_NORMAL), time_str);
			uifont_print(290, 230, UI_COLOR(UI_PAL_NORMAL), TEXT(STATE_VERSION));
			uifont_print(368, 230, UI_COLOR(UI_PAL_NORMAL), stver_str);
		}
	}
	else
	{
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		if (machine_screen_type)
		{
			uifont_print(300, 198, COLOR_GRAY, TEXT(PLAY_DATE));
			uifont_print(378, 198, COLOR_GRAY, date_str);
			uifont_print(300, 218, COLOR_GRAY, TEXT(SAVE_TIME));
			uifont_print(378, 218, COLOR_GRAY, time_str);
			uifont_print(300, 238, COLOR_GRAY, TEXT(STATE_VERSION));
			uifont_print(378, 238, COLOR_GRAY, stver_str);
		}
		else
#endif
		{
			uifont_print(290, 190, COLOR_GRAY, TEXT(PLAY_DATE));
			uifont_print(368, 190, COLOR_GRAY, date_str);
			uifont_print(290, 210, COLOR_GRAY, TEXT(SAVE_TIME));
			uifont_print(368, 210, COLOR_GRAY, time_str);
			uifont_print(290, 230, COLOR_GRAY, TEXT(STATE_VERSION));
			uifont_print(368, 230, COLOR_GRAY, stver_str);
		}
	}
}

static int state_save_slot(void)
{
	if (messagebox(MB_STARTSAVESTATE))
	{
		int res;

		state_refresh_screen(0);
		video_copy_rect(draw_frame, work_frame, &full_rect, &full_rect);

		set_cpu_clock(psp_cpuclock);
		res = state_save(state_sel);
		set_cpu_clock(PSPCLOCK_222);

		load_background(WP_STATE);

		if (res)
		{
			state_refresh_screen(1);
			draw_battery_status(1);
			video_copy_rect(draw_frame, show_frame, &full_rect, &full_rect);
			res = messagebox(MB_FINISHSAVESTATE);
		}
		return res + 1;
	}
	return 0;
}

static int state_load_slot(void)
{
	if (messagebox(MB_STARTLOADSTATE))
	{
		int res;

		state_refresh_screen(0);
		video_copy_rect(draw_frame, work_frame, &full_rect, &full_rect);

		set_cpu_clock(psp_cpuclock);
		res = state_load(state_sel);
		set_cpu_clock(PSPCLOCK_222);

		if (res)
		{
			show_background();
			draw_battery_status(1);
			video_flip_screen(1);

			messagebox(MB_FINISHLOADSTATE);
		}

		load_background(WP_STATE);
		return res + 1;
	}
	return 0;
}

static void state_delete_slot(void)
{
	if (messagebox(MB_DELETESTATE))
	{
		char path[MAX_PATH];

		sprintf(path, "%sstate/%s.sv%d", launchDir, game_name, state_sel);
		if (sceIoRemove(path) < 0)
		{
			ui_popup(TEXT(FAILD_TO_DELETE_FILEx), strrchr(path, '/') + 1);
		}
		else
		{
			slot[state_sel] = 0;
			state_func = STATE_FUNC_SAVE;
			state_clear_thumbnail();
		}
	}
}

static int menu_state(void)
{
	int prev_sel = -1, prev_func = -1, update = 1;
	int thumbnail_loaded = 0;

	state_func = STATE_FUNC_SAVE;
	state_sel = 0;
	thumbnail_loaded = 0;

	pad_wait_clear();
	load_background(WP_STATE);
	ui_popup_reset();

	memset(slot, 0, sizeof(slot));
	state_clear_thumbnail();

	find_state_file(slot);

	do
	{
		if (update & UI_FULL_REFRESH)
		{
			state_refresh_screen((prev_sel == state_sel) ? 0 : 1);

			update  = draw_battery_status(1);
			//update |= draw_volume_status(1);
			update |= ui_show_popup(1);
			video_flip_screen(1);
		}
		else if (update & UI_PARTIAL_REFRESH)
		{
			int x, y, w, h;
			RECT clip1, clip2;

			show_background();

			small_icon_light(12, 38 + state_sel * 22, UI_COLOR(UI_PAL_SELECT), ICON_MEMSTICK);

			x = 12;
			y = 38 + state_sel * 22;
			w = 24 + 8;
			h = 18 + 8;

			clip1.left   = x - 4;
			clip1.top    = y - 4;
			clip1.right  = clip1.left + w;
			clip1.bottom = clip1.top  + h;

			clip2.left   = 0;
			clip2.top    = 112;
			clip2.right  = clip2.left + w;
			clip2.bottom = clip2.top  + h;

			video_copy_rect(draw_frame, tex_frame, &clip1, &clip2);
			video_copy_rect(show_frame, draw_frame, &full_rect, &full_rect);
			video_copy_rect(tex_frame, draw_frame, &clip2, &clip1);

			update  = draw_battery_status(0);
			//update |= draw_volume_status(0);
			update |= ui_show_popup(0);
			video_flip_screen(1);
		}
		else
		{
			update  = draw_battery_status(0);
			//update |= draw_volume_status(0);
			update |= ui_show_popup(0);
			video_wait_vsync();
		}

		update |= ui_light_update();
		prev_sel = state_sel;
		prev_func = state_func;

		if (pad_pressed(PSP_CTRL_UP))
		{
			state_sel--;
			if (state_sel < 0) state_sel = 10 - 1;
		}
		else if (pad_pressed(PSP_CTRL_DOWN))
		{
			state_sel++;
			if (state_sel >= 10) state_sel = 0;
		}
		else if (pad_pressed(PSP_CTRL_LEFT))
		{
			if (state_func > STATE_FUNC_SAVE) state_func--;
			if (state_version != current_state_version && state_func == STATE_FUNC_LOAD)
				state_func--;
		}
		else if (pad_pressed(PSP_CTRL_RIGHT))
		{
			if (slot[state_sel])
			{
				if (state_func < STATE_FUNC_DEL) state_func++;
				if (state_version != current_state_version && state_func == STATE_FUNC_LOAD)
					state_func++;
			}
		}
		else if (pad_pressed(PSP_CTRL_CIRCLE))
		{
			int res = 0;

			if (state_func == STATE_FUNC_LOAD)
			{
				if (slot[state_sel])
				{
					if ((res = state_load_slot()) != 0)
					{
						update = 1;
						res--;
					}
				}
			}
			else if (state_func == STATE_FUNC_DEL)
			{
				if (slot[state_sel])
				{
					state_delete_slot();
					update = 1;
				}
			}
			else
			{
				if ((res = state_save_slot()) != 0)
				{
					update = 1;
					res--;
				}
			}
			if (res) return 1;
		}
		else if (pad_pressed(PSP_CTRL_RTRIGGER))
		{
			help(HELP_STATE);
			update = 1;
		}

		if (prev_sel != state_sel || prev_func != state_func)
			update = 1;

		if (prev_sel != state_sel)
			find_state_file(slot);

		pad_update();

		if (Loop == LOOP_EXIT) break;

	} while (!pad_pressed(PSP_CTRL_LTRIGGER) && !pad_pressed(PSP_CTRL_CROSS));

	return 0;
}

#endif


/*------------------------------------------------------
	メインメニュー
------------------------------------------------------*/

#define MENU_MAX_ITEMS	16

typedef struct
{
	const char *label;
	int (*menu_func)(void);
	int icon;
	const char *help;
} menu_t;

typedef struct
{
	int label;
	int (*menu_func)(void);
	int icon;
	int help;
} menu2_t;


static menu2_t mainmenu2[] =
{
	{ MENU_GAME_CONFIGURATION,  menu_gamecfg,   ICON_CONFIG,    MENUHELP_GAME_CONFIGURATION  },
	{ MENU_KEY_CONFIGURATION,   menu_keycfg,    ICON_KEYCONFIG, MENUHELP_KEY_CONFIGURATION   },
#ifdef COMMAND_LIST
	{ MENU_COMMAND_LIST,        menu_cmdlist,   ICON_CMDLIST,   MENUHELP_COMMAND_LIST        },
#endif
#if (EMU_SYSTEM != CPS2 && EMU_SYSTEM != NCDZ)
	{ MENU_DIP_SWITCH_SETTINGS, menu_dipswitch, ICON_DIPSWITCH, MENUHELP_DIP_SWITCH_SETTINGS },
#endif
	{ MENU_RESET_EMULATION,     menu_reset,     ICON_SYSTEM,    MENUHELP_RESET_EMULATION     },
	{ MENU_RETURN_TO_BROWSER,   menu_browser,   ICON_FOLDER,    MENUHELP_RETURN_TO_BROWSER   },
	{ MENU_RETURN_TO_GAME,      NULL,           ICON_RETURN,    MENUHELP_RETURN_TO_GAME      },
	{ MENU_EXIT_EMULATOR,       menu_exit,      ICON_EXIT,      MENUHELP_EXIT_EMULATOR       },
	MENU_END
};


void showmenu(void)
{
	static int sel = 0;
	int rows = 7, top = 0;
	int i, prev_sel = 0, update = 1;
	int mainmenu_num = 0;
	char buf[128];
	menu_t mainmenu[MENU_MAX_ITEMS];

#ifdef SAVE_STATE
	state_make_thumbnail();
#endif
	video_set_mode(32);

#if (EMU_SYSTEM == NCDZ)
	if (cdda_playing == CDDA_PLAY) mp3_pause(1);
#endif
	sound_mute(1);
#if USE_CACHE
	cache_sleep(1);
#endif

	i = 0;
	while (mainmenu2[i].label)
	{
		mainmenu[i].label     = TEXT(mainmenu2[i].label);
		mainmenu[i].menu_func = mainmenu2[i].menu_func;
		mainmenu[i].icon      = mainmenu2[i].icon;
		mainmenu[i].help      = TEXT(mainmenu2[i].help);
		i++;
	}
	mainmenu_num = i;

	set_cpu_clock(PSPCLOCK_222);
	video_clear_screen();
	load_background(WP_LOGO);
	ui_popup_reset();
	pad_wait_clear();

	do
	{
		if (update & UI_FULL_REFRESH)
		{
			show_background();

#ifdef SAVE_STATE
			sprintf(buf, "%s %s", FONT_LTRIGGER, TEXT(MAIN_MENU));
			i = uifont_get_string_width(buf);
			uifont_print_shadow(8, 5, UI_COLOR(UI_PAL_TITLE), buf);

			sprintf(buf, "%s %s", TEXT(SAVE_LOAD_STATE), FONT_RTRIGGER);
			uifont_print_shadow(i + 16, 5, UI_COLOR(UI_PAL_NORMAL), "|");
			uifont_print_shadow(i + 24, 5, UI_COLOR(UI_PAL_NORMAL), buf);
#else
			small_icon_shadow(8, 3, UI_COLOR(UI_PAL_TITLE), ICON_SYSTEM);
			uifont_print_shadow(36, 5, UI_COLOR(UI_PAL_TITLE), TEXT(MAIN_MENU));
#endif
			draw_scrollbar(470, 26, 479, 270, rows, mainmenu_num, sel);

			for (i = 0; i < rows; i++)
			{
				if ((top + i) >= mainmenu_num) break;

				if (mainmenu[top + i].label[0] == '\n')
					continue;

				if (top + i == sel)
				{
					large_icon_light(12, 40 + i * 32, UI_COLOR(UI_PAL_SELECT), mainmenu[sel].icon);
					uifont_print_shadow(54, 37 + i * 32, UI_COLOR(UI_PAL_SELECT), mainmenu[sel].label);
					uifont_print_shadow(54, 56 + i * 32, UI_COLOR(UI_PAL_SELECT), mainmenu[sel].help);

					hline_gradation(54, 460, 53 + i * 32, UI_COLOR(UI_PAL_NORMAL), UI_COLOR(UI_PAL_SELECT), 14);
					hline_gradation(55, 461, 54 + i * 32, COLOR_BLACK, COLOR_BLACK, 8);
				}
				else
				{
					small_icon(16, 42 + i * 32, UI_COLOR(UI_PAL_NORMAL), mainmenu[top + i].icon);
					uifont_print(56, 44 + i * 32, UI_COLOR(UI_PAL_NORMAL), mainmenu[top + i].label);
				}
			}

			update  = draw_battery_status(1);
			//update |= draw_volume_status(1);
			update |= ui_show_popup(1);
			video_flip_screen(1);
		}
		else if (update & UI_PARTIAL_REFRESH)
		{
			int x, y, w, h;
			RECT clip1, clip2;

			show_background();

			for (i = 0; i < rows; i++)
				if (top + i == sel) break;

			large_icon_light(12, 40 + i * 32, UI_COLOR(UI_PAL_SELECT), mainmenu[sel].icon);

			x = 12;
			y = 40 + i * 32;
			w = 32 + 8;
			h = 24 + 8;

			clip1.left   = x - 4;
			clip1.top    = y - 4;
			clip1.right  = clip1.left + w;
			clip1.bottom = clip1.top  + h;

			clip2.left   = 0;
			clip2.top    = 112;
			clip2.right  = clip2.left + w;
			clip2.bottom = clip2.top  + h;

			video_copy_rect(draw_frame, tex_frame, &clip1, &clip2);
			video_copy_rect(show_frame, draw_frame, &full_rect, &full_rect);
			video_copy_rect(tex_frame, draw_frame, &clip2, &clip1);

			update  = draw_battery_status(0);
			//update |= draw_volume_status(0);
			update |= ui_show_popup(0);
			video_flip_screen(1);
		}
		else
		{
			update  = draw_battery_status(0);
			//update |= draw_volume_status(0);
			update |= ui_show_popup(0);
			video_wait_vsync();
		}

		update |= ui_light_update();
		prev_sel = sel;

		if (pad_pressed(PSP_CTRL_UP))
		{
			sel--;
			if (sel < 0) sel = mainmenu_num - 1;
		}
		else if (pad_pressed(PSP_CTRL_DOWN))
		{
			sel++;
			if (sel >= mainmenu_num) sel = 0;
		}
		else if (pad_pressed(PSP_CTRL_CIRCLE))
		{
			if (mainmenu[sel].menu_func)
			{
				int res = mainmenu[sel].menu_func();

				if (res == 0)
				{
					pad_wait_clear();
					load_background(WP_LOGO);
					update = 1;
				}
				else break;
			}
			else break;
		}
#ifdef SAVE_STATE
		else if (pad_pressed(PSP_CTRL_RTRIGGER))
		{
			if (menu_state()) break;
			pad_wait_clear();
			load_background(WP_LOGO);
			update = 1;
		}
#endif

		if (top > mainmenu_num - rows) top = mainmenu_num - rows;
		if (top < 0) top = 0;
		if (sel >= mainmenu_num) sel = 0;
		if (sel < 0) sel = mainmenu_num - 1;
		if (sel >= top + rows) top = sel - rows + 1;
		if (sel < top) top = sel;

		if (prev_sel != sel) update |= 1;

		pad_update();

		if (Loop == LOOP_EXIT) break;

	} while (!pad_pressed(PSP_CTRL_CROSS));

	autoframeskip_reset();
	blit_clear_all_sprite();

	pad_wait_clear();
	ui_popup_reset();
	video_set_mode(16);
	video_clear_screen();
	video_clear_frame(work_frame);
//#if (EMU_SYSTEM != CPS2)
	//sound_set_samplerate();
//#endif
	set_cpu_clock(psp_cpuclock);

#if USE_CACHE
	cache_sleep(0);
#endif

#if (EMU_SYSTEM == CPS1)
    if (machine_sound_type == SOUND_QSOUND)
        qsound_set_samplerate();
    else
        YM2151_set_samplerate();
#elif (EMU_SYSTEM == CPS2)
    qsound_set_samplerate();
#elif (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
    YM2610_set_samplerate();
	
//#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
	neogeo_reset_driver_type();
#endif

	if (Loop == LOOP_EXEC)
	{
		sound_thread_set_volume();
#if (EMU_SYSTEM == NCDZ)
		mp3_set_volume();

		if (option_mp3_enable && cdda_playing == CDDA_PLAY)
			mp3_pause(0);
		else
			cdda_stop();
#endif
		sound_mute(0);
	}
	else
	{
#if (EMU_SYSTEM == NCDZ)
		cdda_stop();
#endif
		sound_mute(1);
	}
}


#if PSP_VIDEO_32BPP

/*------------------------------------------------------
	カラー設定メニュー
------------------------------------------------------*/

#define COLOR_ITEMS 12

static const int palette[COLOR_ITEMS] =
{
	-1,
	-1,
	UI_PAL_TITLE,
	UI_PAL_SELECT,
	UI_PAL_NORMAL,
	UI_PAL_INFO,
	UI_PAL_WARNING,
	UI_PAL_FILESEL1,
	UI_PAL_FILESEL2,
	UI_PAL_BG1,
	UI_PAL_FRAME,
	UI_PAL_BG2
};

void show_color_menu(void)
{
	static int sel = 0;
	int i, x[3], sel_y = 0, update = 1, reload_bg = 0;
	UI_PALETTE *pal = NULL;
	char buf[256], rgb_value[3][4];
	const char *color_item[COLOR_ITEMS];
	const char *rgb_name[3];
	const char *bgimage_type_name[4];

	for (i = 0; i < COLOR_ITEMS; i++)
		color_item[i] = TEXT(BACKGROUND_IMAGE_TYPE + i);

	rgb_name[0] = TEXT(RED);
	rgb_name[1] = TEXT(GREEN);
	rgb_name[2] = TEXT(BLUE);

	bgimage_type_name[0] = TEXT(DEFAULT_IMAGE);
	bgimage_type_name[1] = TEXT(USERS_IMAGE);
	bgimage_type_name[2] = TEXT(LOGO_PNG_ONLY);
	bgimage_type_name[3] = TEXT(NOT_USE);

	pad_wait_clear();
	load_background(WP_COLORCFG);
	ui_popup_reset();

	do
	{
		if (update)
		{
			show_background();

			small_icon_shadow(8, 3, UI_COLOR(UI_PAL_TITLE), ICON_COLORCFG);
			uifont_print_shadow(36, 5, UI_COLOR(UI_PAL_TITLE), TEXT(COLOR_SETTINGS_MENU));

			for (i = 2; i <= 6; i++)
			{
				if (i < 4)
					uifont_print_shadow(8, 40 + (i - 2) * 18, UI_COLOR(palette[i]), color_item[i]);
				else
					uifont_print(8, 40 + (i - 2) * 18, UI_COLOR(palette[i]), color_item[i]);
			}
			boxfill_gradation(4, 38 + 5 * 18, 475, 58 + 5 * 18, UI_COLOR(UI_PAL_FILESEL1), UI_COLOR(UI_PAL_FILESEL2), 8, 0);
			uifont_print_shadow(8, 40 + 5 * 18, UI_COLOR(UI_PAL_SELECT), TEXT(FILE_SELECT_BAR));

			draw_dialog(48, 160, 431, 258);

			uifont_print_center(174, UI_COLOR(UI_PAL_INFO), color_item[sel]);
			if (!sel_y)
			{
				uifont_print(56, 174, UI_COLOR(UI_PAL_SELECT), FONT_LEFTTRIANGLE);
				uifont_print(409, 174, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
			}

			switch (sel)
			{
			case 0:
				if (sel_y)
				{
					if (bgimage_type > 0)  uifont_print(180, 213, UI_COLOR(UI_PAL_SELECT), FONT_LEFTTRIANGLE);
					if (bgimage_type < 3) uifont_print(282, 213, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
					uifont_print_center(213, UI_COLOR(UI_PAL_SELECT), bgimage_type_name[bgimage_type]);
				}
				else
					uifont_print_center(213, UI_COLOR(UI_PAL_NORMAL), bgimage_type_name[bgimage_type]);
				break;

			case 1:
				sprintf(buf, "%3d%%", bgimage_blightness);
				if (sel_y)
				{
					if (bgimage_blightness != 0)   uifont_print(200, 213, UI_COLOR(UI_PAL_SELECT), FONT_LEFTTRIANGLE);
					if (bgimage_blightness != 100) uifont_print(262, 213, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
					uifont_print_center(213, UI_COLOR(UI_PAL_SELECT), buf);
				}
				else
					uifont_print_center(213, UI_COLOR(UI_PAL_NORMAL), buf);
				break;

			default:
				pal = &ui_palette[palette[sel]];
				sprintf(rgb_value[0], "%d", pal->r);
				sprintf(rgb_value[1], "%d", pal->g);
				sprintf(rgb_value[2], "%d", pal->b);
				x[0] = 420 - uifont_get_string_width(rgb_value[0]);
				x[1] = 420 - uifont_get_string_width(rgb_value[1]);
				x[2] = 420 - uifont_get_string_width(rgb_value[2]);

				if (sel_y != 1)
				{
					uifont_print(59, 198, UI_COLOR(UI_PAL_NORMAL), rgb_name[0]);
					uifont_print(x[0], 198, UI_COLOR(UI_PAL_NORMAL), rgb_value[0]);
				}
				else
				{
					uifont_print(59, 198, UI_COLOR(UI_PAL_SELECT), rgb_name[0]);
					uifont_print(x[0], 198, UI_COLOR(UI_PAL_SELECT), rgb_value[0]);
					box(121, 198, 121 + 255 + 1, 198 + 13, UI_COLOR(UI_PAL_SELECT));
					if (pal->r > 0)   uifont_print(105, 198, UI_COLOR(UI_PAL_SELECT), FONT_LEFTTRIANGLE);
					if (pal->r < 255) uifont_print(377, 198, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
				}
				if (sel_y != 2)
				{
					uifont_print(59, 214, UI_COLOR(UI_PAL_NORMAL), rgb_name[1]);
					uifont_print(x[1], 214, UI_COLOR(UI_PAL_NORMAL), rgb_value[1]);
				}
				else
				{
					uifont_print(59, 214, UI_COLOR(UI_PAL_SELECT), rgb_name[1]);
					uifont_print(x[1], 214, UI_COLOR(UI_PAL_SELECT), rgb_value[1]);
					box(121, 214, 121 + 255 + 1, 214 + 13, UI_COLOR(UI_PAL_SELECT));
					if (pal->g > 0)   uifont_print(105, 214, UI_COLOR(UI_PAL_SELECT), FONT_LEFTTRIANGLE);
					if (pal->g < 255) uifont_print(377, 214, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
				}
				if (sel_y != 3)
				{
					uifont_print(59, 230, UI_COLOR(UI_PAL_NORMAL), rgb_name[2]);
					uifont_print(x[2], 230, UI_COLOR(UI_PAL_NORMAL), rgb_value[2]);
				}
				else
				{
					uifont_print(59, 230, UI_COLOR(UI_PAL_SELECT), rgb_name[2]);
					uifont_print(x[2], 230, UI_COLOR(UI_PAL_SELECT), rgb_value[2]);
					box(121, 230, 121 + 255 + 1, 230 + 13, UI_COLOR(UI_PAL_SELECT));
					if (pal->b > 0)   uifont_print(105, 230, UI_COLOR(UI_PAL_SELECT), FONT_LEFTTRIANGLE);
					if (pal->b < 255) uifont_print(377, 230, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
				}
				boxfill(121 + 1, 198 + 1, 121 + pal->r, 198 + 12, COLOR_RED);
				boxfill(121 + 1, 214 + 1, 121 + pal->g, 214 + 12, COLOR_GREEN);
				boxfill(121 + 1, 230 + 1, 121 + pal->b, 230 + 12, COLOR_BLUE);
				break;
			}

			update  = draw_battery_status(1);
			update |= ui_show_popup(1);
			video_flip_screen(1);
		}
		else
		{
			update  = draw_battery_status(0);
			update |= ui_show_popup(0);
			video_wait_vsync();
		}

		if (pad_pressed(PSP_CTRL_UP))
		{
			if (sel_y > 0)
			{
				sel_y--;
				update = 1;
			}
		}
		else if (pad_pressed(PSP_CTRL_DOWN))
		{
			if (sel < 2)
			{
				if (sel_y == 0)
				{
					sel_y = 1;
					update = 1;
				}
			}
			else
			{
				if (sel_y < 3)
				{
					sel_y++;
					update = 1;
				}
			}
		}
		else if (pad_pressed(PSP_CTRL_LEFT))
		{
			if (sel_y == 0)
			{
				sel--;
				if (sel < 0) sel = COLOR_ITEMS - 1;
			}
			else
			{
				switch (sel)
				{
				case 0:
					if (bgimage_type > 0)
					{
						bgimage_type--;
						set_wallpaper();
						reload_bg = 1;
					}
					break;

				case 1:
					bgimage_blightness -= 5;
					if (bgimage_blightness < 5)
						bgimage_blightness = 5;
					reload_bg = 1;
					break;

				default:
					switch (sel_y)
					{
					case 1: if (pal->r > 0) pal->r--; break;
					case 2: if (pal->g > 0) pal->g--; break;
					case 3: if (pal->b > 0) pal->b--; break;
					}
					if (sel == 2 || sel == 11) reload_bg = 1;
					break;
				}
			}
			update = 1;
		}
		else if (pad_pressed(PSP_CTRL_RIGHT))
		{
			if (sel_y == 0)
			{
				sel++;
				if (sel >= COLOR_ITEMS) sel = 0;
			}
			else
			{
				switch (sel)
				{
				case 0:
					if (bgimage_type < 3)
					{
						bgimage_type++;
						set_wallpaper();
						reload_bg = 1;
					}
					break;

				case 1:
					bgimage_blightness += 5;
					if (bgimage_blightness > 100)
						bgimage_blightness = 100;
					reload_bg = 1;
					break;

				default:
					switch (sel_y)
					{
					case 1: if (pal->r < 255) pal->r++; break;
					case 2: if (pal->g < 255) pal->g++; break;
					case 3: if (pal->b < 255) pal->b++; break;
					}
					if (sel == 2 || sel == 11) reload_bg = 1;
					break;
				}
			}
			update = 1;
		}
		else if (pad_pressed(PSP_CTRL_CIRCLE))
		{
			if (sel >= 2)
			{
				switch (sel_y)
				{
				case 1: pal->r += 5; if (pal->r > 255) pal->r = 255; break;
				case 2: pal->g += 5; if (pal->g > 255) pal->g = 255; break;
				case 3: pal->b += 5; if (pal->b > 255) pal->b = 255; break;
				}
				if (sel == 2 || sel == 11) reload_bg = 1;
			}
			update = 1;
		}
		else if (pad_pressed(PSP_CTRL_SQUARE))
		{
			if (sel >= 2)
			{
				switch (sel_y)
				{
				case 1: pal->r -= 5; if (pal->r < 0) pal->r = 0; break;
				case 2: pal->g -= 5; if (pal->g < 0) pal->g = 0; break;
				case 3: pal->b -= 5; if (pal->b < 0) pal->b = 0; break;
				}
				if (sel == 2 || sel == 11) reload_bg = 1;
			}
			update = 1;
		}
		else if (pad_pressed(PSP_CTRL_TRIANGLE))
		{
			bgimage_type = BG_DEFAULT;
			bgimage_blightness = 75;
			set_ui_color(&ui_palette[UI_PAL_TITLE],    255, 255, 255);
			set_ui_color(&ui_palette[UI_PAL_SELECT],   255, 255, 255);
			set_ui_color(&ui_palette[UI_PAL_NORMAL],   180, 180, 180);
			set_ui_color(&ui_palette[UI_PAL_INFO],     255, 255,  64);
			set_ui_color(&ui_palette[UI_PAL_WARNING],  255,  64,  64);
			set_ui_color(&ui_palette[UI_PAL_BG1],       48,  48,  48);
			set_ui_color(&ui_palette[UI_PAL_BG2],        0,   0, 160);
			set_ui_color(&ui_palette[UI_PAL_FRAME],      0,   0,   0);
			set_ui_color(&ui_palette[UI_PAL_FILESEL1],  40,  40,  40);
			set_ui_color(&ui_palette[UI_PAL_FILESEL2], 120, 120, 120);
			reload_bg = 1;
			update = 1;
		}
		else if (pad_pressed(PSP_CTRL_RTRIGGER))
		{
			help(HELP_COLORSETTINGS);
			update = 1;
		}

		if (reload_bg)
		{
			reload_bg = 0;
			load_background(WP_COLORCFG);
		}

		pad_update();

		if (Loop == LOOP_EXIT) break;

	} while (!pad_pressed(PSP_CTRL_CROSS));

	pad_wait_clear();
	load_background(WP_FILER);
	ui_popup_reset();
}

#endif

