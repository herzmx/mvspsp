/******************************************************************************

	driver.c

	NEOGEO CDZ driver (based on M.A.M.E.'s neogeo driver)

******************************************************************************/

/******************************************************************************

    NEOGEO CDZ Motherboard
    ----------------------

    NEO-CDC1-1
                    L   R   V    S-V    A/V             DC
    |--------------| |-| |-| |--|  |---|   |-----------|  |---------------|
    |              +-+ +-+ +-+  +--+   +---+           +--+               |
    |    D05                   MB3516A                                    |
    |                                                                     |
    |                                                                     |
    |                   74LS05                  518129                    |
    |                            NEO-GRC2-F                               |
    |                                           000-LO                    |
    |                                                                     |
    |      D357J5             LH516  TOP-SPD              LH52B256N       |
    |                                                                     |
    |   KDS-5K                          HC245A    514400  LH52B256N       |
    |  24.000MHz                                                          |
    |                  LC89513K 51832   HC245A    514400  84C00AM-6       |
    |                                                                     |
    |                                                                     |
    |                                                                     |
    |                                                                     |
    |                                                                     |
    |  D05  74HCU04A   HC245A HC245A LS05 HC245A HC245A                   |
    |                                                  HC245A             |
    |     |=====|     +----------+        +----------+                    |
    |-----------------|          |--------|          |--------------------|
                       Controler1          Controler2

    (Back)
    |---------------------------------------------------------------------|
    |                                                                     |
    |                                                                     |
    |                                                                     |
    |                                                                     |
    |                                         LS273  LS273    9480F       |
    |                                                                     |
    |  5118160   NEO-0FC   GC24512          LH52B256N                     |
    |                                                   5118160           |
    |                                       LH52B256N                     |
    |                                                                     |
    |                                                                     |
    |                                                                     |
    |                                                                     |
    |  5118160   NEO-YSA       MC68HC000                                  |
    |                                        NEO-MGA2-SA                  |
    |                                                                     |
    |                                                                     |
    |                                                                     |
    |                                                                     |
    |                                                                     |
    |                                                                     |
    |                 +----------+        +----------+                    |
    |-----------------|          |--------|          |--------------------|
                       Controler2          Controler1

    Notes:
          68k clock: 12.000MHz
          Z80 clock: 6.000MHz
       YM2610 clock: 8.000MHz
              VSync: 60Hz
              HSync: 15.21kHz

             Custom SNK chips
             -------------------
             NEO-GRC2-F
             NEO-0FC
             NEO-YSA   (YM2610?)
             NEO-MGA2-SA
             GC24512

             ROMs        Type
             ----------------------------
             TOP-SPD     TC534200 (DIP40)
             000-L0      TC531000 (DIP28)

             CD-ROM controler
             -----------------
             LC89513K: SANYO

=============================================================================

Points to note, known and proven information deleted from this map:

    0x3000001       Dipswitches
                bit 0 : Selftest
                bit 1 : Unknown (Unused ?) \ something to do with
                bit 2 : Unknown (Unused ?) / auto repeating keys ?
                bit 3 : \
                bit 4 :  | communication setting ?
                bit 5 : /
                bit 6 : free play
                bit 7 : stop mode ?

    0x3c000c        IRQ acknowledge

    0x380011        Backup bank select

    0x3a0001        Enable display.
    0x3a0011        Disable display

    0x3a0003        Swap in Bios (0x80 bytes vector table of BIOS)
    0x3a0013        Swap in Rom  (0x80 bytes vector table of ROM bank)

    0x3a000d        lock backup ram
    0x3a001d        unlock backup ram

    0x3a000b        set game vector table (?)  mirror ?
    0x3a001b        set bios vector table (?)  mirror ?

    0x3a000c        Unknown (ghost pilots)
    0x31001c        Unknown (ghost pilots)

    IO word read

    0x3c0002        return vidram word (pointed to by 0x3c0000)
    0x3c0006        ?????.
    0x3c0008        shadow adress for 0x3c0000 (not confirmed).
    0x3c000a        shadow adress for 0x3c0002 (confirmed, see
                               Puzzle de Pon).
    IO word write

    0x3c0006        Unknown, set vblank counter (?)

    0x3c0008        shadow address for 0x3c0000 (not confirmed)
    0x3c000a        shadow address for 0x3c0002 (not confirmed)

    NEOGEO CDZではNEC 4990は使用されておらず、別のチップが使用されていると
    思われます。Z80のポート$C0～$02に出力されている値が制御を行っているように
    思えますが、ハードウェアには疎いのでさっぱりわからず。

******************************************************************************/

#include "ncdz.h"


/* VBLANK should fire on line 248 */
#define RASTER_COUNTER_START 0x1f0	/* value assumed right after vblank */
#define RASTER_COUNTER_RELOAD 0x0f8	/* value assumed after 0x1ff */
#define RASTER_LINE_RELOAD (0x200 - RASTER_COUNTER_START)

#define SCANLINE_ADJUST 3	/* in theory should be 0, give or take an off-by-one mistake */

/*  flags for irq1control:

    0x07 unused? kof94 sets some random combination of these at the character
         selection screen but only because it does andi.w #$ff2f, $3c0006. It
         clears them immediately after.

    0x08 shocktro2, stops autoanim counter

    Maybe 0x07 writes to the autoanim counter, meaning that in conjunction with
    0x08 one could fine control it. However, if that was the case, writing the
    the IRQ2 control bits would interfere with autoanimation, so I'm probably
    wrong.

    0x10 irq1 enable, tile engine scanline irq that is triggered
    when a certain scanline is reached.

    0x20 when set, the next values written in the irq position register
    sets irq1 to happen N lines after the current one

    0x40 when set, irq position register is automatically loaded at vblank to
    set the irq1 line.

    0x80 when set, every time irq1 is triggered the irq position register is
    automatically loaded to set the next irq1 line.

    0x80 and 0x40 may be set at the same time (Viewpoint does this).
*/

