/******************************************************************************

	state.c

	ステートセーブ/ロード

******************************************************************************/

#ifdef SAVE_STATE

#include "emumain.h"
#include "zlib/zlib.h"
#include <time.h>


/******************************************************************************
	グローバル変数
******************************************************************************/

char date_str[16];
char time_str[16];
char stver_str[16];
int state_version;
UINT8 *state_buffer;
int current_state_version;
#if (EMU_SYSTEM == MVS)
int  state_reload_bios;
#endif


/******************************************************************************
	ローカル変数
******************************************************************************/

#if (EMU_SYSTEM == CPS1)
static const char *current_version_str = "CPS1SV09";
#elif (EMU_SYSTEM == CPS2)
static const char *current_version_str = "CPS2SV08";
#elif (EMU_SYSTEM == MVS)
static const char *current_version_str = "MVSSV010";
#elif (EMU_SYSTEM == NCDZ)
static const char *current_version_str = "NCDZSV08";
#endif


/******************************************************************************
	ローカル関数
******************************************************************************/

/*------------------------------------------------------
	サムネイルをワーク領域からファイルに保存
------------------------------------------------------*/

static void save_thumbnail(void)
{
	int x, y, w, h;
#if PSP_VIDEO_32BPP
	UINT32 *src = (UINT32 *)video_frame_addr(tex_frame, 152, 0);
	UINT16 data;
#else
	UINT16 *src = (UINT16 *)video_frame_addr(tex_frame, 152, 0);
#endif

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
	if (machine_screen_type)
	{
		w = 112;
		h = 152;
	}
	else
#endif
	{
		w = 152;
		h = 112;
	}

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
#if PSP_VIDEO_32BPP
			data = CNVCOL32TO15(src[x]);
			state_save_word(&data, 1);
#else
			state_save_word(&src[x], 1);
#endif
		}
		src += BUF_WIDTH;
	}
}


/*------------------------------------------------------
	サムネイルをファイルからワーク領域に読み込み
------------------------------------------------------*/

static void load_thumbnail(FILE *fp)
{
	int x, y, w, h;
#if PSP_VIDEO_32BPP
	UINT32 *dst = (UINT32 *)video_frame_addr(tex_frame, 0, 0);
	UINT16 data;
#else
	UINT16 *dst = (UINT16 *)video_frame_addr(tex_frame, 0, 0);
#endif

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
	if (machine_screen_type)
	{
		w = 112;
		h = 152;
	}
	else
#endif
	{
		w = 152;
		h = 112;
	}

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
#if PSP_VIDEO_32BPP
#if (EMU_SYSTEM == NCDZ)
			fread(&data, 1, 2, fp);
			dst[x] = CNVCOL15TO32(data);
#else
			state_load_word(&data, 1);
			dst[x] = CNVCOL15TO32(data);
#endif
#else
#if (EMU_SYSTEM == NCDZ)
			fread(&dst[x], 1, 2, fp);
#else
			state_load_word(&dst[x], 1);
#endif
#endif
		}
		dst += BUF_WIDTH;
	}
}


/*------------------------------------------------------
	ワーク領域のサムネイルをクリア
------------------------------------------------------*/

static void clear_thumbnail(void)
{
	int x, y, w, h;
#if PSP_VIDEO_32BPP
	UINT32 *dst = (UINT32 *)video_frame_addr(tex_frame, 0, 0);
#else
	UINT16 *dst = (UINT16 *)video_frame_addr(tex_frame, 0, 0);
#endif

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
	if (machine_screen_type)
	{
		w = 112;
		h = 152;
	}
	else
#endif
	{
		w = 152;
		h = 112;
	}

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			dst[x] = 0;
		}
		dst += BUF_WIDTH;
	}
}


/******************************************************************************
	ステートセーブ/ロード関数
******************************************************************************/

/*------------------------------------------------------
	ステートセーブ
------------------------------------------------------*/

