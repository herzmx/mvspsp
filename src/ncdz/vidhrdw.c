/******************************************************************************

	vidhrdw.c

	NEOGEO CDZ ビデオエミュレーション

******************************************************************************/

#include "ncdz.h"


/******************************************************************************
	グローバル変数
******************************************************************************/

UINT16 ALIGN_DATA neogeo_vidram16[0x20000 / 2];
UINT16 neogeo_vidram16_offset;
UINT16 neogeo_vidram16_modulo;

UINT16 *neogeo_paletteram16;
UINT16 ALIGN_DATA neogeo_palettebank16[2][0x2000 / 2];
UINT32 neogeo_palette_index;

UINT16 *video_palette;
UINT16 ALIGN_PSPDATA video_palettebank[2][0x2000 / 2];
UINT16 ALIGN_DATA video_clut16[0x8000];

UINT8 video_fix_usage[0x20000 / 32];
UINT8 video_spr_usage[0x400000 / 128];

int video_enable;
int spr_disable;
int fix_disable;


/******************************************************************************
	ローカル変数
******************************************************************************/

static UINT32 no_of_tiles;
static int next_update_first_line;

static UINT16 *sprite_scale = &neogeo_vidram16[0x10000 >> 1];
static UINT16 *sprite_yctrl = &neogeo_vidram16[0x10400 >> 1];
static UINT16 *sprite_xctrl = &neogeo_vidram16[0x10800 >> 1];

#include "zoom.c"


/******************************************************************************
	プロトタイプ
******************************************************************************/

static void patch_vram_rbff2(void);
static void patch_vram_adkworld(void);
static void patch_vram_crsword2(void);


/******************************************************************************
	ローカル関数
******************************************************************************/

/*------------------------------------------------------
	FIXスプライト描画
------------------------------------------------------*/

static void draw_fix(void)
{
	UINT16 x, y, code, attr;

	for (x = 1/8; x < 312/8; x++)
	{
		UINT16 *vram = &neogeo_vidram16[(0xe000 >> 1) + (16 / 8) + (x << 5)];

		for (y = 16/8; y < 240/8; y++)
		{
			code = *vram++;
			attr = (code & 0xf000) >> 12;
			code &= 0x0fff;

			if (video_fix_usage[code])
				blit_draw_fix(x << 3, y << 3, code, attr);
		}
	}

	blit_finish_fix();
}


/*------------------------------------------------------
	SPRスプライト描画
------------------------------------------------------*/

#define DRAW_SPRITE()											\
	if (sy2 + zy >= min_y && sy2 <= max_y)						\
	{															\
		code = sprite_base[0];									\
		attr = sprite_base[1];									\
																\
		if (attr & 0x08)										\
			code = (code & ~7) | (neogeo_frame_counter & 7);	\
		else if (attr & 0x04)									\
			code = (code & ~3) | (neogeo_frame_counter & 3);	\
																\
		code %= no_of_tiles;									\
																\
		if (video_spr_usage[code])								\
			blit_draw_spr(sx, sy2, zx + 1, zy, code, attr);		\
	}

