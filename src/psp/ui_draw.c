/******************************************************************************

	ui_draw.c

	PSP ユーザインタフェース描画関数

******************************************************************************/

#include "psp.h"


/******************************************************************************
	定数/マクロ
******************************************************************************/

#define vram_pos16(x, y)	(UINT16 *)video_frame_addr(draw_frame, x, y)
#define vram_pos32(x, y)	(UINT32 *)video_frame_addr(draw_frame, x, y)

#if PSP_VIDEO_32BPP
#define MAKECOL16(r, g, b)	MAKECOL15(r, g, b)
#define GETR16(color)		GETR15(color)
#define GETG16(color)		GETG15(color)
#define GETB16(color)		GETB15(color)
#else
#define MAKECOL16(r, g, b)	((b << 10) | (g << 5) | r)
#define GETR16(color)		((color >>  0) & 0x1f)
#define GETG16(color)		((color >>  5) & 0x1f)
#define GETB16(color)		((color >> 10) & 0x1f)
#endif

#define CMD_RED				(1 << 8)
#define CMD_YELLOW			(2 << 8)
#define CMD_GREEN			(3 << 8)
#define CMD_BLUE			(4 << 8)
#define CMD_MAGENTA			(5 << 8)
#define CMD_PURPLE			(6 << 8)
#define CMD_ORANGE			(7 << 8)
#define CMD_GRAY			(8 << 8)
#define CMD_CYAN			(9 << 8)
#define CMD_PINK			(10 << 8)

#define CODE_HASCOLOR(c)	(c & 0xff00)
#define CODE_NOTFOUND		0xffff
#define CODE_UNDERBAR		0xfffe

#define isascii(c)	((c) >= 0x20 && (c) <= 0x7e)
#define islatin1(c)	((c) >= 0x80)
#define iskana(c)	((c) >= 0xa0 && (c) <= 0xdf)
#define issjis1(c)	(((c) >= 0x81 && (c) <= 0x9f) | ((c) >= 0xe0 && (c) <= 0xfc))
#define issjis2(c)	((c) >= 0x40 && (c) <= 0xfc && (c) != 0x7f)

enum
{
	FONT_TYPE_CONTROL = 0,
	FONT_TYPE_ASCII,
	FONT_TYPE_GRAPHIC,
	FONT_TYPE_JPNHAN,
	FONT_TYPE_JPNZEN,
#ifdef COMMAND_LIST
	FONT_TYPE_LATIN1,
	FONT_TYPE_COMMAND,
#endif
	FONT_TYPE_MAX
};


/******************************************************************************
	グローバル構造体
******************************************************************************/

UI_PALETTE ui_palette[UI_PAL_MAX] =
{
	{ 255, 255, 255 },	// UI_PAL_TITLE
	{ 255, 255, 255 },	// UI_PAL_SELECT
	{ 180, 180, 180 },	// UI_PAL_NORMAL
	{ 255, 255,  64 },	// UI_PAL_INFO
	{ 255,  64,  64 },	// UI_PAL_WARNING
	{  48,  48,  48 },	// UI_PAL_BG1
	{   0,   0, 160 },	// UI_PAL_BG2
	{   0,   0,   0 },	// UI_PAL_FRAME
	{  40,  40,  40 },	// UI_PAL_FILESEL1
	{ 120, 120, 120 }	// UI_PAL_FILESEL2
};


/******************************************************************************
	ローカル構造体
******************************************************************************/

#if PSP_VIDEO_32BPP
static int light_level = 0;
#endif

#ifdef COMMAND_LIST
static UINT32 command_font_color[11] =
{
#if PSP_VIDEO_32BPP
	MAKECOL32(255,255,255),
	MAKECOL32(255, 32,  0),
	MAKECOL32(255,200,  0),
	MAKECOL32(  0,200, 80),
	MAKECOL32(  0, 64,255),
	MAKECOL32(255,  0,128),
	MAKECOL32(200,  0,200),
	MAKECOL32(255,128,  0),
	MAKECOL32(160,160,160),
	MAKECOL32( 64,200,200),
	MAKECOL32(255, 64,128)
#else
	MAKECOL15(255,255,255),
	MAKECOL15(255, 32,  0),
	MAKECOL15(255,200,  0),
	MAKECOL15(  0,200, 80),
	MAKECOL15(  0, 64,255),
	MAKECOL15(255,  0,128),
	MAKECOL15(200,  0,200),
	MAKECOL15(255,128,  0),
	MAKECOL15(160,160,160),
	MAKECOL15( 64,200,200),
	MAKECOL15(255, 64,128)
#endif
};
#endif


/******************************************************************************
	フォントコード取得
******************************************************************************/

#ifdef COMMAND_LIST

/*------------------------------------------------------
	フォントコード取得 (コマンドリスト用)
------------------------------------------------------*/

static UINT16 command_font_get_code(const UINT8 *s)
{
	UINT8 c1 = s[0];
	UINT8 c2 = s[1];

	if (c1 == '_')
	{
		switch (c2)
		{
		case 'A': return 0x00 | CMD_RED;
		case 'B': return 0x01 | CMD_YELLOW;
		case 'C': return 0x02 | CMD_GREEN;
		case 'D': return 0x03 | CMD_BLUE;
		case 'P': return 0x04 | CMD_MAGENTA;
		case 'K': return 0x05 | CMD_PURPLE;
		case 'G': return 0x07 | CMD_BLUE;
		case 'H': return 0x08 | CMD_PINK;
		case 'Z': return 0x09 | CMD_PURPLE;

		case 'S': return 0x14 | CMD_RED;

		case 'a': return 0x16 | CMD_RED;
		case 'b': return 0x17 | CMD_YELLOW;
		case 'c': return 0x18 | CMD_GREEN;
		case 'd': return 0x19 | CMD_BLUE;
		case 'e': return 0x1a | CMD_MAGENTA;
		case 'f': return 0x1b | CMD_PURPLE;
		case 'g': return 0x1c | CMD_CYAN;
		case 'h': return 0x1d | CMD_PINK;
		case 'i': return 0x1e | CMD_ORANGE;
		case 'j': return 0x1f | CMD_GRAY;

		case '1': return 0x20;
		case '2': return 0x21;
		case '3': return 0x22;
		case '4': return 0x23;
		case '5': return 0x24 | CMD_RED;
		case '6': return 0x25;
		case '7': return 0x26;
		case '8': return 0x27;
		case '9': return 0x28;
		case 'N': return 0x29;
		case '+': return 0x2a;
		case '.': return 0x2b;

		case '!': return 0x35;

		case 'k': return 0x36;
		case 'l': return 0x37;
		case 'm': return 0x38;
		case 'n': return 0x39;
		case 'o': return 0x3a;
		case 'p': return 0x3b;
		case 'q': return 0x3c;
		case 'r': return 0x3d;
		case 's': return 0x3e;
		case 't': return 0x3f;
		case 'u': return 0x40;
		case 'v': return 0x41;

		case 'w': return 0x42;
		case 'x': return 0x43;
		case 'y': return 0x44;
		case 'z': return 0x45;
		case 'L': return 0x46;
		case 'M': return 0x47;
		case 'Q': return 0x48;
		case 'R': return 0x49;
		case '^': return 0x4a;
		case '?': return 0x4b;
		case 'X': return 0x4c;

		case 'I': return 0x4e;
		case 'O': return 0x4f;
		case '-': return 0x50;
		case '=': return 0x51;
		case '~': return 0x54;
		case '`': return 0x57;

		case '@': return 0x58;
		case ')': return 0x59;
		case '(': return 0x5a;
		case '*': return 0x5b;
		case '&': return 0x5c;
		case '%': return 0x5d;
		case '$': return 0x5e;
		case '#': return 0x5f;
		case ']': return 0x60;
		case '[': return 0x61;
		case '{': return 0x62;
		case '}': return 0x63;
		case '<': return 0x64;
		case '>': return 0x65;
		case '_': return CODE_UNDERBAR;
		}
	}
	else if (c1 == '^')
	{
		switch (c2)
		{
		case 's': return 0x06 | CMD_ORANGE;
		case 'E': return 0x0a | CMD_YELLOW;
		case 'F': return 0x0b | CMD_ORANGE;
		case 'G': return 0x0c | CMD_RED;
		case 'H': return 0x0d | CMD_GRAY;
		case 'I': return 0x0e | CMD_CYAN;
		case 'J': return 0x0f | CMD_BLUE;
		case 'T': return 0x10 | CMD_PURPLE;
		case 'U': return 0x11 | CMD_MAGENTA;
		case 'V': return 0x12 | CMD_PURPLE;
		case 'W': return 0x13 | CMD_MAGENTA;
		case 'S': return 0x15 | CMD_YELLOW;

		case '1': return 0x2c;
		case '2': return 0x2d;
		case '3': return 0x2e;
		case '4': return 0x2f;
		case '6': return 0x30;
		case '7': return 0x31;
		case '8': return 0x32;
		case '9': return 0x33;
		case '!': return 0x34;

		case 'M': return 0x4d;

		case '-': return 0x52;
		case '=': return 0x53;
		case '*': return 0x55;
		case '?': return 0x56;
		}
	}
	return CODE_NOTFOUND;
}


/*------------------------------------------------------
	フォントコード取得 (Latin-1デコード)
------------------------------------------------------*/

static UINT16 latin1_get_code(const UINT8 *s, int *type)
{
	UINT16 code;

	if ((code = command_font_get_code(s)) != CODE_NOTFOUND)
	{
		*type = FONT_TYPE_COMMAND;
		return code;
	}
	else if (isascii(*s))
	{
		*type = FONT_TYPE_ASCII;
		return *s - 0x20;
	}
	else if (islatin1(*s))
	{
		*type = FONT_TYPE_LATIN1;
		return *s - 0x80;
	}
	*type = FONT_TYPE_CONTROL;
	return *s;
}

/*------------------------------------------------------
	フォントコード取得 (SHIFT-JISデコード)
------------------------------------------------------*/

#if JAPANESE_UI

static UINT16 sjis_get_code(const UINT8 *s, int *type)
{
	UINT8 c1 = s[0];
	UINT8 c2 = s[1];
	UINT16 code;

	if ((code = command_font_get_code(s)) != CODE_NOTFOUND)
	{
		*type = FONT_TYPE_COMMAND;
		return code;
	}
	else if (issjis1(c1) && issjis2(c2))
	{
		*type = FONT_TYPE_JPNZEN;
		return sjis_table[(c2 | (c1 << 8)) - 0x8140];
	}
	else if (isascii(c1))
	{
		if (c1 != '\\')
		{
			*type = FONT_TYPE_ASCII;
			return c1 - 0x20;
		}
		else
		{
			*type = FONT_TYPE_JPNHAN;
			return 0;
		}
	}
	else if (iskana(c1))
	{
		*type = FONT_TYPE_JPNHAN;
		return c1 - 0xa0;
	}
	*type = FONT_TYPE_CONTROL;
	return c1;
}

#else