#define IRQ1CTRL_AUTOANIM_STOP		0x08
#define IRQ1CTRL_ENABLE				0x10
#define IRQ1CTRL_LOAD_RELATIVE		0x20
#define IRQ1CTRL_AUTOLOAD_VBLANK	0x40
#define IRQ1CTRL_AUTOLOAD_REPEAT	0x80


/******************************************************************************
	グローバル変数
******************************************************************************/

const char default_name[16] = "default";
int game_index;
int neogeo_driver_type;
int neogeo_ngh;
int neogeo_raster_enable;
UINT32 neogeo_frame_counter;
UINT32 neogeo_frame_counter_speed;

int watchdog_counter;


/******************************************************************************
	Private variables
******************************************************************************/

static int current_rasterline;
static int current_rastercounter;
static int irq1start;
static int irq1control;
static UINT32 irq1pos_value;
static int scanline_read;
static int vblank_int;
static int scanline_int;

static UINT32 frame_counter;

static int sound_code;
static int result_code;
static int pending_command;

static int hack_irq;

static UINT8  upload_mode;
static UINT8  upload_type;
static UINT32 upload_offset1;
static UINT32 upload_offset2;
static UINT32 upload_length;
static UINT16 upload_pattern;
static int upload_executing;
static UINT32 z80_cdda_offset;

static UINT8  exmem[0x100];
static UINT8  exmem_latch[0x100];
static UINT8  exmem_bank[0x10];
static UINT8  exmem_counter;

static UINT8 neogeo_game_vectors[0x100];
static UINT8 *neogeo_vectors[2];

static UINT8 memory_region_hctrl[0x200];


/*------------------------------------------------------
	ゲームチェック
------------------------------------------------------*/

int neogeo_check_game(void)
{
	FILE *fp;
	char fname[16], path[MAX_PATH], linebuf[128];
	int i, fd, found = 0, NGH_number;

	neogeo_ngh = 0;
	neogeo_driver_type = NORMAL;
	hack_irq = 0;

	if (neogeo_boot_bios)
	{
		strcpy(game_name, games[99].name);
		game_index = 99;
	}
	else
	{
		strcpy(game_name, default_name);
		game_index = 0;

		sprintf(path, "%sIPL.TMP", launchDir);

		zip_open(game_dir);

		i = zlength("IPL.TXT");

		if ((fd = zopen("IPL.TXT")) == -1)
		{
			zip_close();
			return 0;
		}
		zread(fd, memory_region_cpu1, i);
		zclose(fd);
		zip_close();

		if ((fp = fopen(path, "w")) == NULL)
		{
			return 0;
		}
		fwrite(memory_region_cpu1, 1, i, fp);
		fclose(fp);

		if ((fp = fopen(path, "r")) == NULL)
		{
			sceIoRemove(path);
			return 0;
		}

		while (fgets(linebuf, 127, fp))
		{
			char *strfname = strtok(linebuf, ",\r\n");
			char *strbank  = strtok(NULL, ",\r\n");
			char *stroffs  = strtok(NULL, ",\r\n");
			char *ext;
			int bank, offs;

			if (strfname == NULL || strbank == NULL || stroffs == NULL)
				break;

			sscanf(strbank, "%d", &bank);
			sscanf(stroffs, "%x", &offs);
			ext = strrchr(strfname, '.');

			if (stricmp(ext, ".PRG") == 0 || stricmp(ext, ".ARG") == 0)
			{
				if (!bank && !offs)
				{
					strcpy(fname, strfname);
					found = 1;
					break;
				}
			}
		}
		fclose(fp);

		sceIoRemove(path);

		if (!found) return 0;

		zip_open(game_dir);
		if ((fd = zopen(fname)) == -1)
		{
			zip_close();
			return 0;
		}

		zread(fd, memory_region_cpu1, 0x110);
		zclose(fd);
		zip_close();

		swab(memory_region_cpu1, memory_region_cpu1, 0x110);
		memcpy(neogeo_game_vectors, memory_region_cpu1, 0x100);

		NGH_number = m68000_read_memory_16(0x108);

		for (i = 0; games[i].ngh_number; i++)
		{
			if (games[i].ngh_number == NGH_number)
			{
				game_index = i + 1;
				neogeo_ngh = NGH_number;
				strcpy(game_name, games[i].name);

				if (NGH_NUMBER(0x0243))	// lastbld2
					hack_irq = 1;

				break;
			}
		}
	}

	neogeo_reset_driver_type();

	return 1;
}


/*------------------------------------------------------
	Inisialize driver
------------------------------------------------------*/

void neogeo_driver_init(void)
{
	if (neogeo_boot_bios)
		memcpy(neogeo_game_vectors, memory_region_user1, 0x100);

	memcpy(memory_region_cpu1, memory_region_user1, 0x100);
	neogeo_vectors[0] = memory_region_user1;
	neogeo_vectors[1] = neogeo_game_vectors;

	memset(memory_region_hctrl, 0, sizeof(memory_region_hctrl));
	z80_cdda_offset = 0;

	m68000_init();
	z80_init();
}


/*------------------------------------------------------
	Reset driver
------------------------------------------------------*/

void neogeo_driver_reset(void)
{
	memset(exmem, 0, sizeof(exmem));
	memset(exmem_latch, 0, sizeof(exmem_latch));
	memset(exmem_bank, 0, sizeof(exmem_bank));
	exmem_counter = 0;

	upload_mode = UPLOAD_IMMIDIATE;
	upload_type = UNKNOWN_TYPE;
	upload_offset1 = 0;
	upload_offset2 = 0;
	upload_length = 0;
	upload_pattern = 0;
	upload_executing = 0;

	memcpy(memory_region_cpu1, neogeo_vectors[1], 0x100);	// game vector

	watchdog_reset_16_w(0, 0, 0);

	irq1start = 1000;
	irq1control = 0;
	irq1pos_value = 0;
	scanline_read = 0;

	current_rasterline = 0;
	current_rastercounter = RASTER_COUNTER_START;

	vblank_int = 0;
	scanline_int = 0;

	sound_code = 0;
	result_code = 0;
	pending_command = 0;

	frame_counter = 0;
	neogeo_frame_counter = 0;
	neogeo_frame_counter_speed = 4;

	timer_set_update_handler();

	neogeo_reset_driver_type();

	m68000_reset();
	z80_reset();
}