static void draw_spr(UINT32 start, UINT32 end, int min_y, int max_y)
{
	int sx = 0, sy = 0, my = 0, zx = 0, zy = 0, rzy = 0;
	UINT32 offset, code, attr;
	int fullmode = 0, y, sy2;
	UINT16 scale, yctrl;
	UINT16 *sprite_base;

	for (offset = start; offset < end; offset++)
	{
		scale = sprite_scale[offset];
		yctrl = sprite_yctrl[offset];

		if (yctrl & 0x40)
		{
			sx += zx + 1;
		}
		else
		{
			rzy = scale & 0xff;

			sy = 0x200 - (yctrl >> 7);

			my = yctrl & 0x3f;

			if (my > 0x20)
			{
				fullmode = 1;
				my = 0x20;

				if (sy > 0x100) sy -= 0x200;
				while (sy < -16) sy += (rzy + 1) << 1;
			}
			else
				fullmode = 0;

			sx = sprite_xctrl[offset] >> 7;
		}

		if (sx >= 0x1f0) sx -= 0x200;

		zx = (scale >> 8) & 0x0f;

		if (sx + zx < 8 || sx > 311) continue;
		if (!my || !rzy) continue;

		sprite_base = &neogeo_vidram16[offset << 6];

		if (fullmode)
		{
			sy2 = sy;

			for (y = 0; y < my; y++)
			{
				if ((zy = yskip[rzy][y]))
				{
					if (sy2 >= 248) sy2 -= (rzy + 1) << 1;
					DRAW_SPRITE()
					sy2 += zy;
				}

				sprite_base += 2;
			}
		}
		else
		{
			int drawn_lines = 0;

			y = 0;

			while (drawn_lines < my << 4)
			{
				if ((zy = yskip[rzy][y]))
				{
					sy2 = (sy + drawn_lines) & 0x1ff;
					DRAW_SPRITE()
					drawn_lines += zy;
				}

				y++;
				if (y == 0x10 || y == 0x20)
					drawn_lines += (0xff - rzy) << 1;

				sprite_base += 2;
			}
		}
	}

	blit_finish_spr();
}


/*------------------------------------------------------
	SPRスプライト描画 (プライオリティあり/ssrpg専用)
------------------------------------------------------*/

static void draw_spr_prio(int min_y, int max_y)
{
	int start = 0, end = 0x300 >> 1;

	if ((neogeo_vidram16[(0x10400 + 4) >> 1] & 0x40) == 0 &&
		(neogeo_vidram16[(0x10400 + 6) >> 1] & 0x40) != 0)
	{
		start = 6 >> 1;

		while ((neogeo_vidram16[(0x10400 >> 1) + start] & 0x40) != 0)
		    start++;

		if (start == 6 >> 1) start = 0;
	}

    do
	{
		draw_spr(start, end, min_y, max_y);
		end = start;
		start = 0;
	} while (end != 0);
}


/******************************************************************************
	NEOGEO CDZ ビデオ描画処理
******************************************************************************/

/*------------------------------------------------------
	ビデオエミュレーション初期化
------------------------------------------------------*/

void neogeo_video_init(void)
{
	int i, r, g, b;

	for (r = 0; r < 32; r++)
	{
		for (g = 0; g < 32; g++)
		{
			for (b = 0; b < 32; b++)
			{
				int r1 = (r << 3) | (r >> 2);
				int g1 = (g << 3) | (g >> 2);
				int b1 = (b << 3) | (b >> 2);

				UINT16 color = ((r & 1) << 14) | ((r & 0x1e) << 7)
						  | ((g & 1) << 13) | ((g & 0x1e) << 3)
						  | ((b & 1) << 12) | ((b & 0x1e) >> 1);

				video_clut16[color] = MAKECOL15(r1, g1, b1);
			}
		}
	}

	no_of_tiles = 0x400000 / 128;

	memset(neogeo_vidram16, 0, sizeof(neogeo_vidram16));
	memset(neogeo_palettebank16, 0, sizeof(neogeo_palettebank16));
	memset(video_palettebank, 0, sizeof(video_palettebank));

	for (i = 0; i < 0x1000; i += 16)
	{
		video_palettebank[0][i] = 0x8000;
		video_palettebank[1][i] = 0x8000;
	}

	neogeo_video_reset();
}


/*------------------------------------------------------
	ビデオエミュレーション終了
------------------------------------------------------*/

void neogeo_video_exit(void)
{
}


/*------------------------------------------------------
	ビデオエミュレーションリセット
------------------------------------------------------*/

void neogeo_video_reset(void)
{
	neogeo_paletteram16 = neogeo_palettebank16[0];
	video_palette = video_palettebank[0];
	neogeo_palette_index = 0;
	neogeo_vidram16_modulo = 1;
	neogeo_vidram16_offset = 0;

	next_update_first_line = FIRST_VISIBLE_LINE;

	video_enable = 1;
	spr_disable = 0;
	fix_disable = 0;

	blit_reset();
}