static UINT16 sjis_get_code(const UINT8 *s, int *type)
{
	UINT8 c1 = s[0];
	UINT8 c2 = s[1];
	UINT16 code;

	if ((code = command_font_get_code(s)) != CODE_NOTFOUND)
	{
		if (jpnfont)
		{
			*type = FONT_TYPE_COMMAND;
			return code;
		}
	}
	else if (issjis1(c1) && issjis2(c2))
	{
		*type = FONT_TYPE_JPNZEN;
		return sjis_table[(c2 | (c1 << 8)) - 0x8140];
	}
	else if (isascii(c1))
	{
		if (c1 != '\\')
		{
			*type = FONT_TYPE_ASCII;
			return c1 - 0x20;
		}
		else if (jpnfont)
		{
			*type = FONT_TYPE_JPNHAN;
			return 0;
		}
	}
	else if (iskana(c1))
	{
		if (jpnfont)
		{
			*type = FONT_TYPE_JPNHAN;
			return c1 - 0xa0;
		}
	}
	*type = FONT_TYPE_CONTROL;
	return c1;
}

#endif /* JAPANESE_UI */

#endif /* COMMAND_LIST */


/*------------------------------------------------------
	フォントコード取得 (ユーザインタフェース)
------------------------------------------------------*/

#if JAPANESE_UI

INLINE UINT16 uifont_get_code(const UINT8 *s, int *type)
{
	UINT8 c1 = s[0];
	UINT8 c2 = s[1];

	if (issjis1(c1) && issjis2(c2))
	{
		*type = FONT_TYPE_JPNZEN;
		return sjis_table[(c2 | (c1 << 8)) - 0x8140];
	}
	else if (c1 == 0xa5)
	{
		*type = FONT_TYPE_ASCII;
		return 0x7f - 0x20;
	}
	else if (c1 == 0x5c)
	{
		*type = FONT_TYPE_JPNHAN;
		return 0;
	}
	else if (iskana(c1))
	{
		*type = FONT_TYPE_JPNHAN;
		return c1 - 0xa0;
	}
	else if (isascii(c1))
	{
		*type = FONT_TYPE_ASCII;
		return c1 - 0x20;
	}
	else if ((c1 >= 0x10 && c1 <= 0x1e) && c1 != 0x1a)
	{
		*type = FONT_TYPE_GRAPHIC;
		if (c1 < 0x1a)
			return c1 - 0x10;
		else
			return c1 - 0x11;
	}
	*type = FONT_TYPE_CONTROL;
	return c1;
}

#else

INLINE UINT16 uifont_get_code(const UINT8 *s, int *type)
{
	UINT8 c1 = s[0];
	UINT8 c2 = s[1];

	if (issjis1(c1) && issjis2(c2))
	{
		if (jpnfont)
		{
			*type = FONT_TYPE_JPNZEN;
			return sjis_table[(c2 | (c1 << 8)) - 0x8140];
		}
	}
	else if (c1 == 0xa5)
	{
		*type = FONT_TYPE_ASCII;
		return 0x7f - 0x20;
	}
	else if (c1 == 0x5c)
	{
		if (jpnfont)
		{
			*type = FONT_TYPE_JPNHAN;
			return 0;
		}
	}
	else if (iskana(c1))
	{
		if (jpnfont)
		{
			*type = FONT_TYPE_JPNHAN;
			return c1 - 0xa0;
		}
	}
	else if (isascii(c1))
	{
		*type = FONT_TYPE_ASCII;
		return c1 - 0x20;
	}
	else if ((c1 >= 0x10 && c1 <= 0x1e) && c1 != 0x1a)
	{
		*type = FONT_TYPE_GRAPHIC;
		if (c1 < 0x1a)
			return c1 - 0x10;
		else
			return c1 - 0x11;
	}
	*type = FONT_TYPE_CONTROL;
	return c1;
}

#endif /* JAPANESE_UI */


/******************************************************************************
	フォント文字幅取得
******************************************************************************/

/*------------------------------------------------------
	フォント描画幅取得 (ユーザインタフェース)
------------------------------------------------------*/

int uifont_get_string_width(const char *s)
{
	int width, type;
	UINT16 code;
	const UINT8 *p = (const UINT8 *)s;

	width = 0;

	while (*p)
	{
		if ((code = uifont_get_code(p, &type)) != CODE_NOTFOUND)
		{
			switch (type)
			{
			case FONT_TYPE_ASCII:
				width += ascii_14p_get_pitch(code);
				p++;
				break;

			case FONT_TYPE_GRAPHIC:
				width += graphic_font_get_pitch(code);
				p++;
				break;

#if JAPANESE_UI || (EMU_SYSTEM != NCDZ)
			case FONT_TYPE_JPNHAN:
				width += jpn_h14p_get_pitch(code);
				p += 2;
				break;

			case FONT_TYPE_JPNZEN:
				width += jpn_z14p_get_pitch(code);
				p += 2;
				break;
#endif

			case FONT_TYPE_CONTROL:
				width += ascii_14p_get_pitch(0);
				p++;
				break;

			}
		}
		else break;
	}

	return width;
}


/******************************************************************************
	内部フォント描画関数
******************************************************************************/

#if PSP_VIDEO_32BPP

/*------------------------------------------------------
	内部フォント描画関数 (32bit)
------------------------------------------------------*/

static int internal_font_putc32(struct font_t *font, int sx, int sy, int src_r, int src_g, int src_b)
{
	int x, y, p;
	int dst_r, dst_g, dst_b, alpha;
	UINT32 *dst, *vptr, color;
	UINT8 data;

	if (sx + font->pitch < 0 || sx >= SCR_WIDTH)
		return 0;

	vptr = vram_pos32(sx, sy);
	dst = &vptr[font->skipx + (font->skipy << 9)];

	sy += font->skipy;

	p = 0;

	for (y = 0; y < font->height; y++)
	{
		if (sy + y >= 0 && sy + y < SCR_HEIGHT)
		{
			for (x = 0; x < font->width;)
			{
				data = font->data[p++];

				alpha = data & 0x0f;
				if (alpha)
				{
					color = dst[x];

					dst_r = GETR32(color);
					dst_g = GETG32(color);
					dst_b = GETB32(color);

					dst_r = alpha_blend[alpha][src_r][dst_r];
					dst_g = alpha_blend[alpha][src_g][dst_g];
					dst_b = alpha_blend[alpha][src_b][dst_b];

					dst[x] = MAKECOL32(dst_r, dst_g, dst_b);
				}
				x++;

				alpha = data >> 4;
				if (alpha)
				{
					color = dst[x];

					dst_r = GETR32(color);
					dst_g = GETG32(color);
					dst_b = GETB32(color);

					dst_r = alpha_blend[alpha][src_r][dst_r];
					dst_g = alpha_blend[alpha][src_g][dst_g];
					dst_b = alpha_blend[alpha][src_b][dst_b];

					dst[x] = MAKECOL32(dst_r, dst_g, dst_b);
				}
				x++;
			}
			dst += BUF_WIDTH;
		}
	}

	return 1;
}


/*------------------------------------------------------
	内部シャドウ描画関数 (32bit)
------------------------------------------------------*/

static int internal_shadow_putc32(struct font_t *font, int sx, int sy)
{
	int x, y, p;
	int dst_r, dst_g, dst_b, alpha;
	UINT32 *dst, *vptr, color;
	UINT8 data;

	if (sx + font->pitch < 0 || sx >= SCR_WIDTH)
		return 0;

	vptr = vram_pos32(sx, sy);
	dst = &vptr[font->skipx + (font->skipy << 9)];

	sy += font->skipy;

	p = 0;

	for (y = 0; y < font->height; y++)
	{
		if (sy + y >= 0 && sy + y < SCR_HEIGHT)
		{
			for (x = 0; x < font->width;)
			{
				data = font->data[p++];

				alpha = data & 0x0f;
				if (alpha)
				{
					color = dst[x];

					dst_r = GETR32(color);
					dst_g = GETG32(color);
					dst_b = GETB32(color);

					dst_r = alpha_blend[alpha][dst_r >> 1][dst_r];
					dst_g = alpha_blend[alpha][dst_g >> 1][dst_g];
					dst_b = alpha_blend[alpha][dst_b >> 1][dst_b];

					dst[x] = MAKECOL32(dst_r, dst_g, dst_b);
				}
				x++;

				alpha = data >> 4;
				if (alpha)
				{
					color = dst[x];

					dst_r = GETR32(color);
					dst_g = GETG32(color);
					dst_b = GETB32(color);

					dst_r = alpha_blend[alpha][dst_r >> 1][dst_r];
					dst_g = alpha_blend[alpha][dst_g >> 1][dst_g];
					dst_b = alpha_blend[alpha][dst_b >> 1][dst_b];

					dst[x] = MAKECOL32(dst_r, dst_g, dst_b);
				}
				x++;
			}
			dst += BUF_WIDTH;
		}
	}

	return 1;
}


/*------------------------------------------------------
	内部ライト描画関数 (32bit)
------------------------------------------------------*/

static int internal_light_putc32(struct font_t *font, int sx, int sy)
{
	int x, y, p, level;
	int dst_r, dst_g, dst_b, alpha;
	UINT32 *dst, *vptr, color;
	UINT8 data;

	if (sx + font->pitch < 0 || sx >= SCR_WIDTH)
		return 0;

	vptr = vram_pos32(sx, sy);
	dst = &vptr[font->skipx + (font->skipy << 9)];

	sy += font->skipy;
	level = light_level >> 1;
	p = 0;

	for (y = 0; y < font->height; y++)
	{
		if (sy + y >= 0 && sy + y < SCR_HEIGHT)
		{
			for (x = 0; x < font->width;)
			{
				data = font->data[p++];

				alpha = (data & 0x0f) - level;
				if (alpha > 0)
				{
					color = dst[x];

					dst_r = GETR32(color);
					dst_g = GETG32(color);
					dst_b = GETB32(color);

					dst_r = alpha_blend[alpha][255][dst_r];
					dst_g = alpha_blend[alpha][255][dst_g];
					dst_b = alpha_blend[alpha][255][dst_b];

					dst[x] = MAKECOL32(dst_r, dst_g, dst_b);
				}
				x++;

				alpha = (data >> 4) - level;
				if (alpha > 0)
				{
					color = dst[x];

					dst_r = GETR32(color);
					dst_g = GETG32(color);
					dst_b = GETB32(color);

					dst_r = alpha_blend[alpha][255][dst_r];
					dst_g = alpha_blend[alpha][255][dst_g];
					dst_b = alpha_blend[alpha][255][dst_b];

					dst[x] = MAKECOL32(dst_r, dst_g, dst_b);
				}
				x++;
			}
			dst += BUF_WIDTH;
		}
	}

	return 1;
}

#endif /* PSP_VIDEO_32BPP */


/*------------------------------------------------------
	内部フォント描画関数 (16bit color)
------------------------------------------------------*/

