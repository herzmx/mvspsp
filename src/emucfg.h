#ifndef EMUCONFIG_H
#define EMUCONFIG_H

#ifndef EMUSYS_VALUE
#define EMUSYS_VALUE
#define CPS1	0
#define CPS2	1
#define MVS		2
#define NCDZ	3
#endif

#define ALIGN_PSPDATA	__attribute__((aligned(16)))
#define ALIGN_DATA		__attribute__((aligned(4)))
#define MEM_ALIGN		4

#if defined(BUILD_CPS1PSP)

#define APPNAME_STR		"CAPCOM CPS1 Emulator for PSP"
#define machine_main	cps1_main

#define EMU_SYSTEM		CPS1
#define SYSTEM_NAME		"CPS1"
#define FPS				60.0
#define USE_CACHE		0
#define EEPROM_SIZE		128
#define GULIST_SIZE		48*1024		// 48KB

#elif defined(BUILD_CPS2PSP)

#define APPNAME_STR		"CAPCOM CPS2 Emulator for PSP"
#define machine_main	cps2_main

#define EMU_SYSTEM		CPS2
#define SYSTEM_NAME		"CPS2"
#define FPS				60.0
#ifdef PSP_SLIM
#define USE_CACHE		0
#else
#define USE_CACHE		1
#define CACHE_VERSION	"V22"
#endif
#define EEPROM_SIZE		128
#define GULIST_SIZE		48*1024		// 48KB

#elif defined(BUILD_MVSPSP)

#define APPNAME_STR		"NEOGEO Emulator for PSP"
#define machine_main	neogeo_main

#define EMU_SYSTEM		MVS
#define SYSTEM_NAME		"NEO¥GEO"
#define MVS_FPS			(15625.0 / 264.0)
#define PSP_FPS			(1000000.0 / 16683.0)
#define USE_CACHE		1
#define CACHE_VERSION	"V22"
#define GULIST_SIZE		300*1024		// 300KB

#elif defined(BUILD_NCDZPSP)

#define APPNAME_STR		"NEOGEO CDZ Emulator for PSP"
#define machine_main	neogeo_main

#define EMU_SYSTEM		NCDZ
#define SYSTEM_NAME		"NEO¥GEO CDZ"
#define FPS				60.0
#define USE_CACHE		0
#define GULIST_SIZE		300*1024		// 300KB

#endif

#endif /* EMUCONFIG_H */
