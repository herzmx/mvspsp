/******************************************************************************

	memintrf.c

	NEOGEO CDメモリインタフェース関数

******************************************************************************/

#include "ncdz.h"
#include "zlib/zlib.h"

#define M68K_AMASK 0x00ffffff
#define Z80_AMASK 0x0000ffff

#define READ_BYTE(mem, offset)			mem[offset ^ 1]
#define READ_WORD(mem, offset)			*(UINT16 *)&mem[offset]
#define WRITE_BYTE(mem, offset, data)	mem[offset ^ 1] = data
#define WRITE_WORD(mem, offset, data)	*(UINT16 *)&mem[offset] = data

#define READ_MIRROR_BYTE(mem, offset, amask)			mem[(offset & amask) ^ 1]
#define READ_MIRROR_WORD(mem, offset, amask)			*(UINT16 *)&mem[offset & amask]
#define WRITE_MIRROR_BYTE(mem, offset, data, amask)		mem[(offset & amask) ^ 1] = data
#define WRITE_MIRROR_WORD(mem, offset, data, amask)		*(UINT16 *)&mem[offset & amask] = data

#define str_cmp(s1, s2)		strnicmp(s1, s2, strlen(s2))

enum
{
	REGION_CPU1 = 0,
	REGION_CPU2,
	REGION_GFX1,
	REGION_GFX2,
	REGION_SOUND1,
	REGION_USER1,
	REGION_SKIP
};


/******************************************************************************
	グローバル変数
******************************************************************************/

UINT8 *memory_region_cpu1;
UINT8 *memory_region_cpu2;
UINT8 *memory_region_gfx1;
UINT8 *memory_region_gfx2;
UINT8 *memory_region_sound1;
UINT8 *memory_region_user1;

UINT32 memory_length_cpu1;
UINT32 memory_length_cpu2;
UINT32 memory_length_gfx1;
UINT32 memory_length_gfx2;
UINT32 memory_length_sound1;
UINT32 memory_length_user1;

UINT8 neogeo_memcard[0x2000];


/******************************************************************************
	ROM読み込み
******************************************************************************/

/*--------------------------------------------------------
	メモリ確保
--------------------------------------------------------*/

static UINT8 *memory_allocate(int type, UINT32 length)
{
	UINT8 *mem;
	const char *region_name[6] =
	{
		"CPU1","CPU2","GFX1","GFX2","SOUND1","USER1"
	};

	if ((mem = memalign(MEM_ALIGN, length)) == NULL)
	{
		error_memory(region_name[type]);
		return NULL;
	}
	memset(mem, 0, length);

	return (UINT8 *)mem;
}

/*--------------------------------------------------------
	USER1 (M68000 BIOS ROM)
--------------------------------------------------------*/

static int load_rom_user1(void)
{
	FILE *fp;
	const char *bios_name = "neocd.bin";
	char path[MAX_PATH];

	sprintf(path, "%s%s", launchDir, bios_name);

	if ((fp = fopen(path, "rb")) != NULL)
	{
		fread(memory_region_user1, 1, 0x80000, fp);
		fclose(fp);

		if (crc32(0, memory_region_user1, 0x80000) == 0xdf9de490)
		{
			UINT16 *mem16 = (UINT16 *)memory_region_user1;

			// load CD-ROM
			mem16[0xdb70 >> 1] = 0xfac0;
			mem16[0xdb72 >> 1] = 0x4e75;

			// CDDA control
			mem16[0x056a >> 1] = 0xfac1;
			mem16[0x056c >> 1] = 0x4e75;

			// exit
			mem16[0x055e >> 1] = 0xfac2;
			mem16[0x0560 >> 1] = 0x4e75;

			// CD-ROM protect
			mem16[0xb040 >> 1] = 0x4e71;
			mem16[0xb042 >> 1] = 0x4e71;

			// reset
			mem16[0xa87a >> 1] = 0x4239;
			mem16[0xa87c >> 1] = 0x0010;
			mem16[0xa87e >> 1] = 0xfdae;

			return 1;
		}
		else
		{
			error_crc(bios_name);
		}
	}
	else
	{
		error_file(bios_name);
	}

	return 0;
}


/******************************************************************************
	メモリインタフェース関数
******************************************************************************/

/*------------------------------------------------------
	メモリインタフェース初期化
------------------------------------------------------*/