/*------------------------------------------------------
	Reset driver type
------------------------------------------------------*/

void neogeo_reset_driver_type(void)
{
	if (neogeo_raster_enable)
	{
		if (neogeo_ngh == NGH_mosyougi)
			neogeo_driver_type = BUSY;
		else
			neogeo_driver_type = RASTER;
	}
	else
	{
		if (neogeo_ngh == NGH_aof2)
			neogeo_driver_type = RASTER;
		else
			neogeo_driver_type = NORMAL;
	}

	timer_set_update_handler();
}


/*------------------------------------------------------
	Update interrupt
------------------------------------------------------*/

static void update_interrupts(void)
{
	int level = 0;

	/* determine which interrupt is active */
	if (vblank_int) level = 2;
	if (scanline_int) level = 1;

	/* either set or clear the appropriate lines */
	if (level)
		m68000_set_irq_line(level, ASSERT_LINE);
	else
		m68000_set_irq_line(7, CLEAR_LINE);

	if (hack_irq)
	{
		if (level == 2) vblank_int = 0;
		if (level == 1) scanline_int = 0;
	}
}


/*------------------------------------------------------
	MC68000 VBLANK interrupt (IRQ2)
------------------------------------------------------*/

void neogeo_interrupt(void)
{
	current_rasterline = 0;
	current_rastercounter = RASTER_COUNTER_START;

	if (!(irq1control & IRQ1CTRL_AUTOANIM_STOP))
	{
		/* Animation counter */
		if (frame_counter++ > neogeo_frame_counter_speed)	/* fixed animation speed */
		{
			frame_counter = 0;
			neogeo_frame_counter++;
		}
	}

	vblank_int = 1; /* vertical blank */

	if (hack_irq)
	{
		UINT32 pc = m68000_get_reg(M68K_PC);

		if (pc >= 0xbf00 && pc <= 0xbfff)	// 0xbf92
			vblank_int = 0;
	}

	update_interrupts();
}


/*------------------------------------------------------
	MC68000 scanline interrupt (IRQ1)
------------------------------------------------------*/

static void neogeo_interrupt_aof2(int line)
{
	static int next_irq1 = 0;
	static int raster_count = 0;

	if (line == RASTER_LINES)
	{
		current_rasterline = 0;
		current_rastercounter = RASTER_COUNTER_START;

		if (irq1control & IRQ1CTRL_ENABLE)
			irq1start = (irq1pos_value + 3) / 0x180;
		else
			irq1start = 1000;

		if (!(irq1control & IRQ1CTRL_AUTOANIM_STOP))
		{
			/* Animation counter */
			if (frame_counter++ > neogeo_frame_counter_speed)
			{
				frame_counter = 0;
				neogeo_frame_counter++;
			}
		}

		if (!(irq1control & IRQ1CTRL_ENABLE))
		{
			next_irq1 = 0;
		}
		else if (next_irq1 == 0)
		{
			next_irq1 = 1;
			raster_count = irq1start;
		}

		vblank_int = 1; /* vertical blank */
	}
	else
	{
		if (next_irq1 && (irq1control & IRQ1CTRL_ENABLE))
		{
			if (--raster_count == 0)
			{
				scanline_int = 1;
				raster_count = irq1start;
			}
		}
	}

	update_interrupts();
}


void neogeo_raster_interrupt(int line, int busy)
{
	int do_refresh = 0;

	current_rasterline = line;

	if (current_rasterline == RASTER_LINES) current_rasterline = 0;	/* vblank */

	if (current_rasterline < RASTER_LINE_RELOAD)
		current_rastercounter = RASTER_COUNTER_START + current_rasterline;
	else
		current_rastercounter = RASTER_COUNTER_RELOAD + current_rasterline - RASTER_LINE_RELOAD;


	if (neogeo_ngh == NGH_aof2)
	{
		neogeo_interrupt_aof2(line);
		return;
	}

	if (busy)
	{
		if (scanline_read)
		{
			do_refresh = 1;
			scanline_read = 0;
		}
	}

	if (irq1control & IRQ1CTRL_ENABLE)
	{
		if (line == irq1start)
		{
			if (!busy) do_refresh = 1;

			if (irq1control & IRQ1CTRL_AUTOLOAD_REPEAT)
				irq1start += (irq1pos_value + 3) / 0x180;	/* ridhero gives 0x17d */

			scanline_int = 1;
		}
	}

	if (line == RASTER_LINES)
	{
		if (irq1control & IRQ1CTRL_AUTOLOAD_VBLANK)
			irq1start = (irq1pos_value + 3) / 0x180;	/* ridhero gives 0x17d */
		else
			irq1start = 1000;

		if (!(irq1control & IRQ1CTRL_AUTOANIM_STOP))
		{
			/* Animation counter */
			if (frame_counter++ > neogeo_frame_counter_speed)	/* fixed animation speed */
			{
				frame_counter = 0;
				neogeo_frame_counter++;
			}
		}

		vblank_int = 1; /* vertical blank */

		if (hack_irq)
		{
			UINT32 pc = m68000_get_reg(M68K_PC);

			if (pc >= 0xbf00 && pc <= 0xbfff)	// 0xbf92
				vblank_int = 0;
		}
	}

	if (do_refresh && !skip_this_frame())
	{
		if (line > RASTER_LINE_RELOAD)	/* avoid unnecessary updates after start of vblank */
			neogeo_partial_screenrefresh((current_rastercounter - 256) - 1 + SCANLINE_ADJUST);
	}

	update_interrupts();
}


/****************************************************************************
	MC68K memory access
****************************************************************************/