/******************************************************************************
	画面更新処理
******************************************************************************/

/*------------------------------------------------------
	スクリーン更新
------------------------------------------------------*/

void neogeo_screenrefresh(void)
{
	if (video_enable)
	{
		blit_start(FIRST_VISIBLE_LINE, LAST_VISIBLE_LINE);

		if (!spr_disable)
		{
			if (NGH_NUMBER(NGH_ssrpg))
			{
				draw_spr_prio(FIRST_VISIBLE_LINE, LAST_VISIBLE_LINE);
			}
			else
			{
				switch (neogeo_ngh)
				{
				case NGH_crsword2: patch_vram_crsword2(); break;
				case NGH_adkworld: patch_vram_adkworld(); break;
				case NGH_rbff2: patch_vram_rbff2(); break;
				}

				draw_spr(0, 0x300 >> 1, FIRST_VISIBLE_LINE, LAST_VISIBLE_LINE);
			}
		}

		if (!fix_disable)
		{
			draw_fix();
		}

		blit_finish();
	}
	else
	{
		video_clear_frame(draw_frame);
	}

	next_update_first_line = FIRST_VISIBLE_LINE;
}


/*------------------------------------------------------
	スクリーン更新 (Raster driver)
------------------------------------------------------*/

void neogeo_partial_screenrefresh(int current_line)
{
	if (current_line >= FIRST_VISIBLE_LINE && current_line <= LAST_VISIBLE_LINE)
	{
		if (video_enable)
		{
			if (!spr_disable)
			{
				if (current_line >= next_update_first_line)
				{
					blit_start(next_update_first_line, current_line);

					if (NGH_NUMBER(NGH_ssrpg))
					{
						draw_spr_prio(next_update_first_line, current_line);
					}
					else
					{
						switch (neogeo_ngh)
						{
						case NGH_crsword2: patch_vram_crsword2(); break;
						case NGH_adkworld: patch_vram_adkworld(); break;
						case NGH_rbff2: patch_vram_rbff2(); break;
						}

						draw_spr(0, 0x300 >> 1, next_update_first_line, current_line);
					}
				}
			}
		}

		next_update_first_line = current_line + 1;
	}
}


void neogeo_raster_screenrefresh(void)
{
	if (video_enable)
	{
		if (!spr_disable)
		{
			neogeo_partial_screenrefresh(LAST_VISIBLE_LINE);
		}
		else
		{
			blit_start(FIRST_VISIBLE_LINE, LAST_VISIBLE_LINE);
		}

		if (!fix_disable)
		{
			draw_fix();
		}

		blit_finish();
	}
	else
	{
		video_clear_frame(draw_frame);
	}

	next_update_first_line = FIRST_VISIBLE_LINE;
}


/*------------------------------------------------------
	スクリーン更新 (CD-ROMロード画面)
------------------------------------------------------*/

int neogeo_loading_screenrefresh(int flag, int draw)
{
	static TICKER prev;
	static int limit;

	if (flag)
	{
		prev = ticker() - TICKS_PER_SEC / FPS;
		limit = neogeo_cdspeed_limit;
	}

	if (limit)
	{
		TICKER target = prev + TICKS_PER_SEC / FPS;
		TICKER curr = ticker();

		while (curr < target) curr = ticker();

		prev = curr;
	}

#ifdef ADHOC
	if (!adhoc_enable)
#endif
	{
		pad_update();

		if (limit && pad_pressed(PSP_CTRL_LTRIGGER))
		{
			limit = 0;
			ui_popup(TEXT(CDROM_SPEED_LIMIT_OFF));
		}
		else if (!limit && pad_pressed(PSP_CTRL_RTRIGGER))
		{
			limit = 1;
			ui_popup(TEXT(CDROM_SPEED_LIMIT_ON));
		}
	}

	if (limit || draw)
	{
		blit_start(FIRST_VISIBLE_LINE, LAST_VISIBLE_LINE);
		if (video_enable && !fix_disable) draw_fix();
		blit_finish();
		draw = ui_show_popup(1);
		video_flip_screen(0);
	}
	else draw = ui_show_popup(0);

#ifdef ADHOC
	if (adhoc_enable)
		update_inputport();
#endif

	return draw;
}