int memory_init(void)
{
	memory_region_cpu1   = NULL;
	memory_region_cpu2   = NULL;
	memory_region_gfx1   = NULL;
	memory_region_gfx2   = NULL;
	memory_region_sound1 = NULL;
	memory_region_user1  = NULL;

	memory_length_cpu1   = 0x200000;
	memory_length_cpu2   = 0x10000;
	memory_length_gfx1   = 0x20000;
	memory_length_gfx2   = 0x400000;
	memory_length_sound1 = 0x100000;
	memory_length_user1  = 0x80000;

	if ((memory_region_cpu1   = memory_allocate(REGION_CPU1,   memory_length_cpu1)) == NULL) return 0;
	if ((memory_region_cpu2   = memory_allocate(REGION_CPU2,   memory_length_cpu2)) == NULL) return 0;
	if ((memory_region_gfx1   = memory_allocate(REGION_GFX1,   memory_length_gfx1)) == NULL) return 0;
	if ((memory_region_gfx2   = memory_allocate(REGION_GFX2,   memory_length_gfx2)) == NULL) return 0;
	if ((memory_region_sound1 = memory_allocate(REGION_SOUND1, memory_length_sound1)) == NULL) return 0;
	if ((memory_region_user1  = memory_allocate(REGION_USER1,  memory_length_user1)) == NULL) return 0;

	memset(neogeo_memcard, 0, 0x2000);

	if (!neogeo_check_game())
	{
		error_file("IPL.TXT");
		return 0;
	}

	load_gamecfg(game_name);
#ifdef COMMAND_LIST
	load_commandlist(game_name, NULL);
#endif
#ifdef ADHOC
	if (adhoc_enable)
	{
		/* AdHoc通信時は一部オプションで固定の設定を使用 */
		neogeo_raster_enable = 0;
		neogeo_loadscreen    = 1;
		neogeo_cdspeed_limit = 0;
		option_vsync         = 0;
		option_autoframeskip = 0;
		option_frameskip     = 0;
		option_showfps       = 0;
		option_speedlimit    = 1;
		option_sound_enable  = 1;
		option_samplerate    = 1;
		option_mp3_enable    = 1;

		if (adhoc_server)
			option_controller = INPUT_PLAYER1;
		else
			option_controller = INPUT_PLAYER2;
	}
	else
#endif
	{
		SceUID fd;
		char path[MAX_PATH];

		sprintf(path, "%s%s", launchDir, "backup.bin");

		if ((fd = sceIoOpen(path, PSP_O_RDONLY, 0777)) >= 0)
		{
			sceIoRead(fd, neogeo_memcard, 0x2000);
			sceIoClose(fd);
		}
	}

	return load_rom_user1();
}


/*------------------------------------------------------
	メモリインタフェース終了
------------------------------------------------------*/

void memory_shutdown(void)
{
#ifdef ADHOC
	if (!adhoc_enable)
#endif
	{
		SceUID fd;
		char path[MAX_PATH];

		sprintf(path, "%s%s", launchDir, "backup.bin");

		if ((fd = sceIoOpen(path, PSP_O_WRONLY|PSP_O_CREAT, 0777)) >= 0)
		{
			sceIoWrite(fd, neogeo_memcard, 0x2000);
			sceIoClose(fd);
		}
	}

	if (memory_region_user1)  free(memory_region_user1);
	if (memory_region_sound1) free(memory_region_sound1);
	if (memory_region_gfx2)   free(memory_region_gfx2);
	if (memory_region_gfx1)   free(memory_region_gfx1);
	if (memory_region_cpu2)   free(memory_region_cpu2);
	if (memory_region_cpu1)   free(memory_region_cpu1);
}


/******************************************************************************
	M68000 メモリリード/ライト関数
******************************************************************************/

/*------------------------------------------------------
	M68000メモリリード (byte)
------------------------------------------------------*/

UINT8 m68000_read_memory_8(UINT32 offset)
{
	int shift = (~offset & 1) << 3;
	UINT16 mem_mask = ~(0xff << shift);

	offset &= M68K_AMASK;

	switch (offset >> 20)
	{
	case 0x0: return READ_BYTE(memory_region_cpu1, offset);
	case 0x1: return READ_BYTE(memory_region_cpu1, offset);
	case 0xc: return READ_MIRROR_BYTE(memory_region_user1, offset, 0x7ffff);

	case 0x4: return neogeo_paletteram16_r(offset >> 1, mem_mask) >> shift;
	case 0x8: return neogeo_memcard16_r(offset >> 1, mem_mask) >> shift;
	case 0xe: return neogeo_externalmem_16_r(offset >> 1, mem_mask) >> shift;
	case 0xf: return neogeo_hardcontrol_16_r(offset >> 1, mem_mask) >> shift;

	case 0x3:
		switch (offset >> 16)
		{
		case 0x30: return neogeo_controller1_16_r(offset >> 1, mem_mask) >> shift;
		case 0x32: return neogeo_z80_r(offset >> 1, mem_mask) >> shift;
		case 0x34: return neogeo_controller2_16_r(offset >> 1, mem_mask) >> shift;
		case 0x38: return neogeo_controller3_16_r(offset >> 1, mem_mask) >> shift;
		case 0x3c: return neogeo_video_16_r(offset >> 1, mem_mask) >> shift;
		}
		break;
	}

	return 0xff;
}


/*------------------------------------------------------
	M68000リードメモリ (word)
------------------------------------------------------*/