/*------------------------------------------------------
	Select BIOS vector (0x3a0003 / 0x3a0013)
------------------------------------------------------*/

INLINE void neogeo_select_vectors(int vector)
{
	memcpy(memory_region_cpu1, neogeo_vectors[vector], 0x80);
	blit_clear_fix_sprite();
	blit_clear_spr_sprite();
	autoframeskip_reset();
}


/*------------------------------------------------------
	Select palette RAM bank  (0x3a000f / 0x3a001f)
------------------------------------------------------*/

INLINE void neogeo_setpalbank(UINT32 bank)
{
	if (neogeo_palette_index != bank)
	{
		neogeo_palette_index = bank;
		neogeo_paletteram16 = neogeo_palettebank16[bank];
		video_palette = video_palettebank[bank];
	}
}


/*------------------------------------------------------
	Write data to VRAM ($3c0002)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( neogeo_vidram16_data_w )
{
	COMBINE_DATA(&neogeo_vidram16[neogeo_vidram16_offset]);

	neogeo_vidram16_offset = (neogeo_vidram16_offset & 0x8000)
			| ((neogeo_vidram16_offset + neogeo_vidram16_modulo) & 0x7fff);
}


/*---------------------------------------------------------
	Read video control data ($3c0006)
---------------------------------------------------------*/

INLINE UINT16 neogeo_control_16_r(void)
{
	int res;

	scanline_read = 1;

	if (neogeo_driver_type == NORMAL)
	{
		current_rasterline = timer_getscanline();

		if (current_rasterline == RASTER_LINES)
			current_rasterline = 0;

		if (current_rasterline < RASTER_LINE_RELOAD)
			current_rastercounter = RASTER_COUNTER_START + current_rasterline;
		else
			current_rastercounter = RASTER_COUNTER_RELOAD + current_rasterline - RASTER_LINE_RELOAD;
	}

	res = ((current_rastercounter << 7) & 0xff80) |	/* raster counter */
		   (neogeo_frame_counter & 0x0007);			/* frame counter */

	return res;
}


/*---------------------------------------------------------
	Write video control data ($3c0006)
---------------------------------------------------------*/

INLINE void neogeo_control_16_w(UINT16 data)
{
	/* Auto-Anim Speed Control */
	neogeo_frame_counter_speed = (data >> 8) & 0xff;

	irq1control = data & 0xff;
}


/*------------------------------------------------------
	Write IRQ position     (0x3c0008, 0x3c000a)
------------------------------------------------------*/

INLINE void neogeo_irq2pos_16_w(UINT32 offset, UINT16 data)
{
	if (neogeo_driver_type == NORMAL) return;

	if (offset)
		irq1pos_value = (irq1pos_value & 0xffff0000) | (UINT32)data;
	else
		irq1pos_value = (irq1pos_value & 0x0000ffff) | ((UINT32)data << 16);

	if (irq1control & IRQ1CTRL_LOAD_RELATIVE)
		irq1start = current_rasterline + (irq1pos_value + 0x3b) / 0x180;
}


/*------------------------------------------------------
	Write IRQ acknowledge ($3c000c)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( neogeo_irqack_16_w )
{
	if (ACCESSING_LSB)
	{
		if (data & 4) vblank_int = 0;
		if (data & 2) scanline_int = 0;
		update_interrupts();
	}
}


/*------------------------------------------------------
	Hardware upload ($ff0061)
------------------------------------------------------*/

static WRITE16_HANDLER( hardware_upload_16_w )
{
	if (data == 0x00)	// Clear upload info
	{
		upload_mode    = UPLOAD_IMMIDIATE;
		upload_offset1 = 0;
		upload_offset2 = 0;
		upload_length  = 0;
	}
	else if (data == 0x40)	// Execute upload
	{
		UINT32 i;
		UINT8 *src, *dst;

		if (upload_mode == UPLOAD_MEMORY)
		{
			upload_type = upload_get_type() & 0x0f;
		}
		else
		{
			if (upload_offset1 >= 0x400000 && upload_offset1 < 0x500000)
			{
				upload_type = PAL_TYPE;
			}
			else
			{
				upload_type = UNKNOWN_TYPE;
			}
		}

		switch (upload_mode)
		{
		case UPLOAD_IMMIDIATE:
			break;

		case UPLOAD_PATTERN:
			if (upload_type == PAL_TYPE)
			{
				for (i = 0; i < upload_length; i++)
				{
					offset = upload_offset1 + (i * 2);
					m68000_write_memory_16(offset, upload_pattern);
				}
			}
			break;

		case UPLOAD_MEMORY:
			{
				UINT32 length;

				length = upload_length << 1;
				src = memory_region_cpu1 + upload_offset1;

				switch (upload_type & 0x0f)
				{
				case PRG_TYPE:
					dst = memory_region_cpu1;
					memcpy(dst + upload_offset2, src, length);
					break;

				case FIX_TYPE:
					dst = memory_region_gfx1;
					offset = upload_offset2 - 0xe00000;
					swab(src, dst + (offset >> 1), length);
					neogeo_decode_fix(dst, offset, length);
					break;

				case SPR_TYPE:
					dst = memory_region_gfx2;
					offset = (upload_offset2 - 0xe00000) + (upload_get_bank() << 20);
					swab(src, dst + offset, length);
					neogeo_decode_spr(dst, offset, length);
					exmem_bank[EXMEM_OBJ] = ((offset + upload_get_length()) >> 20) & 0x03;
					break;

				case Z80_TYPE:
					dst = memory_region_cpu2;
					offset = upload_offset2 - 0xe00000;
					swab(src, dst + (offset >> 1), length);
					break;

				case PAL_TYPE:
					for (i = 0; i < length; i++)
					{
						data = m68000_read_memory_16(upload_offset1 + i * 2);
						m68000_write_memory_16(upload_offset2 + i * 2, data);
					}
					break;
				}
			}
			break;

		case UPLOAD_FILE:
			// Upload file
			break;
		}
	}
}


