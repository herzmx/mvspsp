/******************************************************************************

	video.c

	PSPビデオ制御関数

******************************************************************************/

#include "psp.h"


#if PSP_VIDEO_32BPP

/******************************************************************************
	グローバル変数/構造体
******************************************************************************/

UINT8 ALIGN_PSPDATA gulist[GULIST_SIZE];
int video_mode = 0;
void *show_frame;
void *draw_frame;
void *work_frame;
void *tex_frame;

RECT full_rect = { 0, 0, SCR_WIDTH, SCR_HEIGHT };


/******************************************************************************
	プロトタイプ
******************************************************************************/

void (*video_init)(void);
void *(*video_frame_addr)(void *frame, int x, int y);
void (*video_clear_screen)(void);
void (*video_clear_frame)(void *frame);
void (*video_clear_rect)(void *frame, RECT *rect);
void (*video_fill_rect)(void *frame, UINT32 color, RECT *rect);
void (*video_fill_frame)(void *frame, UINT32 color);
void (*video_copy_rect)(void *src, void *dst, RECT *src_rect, RECT *dst_rect);
void (*video_copy_rect_flip)(void *src, void *dst, RECT *src_rect, RECT *dst_rect);
void (*video_copy_rect_rotate)(void *src, void *dst, RECT *src_rect, RECT *dst_rect);

static void video_init16(void);
static void *video_frame_addr16(void *frame, int x, int y);
static void video_clear_screen16(void);
static void video_clear_frame16(void *frame);
static void video_clear_rect16(void *frame, RECT *rect);
static void video_fill_rect16(void *frame, UINT32 color, RECT *rect);
static void video_fill_frame16(void *frame, UINT32 color);
static void video_copy_rect16(void *src, void *dst, RECT *src_rect, RECT *dst_rect);
static void video_copy_rect_flip16(void *src, void *dst, RECT *src_rect, RECT *dst_rect);
static void video_copy_rect_rotate16(void *src, void *dst, RECT *src_rect, RECT *dst_rect);

static void video_init32(void);
static void *video_frame_addr32(void *frame, int x, int y);
static void video_clear_screen32(void);
static void video_clear_frame32(void *frame);
static void video_clear_rect32(void *frame, RECT *rect);
static void video_fill_rect32(void *frame, UINT32 color, RECT *rect);
static void video_fill_frame32(void *frame, UINT32 color);
static void video_copy_rect32(void *src, void *dst, RECT *src_rect, RECT *dst_rect);
static void video_copy_rect_flip32(void *src, void *dst, RECT *src_rect, RECT *dst_rect);
static void video_copy_rect_rotate32(void *src, void *dst, RECT *src_rect, RECT *dst_rect);


/******************************************************************************
	ローカル関数
******************************************************************************/

/*--------------------------------------------------------
	VRAMクリア
--------------------------------------------------------*/

static void clear_vram(int flag)
{
	int i;
	UINT32 *vptr = (UINT32 *)(UINT8 *)0x44000000;

	if (flag)
		i = 2048 * 1024 >> 2;	// clear all VRAM
	else
		i = (FRAMESIZE32 * 3) >> 2;	// do not clear texture frame

	while (i--) *vptr++ = 0;
}


/******************************************************************************
	グローバル関数
******************************************************************************/

/*--------------------------------------------------------
	ビデオモード設定
--------------------------------------------------------*/

void video_set_mode(int mode)
{
	if (video_mode != mode)
	{
		if (video_mode)
		{
			video_clear_screen();
			video_exit(0);
		}

		video_mode = mode;

		if (video_mode == 32)
		{
			video_init             = video_init32;
			video_frame_addr       = video_frame_addr32;
			video_clear_screen     = video_clear_screen32;
			video_clear_frame      = video_clear_frame32;
			video_clear_rect       = video_clear_rect32;
			video_fill_frame       = video_fill_frame32;
			video_fill_rect        = video_fill_rect32;
			video_copy_rect        = video_copy_rect32;
			video_copy_rect_flip   = video_copy_rect_flip32;
			video_copy_rect_rotate = video_copy_rect_rotate32;
		}
		else
		{
			video_init             = video_init16;
			video_frame_addr       = video_frame_addr16;
			video_clear_screen     = video_clear_screen16;
			video_clear_frame      = video_clear_frame16;
			video_clear_rect       = video_clear_rect16;
			video_fill_frame       = video_fill_frame16;
			video_fill_rect        = video_fill_rect16;
			video_copy_rect        = video_copy_rect16;
			video_copy_rect_flip   = video_copy_rect_flip16;
			video_copy_rect_rotate = video_copy_rect_rotate16;
		}

		video_init();
	}
}


