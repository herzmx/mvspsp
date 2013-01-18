/******************************************************************************

	ui_text.c

	ƒ†[ƒUƒCƒ“ƒ^ƒtƒF[ƒXƒeƒLƒXƒgŠÇ—

******************************************************************************/

#include "psp.h"
#include <psputility_sysparam.h>


const char *ui_text[UI_TEXT_MAX];

static const char *text[3][UI_TEXT_MAX] =
{
	{
		"\0",
		"\n",

		/* psp/filer.c */
		"Please wait...",
		"Could not open zipname.%s",
#ifdef ADHOC
		"Please turn on the WLAN switch.",
		"Failed to load AdHoc modules.",
#endif

		/* psp/sound.c */
		"Could not reserve audio channel for sound.",
		"Could not start sound thread.",

		/* psp/ui.c */
		"Warning: Battery is low (%d%%). Please charge battery!",

		FONT_CIRCLE " to launch, " FONT_CROSS " to cancel",
		FONT_CIRCLE " to confirm, " FONT_CROSS " to cancel",
#ifdef SAVE_STATE
		FONT_CIRCLE " return to game, " FONT_CROSS " return to menu",
#endif
		"Press any button.",
		"Start emulation.",
#ifdef ADHOC
		"Start emulation. (AdHoc)",
#endif
		"Exit emulation.",
		"Reset emulation.",
		"Need to restart emulation.",
#if (EMU_SYSTEM != NCDZ)
		"THIS GAME DOESN'T WORK.",
		"You won't be able to make it work correctly.",
		"Don't bother.",
#endif
		"Do you make this directory the startup directory?",
#ifdef PSP_SLIM
		"This program requires PSP-2000 + FW 3.71 M33 or later.",
#endif
#ifdef SAVE_STATE
		"Start save state.",
		"Start load state.",
#endif
#if defined(SAVE_STATE) || defined(COMMAND_LIST)
		"Complete.",
#endif
#ifdef SAVE_STATE
		"Delete state file.",
#endif
#if (EMU_SYSTEM == NCDZ)
		"MP3 files not found. CDDA cannot play.",
		"IPL.TXT not found.",
		"Boot NEOï¿½GEO CDZ BIOS.",
		"NEOï¿½GEO CDZ BIOS (neocd.bin) not found.",
		"Invalid NEOï¿½GEO CDZ BIOS found.",
		"Cannot launch game.",
#endif

		"Help - %s",
		"Not use",
		"Press any button to return to menu.",
		"File browser",
		"Scroll",
		"Scroll 1 page",
		"Launch game",
		"Launch game (AdHoc)",
		"Exit emulator",
		"Show BIOS select menu",
		"Open color settings menu",
		"Show this help",
		"Boot BIOS",
		"Set selected directory as startup",
		"Change value",
		"Select",
		"Select slot",
		"Change function",
		"Execute function",
		"Select item",
		"Select item / Change value",
		"RGB value +5",
		"Return to file browser",
		"RGB value -5",
		"Restore all values to default",
		"Scroll / Select item",
		"Open / Close item menu",
		"Return to main menu / game",

		/* psp/ui_menu.c */
		"On",
		"Off",
		"Yes",
		"No",
		"Enable",
		"Disable",
		"Return to main menu",
		"Game configuration menu",
		"Raster Effects",
		"Stretch Screen",
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		"360x270 (4:3)",
		"384x270 (24:17)",
		"480x270 (16:9)",
		"Rotate Screen",
#else
		"360x270 (4:3)",
		"420x270 (14:9)",
		"480x270 (16:9)",
#endif
		"Video Sync",
		"Auto Frameskip",
		"Frameskip",
		"level 1",
		"level 2",
		"level 3",
		"level 4",
		"level 5",
		"level 6",
		"level 7",
		"level 8",
		"level 9",
		"level 10",
		"level 11",
		"Show FPS",
		"60fps Limit",
		"Enable Sound",
		"Sample Rate",
		"11025Hz",
		"22050Hz",
		"44100Hz",
		"Sound Volume",
		"0%",
		"10%",
		"20%",
		"30%",
		"40%",
		"50%",
		"60%",
		"70%",
		"80%",
		"90%",
		"100%",
		"Controller",
		"Player 1",
		"Player 2",
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		"Player 3",
		"Player 4",
#endif
		"PSP clock",
		"222MHz",
		"266MHz",
		"300MHz",
		"333MHz",
#if (EMU_SYSTEM == MVS)
		"Default",
#endif
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"Machine Region",
		"Japan",
		"USA",
		"Europe",
#endif
#if (EMU_SYSTEM == MVS)
		"Machine Mode",
		"Console (AES)",
		"Arcade (MVS)",
#endif
#if (EMU_SYSTEM == NCDZ)
		"Emulate Load Screen",
		"CD-ROM Speed Limit",
		"Enable CDDA",
		"CDDA Volume",
#endif

		"Key configuration menu",
		"Not use",
		FONT_UPARROW,
		FONT_DOWNARROW,
		FONT_LEFTARROW,
		FONT_RIGHTARROW,
		FONT_CIRCLE,
		FONT_CROSS,
		FONT_SQUARE,
		FONT_TRIANGLE,
		"L TRIGGER",
		"R TRIGGER",
		"START",
		"SELECT",
		"Low",
		"Normal",
		"High",
#if (EMU_SYSTEM == CPS2)
		"Player1 Start",
		"Player2 Start",
#endif
		"%d frame",
		"%d frames",
		FONT_UPARROW,
		FONT_DOWNARROW,
		FONT_LEFTARROW,
		FONT_RIGHTARROW,
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"Button A",
		"Button B",
		"Button C",
		"Button D",
#else
		"Button 1",
		"Button 2",
		"Button 3",
		"Button 4",
		"Button 5",
		"Button 6",
#endif
		"Start",
#if (EMU_SYSTEM == NCDZ)
		"Select",
#else
		"Coin",
		"Service Coin",
#endif
#if (EMU_SYSTEM == MVS)
		"Test Switch",
#elif (EMU_SYSTEM != NCDZ)
		"Service Switch",
#endif
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"Autofire A",
		"Autofire B",
		"Autofire C",
		"Autofire D",
#else
		"Autofire 1",
		"Autofire 2",
		"Autofire 3",
		"Autofire 4",
		"Autofire 5",
		"Autofire 6",
#endif
		"Autofire Interval",
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"Hotkey A+B",
		"Hotkey A+C",
		"Hotkey A+D",
		"Hotkey B+C",
		"Hotkey B+D",
		"Hotkey C+D",
		"Hotkey A+B+C",
		"Hotkey A+B+D",
		"Hotkey A+C+D",
		"Hotkey B+C+D",
		"Hotkey A+B+C+D",
#endif
#if (EMU_SYSTEM != NCDZ)
		"Analog Sensitivity",
#endif
#if (EMU_SYSTEM == CPS1)
		"Dial (Left)",
		"Dial (Right)",
		"Pause",
#endif
#if (EMU_SYSTEM == CPS2)
		"Paddle (Left)",
		"Paddle (Right)",
#endif
#if (EMU_SYSTEM == MVS)
		"Big",
		"Small",
		"Double Up",
		"Start/Collect",
		"Payout",
		"Cancel",
		"Bet",
		"Bet/Cancel All",
		"Operator Menu",
		"Clear Credit",
		"Hopper Out",
#endif
		"Save Screenshot",
		"Switch Player",
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"Button Layout",
		"Type 1 (NEOGEO PAD)",
		"Type 2 (MVS)",
		"Type 3 (PlaySation)",
		"User Define",
#endif

		"DIP switch settings menu",
		"This game has no DIP switches.",

		"Save/Load State",
		"Slot %d:",
		"Empty",
		"Load",
		"Delete",
		"Save",
		"Play Date",
		"Save Time",
		"Version",
		"Faild to delete file \"%s\".",

		"Main menu",
		"Game configuration",
		"Change game settings.",
		"Key configuration",
		"Change buttons and autofire/hotkey settings.",
#if (EMU_SYSTEM != NCDZ)
		"DIP switch settings",
		"Change hardware DIP switch settings.",
#endif
		"Reset emulation",
		"Reset " SYSTEM_NAME " emulation.",
		"Return to file browser",
		"Stop emulation and return to file browser.",
		"Return to game",
		"Return to game emulation.",
		"Exit emulator",
		"Exit emulation and return to PSP menu.",
#ifdef COMMAND_LIST
		"Show command list",
		"Show command list. (MAME Plus! format \"command.dat\")",
#endif

#if PSP_VIDEO_32BPP
		"Color settings menu",
		"Background image type",
		"Background image blightness",
		"Title bar text",
		"Selected text",
		"Normal text",
		"Information message text",
		"Warning message text",
		"File select bar (start)",
		"File select bar (end)",
		"Title bar/Message box",
		"Title bar/ Message box frame",
		"Background",
		"Red",
		"Green",
		"Blue",
		"Default image",
		"User's image",
		"logo.png only",
		"File select bar",
#endif

		"Command list",

		/* psp/mp3.c */
#if (EMU_SYSTEM == NCDZ)
		"Counld not reopen mp3 file \"%s\".",
		"MP3 decode error.",
		"Could not reserve audio channel for MP3.",
		"Could not start MP3 thread.",
#endif

		/* psp/adhoc.c*/
#ifdef ADHOC
		"lobby",
		"server",
		"crient",
		"Waiting for another PSP to join.\n",
		"Connecting to the %s.",
		"Connected.",
		"Disconnecting from the %s.",
		"Disconnected.",
		"failed.",
		"Select a server to connect to, or " FONT_TRIANGLE " to return.\n",
		"Waiting for %s to accept the connection.\n",
		"To cancel press " FONT_CROSS ".\n",
		"%s has requested a connection.\n",
		"To accept the connection press " FONT_CIRCLE ", to cancel press " FONT_CROSS ".\n",
		"Wainting for synchronization.",
#endif

		/* psp/png.c */
		"Could not allocate memory for PNG.",
		"Could not enecode PNG image.",
#if PSP_VIDEO_32BPP || (EMU_SYSTEM == NCDZ)
		"Could not decode PNG image.",
#endif
		"%d bit color PNG image not supported.",

		/* emumain.c */
		"Fatal error",
		"Snapshot saved as \"%s_%02d.png\".",

#if USE_CACHE
		/* common/cache.c */
		"Could not open cache file.\n",
		"Memory not enough.\n",
		"Could not allocate cache memory.\n",
		"%dKB cache allocated.\n",
		"Cache load error!!!\n",
		"Loading cache information data...\n",
		"Unsupported version of cache file \"V%c%c\".\n",
		"Current required version is \"" CACHE_VERSION "\".\n",
		"Please rebuild cache file.\n",
#if (EMU_SYSTEM == CPS2)
		"Could not open sprite block %03x\n",
#elif (EMU_SYSTEM == MVS)
		"PCM cache enabled.\n",
#endif
#endif

		/* common/loadrom.c */
		"Press any button.\n",
		"Could not allocate %s memory.\n",
		"CRC32 not correct. \"%s\"\n",
		"File not found. \"%s\"\n",

#ifdef SAVE_STATE
		/* common/state.c */
		"Could not allocate state buffer.",
		"Could not create file \"%s.sv%d\"",
		"Could not open file \"%s.sv%d\"",
		"Saving \"%s.sv%d\"",
		"Loading \"%s.sv%d\"",
#if (EMU_SYSTEM == MVS)
		"Could not reload BIOS. Press any button to exit.",
#elif (EMU_SYSTEM == NCDZ)
		"Could not compress state data.",
		"Could not uncompress state data.",
#endif
#endif

#ifdef COMMAND_LIST
		/* common/cmdlist.c */
		"Command list for this game not found.",
		"Command list - %s",
		"%d/%d items",
		"COMMAND.DAT size reduction",
		"This processing removes the command list of the games not being\n",
		"supported by this emulator from COMMAND.DAT.\n",
		"If you want to reduce the size of command.dat, press " FONT_CIRCLE " button.\n",
		"Otherwise, press " FONT_CROSS " button to return to file browser.\n",
		"Checking COMMAND.DAT format...\n",
		"ERROR: Unknown format.\n",
		"ERROR: Empty file.\n",
		"ERROR: Could not allocate memory.\n",
		"ERROR: Could not rename file.\n",
		"ERROR: Could create output file.\n",
		"Copying \"%s\"...\n",
		"Original size:%dbytes, Result:%dbytes (-%.1f%%)\n",
#endif

		/* emulation core */
		"Done.\n",
		"Exit emulation",
		"Please wait.\n",

		/* inptport.c */
		"Controller: Player 1",
		"Controller: Player %d",
#ifdef ADHOC
		"Lost sync.\n",
		"Paused by %s",
		"Return to Game",
		"Disconnect",
#endif

		/* memintrf.c */
		"Loading \"%s\"\n",
		"Load ROM",
#if (EMU_SYSTEM != NCDZ)
		"Checking ROM info...\n",
		"This game not supported.\n",
		"ROM not found. (zip file name incorrect)\n",
		"Driver for \"%s\" not found.\n",
		"ROM set \"%s\" (parent: %s)\n",
		"ROM set \"%s\"\n",
#endif

#if (EMU_SYSTEM == CPS1)

		/* memintrf.c */
		"rominfo.cps1 not found.\n",
		"Could not allocate memory. (0x8000 bytes)",

#elif (EMU_SYSTEM == CPS2)

		/* cps2crpt.c */
		"Decrypting %d%%\r",
		"Decrypting 100%%\n",

		/* memintrf.c */
		"rominfo.cps2 not found.\n",

#ifdef PSP_SLIM
		/* vidhrdw.c */
		"Decoding GFX...\n",
#endif

#elif (EMU_SYSTEM == MVS)

		/* biosmenu.c */
		"BIOS select menu",
		"BIOS not found.",
		"Select BIOS and press " FONT_CIRCLE " button.",
		"All NVRAM files are removed.\n",

		/* memintrf.c */
		"rominfo.mvs not found.\n",
		"Loading \"%s (%s)\"\n",
		"Loading decrypted GFX2 ROM...\n",
		"Loading decrypted SOUND1 ROM...\n",
		"Could not allocate memory for decrypt ROM.\n",
		"Could not allocate memory for sprite data.\n",
		"Try to use sprite cache...\n",
		"Checking BIOS...\n",

#elif (EMU_SYSTEM == NCDZ)

		/* ncdz.c */
		"Error while processing IPL.TXT.",
		"Reset caused by watchdog counter.",

		/* cdrom.c */
		"Could not open file \"%s\".",
		"Could not insert CD-ROM state data.",

		/* vidhrdw.c */
		"CD-ROM speed limit: Off",
		"CD-ROM speed limit: On",

		/* memintrf.c */
		"Checking game ID...\n",
#endif
		"Cheat menu",
		"Select cheat",

		"Memory free",
		"Mem free",
		NULL
	},
	{
		"\0",
		"\n",

		/* psp/filer.c */
		"‚µ‚Î‚ç‚­‚¨‘Ò‚¿‚­‚¾‚³‚¢B",
		"zipname.%s‚ğƒI[ƒvƒ“‚Å‚«‚Ü‚¹‚ñB",
#ifdef ADHOC
		"WLANƒXƒCƒbƒ`‚ğON‚É‚µ‚Ä‚­‚¾‚³‚¢B",
		"AdHocƒ‚ƒWƒ…[ƒ‹‚ğƒ[ƒho—ˆ‚Ü‚¹‚ñ‚Å‚µ‚½B",
#endif

		/* psp/sound.c */
		"ƒI[ƒfƒBƒIƒ`ƒƒƒlƒ‹‚ğŠm•Û‚Å‚«‚Ü‚¹‚ñB",
		"ƒTƒEƒ“ƒhƒXƒŒƒbƒh‚ğŠJn‚Å‚«‚Ü‚¹‚ñB",

		/* psp/ui.c */
		"Œx: ƒoƒbƒeƒŠ[‚Ìc—e—Ê‚ª‚ ‚è‚Ü‚¹‚ñ(%d%%)B[“d‚µ‚Ä‚­‚¾‚³‚¢I",

		FONT_CIRCLE " ‹N“®      " FONT_CROSS " ’†~",
		FONT_CIRCLE " ‘±s      " FONT_CROSS " ’†~",
#ifdef SAVE_STATE
		FONT_CIRCLE " ƒQ[ƒ€‚É–ß‚é   " FONT_CROSS " ƒƒjƒ…[‚É–ß‚é",
#endif
		"‰½‚©ƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚Ä‚­‚¾‚³‚¢B",
		"ƒGƒ~ƒ…ƒŒ[ƒVƒ‡ƒ“‚ğŠJn‚µ‚Ü‚·B",
#ifdef ADHOC
		"ƒGƒ~ƒ…ƒŒ[ƒVƒ‡ƒ“‚ğŠJn‚µ‚Ü‚·B(AdHoc)",
#endif
		"ƒGƒ~ƒ…ƒŒ[ƒVƒ‡ƒ“‚ğI—¹‚µ‚Ü‚·B",
		"ƒGƒ~ƒ…ƒŒ[ƒVƒ‡ƒ“‚ğƒŠƒZƒbƒg‚µ‚Ü‚·B",
		"ƒGƒ~ƒ…ƒŒ[ƒ^‚ğÄ‹N“®‚µ‚Ü‚·B",
#if (EMU_SYSTEM != NCDZ)
		"‚±‚ÌƒQ[ƒ€‚Í“®ì‚µ‚Ü‚¹‚ñB",
		"‚±‚ÌƒQ[ƒ€‚ÍŒ»ó‚Å‚Í‚Ç‚¤‚â‚Á‚Ä‚à³í‚É“®ì‚µ‚Ü‚¹‚ñ‚ªA",
		"‹C‚É‚µ‚È‚¢‚Å‚­‚¾‚³‚¢B",
#endif
		"‚±‚ÌƒfƒBƒŒƒNƒgƒŠ‚ğ‰ŠúƒfƒBƒŒƒNƒgƒŠ‚Éİ’è‚µ‚Ü‚·B",
#ifdef PSP_SLIM
		"‚±‚ÌƒvƒƒOƒ‰ƒ€‚ÍPSP-2000 + FW 3.71 M33ˆÈ~ê—p‚Å‚·B",
#endif
#ifdef SAVE_STATE
		"ƒXƒe[ƒgƒZ[ƒu‚ğŠJn‚µ‚Ü‚·B",
		"ƒXƒe[ƒgƒ[ƒh‚ğŠJn‚µ‚Ü‚·B",
#endif
#if defined(SAVE_STATE) || defined(COMMAND_LIST)
		"Š®—¹‚µ‚Ü‚µ‚½B",
#endif
#ifdef SAVE_STATE
		"ƒXƒe[ƒgƒf[ƒ^‚ğíœ‚µ‚Ü‚·B",
#endif
#if (EMU_SYSTEM == NCDZ)
		"MP3ƒtƒ@ƒCƒ‹‚ª–³‚¢ˆ×ACDDA‚ÍÄ¶‚³‚ê‚Ü‚¹‚ñB",
		"IPL.TXT‚ª‚ ‚è‚Ü‚¹‚ñB",
		"NEO¥GEO CDZ‚ÌBIOS‚ğ‹N“®‚µ‚Ü‚·B",
		"NEO¥GEO CDZ‚ÌBIOS(neocd.bin)‚ª‚ ‚è‚Ü‚¹‚ñB",
		"NEO¥GEO CDZ‚ÌBIOS‚ª–³Œø‚Å‚·B",
		"ƒQ[ƒ€‚ğ‹N“®‚Å‚«‚Ü‚¹‚ñB",
#endif

		"ƒwƒ‹ƒv - %s",
		"g—p‚µ‚Ü‚¹‚ñ",
		"‰½‚©ƒ{ƒ^ƒ“‚ğ‰Ÿ‚·‚Æƒƒjƒ…[‚É–ß‚è‚Ü‚·",
		"ƒtƒ@ƒCƒ‹ƒuƒ‰ƒEƒU",
		"ƒJ[ƒ\\ƒ‹‚ğˆÚ“®",
		"ƒJ[ƒ\\ƒ‹‚ğ1ƒy[ƒW•ªˆÚ“®",
		"ƒQ[ƒ€‚ğ‹N“®",
		"ƒQ[ƒ€‚ğ‹N“® (AdHoc)",
		"ƒGƒ~ƒ…ƒŒ[ƒ^‚ğI—¹",
		"BIOS‘I‘ğƒƒjƒ…[‚ğ•\\¦",
		"ƒJƒ‰[İ’èƒƒjƒ…[‚ğŠJ‚­",
		"‚±‚Ìƒwƒ‹ƒv‚ğŠJ‚­",
		"BIOS‚ğ‹N“®",
		"‘I‘ğ’†‚ÌƒfƒBƒŒƒNƒgƒŠ‚ğ‹N“®ƒfƒBƒŒƒNƒgƒŠ‚Éİ’è",
		"’l‚ğ•ÏX",
		"‘I‘ğ",
		"ƒXƒƒbƒg‚Ì‘I‘ğ",
		"ˆ—‚ğ•ÏX",
		"ˆ—‚ğÀs",
		"€–Ú‚Ì‘I‘ğ",
		"€–Ú‚Ì‘I‘ğ/’l‚Ì•ÏX",
		"RGB‚Ì’l‚ğ+5",
		"ƒtƒ@ƒCƒ‹ƒuƒ‰ƒEƒU‚É–ß‚é",
		"RGB‚Ì’l‚ğ-5",
		"‘S‚Ä‚Ì’l‚ğ•W€‚É–ß‚·",
		"ƒŠƒXƒg‚ğƒXƒNƒ[ƒ‹/€–Ú‚ğ‘I‘ğ",
		"€–Úƒƒjƒ…[‚ğŠJ‚­/•Â‚¶‚é",
		"ƒQ[ƒ€/ƒƒCƒ“ƒƒjƒ…[‚É–ß‚é",

		/* psp/ui_menu.c */
		"ƒIƒ“",
		"ƒIƒt",
		"‚Í‚¢",
		"‚¢‚¢‚¦",
		"—LŒø",
		"–³Œø",
		"ƒƒCƒ“ƒƒjƒ…[‚É–ß‚é",
		"ƒQ[ƒ€İ’èƒƒjƒ…[",
		"ƒ‰ƒXƒ^ƒGƒtƒFƒNƒg",
		"‰æ–ÊŠg‘å",
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		"360x270 (4:3)",
		"384x270 (24:17)",
		"480x270 (16:9)",
		"‰æ–Ê‚ğ‰ñ“]",
#else
		"360x270 (4:3)",
		"420x270 (14:9)",
		"480x270 (16:9)",
#endif
		"VBLANK“¯Šú",
		"©“®ƒtƒŒ[ƒ€ƒXƒLƒbƒv",
		"ƒtƒŒ[ƒ€ƒXƒLƒbƒv",
		"ƒŒƒxƒ‹1",
		"ƒŒƒxƒ‹2",
		"ƒŒƒxƒ‹3",
		"ƒŒƒxƒ‹4",
		"ƒŒƒxƒ‹5",
		"ƒŒƒxƒ‹6",
		"ƒŒƒxƒ‹7",
		"ƒŒƒxƒ‹8",
		"ƒŒƒxƒ‹9",
		"ƒŒƒxƒ‹10",
		"ƒŒƒxƒ‹11",
		"FPS•\\¦",
		"60fpsƒtƒŒ[ƒ€§ŒÀ",
		"ƒTƒEƒ“ƒhÄ¶",
		"ƒTƒ“ƒvƒ‹ƒŒ[ƒg",
		"11025Hz",
		"22050Hz",
		"44100Hz",
		"ƒTƒEƒ“ƒh‰¹—Ê",
		"0%",
		"10%",
		"20%",
		"30%",
		"40%",
		"50%",
		"60%",
		"70%",
		"80%",
		"90%",
		"100%",
		"ƒRƒ“ƒgƒ[ƒ‰",
		"ƒvƒŒƒCƒ„[1",
		"ƒvƒŒƒCƒ„[2",
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		"ƒvƒŒƒCƒ„[3",
		"ƒvƒŒƒCƒ„[4",
#endif
		"PSP CPUƒNƒƒbƒN",
		"222MHz",
		"266MHz",
		"300MHz",
		"333MHz",
#if (EMU_SYSTEM == MVS)
		"BIOS•W€",
#endif
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"ƒVƒXƒeƒ€’nˆæİ’è",
		"“ú–{",
		"ƒAƒƒŠƒJ",
		"ƒˆ[ƒƒbƒp",
#endif
#if (EMU_SYSTEM == MVS)
		"ƒVƒXƒeƒ€ƒ‚[ƒhİ’è",
		"‰Æ’ë—p (AES)",
		"ƒA[ƒP[ƒh (MVS)",
#endif
#if (EMU_SYSTEM == NCDZ)
		"ƒ[ƒh‰æ–ÊƒGƒ~ƒ…ƒŒ[ƒVƒ‡ƒ“",
		"CD-ROM‘¬“x§ŒÀ",
		"CDDAÄ¶",
		"CDDA‰¹—Ê",
#endif

		"“ü—Íƒ{ƒ^ƒ“İ’èƒƒjƒ…[",
		"g—p‚µ‚È‚¢",
		FONT_UPARROW,
		FONT_DOWNARROW,
		FONT_LEFTARROW,
		FONT_RIGHTARROW,
		FONT_CIRCLE,
		FONT_CROSS,
		FONT_SQUARE,
		FONT_TRIANGLE,
		"LƒgƒŠƒK",
		"RƒgƒŠƒK",
		"START",
		"SELECT",
		"’á",
		"•W€",
		"‚",
#if (EMU_SYSTEM == CPS2)
		"ƒvƒŒƒCƒ„[1 ƒXƒ^[ƒg",
		"ƒvƒŒƒCƒ„[2 ƒXƒ^[ƒg",
#endif
		"%dƒtƒŒ[ƒ€",
		"%dƒtƒŒ[ƒ€",
		FONT_UPARROW,
		FONT_DOWNARROW,
		FONT_LEFTARROW,
		FONT_RIGHTARROW,
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"ƒ{ƒ^ƒ“A",
		"ƒ{ƒ^ƒ“B",
		"ƒ{ƒ^ƒ“C",
		"ƒ{ƒ^ƒ“D",
#else
		"ƒ{ƒ^ƒ“1",
		"ƒ{ƒ^ƒ“2",
		"ƒ{ƒ^ƒ“3",
		"ƒ{ƒ^ƒ“4",
		"ƒ{ƒ^ƒ“5",
		"ƒ{ƒ^ƒ“6",
#endif
		"ƒXƒ^[ƒg",
#if (EMU_SYSTEM == NCDZ)
		"ƒZƒŒƒNƒg",
#else
		"ƒRƒCƒ“",
		"ƒT[ƒrƒXƒRƒCƒ“",
#endif
#if (EMU_SYSTEM == MVS)
		"ƒeƒXƒgƒXƒCƒbƒ`",
#elif (EMU_SYSTEM != NCDZ)
		"ƒT[ƒrƒXƒXƒCƒbƒ`",
#endif
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"ƒ{ƒ^ƒ“A ˜AË",
		"ƒ{ƒ^ƒ“B ˜AË",
		"ƒ{ƒ^ƒ“C ˜AË",
		"ƒ{ƒ^ƒ“D ˜AË",
#else
		"ƒ{ƒ^ƒ“1 ˜AË",
		"ƒ{ƒ^ƒ“2 ˜AË",
		"ƒ{ƒ^ƒ“3 ˜AË",
		"ƒ{ƒ^ƒ“4 ˜AË",
		"ƒ{ƒ^ƒ“5 ˜AË",
		"ƒ{ƒ^ƒ“6 ˜AË",
#endif
		"˜AËŠÔŠu",
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"AB “¯‰Ÿ‚µ",
		"AC “¯‰Ÿ‚µ",
		"AD “¯‰Ÿ‚µ",
		"BC “¯‰Ÿ‚µ",
		"BD “¯‰Ÿ‚µ",
		"CD “¯‰Ÿ‚µ",
		"ABC “¯‰Ÿ‚µ",
		"ABD “¯‰Ÿ‚µ",
		"ACD “¯‰Ÿ‚µ",
		"BCD “¯‰Ÿ‚µ",
		"ABCD “¯‰Ÿ‚µ",
#endif
#if (EMU_SYSTEM != NCDZ)
		"ƒAƒiƒƒOŠ´“x",
#endif
#if (EMU_SYSTEM == CPS1)
		"ƒ_ƒCƒAƒ‹ (¶‰ñ“])",
		"ƒ_ƒCƒAƒ‹ (‰E‰ñ“])",
		"ƒ|[ƒY",
#endif
#if (EMU_SYSTEM == CPS2)
		"ƒpƒhƒ‹ (¶‰ñ“])",
		"ƒpƒhƒ‹ (‰E‰ñ“])",
#endif
#if (EMU_SYSTEM == MVS)
		"ƒrƒbƒO",
		"ƒXƒ‚[ƒ‹",
		"ƒ_ƒuƒ‹ƒAƒbƒv",
		"ƒXƒ^[ƒg/WŒv",
		"•¥‚¢–ß‚µ",
		"ƒLƒƒƒ“ƒZƒ‹",
		"ƒxƒbƒg",
		"‘S‚Ä‚Éƒxƒbƒg/‘S‚ÄƒLƒƒƒ“ƒZƒ‹",
		"ƒIƒyƒŒ[ƒ^ƒƒjƒ…[",
		"ƒNƒŒƒWƒbƒg‚ğƒNƒŠƒA",
		"ƒzƒbƒp[æ‚èo‚µ",
#endif
		"ƒXƒNƒŠ[ƒ“ƒVƒ‡ƒbƒg•Û‘¶",
		"ƒvƒŒƒCƒ„[Ø‚è‘Ö‚¦",
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"ƒ{ƒ^ƒ“”z’u",
		"ƒ^ƒCƒv1 (NEO¥GEOƒpƒbƒh)",
		"ƒ^ƒCƒv2 (MVS)",
		"ƒ†[ƒU’è‹`",
#endif

		"ƒfƒBƒbƒvƒXƒCƒbƒ`İ’èƒƒjƒ…[",
		"‚±‚ÌƒQ[ƒ€‚É‚ÍƒfƒBƒbƒvƒXƒCƒbƒ`‚Í‚ ‚è‚Ü‚¹‚ñB",

		"ƒZ[ƒu/ƒ[ƒhƒXƒe[ƒg",
		"ƒXƒƒbƒg %d:",
		"ƒf[ƒ^‚È‚µ",
		"ƒ[ƒh",
		"íœ",
		"ƒZ[ƒu",
		"ƒvƒŒƒC“ú•t",
		"ƒZ[ƒu",
		"ƒo[ƒWƒ‡ƒ“",
		"ƒtƒ@ƒCƒ‹\"%s\"‚ğíœ‚Å‚«‚Ü‚¹‚ñ‚Å‚µ‚½B",

		"ƒƒCƒ“ƒƒjƒ…[",
		"ƒQ[ƒ€İ’è",
		"ƒQ[ƒ€‚Ì“®ìİ’è‚ğ•ÏX‚µ‚Ü‚·B",
		"“ü—Íƒ{ƒ^ƒ“İ’è",
		"“ü—Íƒ{ƒ^ƒ“‚Æ˜AË/ƒzƒbƒgƒL[‚Ìİ’è‚ğ‚µ‚Ü‚·B",
#if (EMU_SYSTEM != NCDZ)
		"ƒfƒBƒbƒvƒXƒCƒbƒ`İ’è",
		"ƒfƒBƒbƒvƒXƒCƒbƒ`‚Ìİ’è‚ğ•ÏX‚µ‚Ü‚·B",
#endif
		"ƒGƒ~ƒ…ƒŒ[ƒVƒ‡ƒ“‚ğƒŠƒZƒbƒg",
		SYSTEM_NAME "‚ÌƒGƒ~ƒ…ƒŒ[ƒVƒ‡ƒ“‚ğƒŠƒZƒbƒg‚µ‚Ü‚·B",
		"ƒtƒ@ƒCƒ‹ƒuƒ‰ƒEƒU‚É–ß‚é",
		"ƒGƒ~ƒ…ƒŒ[ƒVƒ‡ƒ“‚ğI—¹‚µAƒtƒ@ƒCƒ‹ƒuƒ‰ƒEƒU‚É–ß‚è‚Ü‚·B",
		"ƒQ[ƒ€‚É–ß‚é",
		"ƒQ[ƒ€‰æ–Ê‚É–ß‚è‚Ü‚·B",
		"ƒGƒ~ƒ…ƒŒ[ƒ^‚ğI—¹",
		"‚±‚Ìƒ\\ƒtƒgƒEƒFƒA‚ğI—¹‚µAPSP‚Ìƒƒjƒ…[‚É–ß‚è‚Ü‚·B",
#ifdef COMMAND_LIST
		"ƒRƒ}ƒ“ƒhƒŠƒXƒg•\\¦",
		"‚±‚ÌƒQ[ƒ€‚ÌƒRƒ}ƒ“ƒhˆê——‚ğ•\\¦‚µ‚Ü‚·B",
#endif

#if PSP_VIDEO_32BPP
		"ƒJƒ‰[İ’èƒƒjƒ…[",
		"”wŒi‰æ‘œ‚Ìí—Ş",
		"”wŒi‰æ‘œ‚Ì–¾‚é‚³",
		"ƒ^ƒCƒgƒ‹ƒo[‚Ì•¶š",
		"‘I‘ğ‚³‚ê‚½•¶š",
		"•W€‚Ì•¶š",
		"î•ñƒƒbƒZ[ƒW‚Ì•¶š",
		"ŒxƒƒbƒZ[ƒW‚Ì•¶š",
		"ƒtƒ@ƒCƒ‹‘I‘ğƒo[(ŠJn)",
		"ƒtƒ@ƒCƒ‹‘I‘ğƒo[(I—¹)",
		"ƒ^ƒCƒgƒ‹ƒo[/ƒƒbƒZ[ƒWƒ{ƒbƒNƒX",
		"ƒ^ƒCƒgƒ‹ƒo[/ƒƒbƒZ[ƒWƒ{ƒbƒNƒX‚Ì˜g",
		"”wŒiF(”wŒi‰æ‘œ–¢g—p)",
		"Ô‹P“x",
		"—Î‹P“x",
		"Â‹P“x",
		"•W€‚Ì‰æ‘œ",
		"ƒ†[ƒU‚Ì‰æ‘œ",
		"logo.png‚Ì‚İ",
		"ƒtƒ@ƒCƒ‹‘I‘ğƒo[",
#endif

		"ƒRƒ}ƒ“ƒhƒŠƒXƒg",

		/* psp/mp3.c */
#if (EMU_SYSTEM == NCDZ)
		"MP3ƒtƒ@ƒCƒ‹\"%s\"‚ğÄƒI[ƒvƒ“‚Å‚«‚Ü‚¹‚ñB",
		"MP3ƒfƒR[ƒhƒGƒ‰[‚ª”­¶‚µ‚Ü‚µ‚½B",
		"MP3Ä¶—pƒI[ƒfƒBƒIƒ`ƒƒƒlƒ‹‚ğŠm•Û‚Å‚«‚Ü‚¹‚ñB",
		"MP3ƒXƒŒƒbƒh‚ğŠJn‚Å‚«‚Ü‚¹‚ñB",
#endif

		/* psp/adhoc.c*/
#ifdef ADHOC
		"ƒƒr[",
		"ƒT[ƒo[",
		"ƒNƒ‰ƒCƒAƒ“ƒg",
		"‘¼‚ÌPSP‚ÌÚ‘±‚ğ‘Ò‹@’†B\n",
		"%s‚ÉÚ‘±’†...",
		"Ú‘±‚µ‚Ü‚µ‚½B",
		"%s‚©‚çØ’f’†...",
		"Ø’f‚µ‚Ü‚µ‚½B",
		"Ú‘±¸”s",
		"Ú‘±‚·‚éƒT[ƒo[‚ğ‘I‘ğB" FONT_TRIANGLE "‚ğ‰Ÿ‚·‚Æ’†~‚µ‚Ü‚·B\n",
		"%s‚ÌÚ‘±‹–‰Â‚ğ‘Ò‚Á‚Ä‚¢‚Ü‚·B\n",
		FONT_CROSS "‚ğ‰Ÿ‚·‚Æ’†~‚µ‚Ü‚·B\n",
		"%s‚ªÚ‘±‹–‰Â‚ğ—v‹‚µ‚Ä‚¢‚Ü‚·B\n",
		FONT_CIRCLE "‚ÅÚ‘±‚ğ‹–‰ÂA" FONT_CROSS "‚ÅÚ‘±‚ğ‹‘”Û‚µ‚Ü‚·B\n",
		"“¯Šú‚ğ‘Ò‚Á‚Ä‚¢‚Ü‚·B",
#endif

		/* psp/png.c */
		"PNG‰æ‘œ—p‚Ìƒƒ‚ƒŠ‚ğŠm•Û‚Å‚«‚Ü‚¹‚ñB",
		"PNG‰æ‘œ‚ğì¬‚Å‚«‚Ü‚¹‚ñB",
#if PSP_VIDEO_32BPP || (EMU_SYSTEM == NCDZ)
		"PNG‰æ‘œ‚ğ“WŠJ‚Å‚«‚Ü‚¹‚ñB",
#endif
		"%dbitƒJƒ‰[‚ÌPNG‰æ‘œ‚É‚Í‘Î‰‚µ‚Ä‚¢‚Ü‚¹‚ñB",

		/* emumain.c */
		"’v–½“I‚ÈƒGƒ‰[",
		"ƒXƒNƒŠ[ƒ“ƒVƒ‡ƒbƒg‚ğ\"%s_%02d.png\"‚É•Û‘¶‚µ‚Ü‚µ‚½B",

#if USE_CACHE
		/* common/cache.c */
		"ƒLƒƒƒbƒVƒ…ƒtƒ@ƒCƒ‹‚ğƒI[ƒvƒ“‚Å‚«‚Ü‚¹‚ñB\n",
		"ƒƒ‚ƒŠ‚ª‘«‚è‚Ü‚¹‚ñB\n",
		"ƒLƒƒƒbƒVƒ…ƒƒ‚ƒŠ‚ğŠm•Û‚Å‚«‚Ü‚¹‚ñB\n",
		"%dKB‚ÌƒLƒƒƒbƒVƒ…ƒƒ‚ƒŠ‚ğŠm•Û‚µ‚Ü‚µ‚½B\n",
		"ƒLƒƒƒbƒVƒ…ƒ[ƒh’†‚ÉƒGƒ‰[‚ª”­¶‚µ‚Ü‚µ‚½B\n",
		"ƒLƒƒƒbƒVƒ…î•ñƒf[ƒ^‚ğƒ[ƒh’†...\n",
		"ƒTƒ|[ƒg‚³‚ê‚Ä‚¢‚È‚¢ƒo[ƒWƒ‡ƒ“\"V%c%c\"‚ÌƒLƒƒƒbƒVƒ…ƒtƒ@ƒCƒ‹‚Å‚·B\n",
		"Œ»İ—v‹‚³‚ê‚éƒLƒƒƒbƒVƒ…ƒtƒ@ƒCƒ‹‚Ìƒo[ƒWƒ‡ƒ“‚Í\"" CACHE_VERSION "\"‚Å‚·B\n",
		"ƒLƒƒƒbƒVƒ…ƒtƒ@ƒCƒ‹‚ğì¬‚µ‚È‚¨‚µ‚Ä‚­‚¾‚³‚¢B\n",
#if (EMU_SYSTEM == CPS2)
		"ƒXƒvƒ‰ƒCƒgƒuƒƒbƒN%03x‚ğƒI[ƒvƒ“‚Å‚«‚Ü‚¹‚ñB\n",
#elif (EMU_SYSTEM == MVS)
		"PCMƒLƒƒƒbƒVƒ…‚ğg—p‚µ‚Ü‚·B\n",
#endif
#endif

		/* common/loadrom.c */
		"‰½‚©ƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚Ä‚­‚¾‚³‚¢B\n",
		"ƒƒ‚ƒŠ‚ğŠm•Û‚Å‚«‚Ü‚¹‚ñB(REGION_%s)\n",
		"CRC32‚ª³‚µ‚­‚ ‚è‚Ü‚¹‚ñB\"%s\"\n",
		"ƒtƒ@ƒCƒ‹‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñB\"%s\"\n",

#ifdef SAVE_STATE
		/* common/state.c */
		"ƒXƒe[ƒg—p‚Ìƒoƒbƒtƒ@‚ğŠm•Û‚Å‚«‚Ü‚¹‚ñB",
		"ƒtƒ@ƒCƒ‹\"%s.sv%d\"‚ğì¬‚Å‚«‚Ü‚¹‚ñB",
		"ƒtƒ@ƒCƒ‹\"%s.sv%d\"‚ğƒI[ƒvƒ“‚Å‚«‚Ü‚¹‚ñB",
		"\"%s.sv%d\"‚ğ•Û‘¶’†",
		"\"%s.sv%d\"‚ğƒ[ƒh’†",
#if (EMU_SYSTEM == MVS)
		"BIOS‚ğƒ[ƒh‚Å‚«‚Ü‚¹‚ñB‰½‚©ƒ{ƒ^ƒ“‚ğ‰Ÿ‚·‚ÆI—¹‚µ‚Ü‚·B",
#elif (EMU_SYSTEM == NCDZ)
		"ƒXƒe[ƒgƒf[ƒ^‚ğˆ³k‚Å‚«‚Ü‚¹‚ñB",
		"ƒXƒe[ƒgƒf[ƒ^‚ğ“WŠJ‚Å‚«‚Ü‚¹‚ñB",
#endif
#endif

#ifdef COMMAND_LIST
		/* common/cmdlist.c */
		"‚±‚ÌƒQ[ƒ€—p‚ÌƒRƒ}ƒ“ƒhƒŠƒXƒg‚ª‚ ‚è‚Ü‚¹‚ñB",
		"ƒRƒ}ƒ“ƒhƒŠƒXƒg - %s",
		"%d/%d €–Ú",
		"COMMAND.DAT ƒTƒCƒYk¬ˆ—",
		"‚±‚ÌƒGƒ~ƒ…ƒŒ[ƒ^‚Å‘Î‰‚µ‚Ä‚¢‚È‚¢ƒQ[ƒ€‚ÌƒRƒ}ƒ“ƒhƒŠƒXƒg‚ğíœ‚µA\n",
		"COMMAND.DAT‚Ìƒtƒ@ƒCƒ‹ƒTƒCƒY‚ğk¬‚µ‚Ü‚·B\n",
		"ˆ—‚ğs‚¤ê‡‚ÍA" FONT_CIRCLE "ƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚Ä‚­‚¾‚³‚¢B\n",
		FONT_CROSS "ƒ{ƒ^ƒ“‚ğ‰Ÿ‚·‚Æˆ—‚ğ’†~‚µ‚Äƒtƒ@ƒCƒ‹ƒuƒ‰ƒEƒU‚É–ß‚è‚Ü‚·B\n",
		"COMMAND.DAT ‚ğŒŸ¸’†...\n",
		"ƒGƒ‰[: –¢‘Î‰‚Ì\"COMMAND.DAT\"‚©ƒtƒ@ƒCƒ‹‚ÌŒ`®‚ªˆÙ‚È‚è‚Ü‚·B\n",
		"ƒGƒ‰[: ‹ó‚Ìƒtƒ@ƒCƒ‹‚Å‚·B\n",
		"ƒGƒ‰[: ƒƒ‚ƒŠ‚ğŠm•Û‚Å‚«‚Ü‚¹‚ñB\n",
		"ƒGƒ‰[: ƒtƒ@ƒCƒ‹‚ğƒŠƒl[ƒ€‚Å‚«‚Ü‚¹‚ñB\n",
		"ƒGƒ‰[: o—Íƒtƒ@ƒCƒ‹‚ğì¬‚Å‚«‚Ü‚¹‚ñB\n",
		"\"%s\" ‚ÌƒRƒ}ƒ“ƒh‚ğƒRƒs[’†...\n",
		"ƒTƒCƒY %dbytes ¨ %dbytes (%.1f%%Œ¸)\n",
#endif

		/* emulation core */
		"Š®—¹‚µ‚Ü‚µ‚½B\n",
		"ƒGƒ~ƒ…ƒŒ[ƒVƒ‡ƒ“‚ÌI—¹",
		"‚µ‚Î‚ç‚­‚¨‘Ò‚¿‚­‚¾‚³‚¢B\n",

		/* inptport.c */
		"ƒRƒ“ƒgƒ[ƒ‰[: ƒvƒŒƒCƒ„[1",
		"ƒRƒ“ƒgƒ[ƒ‰[: ƒvƒŒƒCƒ„[%d",
#ifdef ADHOC
		"“¯Šú‚ğ¸‚¢‚Ü‚µ‚½B\n",
		"%s‚É‚æ‚éƒ|[ƒY",
		"ƒQ[ƒ€‚ğÄŠJ",
		"Ø’f‚µ‚ÄI—¹",
#endif

		/* memintrf.c */
		"ƒ[ƒh’† \"%s\"\n",
		"ROM‚Ìƒ[ƒh",
#if (EMU_SYSTEM != NCDZ)
		"ROMî•ñ‚ğƒ`ƒFƒbƒN’†...\n",
		"‚±‚ÌƒQ[ƒ€‚ÍƒTƒ|[ƒg‚³‚ê‚Ä‚¢‚Ü‚¹‚ñB\n",
		"ROM‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñB(zipƒtƒ@ƒCƒ‹–¼‚ª³‚µ‚­‚È‚¢)\n",
		"\"%s\"‚Ìƒhƒ‰ƒCƒo‚ª‚ ‚è‚Ü‚¹‚ñB\n",
		"ROMƒZƒbƒg–¼\"%s\" (eƒZƒbƒg–¼: %s)\n",
		"ROMƒZƒbƒg–¼\"%s\"\n",
#endif

#if (EMU_SYSTEM == CPS1)

		/* memintrf.c */
		"rominfo.cps1‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñB\n",
		"ƒƒ‚ƒŠ‚ğŠm•Û‚Å‚«‚Ü‚¹‚ñB(0x8000ƒoƒCƒg)",

#elif (EMU_SYSTEM == CPS2)

		/* cps2crpt.c */
		"•œ†ˆ—‚ğÀs’† %d%%\r",
		"•œ†ˆ—‚ğÀs’† 100%%\n",

		/* memintrf.c */
		"rominfo.cps2‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñB\n",

#ifdef PSP_SLIM
		/* vidhrdw.c */
		"ƒOƒ‰ƒtƒBƒbƒNƒf[ƒ^‚ğ“WŠJ’†...\n",
#endif

#elif (EMU_SYSTEM == MVS)

		/* biosmenu.c */
		"BIOS‘I‘ğƒƒjƒ…[",
		"BIOS‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñB",
		"BIOS‚ğ‘I‘ğ‚µA" FONT_CIRCLE "ƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚Ä‚­‚¾‚³‚¢B",
		"NVRAMƒtƒ@ƒCƒ‹‚Í‘S‚Äíœ‚³‚ê‚Ü‚µ‚½B\n",

		/* memintrf.c */
		"rominfo.mvs‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñB\n",
		"ƒ[ƒh’† \"%s (%s)\"\n",
		"•œ†Ï‚İGFX2 ROM‚ğƒ[ƒh’†...\n",
		"•œ†Ï‚İSOUND1 ROM‚ğƒ[ƒh’†...\n",
		"ROM•œ†—p‚Ìƒƒ‚ƒŠ‚ğŠm•Û‚Å‚«‚Ü‚¹‚ñB\n",
		"ƒXƒvƒ‰ƒCƒgƒf[ƒ^—p‚Ìƒƒ‚ƒŠ‚ğŠm•Û‚Å‚«‚Ü‚¹‚ñB\n",
		"ƒXƒvƒ‰ƒCƒgƒLƒƒƒbƒVƒ…‚Ìg—p‚ğ‚İ‚Ü‚·B\n",
		"BIOS‚ğƒ`ƒFƒbƒN’†...\n",

#elif (EMU_SYSTEM == NCDZ)

		/* ncdz.c */
		"IPL.TXTˆ—’†‚ÉƒGƒ‰[‚ª”­¶‚µ‚Ü‚µ‚½B",
		"ƒEƒHƒbƒ`ƒhƒbƒOƒJƒEƒ“ƒ^‚É‚æ‚éƒŠƒZƒbƒg‚ª”­¶‚µ‚Ü‚µ‚½B",

		/* cdrom.c */
		"ƒtƒ@ƒCƒ‹\"%s\"‚ğƒI[ƒvƒ“‚Å‚«‚Ü‚¹‚ñB",
		"CD-ROM‚ÌƒXƒe[ƒgƒf[ƒ^‚ğ’Ç‰Á‚Å‚«‚Ü‚¹‚ñB",

		/* vidhrdw.c */
		"CD-ROM‘¬“x§ŒÀ: –³Œø",
		"CD-ROM‘¬“x§ŒÀ: —LŒø",

		/* memintrf.c */
		"ƒQ[ƒ€ID‚ğƒ`ƒFƒbƒN’†...\n",
#endif
		"ƒ`[ƒgƒƒjƒ…[",
		"ƒ`[ƒg‚ğ‘I‘ğ‚µ‚Ü‚·B",

		"ƒƒ‚ƒŠ‰ğ•ú",
		"ƒƒ‚ƒŠ‰ğ•úİ’è‚ğ•ÏX‚µ‚Ü‚·B",
		NULL
	},
	{
		"\0",
		"\n",

		/* psp/filer.c */
		"Por favor espere...",
		"No se pudo abrir zipname.%s",
#ifdef ADHOC
		"Por favor, active el interruptor WLAN.",
		"No se pudo cargar los modulos AdHoc.",
#endif

		/* psp/sound.c */
		"No se puede reservar canal de audio para el sonido.",
		"No se pudo iniciar la tarea de sonido.",

		/* psp/ui.c */
		"Advertencia: La bateria esta baja (%d%%). Por favor, carga la bateria!",

		FONT_CIRCLE " para cargar, " FONT_CROSS " para cancelar",
		FONT_CIRCLE " para confirmar, " FONT_CROSS " para cancelar",
#ifdef SAVE_STATE
		FONT_CIRCLE " regresar al juego, " FONT_CROSS " regresar al menu",
#endif
		"Pulsa cualquier boton.",
		"Comenzar emulacion.",
#ifdef ADHOC
		"Comenzar emulacion. (AdHoc)",
#endif
		"Salir del emulador.",
		"Resetear la emulacion.",
		"Es necesario reiniciar la emulacion.",
#if (EMU_SYSTEM != NCDZ)
		"ESTE JEUGO NO FUNCIONA.",
		"No sera capaz de hacer que funcione correctamente.",
		"No se moleste.",
#endif
		"Convertir este directorio en el de inicio?",
#ifdef PSP_SLIM
		"Este programa requiere PSP-2000 + FW 3.71 M33 o posterior.",
#endif
#ifdef SAVE_STATE
		"Iniciar el guardardado.",
		"Iniciar la carga.",
#endif
#if defined(SAVE_STATE) || defined(COMMAND_LIST)
		"Completo.",
#endif
#ifdef SAVE_STATE
		"Borrar archivo de esatado.",
#endif
#if (EMU_SYSTEM == NCDZ)
		"Los archivos MP3 no se encuentran. CDDA no pueden reproducir.",
		"IPL.TXT no enconrado.",
		"Boot NEOï¿½GEO CDZ BIOS.",
		"NEOï¿½GEO CDZ BIOS (neocd.bin) no encontrado.",
		"Encontrado NEOï¿½GEO CDZ BIOS invalido.",
		"No se puede iniciar juego.",
#endif

		"Ayuda - %s",
		"No usar",
		"Pulsa cualquier boton para regresar al menu.",
		"Explorador de archivos",
		"Desplazar",
		"Desplazar 1 pagina",
		"Cargar juego",
		"Cargar juego (AdHoc)",
		"Salir del emulador",
		"Mostrar menu de seleccion de BIOS",
		"Abrir el menu de ajustes de color",
		"Muestra esta ayuda",
		"Boot BIOS",
		"Fija el directorio seleccionado como inicial",
		"Cambiar valor",
		"Seleccionar",
		"Seleccionar slot",
		"Cambiar funcion",
		"Ejecutar funcion",
		"Seleccionar item",
		"Seleccionar item / Cambiar valor",
		"Valor RGB +5",
		"Regresar al explorador de archivos",
		"Valor RGB -5",
		"Restaurar todos los valores por defecto",
		"Desplazar / Seleccionar item",
		"Abrir / Cerrar elemento del menu",
		"Regresar al menu principal / juego",

		/* psp/ui_menu.c */
		"On",
		"Off",
		"Si",
		"No",
		"Habilitar",
		"Inhabilitar",
		"Regresar al menu principal",
		"Menu de configuracion del juego",
		"Raster Effects",
		"Estirar Pantalla",
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		"360x270 (4:3)",
		"384x270 (24:17)",
		"480x270 (16:9)",
		"Rotar Pantalla",
#else
		"360x270 (4:3)",
		"420x270 (14:9)",
		"480x270 (16:9)",
#endif
		"Video Sync",
		"Frameskip Automatico",
		"Frameskip",
		"nivel 1",
		"nivel 2",
		"nivel 3",
		"nivel 4",
		"nivel 5",
		"nivel 6",
		"nivel 7",
		"nivel 8",
		"nivel 9",
		"nivel 10",
		"nivel 11",
		"Mostrar FPS",
		"Limitar a 60fps",
		"Habilinitar Sonido",
		"Frecuencia de muestreo",
		"11025Hz",
		"22050Hz",
		"44100Hz",
		"Volumen del sonido",
		"0%",
		"10%",
		"20%",
		"30%",
		"40%",
		"50%",
		"60%",
		"70%",
		"80%",
		"90%",
		"100%",
		"Control",
		"Jugador 1",
		"Jugador 2",
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		"Jugador 3",
		"Jugador 4",
#endif
		"PSP clock",
		"222MHz",
		"266MHz",
		"300MHz",
		"333MHz",
#if (EMU_SYSTEM == MVS)
		"Default",
#endif
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"Machine Region",
		"Japan",
		"USA",
		"Europe",
#endif
#if (EMU_SYSTEM == MVS)
		"Machine Mode",
		"Console (AES)",
		"Arcade (MVS)",
#endif
#if (EMU_SYSTEM == NCDZ)
		"Emular la pantalla de carga",
		"Velicida limite de CD-ROM",
		"Habilitar CDDA",
		"CDDA Volume",
#endif

		"Menu de configuracion de botones",
		"Not use",
		FONT_UPARROW,
		FONT_DOWNARROW,
		FONT_LEFTARROW,
		FONT_RIGHTARROW,
		FONT_CIRCLE,
		FONT_CROSS,
		FONT_SQUARE,
		FONT_TRIANGLE,
		"L TRIGGER",
		"R TRIGGER",
		"START",
		"SELECT",
		"Bajo",
		"Normal",
		"Alto",
#if (EMU_SYSTEM == CPS2)
		"Jugador1 Start",
		"Jugador2 Start",
#endif
		"%d frame",
		"%d frames",
		FONT_UPARROW,
		FONT_DOWNARROW,
		FONT_LEFTARROW,
		FONT_RIGHTARROW,
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"Boton A",
		"Boton B",
		"Boton C",
		"Boton D",
#else
		"Boton 1",
		"Boton 2",
		"Boton 3",
		"Boton 4",
		"Boton 5",
		"Boton 6",
#endif
		"Start",
#if (EMU_SYSTEM == NCDZ)
		"Select",
#else
		"Moneda",
		"Moneda de Servicio",
#endif
#if (EMU_SYSTEM == MVS)
		"Interruptor de prueba",
#elif (EMU_SYSTEM != NCDZ)
		"Interruptor de Servicio",
#endif
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"Autodisparo A",
		"Autodisparo B",
		"Autodisparo C",
		"Autodisparo D",
#else
		"Autodisparo 1",
		"Autodisparo 2",
		"Autodisparo 3",
		"Autodisparo 4",
		"Autodisparo 5",
		"Autodisparo 6",
#endif
		"Intervalo de Autodisparo",
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"Hotkey A+B",
		"Hotkey A+C",
		"Hotkey A+D",
		"Hotkey B+C",
		"Hotkey B+D",
		"Hotkey C+D",
		"Hotkey A+B+C",
		"Hotkey A+B+D",
		"Hotkey A+C+D",
		"Hotkey B+C+D",
		"Hotkey A+B+C+D",
#endif
#if (EMU_SYSTEM != NCDZ)
		"Selsibilidar del Analogo",
#endif
#if (EMU_SYSTEM == CPS1)
		"Dial (Izquierda)",
		"Dial (Derecha)",
		"Pausa",
#endif
#if (EMU_SYSTEM == CPS2)
		"Paddle (Izquierda)",
		"Paddle (Derecha)",
#endif
#if (EMU_SYSTEM == MVS)
		"Big",
		"Small",
		"Double Up",
		"Start/Collect",
		"Payout",
		"Cancel",
		"Bet",
		"Bet/Cancel All",
		"Operator Menu",
		"Clear Credit",
		"Hopper Out",
#endif
		"Captura de pantalla",
		"Cambiar Jugador",
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"Plantilla de Botones",
		"Tipo 1 (NEOGEO PAD)",
		"Tipo 2 (MVS)",
		"Tipo 3 (PlayStation)",
		"Definido por el usuario",
#endif

		"Menu de confiracion DIP",
		"Este juego no tiene DIP switches.",

		"Guardar/Cargar State",
		"Slot %d:",
		"Vacio",
		"Cargar",
		"Borrar",
		"Guardar",
		"Fecha",
		"Hora",
		"Version",
		"Fallo al borrar archivo \"%s\".",

		"Menu principal",
		"Configuracion del Juego",
		"Cambiar ajustes del juego.",
		"Configuracion de botones",
		"Cambiar configuracion de botones autodisparo/hotkey.",
#if (EMU_SYSTEM != NCDZ)
		"Configuracion DIP switch",
		"Cambiar la configuracion DIP swich de hardware.",
#endif
		"Reiniciar emulacion",
		"Reinicia la emulacion de "SYSTEM_NAME,
		"Regresar al explorador de archivos",
		"Detiene la emulacion y regresa al explorador de archivos.",
		"Regresar al juego",
		"Regresar a la emulacion del juego",
		"Salir del emulador",
		"Salir del emulador y volver al menu de PSP.",
#ifdef COMMAND_LIST
		"Mostrar lista de comandos",
		"Mostrar lista de comando. (MAME Plus! format \"command.dat\")",
#endif

#if PSP_VIDEO_32BPP
		"Menu de ajustes de color",
		"Tipo de imagen de fondo",
		"Brillo de imagen de fondo",
		"Texto de barra de titulo",
		"Texto seleccionado",
		"Texto normal",
		"Texto de mensajes de informacion",
		"Texto de mensajes de advertencia",
		"Barra de seleccion de archivo (inico)",
		"Barra de seleccion de archivo (fin)",
		"Barra de titulo/Message box",
		"Barra de titulo/ Message box frame",
		"Fondo",
		"Rojo",
		"Verde",
		"Azul",
		"Imagen por defecto",
		"Imagen del usuario",
		"solo logo.png",
		"Barra de seleccion de archivo",
#endif

		"Lista de comandos",

		/* psp/mp3.c */
#if (EMU_SYSTEM == NCDZ)
		"No se pudo entrar de nuevo a archivo mp3 \"%s\".",
		"Error al decodificar MP3.",
		"No se pudo reservar el canal de audio para MP3.",
		"No se pudo iniciar la tarea de MP3.",
#endif

		/* psp/adhoc.c*/
#ifdef ADHOC
		"lobby",
		"server",
		"crient",
		"Esperando a que otra PSP se una.\n",
		"Conectando a %s.",
		"Conectado.",
		"Desconectar de %s.",
		"Desconectado.",
		"fracasado.",
		"Seleccione un servidor al que conectarse, o pulse " FONT_TRIANGLE " para regresar.\n",
		"Esperando a que %s acepte la coneccion.\n",
		"Para cancelar pulsa " FONT_CROSS ".\n",
		"%s has requested a connection.\n",
		"Para aceptar la conexion pulsa " FONT_CIRCLE ", para cancelar pulsa " FONT_CROSS ".\n",
		"Esperando para la sincronizacion.",
#endif

		/* psp/png.c */
		"No se pudo asignar memoria para PNG.",
		"No se pudo codificar la imagen PNG.",
#if PSP_VIDEO_32BPP || (EMU_SYSTEM == NCDZ)
		"No se pudo decodificar la imagen PNG.",
#endif
		"%d bit color imagen PNG no soportada.",

		/* emumain.c */
		"Error fatal",
		"Capura guardada como \"%s_%02d.png\".",

#if USE_CACHE
		/* common/cache.c */
		"No se pudo abrir archivo de cache.\n",
		"No hay suficiente memoria.\n",
		"No se pudo asignar memoria cache.\n",
		"%dKB cache alojado.\n",
		"Error de carga de cache!!!\n",
		"Cargando informacion de tatos de cache...\n",
		"Version de cache no soportada \"V%c%c\".\n",
		"Version actual requerida \"" CACHE_VERSION "\".\n",
		"Por favor, reconstruir la cache de los archivos.\n",
#if (EMU_SYSTEM == CPS2)
		"No se pudo abrir bloque de sprite %03x\n",
#elif (EMU_SYSTEM == MVS)
		"PCM cache habilitado.\n",
#endif
#endif

		/* common/loadrom.c */
		"Pulse cualquier boton.\n",
		"No ha podido asignar %s memory.\n",
		"CRC32 incorrecto. \"%s\"\n",
		"Archivo no encontrado. \"%s\"\n",

#ifdef SAVE_STATE
		/* common/state.c */
		"No se pudo asignar un bufger de estado.",
		"No se pudo crear el archivo \"%s.sv%d\"",
		"No se pudo abrir el archivo \"%s.sv%d\"",
		"Guardando \"%s.sv%d\"",
		"Cargando \"%s.sv%d\"",
#if (EMU_SYSTEM == MVS)
		"No se puedo cargar la BIOS. Pulse cualquier boton para salir.",
#elif (EMU_SYSTEM == NCDZ)
		"No se pueden comprimir los datos de estado.",
		"No se pueden descomprimir los datos de estado.",
#endif
#endif

#ifdef COMMAND_LIST
		/* common/cmdlist.c */
		"La lista de comando para este juejo no se encontro.",
		"Lista de comando - %s",
		"%d/%d items",
		"Reduccion de tamano de COMMAND.DAT",
		"Este proceso elimina la lista de comandos de los juegos que no cuentan\n",
		"con el soporte de este emulador de COMMAND.DAT.\n",
		"Si desea reducir el tamaemulaciono de command.dat, pulse el boton" FONT_CIRCLE "\n",
		"De lo contrario, pulse el boton " FONT_CROSS " para regresar al explorador de archivos.\n",
		"Verifidando el formato de COMMAND.DAT...\n",
		"ERROR: Formato desconocido.\n",
		"ERROR: Archivo vacio.\n",
		"ERROR: No se pudo asignar memoria.\n",
		"ERROR: No se pudo cambiar el nombre de archivo.\n",
		"ERROR: Se pudo crear el archivo de salida.\n",
		"Copiando \"%s\"...\n",
		"Tamano original:%dbytes, Resultado:%dbytes (-%.1f%%)\n",
#endif

		/* emulation core */
		"Terminado.\n",
		"Salir de la emulacion",
		"Por favor espere.\n",

		/* inptport.c */
		"Controller: Player 1",
		"Controller: Player %d",
#ifdef ADHOC
		"Sicronizacion perdida.\n",
		"Pausado por %s",
		"Regresar al juego",
		"Desconectar",
#endif

		/* memintrf.c */
		"Cargando \"%s\"\n",
		"Carga de ROM",
#if (EMU_SYSTEM != NCDZ)
		"Verificando informacion del ROM...\n",
		"Este juego no esta soportado.\n",
		"ROM no encontrado. (zip nombre de archivo incorrecto)\n",
		"Driver para \"%s\" no encontrado.\n",
		"ROM set \"%s\" (parent: %s)\n",
		"ROM set \"%s\"\n",
#endif

#if (EMU_SYSTEM == CPS1)

		/* memintrf.c */
		"rominfo.cps1 no encontrado.\n",
		"No se pudo asignar memoria. (0x8000 bytes)",

#elif (EMU_SYSTEM == CPS2)

		/* cps2crpt.c */
		"Des-encriptando %d%%\r",
		"Des-encriptando 100%%\n",

		/* memintrf.c */
		"rominfo.cps2 no encontrado.\n",

#ifdef PSP_SLIM
		/* vidhrdw.c */
		"Decodificando GFX...\n",
#endif

#elif (EMU_SYSTEM == MVS)

		/* biosmenu.c */
		"Menu de seleccion de BIOS",
		"BIOS no encontrada.",
		"Selecione la BIOS y pulse el boton " FONT_CIRCLE ,
		"Todos los archivos NVRAM se borraron.\n",

		/* memintrf.c */
		"rominfo.mvs no encontrado.\n",
		"Cargando \"%s (%s)\"\n",
		"Cargando GFX2 ROM desencriptado...\n",
		"Cargando SOUND1 ROM desencriptado...\n",
		"No se pudo asignar memoria para desencriptar ROM.\n",
		"No se pudo asignar memoria para los datos de sprite.\n",
		"Trate de usar cache de sprite...\n",
		"Verificando BIOS...\n",

#elif (EMU_SYSTEM == NCDZ)

		/* ncdz.c */
		"Error al procesar IPL.TXT.",
		"Reset caused by watchdog counter.",

		/* cdrom.c */
		"No se pudo abrir el archivo \"%s\".",
		"No se pudo insertar los datos del estado de CD-ROM.",

		/* vidhrdw.c */
		"Velocidad limite de CD-ROM: Off",
		"Velocidad limite de CD-ROM: On",

		/* memintrf.c */
		"Verificando ID de juego...\n",
#endif
		"Cheat menu",
		"Select cheat",

		"Memory free",
		"Mem free",
		NULL
	}
};


static int lang = 0;


void ui_text_init(void)
{
	int i;

	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &lang);

	if (lang == PSP_SYSTEMPARAM_LANGUAGE_JAPANESE)
		lang = 1;
	else if (lang == PSP_SYSTEMPARAM_LANGUAGE_SPANISH)
		lang = 2;

	for (i = 0; i < UI_TEXT_MAX; i++)
		ui_text[i] = text[lang][i];
}

int ui_text_get_language(void)
{
	return lang;
}