static int internal_font_putc16(struct font_t *font, int sx, int sy, int src_r, int src_g, int src_b)
{
	int x, y, p;
	int dst_r, dst_g, dst_b, alpha;
	UINT16 *dst, *vptr, color;
	UINT8 data;

	if (sx + font->pitch < 0 || sx >= SCR_WIDTH)
		return 0;

	vptr = vram_pos16(sx, sy);
	dst = &vptr[font->skipx + (font->skipy << 9)];

	sy += font->skipy;
	p = 0;

	for (y = 0; y < font->height; y++)
	{
		if (sy + y >= 0 && sy + y < SCR_HEIGHT)
		{
			for (x = 0; x < font->width;)
			{
				data = font->data[p++];

				alpha = data & 0x0f;
				if (alpha)
				{
					color = dst[x];

					dst_r = GETR16(color);
					dst_g = GETG16(color);
					dst_b = GETB16(color);

					dst_r = alpha_blend[alpha][src_r][dst_r];
					dst_g = alpha_blend[alpha][src_g][dst_g];
					dst_b = alpha_blend[alpha][src_b][dst_b];

					dst[x] = MAKECOL16(dst_r, dst_g, dst_b);
				}
				x++;

				alpha = data >> 4;
				if (alpha)
				{
					color = dst[x];

					dst_r = GETR16(color);
					dst_g = GETG16(color);
					dst_b = GETB16(color);

					dst_r = alpha_blend[alpha][src_r][dst_r];
					dst_g = alpha_blend[alpha][src_g][dst_g];
					dst_b = alpha_blend[alpha][src_b][dst_b];

					dst[x] = MAKECOL16(dst_r, dst_g, dst_b);
				}
				x++;
			}
			dst += BUF_WIDTH;
		}
	}

	return 1;
}


#if PSP_VIDEO_32BPP

/*------------------------------------------------------
	内部シャドウ描画関数 (16bit color)
------------------------------------------------------*/

static int internal_shadow_putc16(struct font_t *font, int sx, int sy)
{
	int x, y, p;
	int dst_r, dst_g, dst_b, alpha;
	UINT16 *dst, *vptr, color;
	UINT8 data;

	if (sx + font->pitch < 0 || sx >= SCR_WIDTH)
		return 0;

	vptr = vram_pos16(sx, sy);
	dst = &vptr[font->skipx + (font->skipy << 9)];	// font_skipy << 9 = font_skipy * BUF_WIDTH

	sy += font->skipy;
	p = 0;

	for (y = 0; y < font->height; y++)
	{
		if (sy + y >= 0 && sy + y < SCR_HEIGHT)
		{
			for (x = 0; x < font->width;)
			{
				data = font->data[p++];

				alpha = data & 0x0f;
				if (alpha)
				{
					color = dst[x];

					dst_r = GETR16(color);
					dst_g = GETG16(color);
					dst_b = GETB16(color);

					dst_r = alpha_blend[alpha][dst_r >> 1][dst_r];
					dst_g = alpha_blend[alpha][dst_g >> 1][dst_g];
					dst_b = alpha_blend[alpha][dst_b >> 1][dst_b];

					dst[x] = MAKECOL16(dst_r, dst_g, dst_b);
				}
				x++;

				alpha = data >> 4;
				if (alpha)
				{
					color = dst[x];

					dst_r = GETR16(color);
					dst_g = GETG16(color);
					dst_b = GETB16(color);

					dst_r = alpha_blend[alpha][dst_r >> 1][dst_r];
					dst_g = alpha_blend[alpha][dst_g >> 1][dst_g];
					dst_b = alpha_blend[alpha][dst_b >> 1][dst_b];

					dst[x] = MAKECOL16(dst_r, dst_g, dst_b);
				}
				x++;
			}
			dst += BUF_WIDTH;
		}
	}

	return 1;
}


/*------------------------------------------------------
	内部ライト描画関数 (16bit)
------------------------------------------------------*/

static int internal_light_putc16(struct font_t *font, int sx, int sy)
{
	int x, y, p, level;
	int dst_r, dst_g, dst_b, alpha;
	UINT16 *dst, *vptr, color;
	UINT8 data;

	if (sx + font->pitch < 0 || sx >= SCR_WIDTH)
		return 0;

	vptr = vram_pos16(sx, sy);
	dst = &vptr[font->skipx + (font->skipy << 9)];

	sy += font->skipy;
	level = light_level >> 1;
	p = 0;

	for (y = 0; y < font->height; y++)
	{
		if (sy + y >= 0 && sy + y < SCR_HEIGHT)
		{
			for (x = 0; x < font->width;)
			{
				data = font->data[p++];

				alpha = (data & 0x0f) - level;
				if (alpha > 0)
				{
					color = dst[x];

					dst_r = GETR16(color);
					dst_g = GETG16(color);
					dst_b = GETB16(color);

					dst_r = alpha_blend[alpha][255][dst_r];
					dst_g = alpha_blend[alpha][255][dst_g];
					dst_b = alpha_blend[alpha][255][dst_b];

					dst[x] = MAKECOL16(dst_r, dst_g, dst_b);
				}
				x++;

				alpha = (data >> 4) - level;
				if (alpha > 0)
				{
					color = dst[x];

					dst_r = GETR16(color);
					dst_g = GETG16(color);
					dst_b = GETB16(color);

					dst_r = alpha_blend[alpha][255][dst_r];
					dst_g = alpha_blend[alpha][255][dst_g];
					dst_b = alpha_blend[alpha][255][dst_b];

					dst[x] = MAKECOL16(dst_r, dst_g, dst_b);
				}
				x++;
			}
			dst += BUF_WIDTH;
		}
	}

	return 1;
}

#endif /* PSP_VIDEO_32BPP */


/******************************************************************************
	ユーザインタフェース用フォント描画
******************************************************************************/

#if PSP_VIDEO_32BPP

/*------------------------------------------------------
	ユーザインタフェース用文字を描画 (32bit color)
------------------------------------------------------*/