int state_save(int slot)
{
	SceUID fd = -1;
	pspTime nowtime;
	char path[MAX_PATH];
	char error_mes[128];
	char buf[128];
#if (EMU_SYSTEM == NCDZ)
	UINT8 *inbuf, *outbuf;
	unsigned long insize, outsize;
#else
	UINT8 *state_buffer_base;
	UINT32 size;
#endif

	sprintf(path, "%sstate/%s.sv%d", launchDir, game_name, slot);
	sceIoRemove(path);

	sprintf(buf, TEXT(STATE_SAVING), game_name, slot);
	init_progress(6, buf);

	sceRtcGetCurrentClockLocalTime(&nowtime);

	if ((fd = sceIoOpen(path, PSP_O_WRONLY|PSP_O_CREAT, 0777)) >= 0)
#if (EMU_SYSTEM == NCDZ)
	{
		if ((inbuf = memalign(MEM_ALIGN, 3*1024*1024)) == NULL)
		{
			strcpy(error_mes, TEXT(COULD_NOT_ALLOCATE_STATE_BUFFER));
			goto error;
		}
		memset(inbuf, 0, 3*1024*1024);
		state_buffer = inbuf;

		state_save_byte(current_version_str, 8);
		state_save_byte(&nowtime, 16);
		update_progress();

		save_thumbnail();
		update_progress();

		sceIoWrite(fd, inbuf, (UINT32)state_buffer - (UINT32)inbuf);
		update_progress();

		memset(inbuf, 0, 3*1024*1024);
		state_buffer = inbuf;

		state_save_memory();
		state_save_m68000();
		state_save_z80();
		state_save_input();
		state_save_timer();
		state_save_driver();
		state_save_video();
		state_save_ym2610();
		state_save_cdda();
		state_save_cdrom();
		update_progress();

		insize = (UINT32)state_buffer - (UINT32)inbuf;
		outsize = insize * 1.1 + 12;
		if ((outbuf = memalign(MEM_ALIGN, outsize)) == NULL)
		{
			strcpy(error_mes, TEXT(COULD_NOT_ALLOCATE_STATE_BUFFER));
			free(inbuf);
			goto error;
		}
		memset(outbuf, 0, outsize);

		if (compress(outbuf, &outsize, inbuf, insize) != Z_OK)
		{
			strcpy(error_mes, TEXT(COULD_NOT_COMPRESS_STATE_DATA));
			free(inbuf);
			free(outbuf);
			goto error;
		}
		free(inbuf);
		update_progress();

		sceIoWrite(fd, &outsize, 4);
		sceIoWrite(fd, outbuf, outsize);
		sceIoClose(fd);
		free(outbuf);
		update_progress();

		show_progress(buf);
		return 1;
	}
#else
	{
#if (EMU_SYSTEM == CPS1 || (EMU_SYSTEM == CPS2 && defined(PSP_SLIM)))
		state_buffer_base = state_buffer = memalign(MEM_ALIGN, STATE_BUFFER_SIZE);
#else
		state_buffer_base = state_buffer = cache_alloc_state_buffer(STATE_BUFFER_SIZE);
#endif
		if (!state_buffer)
		{
			strcpy(error_mes, TEXT(COULD_NOT_ALLOCATE_STATE_BUFFER));
			goto error;
		}
		memset(state_buffer, 0, STATE_BUFFER_SIZE);
		update_progress();

		state_save_byte(current_version_str, 8);
		state_save_byte(&nowtime, 16);
		update_progress();

		save_thumbnail();
		update_progress();

		state_save_memory();
		state_save_m68000();
		state_save_z80();
		state_save_input();
		state_save_timer();
		state_save_driver();
		state_save_video();
#if (EMU_SYSTEM == CPS1)
		state_save_coin();
		switch (machine_driver_type)
		{
		case MACHINE_qsound:
			state_save_qsound();
			state_save_eeprom();
			break;

		case MACHINE_pang3:
			state_save_eeprom();

		default:
			state_save_ym2151();
			break;
		}
#elif (EMU_SYSTEM == CPS2)
		state_save_coin();
		state_save_qsound();
		state_save_eeprom();
#elif (EMU_SYSTEM == MVS)
		state_save_ym2610();
		state_save_pd4990a();
#endif
		update_progress();

		size = (UINT32)state_buffer - (UINT32)state_buffer_base;
		sceIoWrite(fd, state_buffer_base, size);
		sceIoClose(fd);
		update_progress();

#if (EMU_SYSTEM == CPS1 || (EMU_SYSTEM == CPS2 && defined(PSP_SLIM)))
		free(state_buffer_base);
#else
		cache_free_state_buffer(STATE_BUFFER_SIZE);
#endif
		update_progress();

		show_progress(buf);
		return 1;
	}
#endif
	else
	{
		sprintf(error_mes, TEXT(COULD_NOT_CREATE_STATE_FILE), game_name, slot);
	}

error:
	if (fd >= 0)
	{
		sceIoClose(fd);
		sceIoRemove(path);
	}
	show_progress(error_mes);
	pad_wait_press(PAD_WAIT_INFINITY);

	return 0;
}