/*--------------------------------------------------------
	ビデオ処理終了(共通)
--------------------------------------------------------*/

void video_exit(int flag)
{
	sceGuDisplay(GU_FALSE);
	sceGuTerm();
	clear_vram(flag);
}


/*--------------------------------------------------------
	VSYNCを待つ
--------------------------------------------------------*/

void video_wait_vsync(void)
{
	sceDisplayWaitVblankStart();
}


/*--------------------------------------------------------
	スクリーンをフリップ
--------------------------------------------------------*/

void video_flip_screen(int vsync)
{
	if (vsync) sceDisplayWaitVblankStart();
	show_frame = draw_frame;
	draw_frame = sceGuSwapBuffers();
}


/******************************************************************************
	16bitカラー ビデオ関数
******************************************************************************/

/*--------------------------------------------------------
	ビデオ処理初期化
--------------------------------------------------------*/

static void video_init16(void)
{
	sceDisplaySetMode(PSP_DISPLAY_PIXEL_FORMAT_5551, SCR_WIDTH, SCR_HEIGHT);
	sceDisplaySetFrameBuf((void *)0x4000000, BUF_WIDTH, PSP_DISPLAY_PIXEL_FORMAT_5551, PSP_DISPLAY_SETBUF_IMMEDIATE);

	sceGuInit();
	sceGuDisplay(GU_FALSE);

	draw_frame = sceGuSwapBuffers();

	if ((UINT32)draw_frame == 0)
		show_frame = (void *)(FRAMESIZE * 1);
	else
		show_frame = (void *)(FRAMESIZE * 0);
	work_frame = (void *)(FRAMESIZE * 2);
	tex_frame  = (void *)(FRAMESIZE * 3);

	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBuffer(GU_PSM_5551, draw_frame, BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, show_frame, BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH / 2), 2048 - (SCR_HEIGHT / 2));
	sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);

	sceGuEnable(GU_SCISSOR_TEST);
	sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);

	sceGuEnable(GU_ALPHA_TEST);
	sceGuAlphaFunc(GU_LEQUAL, 0, 0x01);

	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthRange(65535, 0);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDepthMask(GU_TRUE);

	sceGuEnable(GU_TEXTURE_2D);
	sceGuTexMode(GU_PSM_5551, 0, 0, GU_FALSE);
	sceGuTexScale(1.0f / BUF_WIDTH, 1.0f / BUF_WIDTH);
	sceGuTexOffset(0, 0);
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuTexFlush();

	sceGuClutMode(GU_PSM_5551, 0, 0xff, 0);

	sceGuClearDepth(0);
	sceGuClearColor(0);

	sceGuFinish();
	sceGuSync(0, 0);

	sceGuDisplay(GU_TRUE);

	video_clear_screen();
	create_small_font();
}


/*--------------------------------------------------------
	VRAMのアドレスを取得
--------------------------------------------------------*/

static void *video_frame_addr16(void *frame, int x, int y)
{
	return (void *)(((UINT32)frame | 0x44000000) + ((x + (y << 9)) << 1));
}


/*--------------------------------------------------------
	描画/表示フレームをクリア
--------------------------------------------------------*/

static void video_clear_screen16(void)
{
	video_clear_frame16(show_frame);
	video_clear_frame16(draw_frame);
}


/*--------------------------------------------------------
	指定したフレームをクリア
--------------------------------------------------------*/

static void video_clear_frame16(void *frame)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(0, 0, BUF_WIDTH, SCR_HEIGHT);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	指定した矩形範囲をクリア
--------------------------------------------------------*/

static void video_clear_rect16(void *frame, RECT *rect)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(rect->left, rect->top, rect->right, rect->bottom);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	指定したフレームを塗りつぶし
--------------------------------------------------------*/

static void video_fill_frame16(void *frame, UINT32 color)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);
	sceGuClearColor(color);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	指定した矩形範囲を塗りつぶし
--------------------------------------------------------*/