INLINE void uifont_draw32(int sx, int sy, int r, int g, int b, const char *s)
{
	int type, res = 1;
	UINT16 code;
	const UINT8 *p = (const UINT8 *)s;
	struct font_t font;

	while (*p && res)
	{
		code = uifont_get_code(p, &type);
		res = 1;

		switch (type)
		{
		case FONT_TYPE_ASCII:
			if ((res = ascii_14p_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc32(&font, sx, sy, r, g, b);
				sx += font.pitch;
				p++;
			}
			break;

		case FONT_TYPE_GRAPHIC:
			if ((res = graphic_font_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc32(&font, sx, sy, r, g, b);
				sx += font.pitch;
				p++;
			}
			break;

#if JAPANESE_UI
		case FONT_TYPE_JPNHAN:
			if ((res = jpn_h14p_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc32(&font, sx, sy, r, g, b);
				sx += font.pitch;
				p++;
			}
			break;

		case FONT_TYPE_JPNZEN:
			if ((res = jpn_z14p_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc32(&font, sx, sy, r, g, b);
				sx += font.pitch;
				p += 2;
			}
			break;
#endif

		default:
			res = 0;
			break;
		}
	}
}


/*------------------------------------------------------
	文字の影を描画 (ユーザインタフェース用)
------------------------------------------------------*/

INLINE void uifont_draw_shadow32(int sx, int sy, const char *s)
{
	int type, res = 1;
	UINT16 code;
	const UINT8 *p = (const UINT8 *)s;
	struct font_t font;

	while (*p && res)
	{
		code = uifont_get_code(p, &type);

		switch (type)
		{
		case FONT_TYPE_ASCII:
			if ((res = ascii_14p_get_shadow(&font, code)) != 0)
			{
				res = internal_shadow_putc32(&font, sx, sy);
				sx += font.pitch;
				p++;
			}
			break;

		case FONT_TYPE_GRAPHIC:
			if ((res = graphic_font_get_shadow(&font, code)) != 0)
			{
				res = internal_shadow_putc32(&font, sx, sy);
				sx += font.pitch;
				p++;
			}
			break;

#if JAPANESE_UI
		case FONT_TYPE_JPNHAN:
			if ((res = jpn_h14p_get_shadow(&font, code)) != 0)
			{
				res = internal_shadow_putc32(&font, sx, sy);
				sx += font.pitch;
				p++;
			}
			break;

		case FONT_TYPE_JPNZEN:
			if ((res = jpn_z14p_get_shadow(&font, code)) != 0)
			{
				res = internal_shadow_putc32(&font, sx, sy);
				sx += font.pitch;
				p += 2;
			}
			break;
#endif

		default:
			res = 0;
			break;
		}
	}
}

#endif /* PSP_VIDEO_32BPP */


/*------------------------------------------------------
	ユーザインタフェース用文字を描画 (16bit color)
------------------------------------------------------*/

INLINE void uifont_draw16(int sx, int sy, int r, int g, int b, const char *s)
{
	int type, res = 1;
	UINT16 code;
	const UINT8 *p = (const UINT8 *)s;
	struct font_t font;

#if !PSP_VIDEO_32BPP
	r >>= 3;
	g >>= 3;
	b >>= 3;
#endif

	while (*p && res)
	{
		code = uifont_get_code(p, &type);

		switch (type)
		{
		case FONT_TYPE_ASCII:
			if (ascii_14p_get_gryph(&font, code))
			{
				res = internal_font_putc16(&font, sx, sy, r, g, b);
				sx += font.pitch;
			}
			p++;
			break;

		case FONT_TYPE_GRAPHIC:
			if (graphic_font_get_gryph(&font, code))
			{
				res = internal_font_putc16(&font, sx, sy, r, g, b);
				sx += font.pitch;
			}
			p++;
			break;

#if JAPANESE_UI || (EMU_SYSTEM != NCDZ)
		case FONT_TYPE_JPNHAN:
			if (jpn_h14p_get_gryph(&font, code))
			{
				res = internal_font_putc16(&font, sx, sy, r, g, b);
				sx += font.pitch;
			}
			p++;
			break;

		case FONT_TYPE_JPNZEN:
			if (jpn_z14p_get_gryph(&font, code))
			{
				res = internal_font_putc16(&font, sx, sy, r, g, b);
				sx += font.pitch;
			}
			p += 2;
			break;
#endif

		default:
			p++;
			break;
		}
	}
}


#if PSP_VIDEO_32BPP

/*------------------------------------------------------
	ユーザインタフェース用文字の影を描画 (16bit color)
------------------------------------------------------*/

INLINE void uifont_draw_shadow16(int sx, int sy, const char *s)
{
	int type, res = 1;
	UINT16 code;
	const UINT8 *p = (const UINT8 *)s;
	struct font_t font;

	while (*p && res)
	{
		code = uifont_get_code(p, &type);

		switch (type)
		{
		case FONT_TYPE_ASCII:
			if (ascii_14p_get_shadow(&font, code))
			{
				res = internal_shadow_putc16(&font, sx, sy);
				sx += font.pitch;
			}
			p++;
			break;

		case FONT_TYPE_GRAPHIC:
			if (graphic_font_get_shadow(&font, code))
			{
				res = internal_shadow_putc16(&font, sx, sy);
				sx += font.pitch;
			}
			p++;
			break;

#if JAPANESE_UI
		case FONT_TYPE_JPNHAN:
			if (jpn_h14p_get_shadow(&font, code))
			{
				res = internal_shadow_putc16(&font, sx, sy);
				sx += font.pitch;
			}
			p++;
			break;

		case FONT_TYPE_JPNZEN:
			if (jpn_z14p_get_shadow(&font, code))
			{
				res = internal_shadow_putc16(&font, sx, sy);
				sx += font.pitch;
			}
			p += 2;
			break;
#endif

		default:
			p++;
			break;
		}
	}
}

#endif /* PSP_VIDEO_32BPP */


/*------------------------------------------------------
	文字列を描画
------------------------------------------------------*/

void uifont_print(int sx, int sy, int r, int g, int b, const char *s)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		uifont_draw32(sx, sy, r, g, b, s);
	else
#endif
		uifont_draw16(sx, sy, r, g, b, s);
}


/*------------------------------------------------------
	文字列を描画 / センタリング処理
------------------------------------------------------*/

void uifont_print_center(int sy, int r, int g, int b, const char *s)
{
	int width = uifont_get_string_width(s);
	int sx = (SCR_WIDTH - width) / 2;

	uifont_print(sx, sy, r, g, b, s);
}


/*------------------------------------------------------
	文字列を描画 / 影付き
------------------------------------------------------*/

void uifont_print_shadow(int sx, int sy, int r, int g, int b, const char *s)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		uifont_draw_shadow32(sx, sy, s);
	else
		uifont_draw_shadow16(sx, sy, s);
#endif
	uifont_print(sx, sy, r, g, b, s);
}


/*------------------------------------------------------
	文字列を描画 / 影付き / センタリング処理
------------------------------------------------------*/

void uifont_print_shadow_center(int sy, int r, int g, int b, const char *s)
{
	int width = uifont_get_string_width(s);
	int sx = (SCR_WIDTH - width) / 2;

	uifont_print_shadow(sx, sy, r, g, b, s);
}


/******************************************************************************
	テキスト用固定ピッチフォント描画
******************************************************************************/

#ifdef COMMAND_LIST

#if PSP_VIDEO_32BPP

/*------------------------------------------------------
	欧文文字列描画 (32bit color)
------------------------------------------------------*/

INLINE void latin1_draw32(int sx, int sy, int r, int g, int b, const char *s)
{
	int type, res = 1;
	UINT16 code;
	const UINT8 *p = (const UINT8 *)s;
	struct font_t font;

	while (*p && res)
	{
		code = latin1_get_code(p, &type);

		switch (type)
		{
		case FONT_TYPE_ASCII:
			if ((res = ascii_14_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc32(&font, sx, sy, r, g, b);
				sx += FONTSIZE / 2;
				p++;
			}
			break;

		case FONT_TYPE_LATIN1:
			if ((res = latin1_14_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc32(&font, sx, sy, r, g, b);
				sx += FONTSIZE / 2;
				p++;
			}
			break;

		case FONT_TYPE_COMMAND:
			if (code == CODE_UNDERBAR)
			{
				code = *p - 0x20;
				if ((res = ascii_14_get_gryph(&font, code)) != 0)
				{
					res = internal_font_putc32(&font, sx, sy, r, g, b);
					sx += FONTSIZE/2;
					p += 2;
				}
			}
			else
			{
				int r2, g2, b2;

				if (CODE_HASCOLOR(code))
				{
					UINT32 color = command_font_color[code >> 8];

					r2 = GETR32(color);
					g2 = GETG32(color);
					b2 = GETB32(color);
					code &= 0xff;
				}
				else
				{
					r2 = r;
					g2 = g;
					b2 = b;
				}

				if ((res = command_font_get_gryph(&font, code)) != 0)
				{
					res = internal_font_putc32(&font, sx, sy, r2, g2, b2);
					sx += FONTSIZE;
					p += 2;
				}
			}
			break;

		default:
			res = 0;
			break;
		}
	}
}


/*------------------------------------------------------
	日本語フォント描画 (32bit color)
------------------------------------------------------*/

INLINE void sjis_draw32(int sx, int sy, int r, int g, int b, const char *s)
{
	int type, res = 1;
	UINT16 code;
	const UINT8 *p = (const UINT8 *)s;
	struct font_t font;

	while (*p && res)
	{
		code = sjis_get_code(p, &type);

		switch (type)
		{
		case FONT_TYPE_ASCII:
			if ((res = ascii_14_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc32(&font, sx, sy, r, g, b);
				sx += FONTSIZE / 2;
				p++;
			}
			break;

		case FONT_TYPE_JPNHAN:
			if ((res = jpn_h14_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc32(&font, sx, sy, r, g, b);
				sx += FONTSIZE / 2;
				p++;
			}
			break;

		case FONT_TYPE_JPNZEN:
			if ((res = jpn_z14_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc32(&font, sx, sy, r, g, b);
				sx += FONTSIZE;
				p += 2;
			}
			break;

		case FONT_TYPE_COMMAND:
			if (code == CODE_UNDERBAR)
			{
				code = *p - 0x20;
				if ((res = ascii_14_get_gryph(&font, code)) != 0)
				{
					res = internal_font_putc32(&font, sx, sy, r, g, b);
					sx += FONTSIZE/2;
					p += 2;
				}
			}
			else
			{
				int r2, g2, b2;

				if (CODE_HASCOLOR(code))
				{
					UINT32 color = command_font_color[code >> 8];

					r2 = GETR32(color);
					g2 = GETG32(color);
					b2 = GETB32(color);
					code &= 0xff;
				}
				else
				{
					r2 = r;
					g2 = g;
					b2 = b;
				}

				if ((res = command_font_get_gryph(&font, code)) != 0)
				{
					res = internal_font_putc32(&font, sx, sy, r2, g2, b2);
					sx += FONTSIZE;
					p += 2;
				}
			}
			break;

		default:
			res = 0;
			break;
		}
	}
}

#endif /* PSP_VIDEO_32BPP */


/*------------------------------------------------------
	欧文文字列描画 (16bit color)
------------------------------------------------------*/

INLINE void latin1_draw16(int sx, int sy, int r, int g, int b, const char *s)
{
	int type, res = 1;
	UINT16 code;
	const UINT8 *p = (const UINT8 *)s;
	struct font_t font;

#if !PSP_VIDEO_32BPP
	r >>= 3;
	g >>= 3;
	b >>= 3;
#endif

	while (*p && res)
	{
		code = latin1_get_code(p, &type);

		switch (type)
		{
		case FONT_TYPE_ASCII:
			if ((res = ascii_14_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc16(&font, sx, sy, r, g, b);
				sx += FONTSIZE / 2;
				p++;
			}
			break;

		case FONT_TYPE_LATIN1:
			if ((res = latin1_14_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc16(&font, sx, sy, r, g, b);
				sx += FONTSIZE / 2;
				p++;
			}
			break;

		case FONT_TYPE_COMMAND:
			if (code == CODE_UNDERBAR)
			{
				code = *p - 0x20;
				if ((res = ascii_14_get_gryph(&font, code)) != 0)
				{
					res = internal_font_putc16(&font, sx, sy, r, g, b);
					sx += FONTSIZE/2;
					p += 2;
				}
			}
			else
			{
				int r2, g2, b2;

				if (CODE_HASCOLOR(code))
				{
					UINT32 color = command_font_color[code >> 8];

					r2 = GETR16(color);
					g2 = GETG16(color);
					b2 = GETB16(color);
					code &= 0xff;
				}
				else
				{
					r2 = r;
					g2 = g;
					b2 = b;
				}

				if ((res = command_font_get_gryph(&font, code)) != 0)
				{
					res = internal_font_putc16(&font, sx, sy, r2, g2, b2);
					sx += FONTSIZE;
					p += 2;
				}
			}
			break;

		default:
			res = 0;
			break;
		}
	}
}


/*------------------------------------------------------
	日本語フォント描画 (16bit color)
------------------------------------------------------*/

INLINE void sjis_draw16(int sx, int sy, int r, int g, int b, const char *s)
{
	int type, res = 1;
	UINT16 code;
	const UINT8 *p = (const UINT8 *)s;
	struct font_t font;

#if !PSP_VIDEO_32BPP
	r >>= 3;
	g >>= 3;
	b >>= 3;
#endif

	while (*p && res)
	{
		code = sjis_get_code(p, &type);

		switch (type)
		{
		case FONT_TYPE_ASCII:
			if ((res = ascii_14_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc16(&font, sx, sy, r, g, b);
				sx += FONTSIZE / 2;
				p++;
			}
			break;

		case FONT_TYPE_JPNHAN:
			if ((res = jpn_h14_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc16(&font, sx, sy, r, g, b);
				sx += FONTSIZE / 2;
				p++;
			}
			break;

		case FONT_TYPE_JPNZEN:
			if ((res = jpn_z14_get_gryph(&font, code)) != 0)
			{
				res = internal_font_putc16(&font, sx, sy, r, g, b);
				sx += FONTSIZE;
				p += 2;
			}
			break;

		case FONT_TYPE_COMMAND:
			if (code == CODE_UNDERBAR)
			{
				code = *p - 0x20;
				if ((res = ascii_14_get_gryph(&font, code)) != 0)
				{
					res = internal_font_putc16(&font, sx, sy, r, g, b);
					sx += FONTSIZE/2;
					p += 2;
				}
			}
			else
			{
				int r2, g2, b2;

				if (CODE_HASCOLOR(code))
				{
					UINT32 color = command_font_color[code >> 8];

					r2 = GETR16(color);
					g2 = GETG16(color);
					b2 = GETB16(color);
					code &= 0xff;
				}
				else
				{
					r2 = r;
					g2 = g;
					b2 = b;
				}

				if ((res = command_font_get_gryph(&font, code)) != 0)
				{
					res = internal_font_putc16(&font, sx, sy, r2, g2, b2);
					sx += FONTSIZE;
					p += 2;
				}
			}
			break;

		default:
			res = 0;
			break;
		}
	}
}


/*------------------------------------------------------
	フォント描画
------------------------------------------------------*/

void textfont_print(int sx, int sy, int r, int g, int b, const char *s, int flag)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
	{
		if (flag & CHARSET_SHIFTJIS)
			sjis_draw32(sx, sy, r, g, b, s);
		else
			latin1_draw32(sx, sy, r, g, b, s);
	}
	else
#endif
	{
		if (flag & CHARSET_SHIFTJIS)
			sjis_draw16(sx, sy, r, g, b, s);
		else
			latin1_draw16(sx, sy, r, g, b, s);
	}
}

#endif /* COMMAND_LIST */


/******************************************************************************
	アイコン描画
******************************************************************************/

/*------------------------------------------------------
	アイコン(小)を描画
------------------------------------------------------*/

void small_icon(int sx, int sy, int r, int g, int b, int no)
{
	struct font_t font;

#if PSP_VIDEO_32BPP
	if (video_mode == 32)
	{
		if (icon_s_get_gryph(&font, no))
			internal_font_putc32(&font, sx, sy, r, g, b);
	}
	else
#else
	r >>= 3;
	g >>= 3;
	b >>= 3;
#endif
	{
		if (icon_s_get_gryph(&font, no))
		{
			internal_font_putc16(&font, sx, sy, r, g, b);
		}
	}
}


#if PSP_VIDEO_32BPP

/*------------------------------------------------------
	アイコン(小)を描画  (影エフェクトあり)
------------------------------------------------------*/

void small_icon_shadow(int sx, int sy, int r, int g, int b, int no)
{
	struct font_t font;

	if (video_mode == 32)
	{
		if (icon_s_get_shadow(&font, no))
			internal_shadow_putc32(&font, sx, sy);

		if (icon_s_get_gryph(&font, no))
			internal_font_putc32(&font, sx, sy, r, g, b);
	}
	else
	{
		if (icon_s_get_shadow(&font, no))
			internal_shadow_putc16(&font, sx, sy);

		if (icon_s_get_gryph(&font, no))
			internal_font_putc16(&font, sx, sy, r, g, b);
	}
}


/*------------------------------------------------------
	アイコン(小)を描画  (影、光エフェクトあり)
------------------------------------------------------*/

void small_icon_light(int sx, int sy, int r, int g, int b, int no)
{
	struct font_t font;

	if (video_mode == 32)
	{
		if (icon_s_get_shadow(&font, no))
			internal_shadow_putc32(&font, sx, sy);

		if (icon_s_get_light(&font, no))
			internal_light_putc32(&font, sx - 4, sy - 4);

		if (icon_s_get_gryph(&font, no))
			internal_font_putc32(&font, sx, sy, r, g, b);
	}
	else
	{
		if (icon_s_get_shadow(&font, no))
			internal_shadow_putc16(&font, sx, sy);

		if (icon_s_get_light(&font, no))
			internal_light_putc16(&font, sx - 4, sy - 4);

		if (icon_s_get_gryph(&font, no))
			internal_font_putc16(&font, sx, sy, r, g, b);
	}
}

#endif /* PSP_VIDEO_32BPP */


/*------------------------------------------------------
	アイコン(大)を描画
------------------------------------------------------*/

void large_icon(int sx, int sy, int r, int g, int b, int no)
{
	struct font_t font;

#if PSP_VIDEO_32BPP
	if (video_mode == 32)
	{
		if (icon_l_get_gryph(&font, no))
			internal_font_putc32(&font, sx, sy, r, g, b);
	}
	else
#else
	r >>= 3;
	g >>= 3;
	b >>= 3;
#endif
	{
		if (icon_l_get_gryph(&font, no))
			internal_font_putc16(&font, sx, sy, r, g, b);
	}
}


#if PSP_VIDEO_32BPP

/*------------------------------------------------------
	アイコン(大)を描画 (影エフェクトあり)
------------------------------------------------------*/

void large_icon_shadow(int sx, int sy, int r, int g, int b, int no)
{
	struct font_t font;

	if (video_mode == 32)
	{
		if (icon_l_get_shadow(&font, no))
			internal_shadow_putc32(&font, sx, sy);

		if (icon_l_get_gryph(&font, no))
			internal_font_putc32(&font, sx, sy, r, g, b);
	}
	else
	{
		if (icon_l_get_shadow(&font, no))
			internal_shadow_putc16(&font, sx, sy);

		if (icon_l_get_gryph(&font, no))
			internal_font_putc16(&font, sx, sy, r, g, b);
	}
}


/*------------------------------------------------------
	アイコン(大)を描画  (影、光エフェクトあり)
------------------------------------------------------*/

void large_icon_light(int sx, int sy, int r, int g, int b, int no)
{
	struct font_t font;

	if (video_mode == 32)
	{
		if (icon_l_get_shadow(&font, no))
			internal_shadow_putc32(&font, sx, sy);

		if (icon_l_get_light(&font, no))
			internal_light_putc32(&font, sx - 4, sy - 4);

		if (icon_l_get_gryph(&font, no))
			internal_font_putc32(&font, sx, sy, r, g, b);
	}
	else
	{
		if (icon_l_get_shadow(&font, no))
			internal_shadow_putc16(&font, sx, sy);

		if (icon_l_get_light(&font, no))
			internal_light_putc16(&font, sx - 4, sy - 4);

		if (icon_l_get_gryph(&font, no))
			internal_font_putc16(&font, sx, sy, r, g, b);
	}
}


/*------------------------------------------------------
	アイコンの光
------------------------------------------------------*/

int ui_light_update(void)
{
	static int light_dir = 1;
	int prev_level;

	prev_level = light_level >> 1;

	light_level += light_dir;
	if (light_level > 31)
	{
		light_level = 31;
		light_dir = -1;
	}
	else if (light_level < 0)
	{
		light_level = 0;
		light_dir = 1;
	}

	return (prev_level != (light_level >> 1)) << 1;
}

#endif /* PSP_VIDEO_32BPP */


/******************************************************************************
	ゲーム画面用フォント描画 (16bit colorのみ)
******************************************************************************/

/*------------------------------------------------------
	テクスチャ作成
------------------------------------------------------*/

#define NUM_FONTS	0x60
#define MAX_STR_LEN	256

static UINT16 *tex_font;

void create_small_font(void)
{
	int code, x, y;
	UINT16 *dst;
	UINT16 color[8] = {
		MAKECOL15(248,248,248),
		MAKECOL15(240,240,240),
		MAKECOL15(232,232,232),
		MAKECOL15(224,224,224),
		MAKECOL15(216,216,216),
		MAKECOL15(208,208,208),
		MAKECOL15(200,200,200),
		MAKECOL15(192,192,192)
	};

	tex_font = (UINT16 *)video_frame_addr(0, 0, 2032);
	dst = tex_font;

	for (code = 0; code < NUM_FONTS; code++)
	{
		for (y = 0; y < 8; y++)
		{
			UINT8 data = font_s[(code << 3) + y];
			UINT8 mask = 0x80;

			for (x = 0; x < 8; x++)
			{
				*dst++ = (data & mask) ? color[y] : 0x8000;
				mask >>= 1;
			}
		}
	}
}


/*------------------------------------------------------
	文字列描画
------------------------------------------------------*/

void small_font_print(int sx, int sy, const char *s, int bg)
{
	int len = strlen(s);
	struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(len * 2 * sizeof(struct Vertex));

	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, draw_frame, BUF_WIDTH);
	sceGuScissor(sx, sy, sx + 8 * len, sy + 8);

	if (bg)
		sceGuDisable(GU_ALPHA_TEST);
	else
		sceGuEnable(GU_ALPHA_TEST);

	sceGuTexMode(GU_PSM_5551, 0, 0, GU_TRUE);
	sceGuTexImage(0, 512, 512, BUF_WIDTH, tex_font);
	sceGuTexFilter(GU_NEAREST, GU_NEAREST);

	if (vertices)
	{
		int i;
		struct Vertex *vertices_tmp = vertices;

		for (i = 0; i < len; i++)
		{
			UINT8 code = isascii((UINT8)s[i]) ? s[i] - 0x20 : 0x20;
			int u = (code & 63) << 3;
			int v = (code >> 6) << 3;

			vertices_tmp[0].u = u;
			vertices_tmp[0].v = v;
			vertices_tmp[0].x = sx;
			vertices_tmp[0].y = sy;

			vertices_tmp[1].u = u + 8;
			vertices_tmp[1].v = v + 8;
			vertices_tmp[1].x = sx + 8;
			vertices_tmp[1].y = sy + 8;

			vertices_tmp += 2;
			sx += 8;
		}

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2 * len, 0, vertices);
	}

	sceGuFinish();
	sceGuSync(0, 0);
}


/*------------------------------------------------------
	書式付文字列描画
------------------------------------------------------*/

void small_font_printf(int x, int y, const char *text, ...)
{
	char buf[256];
	va_list arg;

	va_start(arg, text);
	vsprintf(buf, text, arg);
	va_end(arg);

	small_font_print(x << 3, y << 3, buf, 1);
}


/*------------------------------------------------------
	文字列描画
------------------------------------------------------*/

static void debug_font_print(void *frame, int sx, int sy, const char *s, int bg)
{
	int len = strlen(s);
	struct Vertex *vertices = (struct Vertex *)sceGuGetMemory(len * 2 * sizeof(struct Vertex));

	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(sx, sy, sx + 8 * len, sy + 8);

	if (bg)
		sceGuDisable(GU_ALPHA_TEST);
	else
		sceGuEnable(GU_ALPHA_TEST);

	sceGuTexMode(GU_PSM_5551, 0, 0, GU_TRUE);
	sceGuTexImage(0, 512, 512, BUF_WIDTH, tex_font);
	sceGuTexFilter(GU_NEAREST, GU_NEAREST);

	if (vertices)
	{
		int i;
		struct Vertex *vertices_tmp = vertices;

		for (i = 0; i < len; i++)
		{
			UINT8 code = isascii((UINT8)s[i]) ? s[i] - 0x20 : 0x20;
			int u = (code & 63) << 3;
			int v = (code >> 6) << 3;

			vertices_tmp[0].u = u;
			vertices_tmp[0].v = v;
			vertices_tmp[0].x = sx;
			vertices_tmp[0].y = sy;

			vertices_tmp[1].u = u + 8;
			vertices_tmp[1].v = v + 8;
			vertices_tmp[1].x = sx + 8;
			vertices_tmp[1].y = sy + 8;

			vertices_tmp += 2;
			sx += 8;
		}

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2 * len, 0, vertices);
	}

	sceGuFinish();
	sceGuSync(0, 0);
}


/*------------------------------------------------------
	書式付文字列描画
------------------------------------------------------*/

void debug_font_printf(void *frame, int x, int y, const char *text, ...)
{
	char buf[256];
	va_list arg;

	va_start(arg, text);
	vsprintf(buf, text, arg);
	va_end(arg);

	debug_font_print(frame, x << 3, y << 3, buf, 1);
}


/******************************************************************************
	図形描画
******************************************************************************/

#if PSP_VIDEO_32BPP

/*------------------------------------------------------
	水平線描画 (32bit)
------------------------------------------------------*/

static void hline32(int sx, int ex, int y, int r, int g, int b)
{
	int x;
	int width  = (ex - sx) + 1;
	UINT32 *dst = vram_pos32(sx, y);
	UINT32 color = MAKECOL32(r, g, b);

	for (x = 0; x < width; x++)
		*dst++ = color;
}


/*------------------------------------------------------
	水平線描画 (32bit/アルファブレンド)
------------------------------------------------------*/

static void hline_alpha32(int sx, int ex, int y, int r, int g, int b, int alpha)
{
	int x, dst_r, dst_g, dst_b;
	int width  = (ex - sx) + 1;
	UINT32 *dst = vram_pos32(sx, y);

	for (x = 0; x < width; x++)
	{
		dst_r = GETR32(dst[x]);
		dst_g = GETG32(dst[x]);
		dst_b = GETB32(dst[x]);

		dst_r = alpha_blend[alpha][r][dst_r];
		dst_g = alpha_blend[alpha][g][dst_g];
		dst_b = alpha_blend[alpha][b][dst_b];

		dst[x] = MAKECOL32(dst_r, dst_g, dst_b);
	}
}


/*------------------------------------------------------
	水平線描画 (32bit/アルファブレンド/グラデーション)
------------------------------------------------------*/

static void hline_gradation32(int sx, int ex, int y, int r1, int g1, int b1, int r2, int g2, int b2, int alpha)
{
	int x;
	int dr, dg, db;
	int ir, ig, ib;
	int cr, cg, cb;
	int width  = (ex - sx) + 1;
	UINT32 *dst = vram_pos32(sx, y);

	dr = dg = db = 0;
	cr = cg = cb = 0;
	ir = ig = ib = 0;

	if (r1 != r2)
	{
		dr = (r1 < r2) ? width / (r2 - r1) : width / (r1 - r2);
		ir = (r1 < r2) ? 1 : -1;
	}
	if (g1 != g2)
	{
		dg = (g1 < g2) ? width / (g2 - g1) : width / (g1 - g2);
		ig = (g1 < g2) ? 1 : -1;
	}
	if (b1 != b2)
	{
		db = (b1 < b2) ? width / (b2 - b1) : width / (b1 - b2);
		ib = (b1 < b2) ? 1 : -1;
	}

	if (alpha == 10)
	{
		for (x = 0; x < width; x++)
		{
			dst[x] = MAKECOL32(r1, g1, b1);

			if (dr) { if (cr++ == dr) { r1 += ir; cr = 0; } }
			if (dg) { if (cg++ == dg) { g1 += ig; cg = 0; } }
			if (db) { if (cb++ == db) { b1 += ib; cb = 0; } }
		}
	}
	else
	{
		for (x = 0; x < width; x++)
		{
			r2 = GETR32(dst[x]);
			g2 = GETG32(dst[x]);
			b2 = GETB32(dst[x]);

			r2 = alpha_blend[alpha][r1][r2];
			g2 = alpha_blend[alpha][g1][g2];
			b2 = alpha_blend[alpha][b1][b2];

			dst[x] = MAKECOL32(r2, g2, b2);

			if (dr) { if (cr++ == dr) { r1 += ir; cr = 0; } }
			if (dg) { if (cg++ == dg) { g1 += ig; cg = 0; } }
			if (db) { if (cb++ == db) { b1 += ib; cb = 0; } }
		}
	}
}


/*------------------------------------------------------
	垂直線描画 (32bit)
------------------------------------------------------*/

static void vline32(int x, int sy, int ey, int r, int g, int b)
{
	int y;
	int height = (ey - sy) + 1;
	UINT32 *dst = vram_pos32(x, sy);
	UINT32 color = MAKECOL32(r, g, b);

	for (y = 0; y < height; y++)
	{
		*dst = color;
		dst += BUF_WIDTH;
	}
}


/*------------------------------------------------------
	垂直線描画 (32bit/アルファブレンド)
------------------------------------------------------*/

static void vline_alpha32(int x, int sy, int ey, int r, int g, int b, int alpha)
{
	int y, dst_r, dst_g, dst_b;
	int height = (ey - sy) + 1;
	UINT32 *dst = vram_pos32(x, sy);

	for (y = 0; y < height; y++)
	{
		dst_r = GETR32(*dst);
		dst_g = GETG32(*dst);
		dst_b = GETB32(*dst);

		dst_r = alpha_blend[alpha][r][dst_r];
		dst_g = alpha_blend[alpha][g][dst_g];
		dst_b = alpha_blend[alpha][b][dst_b];

		*dst = MAKECOL32(dst_r, dst_g, dst_b);

		dst += BUF_WIDTH;
	}
}


/*------------------------------------------------------
	垂直線描画 (32bit/アルファブレンド/グラデーション)
------------------------------------------------------*/

static void vline_gradation32(int x, int sy, int ey, int r1, int g1, int b1, int r2, int g2, int b2, int alpha)
{
	int y;
	int dr, dg, db;
	int ir, ig, ib;
	int cr, cg, cb;
	int height  = (ey - sy) + 1;
	UINT32 *dst = vram_pos32(x, sy);

	dr = dg = db = 0;
	cr = cg = cb = 0;
	ir = ig = ib = 0;

	if (r1 != r2)
	{
		dr = (r1 < r2) ? height / (r2 - r1) : height / (r1 - r2);
		ir = (r1 < r2) ? 1 : -1;
	}
	if (g1 != g2)
	{
		dg = (g1 < g2) ? height / (g2 - g1) : height / (g1 - g2);
		ig = (g1 < g2) ? 1 : -1;
	}
	if (b1 != b2)
	{
		db = (b1 < b2) ? height / (b2 - b1) : height / (b1 - b2);
		ib = (b1 < b2) ? 1 : -1;
	}

	if (alpha == 10)
	{
		for (y = 0; y < height; y++)
		{
			*dst = MAKECOL32(r1, g1, b1);
			dst += BUF_WIDTH;

			if (dr) { if (cr++ == dr) { r1 += ir; cr = 0; } }
			if (dg) { if (cg++ == dg) { g1 += ig; cg = 0; } }
			if (db) { if (cb++ == db) { b1 += ib; cb = 0; } }
		}
	}
	else
	{
		for (y = 0; y < height; y++)
		{
			r2 = GETR32(*dst);
			g2 = GETG32(*dst);
			b2 = GETB32(*dst);

			r2 = alpha_blend[alpha][r1][r2];
			g2 = alpha_blend[alpha][g1][g2];
			b2 = alpha_blend[alpha][b1][b2];

			*dst = MAKECOL32(r2, g2, b2);

			dst += BUF_WIDTH;

			if (dr) { if (cr++ == dr) { r1 += ir; cr = 0; } }
			if (dg) { if (cg++ == dg) { g1 += ig; cg = 0; } }
			if (db) { if (cb++ == db) { b1 += ib; cb = 0; } }
		}
	}
}


/*------------------------------------------------------
	矩形描画 (32bit)
------------------------------------------------------*/

static void box32(int sx, int sy, int ex, int ey, int r, int g, int b)
{
	hline32(sx, ex - 1, sy, r, g, b);
	vline32(ex, sy, ey - 1, r, g, b);
	hline32(sx + 1, ex, ey, r, g, b);
	vline32(sx, sy + 1, ey, r, g, b);
}

/*------------------------------------------------------
	矩形塗りつぶし (32bit)
------------------------------------------------------*/

static void boxfill32(int sx, int sy, int ex, int ey, int r, int g, int b)
{
	int x, y;
	int width  = (ex - sx) + 1;
	int height = (ey - sy) + 1;
	UINT32 color, *dst = vram_pos32(sx, sy);

	color = MAKECOL32(r, g, b);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			dst[x] = color;
		}
		dst += BUF_WIDTH;
	}
}


/*------------------------------------------------------
	矩形塗りつぶし (32bit/アルファブレンド)
------------------------------------------------------*/

static void boxfill_alpha32(int sx, int sy, int ex, int ey, int r, int g, int b, int alpha)
{
	int x, y;
	int dst_r, dst_g, dst_b;
	int width  = (ex - sx) + 1;
	int height = (ey - sy) + 1;
	UINT32 color, *dst = vram_pos32(sx, sy);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			color = dst[x];

			dst_r = GETR32(color);
			dst_g = GETG32(color);
			dst_b = GETB32(color);

			dst_r = alpha_blend[alpha][r][dst_r];
			dst_g = alpha_blend[alpha][g][dst_g];
			dst_b = alpha_blend[alpha][b][dst_b];

			dst[x] = MAKECOL32(dst_r, dst_g, dst_b);
		}
		dst += BUF_WIDTH;
	}
}


/*---------------------------------------------------------
	矩形塗りつぶし (32bit/アルファブレンド/グラデーション)
---------------------------------------------------------*/

static void boxfill_gradation32(int sx, int sy, int ex, int ey, int r1, int g1, int b1, int r2, int g2, int b2, int alpha, int dir)
{
	int x, y;
	int width  = (ex - sx) + 1;
	int height = (ey - sy) + 1;

	if (dir)
	{
		// 縦方向
		for (x = 0; x < width; x++)
			vline_gradation32(sx + x, sy, ey, r1, g1, b1, r2, g2, b2, alpha);
	}
	else
	{
		// 横方向
		for (y = 0; y < height; y++)
			hline_gradation32(sx, ex, sy + y, r1, g1, b1, r2, g2, b2, alpha);
	}
}

#endif /* PSP_VIDEO_32BPP */


/*------------------------------------------------------
	水平線描画 (16bit)
------------------------------------------------------*/

static void hline16(int sx, int ex, int y, int r, int g, int b)
{
#if 0
	int x;
	int width  = (ex - sx) + 1;
	UINT16 *dst = vram_pos16(sx, y);
	UINT16 color = MAKECOL15(r, g, b);

	for (x = 0; x < width; x++)
		*dst++ = color;
#else
	struct Vertex *vertices;

	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, draw_frame, BUF_WIDTH);
	sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);
	sceGuDisable(GU_ALPHA_TEST);
	sceGuDisable(GU_TEXTURE_2D);

	vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

	if (vertices)
	{
		memset(vertices, 0, 2 * sizeof(struct Vertex));

		vertices[0].x = sx;
		vertices[0].y = y;
		vertices[0].color = MAKECOL15(r, g, b);

		vertices[1].x = ex;
		vertices[1].y = y;
		vertices[1].color = MAKECOL15(r, g, b);

		sceGuDrawArray(GU_LINES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	sceGuEnable(GU_TEXTURE_2D);

	sceGuFinish();
	sceGuSync(0, 0);
#endif
}


/*------------------------------------------------------
	水平線描画 (16bit/アルファブレンド)
------------------------------------------------------*/

static void hline_alpha16(int sx, int ex, int y, int r, int g, int b, int alpha)
{
	int x, dst_r, dst_g, dst_b;
	int width  = (ex - sx) + 1;
	UINT16 *dst = vram_pos16(sx, y);

#if !PSP_VIDEO_32BPP
	r >>= 3;
	g >>= 3;
	b >>= 3;
#endif

	for (x = 0; x < width; x++)
	{
		dst_r = GETR16(dst[x]);
		dst_g = GETG16(dst[x]);
		dst_b = GETB16(dst[x]);

		dst_r = alpha_blend[alpha][r][dst_r];
		dst_g = alpha_blend[alpha][g][dst_g];
		dst_b = alpha_blend[alpha][b][dst_b];

		dst[x] = MAKECOL16(dst_r, dst_g, dst_b);
	}
}


/*------------------------------------------------------
	水平線描画 (16bit/アルファブレンド/グラデーション)
------------------------------------------------------*/

static void hline_gradation16(int sx, int ex, int y, int r1, int g1, int b1, int r2, int g2, int b2, int alpha)
{
#if PSP_VIDEO_32BPP
	int x;
	int dr, dg, db;
	int ir, ig, ib;
	int cr, cg, cb;
	int width  = (ex - sx) + 1;
	UINT16 *dst = vram_pos16(sx, y);

	dr = dg = db = 0;
	cr = cg = cb = 0;
	ir = ig = ib = 0;

	if (r1 != r2)
	{
		dr = (r1 < r2) ? width / (r2 - r1) : width / (r1 - r2);
		ir = (r1 < r2) ? 1 : -1;
	}
	if (g1 != g2)
	{
		dg = (g1 < g2) ? width / (g2 - g1) : width / (g1 - g2);
		ig = (g1 < g2) ? 1 : -1;
	}
	if (b1 != b2)
	{
		db = (b1 < b2) ? width / (b2 - b1) : width / (b1 - b2);
		ib = (b1 < b2) ? 1 : -1;
	}

	if (alpha == 15)
	{
		for (x = 0; x < width; x++)
		{
			dst[x] = MAKECOL16(r1, g1, b1);

			if (dr) { if (cr++ == dr) { r1 += ir; cr = 0; } }
			if (dg) { if (cg++ == dg) { g1 += ig; cg = 0; } }
			if (db) { if (cb++ == db) { b1 += ib; cb = 0; } }
		}
	}
	else
	{
		for (x = 0; x < width; x++)
		{
			r2 = GETR16(dst[x]);
			g2 = GETG16(dst[x]);
			b2 = GETB16(dst[x]);

			r2 = alpha_blend[alpha][r1][r2];
			g2 = alpha_blend[alpha][g1][g2];
			b2 = alpha_blend[alpha][b1][b2];

			dst[x] = MAKECOL16(r2, g2, b2);

			if (dr) { if (cr++ == dr) { r1 += ir; cr = 0; } }
			if (dg) { if (cg++ == dg) { g1 += ig; cg = 0; } }
			if (db) { if (cb++ == db) { b1 += ib; cb = 0; } }
		}
	}
#else
	hline_alpha16(sx, ex, y, r2, g2, b2, alpha);
#endif
}


/*------------------------------------------------------
	垂直線描画 (16bit)
------------------------------------------------------*/

static void vline16(int x, int sy, int ey, int r, int g, int b)
{
	int y;
	int height = (ey - sy) + 1;
	UINT16 *dst = vram_pos16(x, sy);
	UINT16 color = MAKECOL15(r, g, b);

	for (y = 0; y < height; y++)
	{
		*dst = color;
		dst += BUF_WIDTH;
	}
}


/*------------------------------------------------------
	垂直線描画 (16bit/アルファブレンド)
------------------------------------------------------*/

static void vline_alpha16(int x, int sy, int ey, int r, int g, int b, int alpha)
{
	int y, dst_r, dst_g, dst_b;
	int height = (ey - sy) + 1;
	UINT16 *dst = vram_pos16(x, sy);

#if !PSP_VIDEO_32BPP
	r >>= 3;
	g >>= 3;
	b >>= 3;
#endif

	for (y = 0; y < height; y++)
	{
		dst_r = GETR16(*dst);
		dst_g = GETG16(*dst);
		dst_b = GETB16(*dst);

		dst_r = alpha_blend[alpha][r][dst_r];
		dst_g = alpha_blend[alpha][g][dst_g];
		dst_b = alpha_blend[alpha][b][dst_b];

		*dst = MAKECOL16(dst_r, dst_g, dst_b);

		dst += BUF_WIDTH;
	}
}


/*------------------------------------------------------
	垂直線描画 (16bit/アルファブレンド/グラデーション)
------------------------------------------------------*/

static void vline_gradation16(int x, int sy, int ey, int r1, int g1, int b1, int r2, int g2, int b2, int alpha)
{
#if PSP_VIDEO_32BPP
	int y;
	int dr, dg, db;
	int ir, ig, ib;
	int cr, cg, cb;
	int height  = (ey - sy) + 1;
	UINT16 *dst = vram_pos16(x, sy);

	dr = dg = db = 0;
	cr = cg = cb = 0;
	ir = ig = ib = 0;

	if (r1 != r2)
	{
		dr = (r1 < r2) ? height / (r2 - r1) : height / (r1 - r2);
		ir = (r1 < r2) ? 1 : -1;
	}
	if (g1 != g2)
	{
		dg = (g1 < g2) ? height / (g2 - g1) : height / (g1 - g2);
		ig = (g1 < g2) ? 1 : -1;
	}
	if (b1 != b2)
	{
		db = (b1 < b2) ? height / (b2 - b1) : height / (b1 - b2);
		ib = (b1 < b2) ? 1 : -1;
	}

	if (alpha == 16)
	{
		for (y = 0; y < height; y++)
		{
			*dst = MAKECOL16(r1, g1, b1);
			dst += BUF_WIDTH;

			if (dr) { if (cr++ == dr) { r1 += ir; cr = 0; } }
			if (dg) { if (cg++ == dg) { g1 += ig; cg = 0; } }
			if (db) { if (cb++ == db) { b1 += ib; cb = 0; } }
		}
	}
	else
	{
		for (y = 0; y < height; y++)
		{
			r2 = GETR16(*dst);
			g2 = GETG16(*dst);
			b2 = GETB16(*dst);

			r2 = alpha_blend[alpha][r1][r2];
			g2 = alpha_blend[alpha][g1][g2];
			b2 = alpha_blend[alpha][b1][b2];

			*dst = MAKECOL16(r2, g2, b2);

			dst += BUF_WIDTH;

			if (dr) { if (cr++ == dr) { r1 += ir; cr = 0; } }
			if (dg) { if (cg++ == dg) { g1 += ig; cg = 0; } }
			if (db) { if (cb++ == db) { b1 += ib; cb = 0; } }
		}
	}
#else
	vline_alpha16(x, sy, ey, r2, g2, b2, alpha);
#endif
}


/*------------------------------------------------------
	矩形描画 (16bit)
------------------------------------------------------*/

static void box16(int sx, int sy, int ex, int ey, int r, int g, int b)
{
	hline16(sx, ex - 1, sy, r, g, b);
	vline16(ex, sy, ey - 1, r, g, b);
	hline16(sx + 1, ex, ey, r, g, b);
	vline16(sx, sy + 1, ey, r, g, b);
}


/*------------------------------------------------------
	矩形塗りつぶし (16bit)
------------------------------------------------------*/

static void boxfill16(int sx, int sy, int ex, int ey, int r, int g, int b)
{
	int x, y;
	int width  = (ex - sx) + 1;
	int height = (ey - sy) + 1;
	UINT16 *dst = vram_pos16(sx, sy);
	UINT16 color = MAKECOL15(r, g, b);

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			dst[x] = color;
		}
		dst += BUF_WIDTH;
	}
}


/*------------------------------------------------------
	矩形塗りつぶし (16bit/アルファブレンド)
------------------------------------------------------*/

static void boxfill_alpha16(int sx, int sy, int ex, int ey, int r, int g, int b, int alpha)
{
	int x, y;
	int dst_r, dst_g, dst_b;
	int width  = (ex - sx) + 1;
	int height = (ey - sy) + 1;
	UINT16 color, *dst = vram_pos16(sx, sy);

#if !PSP_VIDEO_32BPP
	r >>= 3;
	g >>= 3;
	b >>= 3;
#endif

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			color = dst[x];

			dst_r = GETR16(dst[x]);
			dst_g = GETG16(dst[x]);
			dst_b = GETB16(dst[x]);

			dst_r = alpha_blend[alpha][r][dst_r];
			dst_g = alpha_blend[alpha][g][dst_g];
			dst_b = alpha_blend[alpha][b][dst_b];

			dst[x] = MAKECOL16(dst_r, dst_g, dst_b);
		}

		dst += BUF_WIDTH;
	}
}