/*------------------------------------------------------
	Set upload offset 1   ($ff0064)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( upload_offset1_16_w )
{
	if (offset)
		upload_offset1 = (upload_offset1 & 0xffff0000) | (UINT32)data;
	else
		upload_offset1 = (upload_offset1 & 0x0000ffff) | ((UINT32)data << 16);
}


/*------------------------------------------------------
	Set upload offset 2   ($ff0068)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( upload_offset2_16_w )
{
	if (offset)
		upload_offset2 = (upload_offset2 & 0xffff0000) | (UINT32)data;
	else
		upload_offset2 = (upload_offset2 & 0x0000ffff) | ((UINT32)data << 16);

	upload_mode = UPLOAD_MEMORY;
}


/*------------------------------------------------------
	Set upload pattern data   ($ff006c)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( upload_pattern_16_w )
{
	upload_pattern = data;
	upload_mode = UPLOAD_PATTERN;
}


/*------------------------------------------------------
	Set upload length  ($ff0070)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( upload_length_16_w )
{
	if (offset)
		upload_length = (upload_length & 0xffff0000) | (UINT32)data;
	else
		upload_length = (upload_length & 0x0000ffff) | ((UINT32)data << 16);
}


/*------------------------------------------------------
	Set external memory type ($ff0105)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( exmem_type_16_w )
{
	exmem[exmem_counter] = data;

	if (exmem[exmem_counter] == EXMEM_Z80)
	{
		z80_cdda_offset = m68000_read_memory_32(0x108000 + 0x76ea);
		if (z80_cdda_offset)
			z80_cdda_offset = (z80_cdda_offset - 0xe00000) >> 1;
	}

	if (exmem[exmem_counter] == EXMEM_OBJ) exmem_bank[EXMEM_OBJ] = 0;
	if (exmem[exmem_counter] == EXMEM_PCMA) exmem_bank[EXMEM_PCMA] = 0;
}


/*------------------------------------------------------
	Disable draw SPR plane ($ff0111)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( spr_disable_16_w )
{
	spr_disable = data & 0xff;
}


/*------------------------------------------------------
	Disable draw FIX plane ($ff0115)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( fix_disable_16_w )
{
	fix_disable = data & 0xff;
}


/*------------------------------------------------------
	Enable video output ($ff0119)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( video_enable_16_w )
{
	video_enable = data & 0xff;
}


/*------------------------------------------------------
	Write latch external memory  ($ff0121) - OBJ RAM
	                             ($ff0123) - PCM-A RAM
	                             ($ff0127) - Z80 RAM
	                             ($ff0129) - FIX RAM
------------------------------------------------------*/

INLINE WRITE16_HANDLER( exmem_latch_16_w )
{
	if (exmem_counter < 0xff)
	{
		exmem_counter++;
		exmem_latch[exmem_counter] = offset & 0x0f;
	}
}


/*------------------------------------------------------
	Clear latch external memory  ($ff0141) - OBJ RAM
	                             ($ff0143) - PCM-A RAM
	                             ($ff0147) - Z80 RAM
	                             ($ff0149) - FIX RAM
------------------------------------------------------*/

INLINE WRITE16_HANDLER( exmem_latch_clear_16_w )
{
	offset &= 0x0f;

	if (exmem_latch[exmem_counter] == offset)
	{
		if (exmem_counter > 0)
		{
			exmem_latch[exmem_counter] = EXMEM_UNKNOWN;
			exmem_counter--;
		}
	}
}


/*------------------------------------------------------
	Set uploading flag ($ff0161)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( upload_executing_16_w )
{
	upload_executing = data & 0xff;
}


/*------------------------------------------------------
	Z80 reset / enable ($ff0183)
------------------------------------------------------*/

INLINE WRITE16_HANDLER( z80_reset_16_w )
{
	z80_set_reset_line(data ? CLEAR_LINE : ASSERT_LINE);
}


/*------------------------------------------------------
	Set external memory bank  ($ff01a1) - OBJ RAM
	                          ($ff01a3) - PCM-A RAM
------------------------------------------------------*/

INLINE WRITE16_HANDLER( exmem_bank_16_w )
{
	offset &= 0x0f;
	exmem_bank[offset] = data & 0x03;
}


/****************************************************************************
	M68000 memory handlers
****************************************************************************/

/*------------------------------------------------------
	Reset watchdog counter ($300000 - $300001)
------------------------------------------------------*/

WRITE16_HANDLER( watchdog_reset_16_w )
{
	watchdog_counter = FPS * 3;
}


/*------------------------------------------------------
	Read controller 1 ($300000 - $300001)
------------------------------------------------------*/

READ16_HANDLER( neogeo_controller1_16_r )
{
	return neogeo_port_value[0] << 8;
}


/*------------------------------------------------------
	Read controller 2 ($340000 - $340001)
------------------------------------------------------*/

READ16_HANDLER( neogeo_controller2_16_r )
{
	return neogeo_port_value[1] << 8;
}


/*------------------------------------------------------
	Read controller 3 ($380000 - $380001)
------------------------------------------------------*/

READ16_HANDLER( neogeo_controller3_16_r )
{
	return neogeo_port_value[2] << 8;
}


/*------------------------------------------------------
	Read Z80 communication data ($320001)
------------------------------------------------------*/

READ16_HANDLER( neogeo_z80_r )
{
	UINT16 res = 0x3f;

	res |= result_code << 8;
	if (pending_command) res &= 0x7fff;

	return res;
}


/*------------------------------------------------------
	Write Z80 communication data ($320001)
------------------------------------------------------*/

WRITE16_HANDLER( neogeo_z80_w )
{
	pending_command = 1;
	timer_set(SOUNDLATCH_TIMER, TIME_NOW, (data >> 8) & 0xff, neogeo_sound_write);
}


/*------------------------------------------------------
	Write system control switch 2 ($3a0000 - $3a001f)
------------------------------------------------------*/