static void video_fill_rect16(void *frame, UINT32 color, RECT *rect)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(rect->left, rect->top, rect->right, rect->bottom);
	sceGuClearColor(color);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	矩形範囲をコピー
--------------------------------------------------------*/

static void video_copy_rect16(void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
	int j, sw, dw, sh, dh;
	struct Vertex *vertices;

	sw = src_rect->right - src_rect->left;
	dw = dst_rect->right - dst_rect->left;
	sh = src_rect->bottom - src_rect->top;
	dh = dst_rect->bottom - dst_rect->top;

	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBufferList(GU_PSM_5551, dst, BUF_WIDTH);
	sceGuScissor(dst_rect->left, dst_rect->top, dst_rect->right, dst_rect->bottom);
	sceGuDisable(GU_ALPHA_TEST);

	sceGuTexMode(GU_PSM_5551, 0, 0, GU_FALSE);
	sceGuTexImage(0, BUF_WIDTH, BUF_WIDTH, BUF_WIDTH, GU_FRAME_ADDR(src));
	if (sw == dw && sh == dh)
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);
	else
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);

	for (j = 0; (j + SLICE_SIZE) < sw; j = j + SLICE_SIZE)
	{
    	vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->left + j;
		vertices[0].v = src_rect->top;
		vertices[0].x = dst_rect->left + j * dw / sw;
		vertices[0].y = dst_rect->top;
		vertices[0].z = 0;

		vertices[1].u = src_rect->left + j + SLICE_SIZE;
		vertices[1].v = src_rect->bottom;
		vertices[1].x = dst_rect->left + (j + SLICE_SIZE) * dw / sw;
		vertices[1].y = dst_rect->bottom;
		vertices[1].z = 0;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	if (j < sw)
	{
		vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->left + j;
		vertices[0].v = src_rect->top;
		vertices[0].x = dst_rect->left + j * dw / sw;
		vertices[0].y = dst_rect->top;
		vertices[0].z = 0;

		vertices[1].u = src_rect->right;
		vertices[1].v = src_rect->bottom;
		vertices[1].x = dst_rect->right;
		vertices[1].y = dst_rect->bottom;
		vertices[1].z = 0;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	矩形範囲を左右反転してコピー
--------------------------------------------------------*/

static void video_copy_rect_flip16(void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
	INT16 j, sw, dw, sh, dh;
	struct Vertex *vertices;

	sw = src_rect->right - src_rect->left;
	dw = dst_rect->right - dst_rect->left;
	sh = src_rect->bottom - src_rect->top;
	dh = dst_rect->bottom - dst_rect->top;

	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBufferList(GU_PSM_5551, dst, BUF_WIDTH);
	sceGuScissor(dst_rect->left, dst_rect->top, dst_rect->right, dst_rect->bottom);
	sceGuDisable(GU_ALPHA_TEST);

	sceGuTexMode(GU_PSM_5551, 0, 0, GU_FALSE);
	sceGuTexImage(0, 512, 512, BUF_WIDTH, GU_FRAME_ADDR(src));
	if (sw == dw && sh == dh)
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);
	else
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);

	for (j = 0; (j + SLICE_SIZE) < sw; j = j + SLICE_SIZE)
	{
    	vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->left + j;
		vertices[0].v = src_rect->top;
		vertices[0].x = dst_rect->right - j * dw / sw;
		vertices[0].y = dst_rect->bottom;

		vertices[1].u = src_rect->left + j + SLICE_SIZE;
		vertices[1].v = src_rect->bottom;
		vertices[1].x = dst_rect->right - (j + SLICE_SIZE) * dw / sw;
		vertices[1].y = dst_rect->top;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	if (j < sw)
	{
		vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->left + j;
		vertices[0].v = src_rect->top;
		vertices[0].x = dst_rect->right - j * dw / sw;
		vertices[0].y = dst_rect->bottom;

		vertices[1].u = src_rect->right;
		vertices[1].v = src_rect->bottom;
		vertices[1].x = dst_rect->left;
		vertices[1].y = dst_rect->top;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	矩形範囲を270度回転してコピー
--------------------------------------------------------*/

static void video_copy_rect_rotate16(void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
	INT16 sw, dw, sh, dh;
	struct Vertex *vertices;

	sw = src_rect->right - src_rect->left;
	dw = dst_rect->right - dst_rect->left;
	sh = src_rect->bottom - src_rect->top;
	dh = dst_rect->bottom - dst_rect->top;

	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBufferList(GU_PSM_5551, dst, BUF_WIDTH);
	sceGuScissor(dst_rect->left, dst_rect->top, dst_rect->right, dst_rect->bottom);
	sceGuDisable(GU_ALPHA_TEST);

	sceGuTexMode(GU_PSM_5551, 0, 0, GU_FALSE);
	sceGuTexImage(0, 512, 512, BUF_WIDTH, GU_FRAME_ADDR(src));
	if (sw == dh && sh == dw)
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);
	else
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);

	vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

	vertices[0].u = src_rect->right;
	vertices[1].v = src_rect->top;
	vertices[0].x = dst_rect->right;
	vertices[0].y = dst_rect->top;

	vertices[1].u = src_rect->left;
	vertices[0].v = src_rect->bottom;
	vertices[1].x = dst_rect->left;
	vertices[1].y = dst_rect->bottom;

	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);

	sceGuFinish();
	sceGuSync(0, 0);
}