/*---------------------------------------------------------
	矩形塗りつぶし (16bit/アルファブレンド/グラデーション)
---------------------------------------------------------*/

static void boxfill_gradation16(int sx, int sy, int ex, int ey, int r1, int g1, int b1, int r2, int g2, int b2, int alpha, int dir)
{
#if PSP_VIDEO_32BPP
	int x, y;
	int width  = (ex - sx) + 1;
	int height = (ey - sy) + 1;

	if (dir)
	{
		// 縦方向
		for (x = 0; x < width; x++)
			vline_gradation16(sx + x, sy, ey, r1, g1, b1, r2, g2, b2, alpha);
	}
	else
	{
		// 横方向
		for (y = 0; y < height; y++)
			hline_gradation16(sx, ex, sy + y, r1, g1, b1, r2, g2, b2, alpha);
	}
#else
	boxfill_alpha16(sx, sy, ex, ey, r1, g1, b1, alpha);
#endif
}


/*------------------------------------------------------
	水平線描画
------------------------------------------------------*/

void hline(int sx, int ex, int y, int r, int g, int b)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		hline32(sx, ex, y, r, g, b);
	else
#endif
		hline16(sx, ex, y, r, g, b);
}


/*------------------------------------------------------
	水平線描画 (アルファブレンド)
------------------------------------------------------*/