/*------------------------------------------------------
	ステートロード
------------------------------------------------------*/

int state_load(int slot)
{
	FILE *fp;
	char path[MAX_PATH];
	char error_mes[128];
	char buf[128];
#if (EMU_SYSTEM == NCDZ)
	UINT8 *inbuf, *outbuf;
	unsigned long insize, outsize;
#endif

	sprintf(path, "%sstate/%s.sv%d", launchDir, game_name, slot);

#if (EMU_SYSTEM == MVS)
	state_reload_bios = 0;
#endif

	sprintf(buf, TEXT(STATE_LOADING), game_name, slot);
#if (EMU_SYSTEM == NCDZ)
	init_progress(6, buf);
#else
	init_progress(4, buf);
#endif

#if (EMU_SYSTEM == NCDZ)
	if ((fp = fopen(path, "rb")) != NULL)
	{
		fseek(fp, (8+16) + (152*112*2), SEEK_SET);
		update_progress();

		fread(&insize, 1, 4, fp);
		if ((inbuf = memalign(MEM_ALIGN, insize)) == NULL)
		{
			strcpy(error_mes, TEXT(COULD_NOT_ALLOCATE_STATE_BUFFER));
			fclose(fp);
			goto error;
		}
		memset(inbuf, 0, insize);
		update_progress();

		fread(inbuf, 1, insize, fp);
		fclose(fp);
		update_progress();

		outsize = 3*1024*1024;
		if ((outbuf = memalign(MEM_ALIGN, outsize)) == NULL)
		{
			strcpy(error_mes, TEXT(COULD_NOT_ALLOCATE_STATE_BUFFER));
			free(inbuf);
			goto error;
		}
		memset(outbuf, 0, outsize);

		if (uncompress(outbuf, &outsize, inbuf, insize) != Z_OK)
		{
			strcpy(error_mes, TEXT(COULD_NOT_UNCOMPRESS_STATE_DATA));
			free(inbuf);
			free(outbuf);
			goto error;
		}
		free(inbuf);
		update_progress();

		state_buffer = outbuf;

		state_load_memory();
		state_load_m68000();
		state_load_z80();
		state_load_input();
		state_load_timer();
		state_load_driver();
		state_load_video();
		state_load_ym2610();
		state_load_cdda();
		state_load_cdrom();
		update_progress();

		free(outbuf);

		if (mp3_get_status() == MP3_SEEK)
		{
			mp3_seek_start();

			while (mp3_get_status() == MP3_SEEK)
				video_wait_vsync();
		}
		update_progress();

		show_progress(buf);
		return 1;
	}
#else
	if ((fp = fopen(path, "rb")) != NULL)
	{
		state_load_skip((8+16));
		update_progress();

		state_load_skip((152*112*2));
		update_progress();

		state_load_memory(fp);
		state_load_m68000(fp);
		state_load_z80(fp);
		state_load_input(fp);
		state_load_timer(fp);
		state_load_driver(fp);
		state_load_video(fp);
#if (EMU_SYSTEM == CPS1)

		state_load_coin(fp);
		switch (machine_driver_type)
		{
		case MACHINE_qsound:
			state_load_qsound(fp);
			state_load_eeprom(fp);
			break;

		case MACHINE_pang3:
			state_load_eeprom(fp);

		default:
			state_load_ym2151(fp);
			break;
		}
		fclose(fp);
#elif (EMU_SYSTEM == CPS2)
		state_load_coin(fp);
		state_load_qsound(fp);
		state_load_eeprom(fp);
		fclose(fp);
#elif (EMU_SYSTEM == MVS)
		state_load_ym2610(fp);
		state_load_pd4990a(fp);
		fclose(fp);

		if (state_reload_bios)
		{
			state_reload_bios = 0;

			if (!reload_bios())
			{
				show_progress(TEXT(COULD_NOT_RELOAD_BIOS));
				pad_wait_press(PAD_WAIT_INFINITY);
				Loop = LOOP_BROWSER;
				return 0;
			}
		}
#endif
		update_progress();

		show_progress(buf);
		return 1;
	}
#endif
	else
	{
		sprintf(error_mes, TEXT(COULD_NOT_OPEN_STATE_FILE), game_name, slot);
	}

#if (EMU_SYSTEM == NCDZ)
error:
#endif
	show_progress(error_mes);
	pad_wait_press(PAD_WAIT_INFINITY);

	return 0;
}