/******************************************************************************
	32bitカラー ビデオ関数
******************************************************************************/

/*--------------------------------------------------------
	ビデオ処理初期化
--------------------------------------------------------*/

static void video_init32(void)
{
	sceDisplaySetMode(PSP_DISPLAY_PIXEL_FORMAT_8888, SCR_WIDTH, SCR_HEIGHT);
	sceDisplaySetFrameBuf((void *)0x4000000, BUF_WIDTH, PSP_DISPLAY_PIXEL_FORMAT_8888, PSP_DISPLAY_SETBUF_IMMEDIATE);

	sceGuInit();
	sceGuDisplay(GU_FALSE);

	draw_frame = sceGuSwapBuffers();

	if ((UINT32)draw_frame == 0)
		show_frame = (void *)(FRAMESIZE32 * 1);
	else
		show_frame = (void *)(FRAMESIZE32 * 0);
	work_frame = (void *)(FRAMESIZE32 * 2);
	tex_frame  = (void *)(FRAMESIZE32 * 3);

	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBuffer(GU_PSM_8888, draw_frame, BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, show_frame, BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH / 2), 2048 - (SCR_HEIGHT / 2));
	sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);

	sceGuEnable(GU_SCISSOR_TEST);
	sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);

	sceGuDisable(GU_ALPHA_TEST);

	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthRange(65535, 0);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDepthMask(GU_TRUE);

	sceGuEnable(GU_TEXTURE_2D);
	sceGuTexMode(GU_PSM_8888, 0, 0, GU_FALSE);
	sceGuTexScale(1.0f / BUF_WIDTH, 1.0f / BUF_WIDTH);
	sceGuTexOffset(0, 0);
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuTexFlush();

	sceGuClearDepth(0);
	sceGuClearColor(0);

	sceGuFinish();
	sceGuSync(0, 0);

	sceGuDisplay(GU_TRUE);

	video_clear_screen();
}


/*--------------------------------------------------------
	VRAMのアドレスを取得
--------------------------------------------------------*/

static void *video_frame_addr32(void *frame, int x, int y)
{
	return (void *)(((UINT32)frame | 0x44000000) + ((x + (y << 9)) << 2));
}


/*--------------------------------------------------------
	描画/表示フレームをクリア
--------------------------------------------------------*/

static void video_clear_screen32(void)
{
	video_clear_frame32(show_frame);
	video_clear_frame32(draw_frame);
}


/*--------------------------------------------------------
	指定したフレームをクリア
--------------------------------------------------------*/

static void video_clear_frame32(void *frame)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_8888, frame, BUF_WIDTH);
	sceGuScissor(0, 0, BUF_WIDTH, SCR_HEIGHT);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	指定した矩形範囲をクリア
--------------------------------------------------------*/

static void video_clear_rect32(void *frame, RECT *rect)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_8888, frame, BUF_WIDTH);
	sceGuScissor(rect->left, rect->top, rect->right, rect->bottom);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	指定したフレームを塗りつぶし
--------------------------------------------------------*/

static void video_fill_frame32(void *frame, UINT32 color)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_8888, frame, BUF_WIDTH);
	sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);
	sceGuClearColor(color);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	指定した矩形範囲を塗りつぶし
--------------------------------------------------------*/