void hline_alpha(int sx, int ex, int y, int r, int g, int b, int alpha)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		hline_alpha32(sx, ex, y, r, g, b, alpha);
	else
#endif
		hline_alpha16(sx, ex, y, r, g, b, alpha);
}


/*------------------------------------------------------
	水平線描画 (アルファブレンド/グラデーション)
------------------------------------------------------*/

void hline_gradation(int sx, int ex, int y, int r1, int g1, int b1, int r2, int g2, int b2, int alpha)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		hline_gradation32(sx, ex, y, r1, g1, b1, r2, g2, b2, alpha);
	else
#endif
		hline_gradation16(sx, ex, y, r1, g1, b1, r2, g2, b2, alpha);
}


/*------------------------------------------------------
	垂直線描画
------------------------------------------------------*/

void vline(int x, int sy, int ey, int r, int g, int b)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		vline32(x, sy, ey, r, g, b);
	else
#endif
		vline16(x, sy, ey, r, g, b);
}


/*------------------------------------------------------
	垂直線描画 (アルファブレンド)
------------------------------------------------------*/

void vline_alpha(int x, int sy, int ey, int r, int g, int b, int alpha)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		vline_alpha32(x, sy, ey, r, g, b, alpha);
	else
#endif
		vline_alpha16(x, sy, ey, r, g, b, alpha);
}