/*------------------------------------------------------
	サムネイル作成
------------------------------------------------------*/

void state_make_thumbnail(void)
{
#if PSP_VIDEO_32BPP
	int x, y, w, h;
	UINT16 *p, buf[152 * 112];
	UINT16 *src = (UINT16 *)video_frame_addr(tex_frame, 152, 0);
	UINT32 *dst;

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
	RECT clip1 = { 64, 16, 64 + 384, 16 + 224 };
	RECT clip3 = { 0, 0, BUF_WIDTH, 208 };

	if (machine_screen_type)
	{
		RECT clip2 = { 152, 0, 152 + 112, 152 };

		w = 112;
		h = 152;

		video_copy_rect_rotate(work_frame, tex_frame, &clip1, &clip2);
	}
	else
	{
		RECT clip2 = { 152, 0, 152 + 152, 112 };

		w = 152;
		h = 112;

		video_copy_rect(work_frame, tex_frame, &clip1, &clip2);
	}
#elif (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
	RECT clip1 = { 8, 16, 8 + 304, 16 + 224 };
	RECT clip2 = { 152, 0, 152 + 152, 112 };
	RECT clip3 = { 0, 0, BUF_WIDTH, 208 };

	w = 152;
	h = 112;

	video_copy_rect(work_frame, tex_frame, &clip1, &clip2);
#endif

	p = buf;
	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			*p++ = src[x];
		}
		src += BUF_WIDTH;
	}

	video_set_mode(32);
	video_clear_rect(tex_frame, &clip3);

	src = buf;
	dst = (UINT32 *)video_frame_addr(tex_frame, 152, 0);

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			dst[x] = CNVCOL15TO32(*src);
			src++;
		}
		dst += BUF_WIDTH;
	}
#else

#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
	RECT clip1 = { 64, 16, 64 + 384, 16 + 224 };

	if (machine_screen_type)
	{
		RECT clip2 = { 152, 0, 152 + 112, 152 };
		video_copy_rect_rotate(work_frame, tex_frame, &clip1, &clip2);
	}
	else
	{
		RECT clip2 = { 152, 0, 152 + 152, 112 };
		video_copy_rect(work_frame, tex_frame, &clip1, &clip2);
	}
#elif (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
	RECT clip1 = { 8, 16, 8 + 304, 16 + 224 };
	RECT clip2 = { 152, 0, 152 + 152, 112 };

	video_copy_rect(work_frame, tex_frame, &clip1, &clip2);
#endif

#endif
}


/*------------------------------------------------------
	サムネイル読み込み
------------------------------------------------------*/

int state_load_thumbnail(int slot)
{
	FILE *fp;
	char path[MAX_PATH];

	clear_thumbnail();

	sprintf(path, "%sstate/%s.sv%d", launchDir, game_name, slot);

	if ((fp = fopen(path, "rb")) != NULL)
	{
		pspTime t;

		memset(stver_str, 0, 16);

		fread(stver_str, 1, 8, fp);
		fread(&t, 1, 16, fp);
		load_thumbnail(fp);
		fclose(fp);

		current_state_version = current_version_str[7] - '0';
		state_version = stver_str[7] - '0';

		sprintf(date_str, "%04d/%02d/%02d", t.year, t.month, t.day);
		sprintf(time_str, "%02d:%02d:%02d", t.hour, t.minutes, t.seconds);

		return 1;
	}

	ui_popup(TEXT(COULD_NOT_OPEN_STATE_FILE), game_name, slot);

	return 0;
}


/*------------------------------------------------------
	サムネイル消去
------------------------------------------------------*/

void state_clear_thumbnail(void)
{
	strcpy(date_str, "----/--/--");
	strcpy(time_str, "--:--:--");
	strcpy(stver_str, "--------");

	state_version = 0;

	clear_thumbnail();
}

#endif /* SAVE_STATE */