static void video_fill_rect32(void *frame, UINT32 color, RECT *rect)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_8888, frame, BUF_WIDTH);
	sceGuScissor(rect->left, rect->top, rect->right, rect->bottom);
	sceGuClearColor(color);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	矩形範囲をコピー
--------------------------------------------------------*/

static void video_copy_rect32(void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
	int j, sw, dw, sh, dh;
	struct Vertex *vertices;

	sw = src_rect->right - src_rect->left;
	dw = dst_rect->right - dst_rect->left;
	sh = src_rect->bottom - src_rect->top;
	dh = dst_rect->bottom - dst_rect->top;

	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBufferList(GU_PSM_8888, dst, BUF_WIDTH);
	sceGuScissor(dst_rect->left, dst_rect->top, dst_rect->right, dst_rect->bottom);

	sceGuTexMode(GU_PSM_8888, 0, 0, GU_FALSE);
	sceGuTexImage(0, BUF_WIDTH, BUF_WIDTH, BUF_WIDTH, GU_FRAME_ADDR(src));
	if (sw == dw && sh == dh)
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);
	else
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);

	for (j = 0; (j + SLICE_SIZE) < sw; j = j + SLICE_SIZE)
	{
    	vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->left + j;
		vertices[0].v = src_rect->top;
		vertices[0].x = dst_rect->left + j * dw / sw;
		vertices[0].y = dst_rect->top;
		vertices[0].z = 0;

		vertices[1].u = src_rect->left + j + SLICE_SIZE;
		vertices[1].v = src_rect->bottom;
		vertices[1].x = dst_rect->left + (j + SLICE_SIZE) * dw / sw;
		vertices[1].y = dst_rect->bottom;
		vertices[1].z = 0;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	if (j < sw)
	{
		vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->left + j;
		vertices[0].v = src_rect->top;
		vertices[0].x = dst_rect->left + j * dw / sw;
		vertices[0].y = dst_rect->top;
		vertices[0].z = 0;

		vertices[1].u = src_rect->right;
		vertices[1].v = src_rect->bottom;
		vertices[1].x = dst_rect->right;
		vertices[1].y = dst_rect->bottom;
		vertices[1].z = 0;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	矩形範囲を左右反転してコピー
--------------------------------------------------------*/

static void video_copy_rect_flip32(void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
}


/*--------------------------------------------------------
	矩形範囲を270度回転してコピー
--------------------------------------------------------*/

static void video_copy_rect_rotate32(void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
}

#else


/******************************************************************************
	グローバル変数/構造体
******************************************************************************/

UINT8 ALIGN_PSPDATA gulist[GULIST_SIZE];
void *show_frame;
void *draw_frame;
void *work_frame;
void *tex_frame;

RECT full_rect = { 0, 0, SCR_WIDTH, SCR_HEIGHT };


/******************************************************************************
	ローカル関数
******************************************************************************/

/*--------------------------------------------------------
	VRAMクリア
--------------------------------------------------------*/

static void clear_vram(int flag)
{
	int i;
	UINT32 *vptr = (UINT32 *)(UINT8 *)0x44000000;

	if (flag)
		i = 2048 * 1024 >> 2;	// clear all VRAM
	else
		i = (FRAMESIZE32 * 3) >> 2;	// do not clear texture frame

	while (i--) *vptr++ = 0;
}


/******************************************************************************
	グローバル関数
******************************************************************************/

/*--------------------------------------------------------
	VSYNCを待つ
--------------------------------------------------------*/

void video_wait_vsync(void)
{
	sceDisplayWaitVblankStart();
}


/*--------------------------------------------------------
	スクリーンをフリップ
--------------------------------------------------------*/

void video_flip_screen(int vsync)
{
	if (vsync) sceDisplayWaitVblankStart();
	show_frame = draw_frame;
	draw_frame = sceGuSwapBuffers();
}


/*--------------------------------------------------------
	ビデオ処理初期化
--------------------------------------------------------*/

void video_init(void)
{
	sceDisplaySetMode(PSP_DISPLAY_PIXEL_FORMAT_5551, SCR_WIDTH, SCR_HEIGHT);
	sceDisplaySetFrameBuf((void *)0x4000000, BUF_WIDTH, PSP_DISPLAY_PIXEL_FORMAT_5551, PSP_DISPLAY_SETBUF_IMMEDIATE);

	sceGuInit();
	sceGuDisplay(GU_FALSE);

	draw_frame = sceGuSwapBuffers();

	if ((UINT32)draw_frame == 0)
		show_frame = (void *)(FRAMESIZE * 1);
	else
		show_frame = (void *)(FRAMESIZE * 0);
	work_frame = (void *)(FRAMESIZE * 2);
	tex_frame  = (void *)(FRAMESIZE * 3);

	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBuffer(GU_PSM_5551, draw_frame, BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, show_frame, BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH / 2), 2048 - (SCR_HEIGHT / 2));
	sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);

	sceGuEnable(GU_SCISSOR_TEST);
	sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);

	sceGuDisable(GU_ALPHA_TEST);
	sceGuAlphaFunc(GU_LEQUAL, 0, 1);

	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthRange(65535, 0);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDepthMask(GU_TRUE);

	sceGuEnable(GU_TEXTURE_2D);
	sceGuTexMode(GU_PSM_5551, 0, 0, GU_FALSE);
	sceGuTexScale(1.0f / BUF_WIDTH, 1.0f / BUF_WIDTH);
	sceGuTexOffset(0, 0);
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuTexFlush();

	sceGuClutMode(GU_PSM_5551, 0, 0xff, 0);

	sceGuClearDepth(0);
	sceGuClearColor(0);
	sceGuClear(GU_COLOR_BUFFER_BIT);

	sceGuFinish();
	sceGuSync(0, 0);

	video_flip_screen(1);

	sceGuDisplay(GU_TRUE);

	clear_vram(0);

	create_small_font();
}