/*------------------------------------------------------
	垂直線描画 (アルファブレンド/グラデーション)
------------------------------------------------------*/

void vline_gradation(int x, int sy, int ey, int r1, int g1, int b1, int r2, int g2, int b2, int alpha)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		vline_gradation32(x, sy, ey, r1, g1, b1, r2, g2, b2, alpha);
	else
#endif
		vline_gradation16(x, sy, ey, r1, g1, b1, r2, g2, b2, alpha);
}


/*------------------------------------------------------
	矩形描画
------------------------------------------------------*/

void box(int sx, int sy, int ex, int ey, int r, int g, int b)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		box32(sx, sy, ex, ey, r, g, b);
	else
#endif
		box16(sx, sy, ex, ey, r, g, b);
}


/*------------------------------------------------------
	矩形塗りつぶし
------------------------------------------------------*/

void boxfill(int sx, int sy, int ex, int ey, int r, int g, int b)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		boxfill32(sx, sy, ex, ey, r, g, b);
	else
#endif
		boxfill16(sx, sy, ex, ey, r, g, b);
}


/*------------------------------------------------------
	矩形塗りつぶし (アルファブレンド)
------------------------------------------------------*/

void boxfill_alpha(int sx, int sy, int ex, int ey, int r, int g, int b, int alpha)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		boxfill_alpha32(sx, sy, ex, ey, r, g, b, alpha);
	else
#endif
		boxfill_alpha16(sx, sy, ex, ey, r, g, b, alpha);
}


/*---------------------------------------------------------
	矩形塗りつぶし (アルファブレンド/グラデーション)
---------------------------------------------------------*/

void boxfill_gradation(int sx, int sy, int ex, int ey, int r1, int g1, int b1, int r2, int g2, int b2, int alpha, int dir)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		boxfill_gradation32(sx, sy, ex, ey, r1, g1, b1, r2, g2, b2, alpha, dir);
	else
#endif
		boxfill_gradation16(sx, sy, ex, ey, r1, g1, b1, r2, g2, b2, alpha, dir);
}


/******************************************************************************
	ユーザインタフェースのパーツを描画
******************************************************************************/

#if PSP_VIDEO_32BPP

/*------------------------------------------------------
	矩形の影を描画 (32bit)
------------------------------------------------------*/