WRITE16_HANDLER( neogeo_syscontrol2_16_w )
{
	int flag = (offset >> 3) & 1;

	switch (offset & 7)
	{
	case 1: neogeo_select_vectors(flag); break;
	case 7: neogeo_setpalbank(flag); break;
	}
}


/*------------------------------------------------------
	Read VRAM register (0x3c0000 - 0x3c000f)
------------------------------------------------------*/

READ16_HANDLER( neogeo_video_16_r )
{
	if (!ACCESSING_MSB) return 0xff;

	switch (offset & 3)
	{
	case 0: return neogeo_vidram16[neogeo_vidram16_offset];
	case 1: return neogeo_vidram16[neogeo_vidram16_offset];
	case 2: return neogeo_vidram16_modulo;
	case 3: return neogeo_control_16_r();
	}
	return 0xffff;
}


/*------------------------------------------------------
	Write VRAM register (0x3c0000 - 0x3c000f)
------------------------------------------------------*/

WRITE16_HANDLER( neogeo_video_16_w )
{
	switch (offset & 7)
	{
	case 0: COMBINE_DATA(&neogeo_vidram16_offset); break;
	case 1: neogeo_vidram16_data_w(0, data, mem_mask); break;
	case 2: COMBINE_DATA(&neogeo_vidram16_modulo); break;
	case 3: neogeo_control_16_w(data); break;
	case 4: neogeo_irq2pos_16_w(0, data); break;
	case 5: neogeo_irq2pos_16_w(1, data); break;
	case 6: neogeo_irqack_16_w(0, data, mem_mask); break;
	case 7: break; /* Unknown, see control_r */
	}
}


/*------------------------------------------------------
	Write to palette RAM (0x400000 - 0x40ffff)
------------------------------------------------------*/

WRITE16_HANDLER( neogeo_paletteram16_w )
{
	UINT16 color;

	offset &= 0xfff;
	color = COMBINE_DATA(&neogeo_paletteram16[offset]);

	if (offset & 0x0f)
		video_palette[offset] = video_clut16[color & 0x7fff];
}


/*------------------------------------------------------
	Read from backup memory ($800000 - $803fff)
------------------------------------------------------*/

READ16_HANDLER( neogeo_memcard16_r )
{
	return neogeo_memcard[offset & 0x1fff] | 0xff00;
}


/*------------------------------------------------------
	Write to backup memory ($800000 - $803fff)
------------------------------------------------------*/

WRITE16_HANDLER( neogeo_memcard16_w )
{
	if (ACCESSING_LSB)
	{
		neogeo_memcard[offset & 0x1fff] = data & 0xff;
	}
}


/*------------------------------------------------------
	Read from external memory ($e00000 - $efffff)
------------------------------------------------------*/

READ16_HANDLER( neogeo_externalmem_16_r )
{
	offset &= 0x7ffff;

	switch (exmem[exmem_counter])
	{
	case EXMEM_OBJ:
		offset  = (offset << 1) + (exmem_bank[EXMEM_OBJ] << 20);
		return (memory_region_gfx2[offset] << 8) | memory_region_gfx2[offset + 1];

	case EXMEM_PCMA:
		offset  = offset + (exmem_bank[EXMEM_PCMA] << 19);
		return memory_region_sound1[offset] | 0xff00;

	case EXMEM_Z80:
		if (z80_cdda_offset)
		{
			if (offset == z80_cdda_offset || offset == z80_cdda_offset + 1)
				return 0;
		}
		return memory_region_cpu2[offset] | 0xff00;

	case EXMEM_FIX:
		return memory_region_gfx1[offset] | 0xff00;
	}

	return 0xffff;
}


/*------------------------------------------------------
	Write to external memory ($e00000 - $efffff)
------------------------------------------------------*/

WRITE16_HANDLER( neogeo_externalmem_16_w )
{
	offset &= 0x7ffff;

	switch (exmem[exmem_counter])
	{
	case EXMEM_OBJ:
		offset = (offset << 1) + (exmem_bank[EXMEM_OBJ] << 20);
		data = (data << 8) | (data >> 8);
		COMBINE_SWABDATA((UINT16 *)(memory_region_gfx2 + offset));
		if ((offset & 0x7f) == 0x7e)
			neogeo_decode_spr(memory_region_gfx2, (offset & ~0x7f), 128);
		break;

	case EXMEM_PCMA:
		offset += exmem_bank[EXMEM_PCMA] << 19;
		memory_region_sound1[offset] = data & 0xff;
		break;

	case EXMEM_Z80:
		if (z80_cdda_offset)
		{
			if (offset == z80_cdda_offset || offset == z80_cdda_offset + 1)
				return;
		}
		memory_region_cpu2[offset] = data & 0xff;
		break;

	case EXMEM_FIX:
		memory_region_gfx1[offset] = data & 0xff;
		if ((offset & 0x1f) == 0x1f)
			neogeo_decode_fix(memory_region_gfx1, (offset & ~0x1f) << 1, 32);
		break;
	}
}


/*------------------------------------------------------
	Read hadware control data ($ff0000 - $ff0fff)
------------------------------------------------------*/

READ16_HANDLER( neogeo_hardcontrol_16_r )
{
	if ((offset >> 15) == 0xff)
	{
		UINT16 *mem = (UINT16 *)memory_region_hctrl;
		return mem[offset & 0xff];
	}
	return 0xffff;
}


/*------------------------------------------------------
	Write hardware control data ($ff0000 - $ff0fff)
------------------------------------------------------*/