UINT16 m68000_read_memory_16(UINT32 offset)
{
	offset &= M68K_AMASK;

	switch (offset >> 20)
	{
	case 0x0: return READ_WORD(memory_region_cpu1, offset);
	case 0x1: return READ_WORD(memory_region_cpu1, offset);
	case 0xc: return READ_MIRROR_WORD(memory_region_user1, offset, 0x7ffff);

	case 0x4: return neogeo_paletteram16_r(offset >> 1, 0);
	case 0x8: return neogeo_memcard16_r(offset >> 1, 0);
	case 0xe: return neogeo_externalmem_16_r(offset >> 1, 0);
	case 0xf: return neogeo_hardcontrol_16_r(offset >> 1, 0);

	case 0x3:
		switch (offset >> 16)
		{
		case 0x30: return neogeo_controller1_16_r(offset >> 1, 0);
		case 0x32: return neogeo_z80_r(offset >> 1, 0);
		case 0x34: return neogeo_controller2_16_r(offset >> 1, 0);
		case 0x38: return neogeo_controller3_16_r(offset >> 1, 0);
		case 0x3c: return neogeo_video_16_r(offset >> 1, 0);
		}
		break;
	}

	return 0xffff;
}


/*------------------------------------------------------
	M68000リードメモリ (long)
------------------------------------------------------*/

UINT32 m68000_read_memory_32(UINT32 offset)
{
	return (m68000_read_memory_16(offset) << 16) | m68000_read_memory_16(offset + 2);
}


/*------------------------------------------------------
	M68000ライトメモリ (byte)
------------------------------------------------------*/

void m68000_write_memory_8(UINT32 offset, UINT8 data)
{
	int shift = (~offset & 1) << 3;
	UINT16 mem_mask = ~(0xff << shift);

	offset &= M68K_AMASK;

	switch (offset >> 20)
	{
	case 0x0: WRITE_BYTE(memory_region_cpu1, offset, data); return;
	case 0x1: WRITE_BYTE(memory_region_cpu1, offset, data); return;

	case 0x4: neogeo_paletteram16_w(offset >> 1, data << shift, mem_mask); return;
	case 0x8: neogeo_memcard16_w(offset >> 1, data << shift, mem_mask); return;
	case 0xe: neogeo_externalmem_16_w(offset >> 1, data << shift, mem_mask); return;
	case 0xf: neogeo_hardcontrol_16_w(offset >> 1, data << shift, mem_mask); return;

	case 0x3:
		switch (offset >> 16)
		{
		case 0x30: watchdog_counter = 3 * FPS; return;
		case 0x32: neogeo_z80_w(offset >> 1, data << shift, mem_mask); return;
		case 0x3a: neogeo_syscontrol2_16_w(offset >> 1, data << shift, mem_mask); return;
		case 0x3c: neogeo_video_16_w(offset >> 1, data << shift, mem_mask); return;
		}
		break;
	}
}


/*------------------------------------------------------
	M68000ライトメモリ (word)
------------------------------------------------------*/

void m68000_write_memory_16(UINT32 offset, UINT16 data)
{
	offset &= M68K_AMASK;

	switch (offset >> 20)
	{
	case 0x0: WRITE_WORD(memory_region_cpu1, offset, data); return;
	case 0x1: WRITE_WORD(memory_region_cpu1, offset, data); return;

	case 0x4: neogeo_paletteram16_w(offset >> 1, data, 0); return;
	case 0x8: neogeo_memcard16_w(offset >> 1, data, 0); return;
	case 0xe: neogeo_externalmem_16_w(offset >> 1, data, 0); return;
	case 0xf: neogeo_hardcontrol_16_w(offset >> 1, data, 0); return;

	case 0x3:
		switch (offset >> 16)
		{
		case 0x30: watchdog_counter = 3 * FPS; return;
		case 0x32: neogeo_z80_w(offset >> 1, data, 0); return;
		case 0x3a: neogeo_syscontrol2_16_w(offset >> 1, data, 0); return;
		case 0x3c: neogeo_video_16_w(offset >> 1, data, 0); return;
		}
		break;
	}
}


/*------------------------------------------------------
	M68000ライトメモリ (long)
------------------------------------------------------*/

void m68000_write_memory_32(UINT32 offset, UINT32 data)
{
	m68000_write_memory_16(offset, data >> 16);
	m68000_write_memory_16(offset + 2, data);
}


/******************************************************************************
	Z80 メモリリード/ライト関数
******************************************************************************/

/*------------------------------------------------------
	Z80リードメモリ (byte)
------------------------------------------------------*/

UINT8 z80_read_memory_8(UINT32 offset)
{
	offset &= Z80_AMASK;

	return memory_region_cpu2[offset];
}


/*------------------------------------------------------
	Z80ライトメモリ (byte)
------------------------------------------------------*/

void z80_write_memory_8(UINT32 offset, UINT8 data)
{
	offset &= Z80_AMASK;

	memory_region_cpu2[offset] = data;
}


/******************************************************************************
	セーブ/ロード ステート
******************************************************************************/

#ifdef SAVE_STATE

STATE_SAVE( memory )
{
	state_save_byte(memory_region_cpu1, memory_length_cpu1);
	state_save_byte(memory_region_cpu2, memory_length_cpu2);
}

STATE_LOAD( memory )
{
	state_load_byte(memory_region_cpu1, memory_length_cpu1);
	state_load_byte(memory_region_cpu2, memory_length_cpu2);
}

#endif /* SAVE_STATE */