static void draw_boxshadow32(UINT32 *vptr, int w, int h, int no)
{
	int x, y;
	int dst_r, dst_g, dst_b, a;
	UINT32 *dst = vptr, color;

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			if (x & 1)
				a = shadow[no][y][x >> 1] >> 4;
			else
				a = shadow[no][y][x >> 1] & 0x0f;

			if (a)
			{
				color = dst[x];

				dst_r = GETR32(color);
				dst_g = GETG32(color);
				dst_b = GETB32(color);

				dst_r = alpha_blend[a][dst_r >> 2][dst_r];
				dst_g = alpha_blend[a][dst_g >> 2][dst_g];
				dst_b = alpha_blend[a][dst_b >> 2][dst_b];

				dst[x] = MAKECOL32(dst_r, dst_g, dst_b);
			}
		}
		dst += BUF_WIDTH;
	}
}


static void fill_boxshadow32(UINT32 *vptr, int w, int h)
{
	int x, y;
	int dst_r, dst_g, dst_b;
	UINT32 *dst = vptr, color;

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			color = dst[x];

			dst_r = GETR32(color);
			dst_g = GETG32(color);
			dst_b = GETB32(color);

			dst_r = alpha_blend[15][dst_r >> 2][dst_r];
			dst_g = alpha_blend[15][dst_g >> 2][dst_g];
			dst_b = alpha_blend[15][dst_b >> 2][dst_b];

			dst[x] = MAKECOL32(dst_r, dst_g, dst_b);
		}
		dst += BUF_WIDTH;
	}
}


static void draw_box_shadow32(int sx, int sy, int ex, int ey)
{
	int i, width, height;
	int w, h, nw, nh, sx2, sy2;
	UINT32 *vptr;

	width = (ex - sx) + 1;
	height = (ey - sy) + 1;

	width  -= 14;
	height -= 14;

	nw = width / 8;
	nh = height / 8;

	w = width % 8;
	h = height % 8;

	/*----------------------------------*/
	// top
	/*----------------------------------*/
	sx2 = sx + 2;
	sy2 = sy + 2;
	vptr = vram_pos32(sx2, sy2);

	// left top
	draw_boxshadow32(vptr, 8, 8, 0);
	vptr += 8;

	// top
	for (i = 0; i < nw; i++)
	{
		draw_boxshadow32(vptr, 8, 8, 1);
		vptr += 8;
	}
	draw_boxshadow32(vptr, w, 8, 1);
	vptr += w;

	// right top
	draw_boxshadow32(vptr, 8, 8, 2);

	/*----------------------------------*/
	// left
	/*----------------------------------*/
	sx2 = sx + 2;
	sy2 = sy + 10;
	vptr = vram_pos32(sx2, sy2);

	for (i = 0; i < nh; i++)
	{
		draw_boxshadow32(vptr, 8, 8, 3);
		vptr += 8 * BUF_WIDTH;
	}
	draw_boxshadow32(vptr, 8, h, 3);

	/*----------------------------------*/
	// right
	/*----------------------------------*/
	sx2 = sx + 10 + width;
	sy2 = sy + 10;
	vptr = vram_pos32(sx2, sy2);

	for (i = 0; i < nh; i++)
	{
		draw_boxshadow32(vptr, 8, 8, 5);
		vptr += 8 * BUF_WIDTH;
	}
	draw_boxshadow32(vptr, 8, h, 5);

	/*----------------------------------*/
	// bottom
	/*----------------------------------*/
	sx2 = sx + 2;
	sy2 = sy + 10 + height;
	vptr = vram_pos32(sx2, sy2);

	// left bottom
	draw_boxshadow32(vptr, 8, 8, 6);
	vptr += 8;

	// bottom
	for (i = 0; i < nw; i++)
	{
		draw_boxshadow32(vptr, 8, 8, 7);
		vptr += 8;
	}
	draw_boxshadow32(vptr, w, 8, 7);
	vptr += w;

	// right bottom
	draw_boxshadow32(vptr, 8, 8, 8);

	vptr = vram_pos32(sx + 10, sy + 10);
	fill_boxshadow32(vptr, width, height);
}


/*------------------------------------------------------
	上部バーの影を描画
------------------------------------------------------*/

static void draw_bar_shadow32(void)
{
	int i;
	UINT32 *vptr = vram_pos32(0, 20);

	for (i = 0; i < SCR_WIDTH / 8; i++)
	{
		draw_boxshadow32(vptr, 8, 8, 7);
		vptr += 8;
	}

	vptr = vram_pos32(0, 0);
	fill_boxshadow32(vptr, SCR_WIDTH, 20);
}

#endif /* PSP_VIDEO_32BPP */


/*------------------------------------------------------
	矩形の影を描画 (16bit)
------------------------------------------------------*/

static void draw_boxshadow16(UINT16 *vptr, int w, int h, int no)
{
	int x, y;
	int dst_r, dst_g, dst_b, a;
	UINT16 *dst = vptr, color;

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			if (x & 1)
				a = shadow[no][y][x >> 1] >> 4;
			else
				a = shadow[no][y][x >> 1] & 0x0f;

			if (a)
			{
				color = dst[x];

				dst_r = GETR16(color);
				dst_g = GETG16(color);
				dst_b = GETB16(color);

				dst_r = alpha_blend[a][dst_r >> 2][dst_r];
				dst_g = alpha_blend[a][dst_g >> 2][dst_g];
				dst_b = alpha_blend[a][dst_b >> 2][dst_b];

				dst[x] = MAKECOL16(dst_r, dst_g, dst_b);
			}
		}
		dst += BUF_WIDTH;
	}
}


static void fill_boxshadow16(UINT16 *vptr, int w, int h)
{
	int x, y;
	int dst_r, dst_g, dst_b;
	UINT16 *dst = vptr, color;

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			color = dst[x];

			dst_r = GETR16(color);
			dst_g = GETG16(color);
			dst_b = GETB16(color);

			dst_r = alpha_blend[15][dst_r >> 2][dst_r];
			dst_g = alpha_blend[15][dst_g >> 2][dst_g];
			dst_b = alpha_blend[15][dst_b >> 2][dst_b];

			dst[x] = MAKECOL16(dst_r, dst_g, dst_b);
		}
		dst += BUF_WIDTH;
	}
}


static void draw_box_shadow16(int sx, int sy, int ex, int ey)
{
	int i, width, height;
	int w, h, nw, nh, sx2, sy2;
	UINT16 *vptr;

	width = (ex - sx) + 1;
	height = (ey - sy) + 1;

	width  -= 14;
	height -= 14;

	nw = width / 8;
	nh = height / 8;

	w = width % 8;
	h = height % 8;

	/*----------------------------------*/
	// top
	/*----------------------------------*/
	sx2 = sx + 2;
	sy2 = sy + 2;
	vptr = vram_pos16(sx2, sy2);

	// left top
	draw_boxshadow16(vptr, 8, 8, 0);
	vptr += 8;

	// top
	for (i = 0; i < nw; i++)
	{
		draw_boxshadow16(vptr, 8, 8, 1);
		vptr += 8;
	}
	draw_boxshadow16(vptr, w, 8, 1);
	vptr += w;

	// right top
	draw_boxshadow16(vptr, 8, 8, 2);

	/*----------------------------------*/
	// left
	/*----------------------------------*/
	sx2 = sx + 2;
	sy2 = sy + 10;
	vptr = vram_pos16(sx2, sy2);

	for (i = 0; i < nh; i++)
	{
		draw_boxshadow16(vptr, 8, 8, 3);
		vptr += 8 * BUF_WIDTH;
	}
	draw_boxshadow16(vptr, 8, h, 3);

	/*----------------------------------*/
	// right
	/*----------------------------------*/
	sx2 = sx + 10 + width;
	sy2 = sy + 10;
	vptr = vram_pos16(sx2, sy2);

	for (i = 0; i < nh; i++)
	{
		draw_boxshadow16(vptr, 8, 8, 5);
		vptr += 8 * BUF_WIDTH;
	}
	draw_boxshadow16(vptr, 8, h, 5);

	/*----------------------------------*/
	// bottom
	/*----------------------------------*/
	sx2 = sx + 2;
	sy2 = sy + 10 + height;
	vptr = vram_pos16(sx2, sy2);

	// left bottom
	draw_boxshadow16(vptr, 8, 8, 6);
	vptr += 8;

	// bottom
	for (i = 0; i < nw; i++)
	{
		draw_boxshadow16(vptr, 8, 8, 7);
		vptr += 8;
	}
	draw_boxshadow16(vptr, w, 8, 7);
	vptr += w;

	// right bottom
	draw_boxshadow16(vptr, 8, 8, 8);

	vptr = vram_pos16(sx + 10, sy + 10);
	fill_boxshadow16(vptr, width, height);
}


/*------------------------------------------------------
	上部バーの影を描画 (16bit)
------------------------------------------------------*/

static void draw_bar_shadow16(void)
{
	int i;
	UINT16 *vptr = vram_pos16(0, 20);

	for (i = 0; i < SCR_WIDTH / 8; i++)
	{
		draw_boxshadow16(vptr, 8, 8, 7);
		vptr += 8;
	}

	vptr = vram_pos16(0, 0);
	fill_boxshadow16(vptr, SCR_WIDTH, 20);
}


/*------------------------------------------------------
	矩形の影を描画
------------------------------------------------------*/

void draw_box_shadow(int sx, int sy, int ex, int ey)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		draw_box_shadow32(sx, sy, ex, ey);
	else
#endif
		draw_box_shadow16(sx, sy, ex, ey);
}


/*------------------------------------------------------
	上部バーの影を描画
------------------------------------------------------*/

void draw_bar_shadow(void)
{
#if PSP_VIDEO_32BPP
	if (video_mode == 32)
		draw_bar_shadow32();
	else
#endif
		draw_bar_shadow16();
}


#if PSP_VIDEO_32BPP

/*******************************************************
	ユーザインタフェース色設定
*******************************************************/

/*------------------------------------------------------
	UI_PALETTE取得
------------------------------------------------------*/

void get_ui_color(UI_PALETTE *pal, int *r, int *g, int *b)
{
	*r = pal->r;
	*g = pal->g;
	*b = pal->b;
}


/*------------------------------------------------------
	UI_PALETTE設定
------------------------------------------------------*/

void set_ui_color(UI_PALETTE *pal, int r, int g, int b)
{
	pal->r = r;
	pal->g = g;
	pal->b = b;
}

/******************************************************************************
	ロゴ描画
******************************************************************************/

#include "psp/font/logo.c"

/*------------------------------------------------------
	ロゴ描画
------------------------------------------------------*/

void logo(int sx, int sy, int r, int g, int b)
{
	int x, y, dst_r, dst_g, dst_b, alpha;
	UINT32 color, *dst = vram_pos32(sx, sy);

	for (y = 0; y < 14; y++)
	{
#if (EMU_SYSTEM == MVS)
		for (x = 0; x < 208; x++)
#else
		for (x = 0; x < 232; x++)
#endif
		{
			if (x & 1)
				alpha = logo_data[y][x >> 1] >> 4;
			else
				alpha = logo_data[y][x >> 1] & 0x0f;

			if (alpha)
			{
				color = dst[x];

				dst_r = GETR32(color);
				dst_g = GETG32(color);
				dst_b = GETB32(color);

				dst_r = alpha_blend[alpha][r][dst_r];
				dst_g = alpha_blend[alpha][g][dst_g];
				dst_b = alpha_blend[alpha][b][dst_b];

				dst[x] = MAKECOL32(dst_r, dst_g, dst_b);
			}
		}
		dst += BUF_WIDTH;
	}
}

#endif /* PSP_VIDEO_32BPP */
