/******************************************************************************

	ui_menu.c

	PSP ÉÅÉjÉÖÅ[

******************************************************************************/

#ifndef PSP_UI_MENU_H
#define PSP_UI_MENU_H

#define UI_TEXTURE		(void *)(0x44000000 + (((SCR_HEIGHT * 3) << 9) << 2))

#if PSP_VIDEO_32BPP
#define VRAM_FMT	GU_PSM_8888
#else
#define VRAM_FMT	GU_PSM_5551
#endif

void showmenu(void);
#if PSP_VIDEO_32BPP
void show_color_menu(void);
#endif

#endif /* PSP_UI_MENU_H */