/*------------------------------------------------------
	VRAMパッチ (Realbout Fatal Fury 2)
------------------------------------------------------*/

static void patch_vram_rbff2(void)
{
	UINT16 offs;

	for (offs = 0; offs < ((0x300 >> 1) << 6) ; offs += 2)
	{
		UINT16 tileno  = neogeo_vidram16[offs];
		UINT16 tileatr = neogeo_vidram16[offs + 1];

		if (tileno == 0x7a00 && (tileatr == 0x4b00 || tileatr == 0x1400))
		{
			neogeo_vidram16[offs] = 0x7ae9;
			return;
		}
	}
}


/*------------------------------------------------------
	VRAMパッチ (ADK World)
------------------------------------------------------*/

static void patch_vram_adkworld(void)
{
	UINT16 offs;

	for (offs = 0; offs < ((0x300 >> 1) << 6) ; offs += 2)
	{
		UINT16 tileno  = neogeo_vidram16[offs];
		UINT16 tileatr = neogeo_vidram16[offs + 1];

		if ((tileno == 0x14c0 || tileno == 0x1520) && tileatr == 0x0000)
			neogeo_vidram16[offs] = 0x0000;
	}
}


/*------------------------------------------------------
	VRAMパッチ (Crossed Swords 2)
------------------------------------------------------*/

static void patch_vram_crsword2(void)
{
	UINT16 offs;

	for (offs = 0; offs < ((0x300 >> 1) << 6) ; offs += 2)
	{
		UINT16 tileno  = neogeo_vidram16[offs];
		UINT16 tileatr = neogeo_vidram16[offs + 1];

		if (tileno == 0x52a0 && tileatr == 0x0000)
			neogeo_vidram16[offs] = 0x0000;
	}
}


/*------------------------------------------------------
	セーブ/ロード ステート
------------------------------------------------------*/

#ifdef SAVE_STATE

STATE_SAVE( video )
{
	state_save_word(neogeo_vidram16, 0x10000);
	state_save_word(neogeo_palettebank16[0], 0x1000);
	state_save_word(neogeo_palettebank16[1], 0x1000);
	state_save_word(&neogeo_vidram16_offset, 1);
	state_save_word(&neogeo_vidram16_modulo, 1);
	state_save_long(&neogeo_palette_index, 1);

	state_save_long(&video_enable, 1);
	state_save_long(&fix_disable, 1);
	state_save_long(&spr_disable, 1);
}

STATE_LOAD( video )
{
	int i;

	state_load_word(neogeo_vidram16, 0x10000);
	state_load_word(neogeo_palettebank16[0], 0x1000);
	state_load_word(neogeo_palettebank16[1], 0x1000);
	state_load_word(&neogeo_vidram16_offset, 1);
	state_load_word(&neogeo_vidram16_modulo, 1);
	state_load_long(&neogeo_palette_index, 1);

	state_load_long(&video_enable, 1);
	state_load_long(&fix_disable, 1);
	state_load_long(&spr_disable, 1);

	for (i = 0; i < 0x1000; i++)
	{
		if (i & 0x0f)
		{
			video_palettebank[0][i] = video_clut16[neogeo_palettebank16[0][i] & 0x7fff];
			video_palettebank[1][i] = video_clut16[neogeo_palettebank16[1][i] & 0x7fff];
		}
	}

	neogeo_paletteram16 = neogeo_palettebank16[neogeo_palette_index];
	video_palette = video_palettebank[neogeo_palette_index];
}

#endif /* SAVE_STATE */
