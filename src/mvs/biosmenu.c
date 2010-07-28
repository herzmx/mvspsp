/******************************************************************************

	biosmenu.c

	MVS BIOS選択メニュー

******************************************************************************/

#include "mvs.h"


/******************************************************************************
	グローバル変数
******************************************************************************/

const char *bios_name[BIOS_MAX] =
{
	"Europe MVS (Ver. 2)",
	"Europe MVS (Ver. 1)",
	"US MVS (Ver. 2?)",
	"US MVS (Ver. 1)",
	"Asia MVS (Ver. 3)",
	"Japan MVS (Ver. 3)",
	"Japan MVS (Ver. 2)",
	"Japan MVS (Ver. 1)",

	"Asia AES",

	"Unibios MVS (Hack, Ver. 1.0)",
	"Unibios MVS (Hack, Ver. 1.1)",
	"Unibios MVS (Hack, Ver. 1.2 (old))",
	"Unibios MVS (Hack, Ver. 1.2)",
	"Unibios MVS (Hack, Ver. 1.3)",
	"Unibios MVS (Hack, Ver. 2.0)",
	"Unibios MVS (Hack, Ver. 2.1)",
	"Unibios MVS (Hack, Ver. 2.2)",
	"Debug MVS (Hack?)"
};

const UINT32 bios_crc[BIOS_MAX] =
{
	0x9036d879,	// Europe Ver.2
	0xc7f2fa45,	// Europe Ver.1
	0xe72943de,	// USA Ver.2
	0x2723a5b5,	// USA Ver.1
	0x91b64be3,	// Asia Ver.3
	0xf0e8f27d,	// Japan Ver.3
	0xacede59c,	// Japan Ver.2
	0x9fb0abe4,	// Japan ver.1

	0xd27a71f1,	// Asia AES

	0x0ce453a0,	// Unibios V1.0
	0x5dda0d84,	// Unibios V1.1
	0xe19d3ce9,	// UniBIOS V1.2 (old)
	0x4fa698e9,	// Unibios V1.2
	0xb24b44a0,	// Unibios V1.3
	0x0c12c2ad,	// Unibios V2.0
	0x8dabf76b,	// Unibios V2.1
	0x2d50996a,	// Unibios V2.2
	0x698ebb7d	// Debug BIOS
};

const UINT32 bios_patch_address[BIOS_MAX] =
{
	0x011c62,	// Europe Ver.2
	0x011c62,	// Europe Ver.1
	0x011c38,	// USA Ver.2
	0x011c38,	// USA Ver.1
	0x010c62,	// Asia Ver.3
	0x011d8a,	// Japan Ver.3
	0x011c62,	// Japan Ver.2
	0x011c62,	// Japan Ver.1

	0x000000,	// Asia AES

	0x000000,	// Unibios V1.0
	0x000000,	// Unibios V1.1
	0x000000,	// UniBIOS V1.2 (old)
	0x000000,	// Unibios V1.2
	0x000000,	// Unibios V1.3
	0x000000,	// Unibios V2.0
	0x000000,	// Unibios V2.1
	0x000000,	// Unibios V2.2
	0x000000	// Debug BIOS
};


const UINT32 sfix_crc = 0x354029fc;


/******************************************************************************
	ローカル変数
******************************************************************************/

static int bios_exist[BIOS_MAX];
static int sfix_exist;


/******************************************************************************
	ローカル関数
******************************************************************************/

/*------------------------------------------------------
	BIOS存在チェック
------------------------------------------------------*/

static int bios_check(int flag)
{
	int i, f, count;

	if (!flag) ui_popup_reset(POPUP_MENU);

	video_copy_rect(show_frame, draw_frame, &full_rect, &full_rect);
	video_flip_screen(1);

#ifdef ADHOC
	if (flag == 2)
	{
		neogeo_bios = -1;

		for (i = 0; i < BIOS_MAX; i++)
			bios_exist[i] = 0;

		for (i = 0; i < ASIA_AES; i++)
		{
			if (file_open("neogeo", NULL, bios_crc[i], NULL) != -1)
			{
				bios_exist[i] = 1;
				file_close();
			}
		}
	}
	else
#endif
	{
		for (i = 0; i < BIOS_MAX; i++)
		{
			bios_exist[i] = 0;

			if (file_open("neogeo", NULL, bios_crc[i], NULL) != -1)
			{
				bios_exist[i] = 1;
				file_close();
			}
		}
	}

	count = 0;

	for (i = 0; i < BIOS_MAX; i++)
		count += bios_exist[i];

	if (count == 0)
	{
		if (!flag)
			ui_popup(TEXT(BIOS_NOT_FOUND));
		else
			fatalerror(TEXT(BIOS_NOT_FOUND));
		return 0;
	}

	if ((f = file_open("neogeo", NULL, sfix_crc, NULL)) != -1)
	{
		sfix_exist = 1;
		file_close();
	}
	else
	{
		sfix_exist = 0;
		if (!flag)
			ui_popup(TEXT(SFIX_NOT_FOUND));
		else
			fatalerror(TEXT(SFIX_NOT_FOUND));
		return 0;
	}

	return 1;
}