WRITE16_HANDLER( neogeo_hardcontrol_16_w )
{
	if ((offset >> 15) == 0xff)
	{
		UINT16 *mem = (UINT16 *)memory_region_hctrl;
		offset &= 0xff;

		switch (offset)
		{
		case 0x00/2: break;	// unknown
		case 0x02/2: break;	// unknown, CD-ROM control ?
		case 0x04/2: break;	// unknown, CD-ROM control ?
		case 0x06/2: break;	// unknown

		case 0x60/2: hardware_upload_16_w(0, data, mem_mask); break;
		case 0x64/2: upload_offset1_16_w(0, data, mem_mask); break;
		case 0x66/2: upload_offset1_16_w(1, data, mem_mask); break;
		case 0x68/2: upload_offset2_16_w(0, data, mem_mask); break;
		case 0x6a/2: upload_offset2_16_w(1, data, mem_mask); break;
		case 0x6c/2: upload_pattern_16_w(0, data, mem_mask); break;
		case 0x70/2: upload_length_16_w(0, data, mem_mask); break;
		case 0x72/2: upload_length_16_w(1, data, mem_mask); break;

		case 0x7e/2: break;	// unknown, Hardware upload settings ?
		case 0x80/2: break;	// unknown, Hardware upload settings ?
		case 0x82/2: break;	// unknown, Hardware upload settings ?
		case 0x84/2: break;	// unknown, Hardware upload settings ?
		case 0x86/2: break;	// unknown, Hardware upload settings ?
		case 0x88/2: break;	// unknown, Hardware upload settings ?
		case 0x8a/2: break;	// unknown, Hardware upload settings ?
		case 0x8c/2: break;	// unknown, Hardware upload settings ?
		case 0x8e/2: break;	// unknown, Hardware upload settings ?

		case 0x104/2: exmem_type_16_w(0, data, mem_mask); break;

		case 0x110/2: spr_disable_16_w(0, data, mem_mask); break;
		case 0x114/2: fix_disable_16_w(0, data, mem_mask); break;
		case 0x118/2: video_enable_16_w(0, data, mem_mask); break;
		case 0x11c/2: break;	// machine settings (read only)

		case 0x120/2:	// OBJ RAM latch
		case 0x122/2:	// PCM-A RAM latch
		case 0x126/2:	// Z80 RAM latch
		case 0x128/2:	// FIX RAM latch
			exmem_latch_16_w(offset, data, mem_mask);
			break;

		case 0x140/2:	// OBJ RAM latch clear
		case 0x142/2:	// PCM-A RAM latch clear
		case 0x146/2:	// Z80 RAM latch clear
		case 0x148/2:	// FIX RAM latch clear
			exmem_latch_clear_16_w(offset, data, mem_mask);
			break;

		case 0x180/2: break;	// unknown
		case 0x182/2: z80_reset_16_w(0, data, mem_mask); break;

		case 0x1a0/2:	// OBJ RAM bank
		case 0x1a6/2:	// unknown (PAT bank?)
			exmem_bank_16_w(offset, data, mem_mask);
			break;

		case 0x1a2/2:	// PCM-A RAM bank
			exmem_bank_16_w(offset, data, mem_mask);
			break;

		default:
			break;
		}

		COMBINE_DATA(&mem[offset]);
	}
}


/****************************************************************************
	Z80 port access
****************************************************************************/

/*------------------------------------------------------
	Read Z80 port
------------------------------------------------------*/

UINT8 neogeo_z80_port_r(UINT16 port)
{
	switch (port & 0xff)
	{
	case 0x00:
		pending_command = 0;
		return sound_code;

	case 0x04:
		return YM2610_status_port_A_r(0);

	case 0x05:
		return YM2610_read_port_r(0);

	case 0x06:
		return YM2610_status_port_B_r(0);

	case 0x08:
	case 0x09:
	case 0x0a:
	case 0x0b:
		// set bank
		return 0;

	default:
		break;
	};

	return 0;
}


/*------------------------------------------------------
	Write Z80 port
------------------------------------------------------*/

void neogeo_z80_port_w(UINT16 port, UINT8 data)
{
	switch (port & 0xff)
	{
	case 0x04:
		YM2610_control_port_A_w(0, data);
		break;

	case 0x05:
		YM2610_data_port_A_w(0, data);
		break;

	case 0x06:
		YM2610_control_port_B_w(0, data);
		break;

	case 0x07:
		YM2610_data_port_B_w(0, data);
		break;

	case 0x08:
		/* NMI enable / acknowledge? (the data written doesn't matter) */
		break;

	case 0x0c:
		result_code = data;
		break;

	case 0x18:
		/* NMI disable? (the data written doesn't matter) */
		break;

	default:
		break;
	}
}


/****************************************************************************
	Sound data write handler
****************************************************************************/

void neogeo_sound_write(int data)
{
	sound_code = data;
	z80_set_irq_line(IRQ_LINE_NMI, PULSE_LINE);
}


/****************************************************************************
	Sound IRQ handler
****************************************************************************/

void neogeo_sound_irq(int irq)
{
	z80_set_irq_line(1, irq ? ASSERT_LINE : CLEAR_LINE);
}


/****************************************************************************
	Save / Load state
****************************************************************************/

#ifdef SAVE_STATE

STATE_SAVE( driver )
{
	state_save_long(&neogeo_driver_type, 1);
	state_save_long(&irq1start, 1);
	state_save_long(&irq1control, 1);
	state_save_long(&irq1pos_value, 1);
	state_save_long(&vblank_int, 1);
	state_save_long(&scanline_int, 1);
	state_save_long(&scanline_read, 1);
	state_save_long(&frame_counter, 1);
	state_save_long(&sound_code, 1);
	state_save_long(&result_code, 1);
	state_save_long(&pending_command, 1);
	state_save_long(&neogeo_frame_counter, 1);
	state_save_long(&neogeo_frame_counter_speed, 1);

	state_save_byte(memory_region_hctrl, 0x200);
}