/*--------------------------------------------------------
	ビデオ処理終了
--------------------------------------------------------*/

void video_exit(int flag)
{
	sceGuDisplay(GU_FALSE);
	sceGuTerm();
	clear_vram(flag);
}


/*--------------------------------------------------------
	VRAMのアドレスを取得
--------------------------------------------------------*/

void *video_frame_addr(void *frame, int x, int y)
{
	return (void *)(((UINT32)frame | 0x44000000) + ((x + (y << 9)) << 1));
}


/*--------------------------------------------------------
	描画/表示フレームをクリア
--------------------------------------------------------*/

void video_clear_screen(void)
{
	video_clear_frame(show_frame);
	video_clear_frame(draw_frame);
}


/*--------------------------------------------------------
	指定したフレームをクリア
--------------------------------------------------------*/

void video_clear_frame(void *frame)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(0, 0, BUF_WIDTH, SCR_HEIGHT);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	指定した矩形範囲をクリア
--------------------------------------------------------*/

void video_clear_rect(void *frame, RECT *rect)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(rect->left, rect->top, rect->right, rect->bottom);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	指定したフレームを塗りつぶし
--------------------------------------------------------*/

void video_fill_frame(void *frame, UINT32 color)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(0, 0, BUF_WIDTH, SCR_HEIGHT);
	sceGuClearColor(color);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuClearColor(0);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	指定した矩形範囲を塗りつぶし
--------------------------------------------------------*/

void video_fill_rect(void *frame, UINT32 color, RECT *rect)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(rect->left, rect->top, rect->right, rect->bottom);
	sceGuClearColor(color);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuClearColor(0);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	指定したDepthBufferをクリア
--------------------------------------------------------*/