/******************************************************************************
	グローバル関数
******************************************************************************/

/*------------------------------------------------------
	BIOS選択メニュー
------------------------------------------------------*/

void bios_select(int flag)
{
	int sel = 0, rows = 13, top = 0;
	int i, prev_sel, update = 1;
	int old_bios = neogeo_bios;

	if (!bios_check(flag)) return;

	if (neogeo_bios == -1)
	{
		sel = 0;
		while (sel < BIOS_MAX)
		{
			if (bios_exist[sel]) break;
			sel++;
		}
	}
	else sel = neogeo_bios;

	if (top > BIOS_MAX - rows) top = BIOS_MAX - rows;
	if (top < 0) top = 0;
	if (sel >= BIOS_MAX) sel = 0;
	if (sel < 0) sel = BIOS_MAX - 1;
	if (sel >= top + rows) top = sel - rows + 1;
	if (sel < top) top = sel;

	pad_wait_clear();
	load_background(BG_DEFAULT);
	ui_popup_reset(POPUP_MENU);

	while (1)
	{
		if (update)
		{
			int width = uifont_get_string_width(TEXT(SELECT_BIOS_AND_PRESS_CIRCLE_BUTTON));

			show_background();

			small_icon(8, 3, UI_COLOR(UI_PAL_TITLE), ICON_SYSTEM);
			uifont_print(36, 5, UI_COLOR(UI_PAL_TITLE), TEXT(BIOS_SELECT_MENU));
			uifont_print(477 - width, 271 - 16, UI_COLOR(UI_PAL_SELECT), TEXT(SELECT_BIOS_AND_PRESS_CIRCLE_BUTTON));

			if (sel != 0)
				uifont_print(118, 24, UI_COLOR(UI_PAL_SELECT), FONT_UPTRIANGLE);

			for (i = 0; i < rows; i++)
			{
				if (top + i >= BIOS_MAX) break;

				if (top + i == sel)
				{
					uifont_print(12, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), FONT_RIGHTTRIANGLE);
					uifont_print(32, 40 + i * 17, UI_COLOR(UI_PAL_SELECT), bios_name[top + i]);
				}
				else
				{
					if (bios_exist[top + i])
						uifont_print(32, 40 + i * 17, UI_COLOR(UI_PAL_NORMAL), bios_name[top + i]);
					else
						uifont_print(32, 40 + i * 17, COLOR_DARKGRAY, bios_name[top + i]);
				}
			}

			if (sel + rows < BIOS_MAX)
				uifont_print(118, 260, UI_COLOR(UI_PAL_SELECT), FONT_DOWNTRIANGLE);

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

		prev_sel = sel;

		if (pad_pressed(PSP_CTRL_UP))
		{
			if (sel > 0)
			{
				if (bios_exist[sel - 1])
				{
					sel--;
				}
				else
				{
					for (i = sel - 2; i >= 0; i--)
						if (bios_exist[i]) break;

					if (i != -1) sel = i;
				}
			}
		}
		else if (pad_pressed(PSP_CTRL_DOWN))
		{
			if (sel < BIOS_MAX - 1)
			{
				if (bios_exist[sel + 1])
				{
					sel++;
				}
				else
				{
					for (i = sel + 2; i < BIOS_MAX; i++)
						if (bios_exist[i]) break;

					if (i != BIOS_MAX) sel = i;
				}
			}
		}
		else if (pad_pressed(PSP_CTRL_CIRCLE))
		{
			neogeo_bios = sel;
			break;
		}

		if (top > BIOS_MAX - rows) top = BIOS_MAX - rows;
		if (top < 0) top = 0;
		if (sel >= BIOS_MAX) sel = 0;
		if (sel < 0) sel = BIOS_MAX - 1;
		if (sel >= top + rows) top = sel - rows + 1;
		if (sel < top) top = sel;

		if (prev_sel != sel) update = 1;

		pad_update();

		if (Loop == LOOP_EXIT) break;
	}

	pad_wait_clear();
	ui_popup_reset(POPUP_MENU);
	if (flag)
		load_background(WP_LOGO);
	else
		load_background(WP_FILER);

#ifdef ADHOC
	if (flag != 2)
#endif
	{
		if (old_bios != neogeo_bios)
		{
			if (!flag) ui_popup(TEXT(ALL_NVRAM_FILES_ARE_REMOVED));
			delete_files("nvram", ".nv");
		}
	}
}