STATE_LOAD( driver )
{
	state_load_long(&neogeo_driver_type, 1);
	state_load_long(&irq1start, 1);
	state_load_long(&irq1control, 1);
	state_load_long(&irq1pos_value, 1);
	state_load_long(&vblank_int, 1);
	state_load_long(&scanline_int, 1);
	state_load_long(&scanline_read, 1);
	state_load_long(&frame_counter, 1);
	state_load_long(&sound_code, 1);
	state_load_long(&result_code, 1);
	state_load_long(&pending_command, 1);
	state_load_long(&neogeo_frame_counter, 1);
	state_load_long(&neogeo_frame_counter_speed, 1);

	state_load_byte(memory_region_hctrl, 0x200);

	current_rasterline = 0;
	current_rastercounter = RASTER_COUNTER_START;

	memset(exmem, 0, sizeof(exmem));
	memset(exmem_latch, 0, sizeof(exmem_latch));
	memset(exmem_bank, 0, sizeof(exmem_bank));
	exmem_counter = 0;

	upload_mode = UPLOAD_IMMIDIATE;
	upload_type = UNKNOWN_TYPE;
	upload_offset1 = 0;
	upload_offset2 = 0;
	upload_length = 0;
	upload_pattern = 0;
	upload_executing = 0;
}

#endif /* SAVE_STATE */


/****************************************************************************
	Driver structure
****************************************************************************/

const GAMES games[100] =
{
	{ "nam1975",  NGH_nam1975  },
	{ "bstars",   NGH_bstars   },
	{ "tpgolf",   NGH_tpgolf   },
	{ "mahretsu", NGH_mahretsu },
	{ "ridhero",  NGH_ridhero  },
	{ "alpham2",  NGH_alpham2  },
	{ "cyberlip", NGH_cyberlip },
	{ "superspy", NGH_superspy },
	{ "mutnat",   NGH_mutnat   },
	{ "sengoku",  NGH_sengoku  },
	{ "burningf", NGH_burningf },
	{ "lbowling", NGH_lbowling },
	{ "gpilots",  NGH_gpilots  },
	{ "joyjoy",   NGH_joyjoy   },
	{ "lresort",  NGH_lresort  },
	{ "2020bb",   NGH_2020bb   },
	{ "socbrawl", NGH_socbrawl },
	{ "roboarmy", NGH_roboarmy },
	{ "fatfury1", NGH_fatfury1 },
	{ "fbfrenzy", NGH_fbfrenzy },
	{ "kotm2",    NGH_kotm2    },
	{ "sengoku2", NGH_sengoku2 },
	{ "bstars2",  NGH_bstars2  },
	{ "3countb",  NGH_3countb  },
	{ "aof",      NGH_aof      },
	{ "samsho",   NGH_samsho   },
	{ "tophuntr", NGH_tophuntr },
	{ "fatfury2", NGH_fatfury2 },
	{ "ssideki",  NGH_ssideki  },
	{ "kof94",    NGH_kof94    },
	{ "aof2",     NGH_aof2     },
	{ "fatfursp", NGH_fatfursp },
	{ "savagere", NGH_savagere },
	{ "ssideki2", NGH_ssideki2 },
	{ "samsho2",  NGH_samsho2  },
	{ "pspikes2", NGH_pspikes2 },
	{ "fatfury3", NGH_fatfur3c },
	{ "sonicwi2", NGH_sonicwi2 },
	{ "galaxyfg", NGH_galaxyfg },
	{ "quizkof",  NGH_quizkof  },
	{ "ssideki3", NGH_ssideki3 },
	{ "doubledr", NGH_doubledr },
	{ "pbobblen", NGH_pbobblen },
	{ "kof95",    NGH_kof95    },
	{ "ssrpg",    NGH_ssrpg    },
	{ "samsho3",  NGH_samsho3  },
	{ "stakwin",  NGH_stakwin  },
	{ "pulstar",  NGH_pulstar  },
	{ "kabukikl", NGH_kabukikl },
	{ "gowcaizr", NGH_gowcaizr },
	{ "rbff1",    NGH_rbff1    },
	{ "aof3",     NGH_aof3     },
	{ "sonicwi3", NGH_sonicwi3 },
	{ "fromanc2", NGH_fromanc2 },
	{ "turfmast", NGH_turfmast },
	{ "mslug",    NGH_mslug    },
	{ "ngcdsp",   NGH_ngcdsp   },
	{ "neodrift", NGH_neodrift },
	{ "kof96",    NGH_kof96    },
	{ "ragnagrd", NGH_ragnagrd },
	{ "pgoal",    NGH_pgoal    },
	{ "ironclad", NGH_ironclad },
	{ "magdrop2", NGH_magdrop2 },
	{ "samsho4",  NGH_samsho4  },
	{ "kof96ngc", NGH_kof96ngc },
	{ "rbffspec", NGH_rbffspec },
	{ "breakers", NGH_breakers },
	{ "kof97",    NGH_kof97    },
	{ "lastblad", NGH_lastblad },
	{ "rbff2",    NGH_rbff2    },
	{ "mslug2",   NGH_mslug2   },
	{ "kof98",    NGH_kof98    },
	{ "lastbld2", NGH_lastbld2 },
	{ "kof99",    NGH_kof99    },

	{ "bjourney", NGH_bjourney },
	{ "crsword",  NGH_crsword  },
	{ "rallych",  NGH_trally   },
	{ "ncommand", NGH_ncommand },
	{ "wh1",      NGH_wh1      },
	{ "wh2",      NGH_wh2      },
	{ "wh2j",     NGH_wh2j     },
	{ "aodk",     NGH_aodk     },
	{ "whp",      NGH_whp      },
	{ "mosyougi", NGH_mosyougi },
	{ "overtop",  NGH_overtop  },
	{ "ninjamas", NGH_ninjamas },
	{ "twinspri", NGH_twinspri },
	{ "maglord",  NGH_maglord  },
	{ "ncombat",  NGH_ncombat  },
	{ "crsword2", NGH_crsword2 },
	{ "adkworld", NGH_adkworld },
	{ "zintrick", NGH_zintrick },

	{ "janshin",  NGH_janshin  },
	{ "viewpoin", NGH_viewpoin },

	{ "wjammers", NGH_wjammers },
	{ "karnovr",  NGH_karnovr  },
	{ "strhoop",  NGH_strhoop  },

	{ "default",  0 },
	{ "default",  0 },
	{ "neogeocd", 0 }
};