void video_clear_depth(void *frame)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDepthBuffer(frame, BUF_WIDTH);
	sceGuScissor(0, 0, BUF_WIDTH, SCR_HEIGHT);
	sceGuClear(GU_DEPTH_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	矩形範囲をコピー
--------------------------------------------------------*/

void video_copy_rect(void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
	INT16 j, sw, dw, sh, dh;
	struct Vertex *vertices;

	sw = src_rect->right - src_rect->left;
	dw = dst_rect->right - dst_rect->left;
	sh = src_rect->bottom - src_rect->top;
	dh = dst_rect->bottom - dst_rect->top;

	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBufferList(GU_PSM_5551, dst, BUF_WIDTH);
	sceGuScissor(dst_rect->left, dst_rect->top, dst_rect->right, dst_rect->bottom);
	sceGuDisable(GU_ALPHA_TEST);

	sceGuTexMode(GU_PSM_5551, 0, 0, GU_FALSE);
	sceGuTexImage(0, 512, 512, BUF_WIDTH, GU_FRAME_ADDR(src));
	if (sw == dw && sh == dh)
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);
	else
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);

	for (j = 0; (j + SLICE_SIZE) < sw; j = j + SLICE_SIZE)
	{
    	vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->left + j;
		vertices[0].v = src_rect->top;
		vertices[0].x = dst_rect->left + j * dw / sw;
		vertices[0].y = dst_rect->top;

		vertices[1].u = src_rect->left + j + SLICE_SIZE;
		vertices[1].v = src_rect->bottom;
		vertices[1].x = dst_rect->left + (j + SLICE_SIZE) * dw / sw;
		vertices[1].y = dst_rect->bottom;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	if (j < sw)
	{
		vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->left + j;
		vertices[0].v = src_rect->top;
		vertices[0].x = dst_rect->left + j * dw / sw;
		vertices[0].y = dst_rect->top;

		vertices[1].u = src_rect->right;
		vertices[1].v = src_rect->bottom;
		vertices[1].x = dst_rect->right;
		vertices[1].y = dst_rect->bottom;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	矩形範囲を左右反転してコピー
--------------------------------------------------------*/

void video_copy_rect_flip(void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
	INT16 j, sw, dw, sh, dh;
	struct Vertex *vertices;

	sw = src_rect->right - src_rect->left;
	dw = dst_rect->right - dst_rect->left;
	sh = src_rect->bottom - src_rect->top;
	dh = dst_rect->bottom - dst_rect->top;

	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBufferList(GU_PSM_5551, dst, BUF_WIDTH);
	sceGuScissor(dst_rect->left, dst_rect->top, dst_rect->right, dst_rect->bottom);
	sceGuDisable(GU_ALPHA_TEST);

	sceGuTexMode(GU_PSM_5551, 0, 0, GU_FALSE);
	sceGuTexImage(0, 512, 512, BUF_WIDTH, GU_FRAME_ADDR(src));
	if (sw == dw && sh == dh)
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);
	else
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);

	for (j = 0; (j + SLICE_SIZE) < sw; j = j + SLICE_SIZE)
	{
    	vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->left + j;
		vertices[0].v = src_rect->top;
		vertices[0].x = dst_rect->right - j * dw / sw;
		vertices[0].y = dst_rect->bottom;

		vertices[1].u = src_rect->left + j + SLICE_SIZE;
		vertices[1].v = src_rect->bottom;
		vertices[1].x = dst_rect->right - (j + SLICE_SIZE) * dw / sw;
		vertices[1].y = dst_rect->top;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	if (j < sw)
	{
		vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->left + j;
		vertices[0].v = src_rect->top;
		vertices[0].x = dst_rect->right - j * dw / sw;
		vertices[0].y = dst_rect->bottom;

		vertices[1].u = src_rect->right;
		vertices[1].v = src_rect->bottom;
		vertices[1].x = dst_rect->left;
		vertices[1].y = dst_rect->top;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	sceGuFinish();
	sceGuSync(0, 0);
}


/*--------------------------------------------------------
	矩形範囲を270度回転してコピー
--------------------------------------------------------*/

void video_copy_rect_rotate(void *src, void *dst, RECT *src_rect, RECT *dst_rect)
{
	INT16 sw, dw, sh, dh;
	struct Vertex *vertices;

	sw = src_rect->right - src_rect->left;
	dw = dst_rect->right - dst_rect->left;
	sh = src_rect->bottom - src_rect->top;
	dh = dst_rect->bottom - dst_rect->top;

	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBufferList(GU_PSM_5551, dst, BUF_WIDTH);
	sceGuScissor(dst_rect->left, dst_rect->top, dst_rect->right, dst_rect->bottom);
	sceGuDisable(GU_ALPHA_TEST);

	sceGuTexMode(GU_PSM_5551, 0, 0, GU_FALSE);
	sceGuTexImage(0, 512, 512, BUF_WIDTH, GU_FRAME_ADDR(src));
	if (sw == dh && sh == dw)
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);
	else
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);

	vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

	vertices[0].u = src_rect->right;
	vertices[1].v = src_rect->top;
	vertices[0].x = dst_rect->right;
	vertices[0].y = dst_rect->top;

	vertices[1].u = src_rect->left;
	vertices[0].v = src_rect->bottom;
	vertices[1].x = dst_rect->left;
	vertices[1].y = dst_rect->bottom;

	sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);

	sceGuFinish();
	sceGuSync(0, 0);
}

#endif /* PSP_VIDEO_32BPP */
