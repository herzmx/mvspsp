/******************************************************************************

	ui_text.c

	���[�U�C���^�t�F�[�X�e�L�X�g�Ǘ�

******************************************************************************/

#include "psp.h"
#include <psputility_sysparam.h>


const char *ui_text[UI_TEXT_MAX];

static const char *text_ENGLISH[UI_TEXT_MAX] =
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
		"Boot NEO�GEO CDZ BIOS.",
		"NEO�GEO CDZ BIOS (neocd.bin) not found.",
		"Invalid NEO�GEO CDZ BIOS found.",
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
		"Type 3 (PlayStation)",
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
};
static const char *text_JAPANESE[UI_TEXT_MAX] =
{
		"\0",
		"\n",

		/* psp/filer.c */
		"���΂炭���҂����������B",
		"zipname.%s���I�[�v���ł��܂���B",
#ifdef ADHOC
		"WLAN�X�C�b�`��ON�ɂ��Ă��������B",
		"AdHoc���W���[�������[�h�o���܂���ł����B",
#endif

		/* psp/sound.c */
		"�I�[�f�B�I�`���l�����m�ۂł��܂���B",
		"�T�E���h�X���b�h���J�n�ł��܂���B",

		/* psp/ui.c */
		"�x��: �o�b�e���[�̎c�e�ʂ�����܂���(%d%%)�B�[�d���Ă��������I",

		FONT_CIRCLE " �N��      " FONT_CROSS " ���~",
		FONT_CIRCLE " ���s      " FONT_CROSS " ���~",
#ifdef SAVE_STATE
		FONT_CIRCLE " �Q�[���ɖ߂�   " FONT_CROSS " ���j���[�ɖ߂�",
#endif
		"�����{�^���������Ă��������B",
		"�G�~�����[�V�������J�n���܂��B",
#ifdef ADHOC
		"�G�~�����[�V�������J�n���܂��B(AdHoc)",
#endif
		"�G�~�����[�V�������I�����܂��B",
		"�G�~�����[�V���������Z�b�g���܂��B",
		"�G�~�����[�^���ċN�����܂��B",
#if (EMU_SYSTEM != NCDZ)
		"���̃Q�[���͓��삵�܂���B",
		"���̃Q�[���͌���ł͂ǂ�����Ă�����ɓ��삵�܂��񂪁A",
		"�C�ɂ��Ȃ��ł��������B",
#endif
		"���̃f�B���N�g���������f�B���N�g���ɐݒ肵�܂��B",
#ifdef PSP_SLIM
		"���̃v���O������PSP-2000 + FW 3.71 M33�ȍ~��p�ł��B",
#endif
#ifdef SAVE_STATE
		"�X�e�[�g�Z�[�u���J�n���܂��B",
		"�X�e�[�g���[�h���J�n���܂��B",
#endif
#if defined(SAVE_STATE) || defined(COMMAND_LIST)
		"�������܂����B",
#endif
#ifdef SAVE_STATE
		"�X�e�[�g�f�[�^���폜���܂��B",
#endif
#if (EMU_SYSTEM == NCDZ)
		"MP3�t�@�C���������ׁACDDA�͍Đ�����܂���B",
		"IPL.TXT������܂���B",
		"NEO�GEO CDZ��BIOS���N�����܂��B",
		"NEO�GEO CDZ��BIOS(neocd.bin)������܂���B",
		"NEO�GEO CDZ��BIOS�������ł��B",
		"�Q�[�����N���ł��܂���B",
#endif

		"�w���v - %s",
		"�g�p���܂���",
		"�����{�^���������ƃ��j���[�ɖ߂�܂�",
		"�t�@�C���u���E�U",
		"�J�[�\\�����ړ�",
		"�J�[�\\����1�y�[�W���ړ�",
		"�Q�[�����N��",
		"�Q�[�����N�� (AdHoc)",
		"�G�~�����[�^���I��",
		"BIOS�I�����j���[��\\��",
		"�J���[�ݒ胁�j���[���J��",
		"���̃w���v���J��",
		"BIOS���N��",
		"�I�𒆂̃f�B���N�g�����N���f�B���N�g���ɐݒ�",
		"�l��ύX",
		"�I��",
		"�X���b�g�̑I��",
		"������ύX",
		"���������s",
		"���ڂ̑I��",
		"���ڂ̑I��/�l�̕ύX",
		"RGB�̒l��+5",
		"�t�@�C���u���E�U�ɖ߂�",
		"RGB�̒l��-5",
		"�S�Ă̒l��W���ɖ߂�",
		"���X�g���X�N���[��/���ڂ�I��",
		"���ڃ��j���[���J��/����",
		"�Q�[��/���C�����j���[�ɖ߂�",

		/* psp/ui_menu.c */
		"�I��",
		"�I�t",
		"�͂�",
		"������",
		"�L��",
		"����",
		"���C�����j���[�ɖ߂�",
		"�Q�[���ݒ胁�j���[",
		"���X�^�G�t�F�N�g",
		"��ʊg��",
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		"360x270 (4:3)",
		"384x270 (24:17)",
		"480x270 (16:9)",
		"��ʂ���]",
#else
		"360x270 (4:3)",
		"420x270 (14:9)",
		"480x270 (16:9)",
#endif
		"VBLANK����",
		"�����t���[���X�L�b�v",
		"�t���[���X�L�b�v",
		"���x��1",
		"���x��2",
		"���x��3",
		"���x��4",
		"���x��5",
		"���x��6",
		"���x��7",
		"���x��8",
		"���x��9",
		"���x��10",
		"���x��11",
		"FPS�\\��",
		"60fps�t���[������",
		"�T�E���h�Đ�",
		"�T���v�����[�g",
		"11025Hz",
		"22050Hz",
		"44100Hz",
		"�T�E���h����",
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
		"�R���g���[��",
		"�v���C���[1",
		"�v���C���[2",
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		"�v���C���[3",
		"�v���C���[4",
#endif
		"PSP CPU�N���b�N",
		"222MHz",
		"266MHz",
		"300MHz",
		"333MHz",
#if (EMU_SYSTEM == MVS)
		"BIOS�W��",
#endif
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"�V�X�e���n��ݒ�",
		"���{",
		"�A�����J",
		"���[���b�p",
#endif
#if (EMU_SYSTEM == MVS)
		"�V�X�e�����[�h�ݒ�",
		"�ƒ�p (AES)",
		"�A�[�P�[�h (MVS)",
#endif
#if (EMU_SYSTEM == NCDZ)
		"���[�h��ʃG�~�����[�V����",
		"CD-ROM���x����",
		"CDDA�Đ�",
		"CDDA����",
#endif

		"���̓{�^���ݒ胁�j���[",
		"�g�p���Ȃ�",
		FONT_UPARROW,
		FONT_DOWNARROW,
		FONT_LEFTARROW,
		FONT_RIGHTARROW,
		FONT_CIRCLE,
		FONT_CROSS,
		FONT_SQUARE,
		FONT_TRIANGLE,
		"L�g���K",
		"R�g���K",
		"START",
		"SELECT",
		"��",
		"�W��",
		"��",
#if (EMU_SYSTEM == CPS2)
		"�v���C���[1 �X�^�[�g",
		"�v���C���[2 �X�^�[�g",
#endif
		"%d�t���[��",
		"%d�t���[��",
		FONT_UPARROW,
		FONT_DOWNARROW,
		FONT_LEFTARROW,
		FONT_RIGHTARROW,
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"�{�^��A",
		"�{�^��B",
		"�{�^��C",
		"�{�^��D",
#else
		"�{�^��1",
		"�{�^��2",
		"�{�^��3",
		"�{�^��4",
		"�{�^��5",
		"�{�^��6",
#endif
		"�X�^�[�g",
#if (EMU_SYSTEM == NCDZ)
		"�Z���N�g",
#else
		"�R�C��",
		"�T�[�r�X�R�C��",
#endif
#if (EMU_SYSTEM == MVS)
		"�e�X�g�X�C�b�`",
#elif (EMU_SYSTEM != NCDZ)
		"�T�[�r�X�X�C�b�`",
#endif
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"�{�^��A �A��",
		"�{�^��B �A��",
		"�{�^��C �A��",
		"�{�^��D �A��",
#else
		"�{�^��1 �A��",
		"�{�^��2 �A��",
		"�{�^��3 �A��",
		"�{�^��4 �A��",
		"�{�^��5 �A��",
		"�{�^��6 �A��",
#endif
		"�A�ˊԊu",
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"AB ��������",
		"AC ��������",
		"AD ��������",
		"BC ��������",
		"BD ��������",
		"CD ��������",
		"ABC ��������",
		"ABD ��������",
		"ACD ��������",
		"BCD ��������",
		"ABCD ��������",
#endif
#if (EMU_SYSTEM != NCDZ)
		"�A�i���O���x",
#endif
#if (EMU_SYSTEM == CPS1)
		"�_�C�A�� (����])",
		"�_�C�A�� (�E��])",
		"�|�[�Y",
#endif
#if (EMU_SYSTEM == CPS2)
		"�p�h�� (����])",
		"�p�h�� (�E��])",
#endif
#if (EMU_SYSTEM == MVS)
		"�r�b�O",
		"�X���[��",
		"�_�u���A�b�v",
		"�X�^�[�g/�W�v",
		"�����߂�",
		"�L�����Z��",
		"�x�b�g",
		"�S�ĂɃx�b�g/�S�ăL�����Z��",
		"�I�y���[�^���j���[",
		"�N���W�b�g���N���A",
		"�z�b�p�[���o��",
#endif
		"�X�N���[���V���b�g�ۑ�",
		"�v���C���[�؂�ւ�",
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"�{�^���z�u",
		"�^�C�v1 (NEO�GEO�p�b�h)",
		"�^�C�v2 (MVS)",
		"�^�C�v3 (PlayStation)",
		"���[�U��`",
#endif

		"�f�B�b�v�X�C�b�`�ݒ胁�j���[",
		"���̃Q�[���ɂ̓f�B�b�v�X�C�b�`�͂���܂���B",

		"�Z�[�u/���[�h�X�e�[�g",
		"�X���b�g %d:",
		"�f�[�^�Ȃ�",
		"���[�h",
		"�폜",
		"�Z�[�u",
		"�v���C���t",
		"�Z�[�u����",
		"�o�[�W����",
		"�t�@�C��\"%s\"���폜�ł��܂���ł����B",

		"���C�����j���[",
		"�Q�[���ݒ�",
		"�Q�[���̓���ݒ��ύX���܂��B",
		"���̓{�^���ݒ�",
		"���̓{�^���ƘA��/�z�b�g�L�[�̐ݒ�����܂��B",
#if (EMU_SYSTEM != NCDZ)
		"�f�B�b�v�X�C�b�`�ݒ�",
		"�f�B�b�v�X�C�b�`�̐ݒ��ύX���܂��B",
#endif
		"�G�~�����[�V���������Z�b�g",
		SYSTEM_NAME "�̃G�~�����[�V���������Z�b�g���܂��B",
		"�t�@�C���u���E�U�ɖ߂�",
		"�G�~�����[�V�������I�����A�t�@�C���u���E�U�ɖ߂�܂��B",
		"�Q�[���ɖ߂�",
		"�Q�[����ʂɖ߂�܂��B",
		"�G�~�����[�^���I��",
		"���̃\\�t�g�E�F�A���I�����APSP�̃��j���[�ɖ߂�܂��B",
#ifdef COMMAND_LIST
		"�R�}���h���X�g�\\��",
		"���̃Q�[���̃R�}���h�ꗗ��\\�����܂��B",
#endif

#if PSP_VIDEO_32BPP
		"�J���[�ݒ胁�j���[",
		"�w�i�摜�̎��",
		"�w�i�摜�̖��邳",
		"�^�C�g���o�[�̕���",
		"�I�����ꂽ����",
		"�W���̕���",
		"��񃁃b�Z�[�W�̕���",
		"�x�����b�Z�[�W�̕���",
		"�t�@�C���I���o�[(�J�n)",
		"�t�@�C���I���o�[(�I��)",
		"�^�C�g���o�[/���b�Z�[�W�{�b�N�X",
		"�^�C�g���o�[/���b�Z�[�W�{�b�N�X�̘g",
		"�w�i�F(�w�i�摜���g�p��)",
		"�ԋP�x",
		"�΋P�x",
		"�P�x",
		"�W���̉摜",
		"���[�U�̉摜",
		"logo.png�̂�",
		"�t�@�C���I���o�[",
#endif

		"�R�}���h���X�g",

		/* psp/mp3.c */
#if (EMU_SYSTEM == NCDZ)
		"MP3�t�@�C��\"%s\"���ăI�[�v���ł��܂���B",
		"MP3�f�R�[�h�G���[���������܂����B",
		"MP3�Đ��p�I�[�f�B�I�`���l�����m�ۂł��܂���B",
		"MP3�X���b�h���J�n�ł��܂���B",
#endif

		/* psp/adhoc.c*/
#ifdef ADHOC
		"���r�[",
		"�T�[�o�[",
		"�N���C�A���g",
		"����PSP�̐ڑ���ҋ@���B\n",
		"%s�ɐڑ���...",
		"�ڑ����܂����B",
		"%s����ؒf��...",
		"�ؒf���܂����B",
		"�ڑ����s",
		"�ڑ�����T�[�o�[��I���B" FONT_TRIANGLE "�������ƒ��~���܂��B\n",
		"%s�̐ڑ�����҂��Ă��܂��B\n",
		FONT_CROSS "�������ƒ��~���܂��B\n",
		"%s���ڑ�����v�����Ă��܂��B\n",
		FONT_CIRCLE "�Őڑ������A" FONT_CROSS "�Őڑ������ۂ��܂��B\n",
		"������҂��Ă��܂��B",
#endif

		/* psp/png.c */
		"PNG�摜�p�̃��������m�ۂł��܂���B",
		"PNG�摜���쐬�ł��܂���B",
#if PSP_VIDEO_32BPP || (EMU_SYSTEM == NCDZ)
		"PNG�摜��W�J�ł��܂���B",
#endif
		"%dbit�J���[��PNG�摜�ɂ͑Ή����Ă��܂���B",

		/* emumain.c */
		"�v���I�ȃG���[",
		"�X�N���[���V���b�g��\"%s_%02d.png\"�ɕۑ����܂����B",

#if USE_CACHE
		/* common/cache.c */
		"�L���b�V���t�@�C�����I�[�v���ł��܂���B\n",
		"������������܂���B\n",
		"�L���b�V�����������m�ۂł��܂���B\n",
		"%dKB�̃L���b�V�����������m�ۂ��܂����B\n",
		"�L���b�V�����[�h���ɃG���[���������܂����B\n",
		"�L���b�V�����f�[�^�����[�h��...\n",
		"�T�|�[�g����Ă��Ȃ��o�[�W����\"V%c%c\"�̃L���b�V���t�@�C���ł��B\n",
		"���ݗv�������L���b�V���t�@�C���̃o�[�W������\"" CACHE_VERSION "\"�ł��B\n",
		"�L���b�V���t�@�C�����쐬���Ȃ����Ă��������B\n",
#if (EMU_SYSTEM == CPS2)
		"�X�v���C�g�u���b�N%03x���I�[�v���ł��܂���B\n",
#elif (EMU_SYSTEM == MVS)
		"PCM�L���b�V�����g�p���܂��B\n",
#endif
#endif

		/* common/loadrom.c */
		"�����{�^���������Ă��������B\n",
		"���������m�ۂł��܂���B(REGION_%s)\n",
		"CRC32������������܂���B\"%s\"\n",
		"�t�@�C����������܂���B\"%s\"\n",

#ifdef SAVE_STATE
		/* common/state.c */
		"�X�e�[�g�p�̃o�b�t�@���m�ۂł��܂���B",
		"�t�@�C��\"%s.sv%d\"���쐬�ł��܂���B",
		"�t�@�C��\"%s.sv%d\"���I�[�v���ł��܂���B",
		"\"%s.sv%d\"��ۑ���",
		"\"%s.sv%d\"�����[�h��",
#if (EMU_SYSTEM == MVS)
		"BIOS�����[�h�ł��܂���B�����{�^���������ƏI�����܂��B",
#elif (EMU_SYSTEM == NCDZ)
		"�X�e�[�g�f�[�^�����k�ł��܂���B",
		"�X�e�[�g�f�[�^��W�J�ł��܂���B",
#endif
#endif

#ifdef COMMAND_LIST
		/* common/cmdlist.c */
		"���̃Q�[���p�̃R�}���h���X�g������܂���B",
		"�R�}���h���X�g - %s",
		"%d/%d ����",
		"COMMAND.DAT �T�C�Y�k������",
		"���̃G�~�����[�^�őΉ����Ă��Ȃ��Q�[���̃R�}���h���X�g���폜���A\n",
		"COMMAND.DAT�̃t�@�C���T�C�Y���k�����܂��B\n",
		"�������s���ꍇ�́A" FONT_CIRCLE "�{�^���������Ă��������B\n",
		FONT_CROSS "�{�^���������Ə����𒆎~���ăt�@�C���u���E�U�ɖ߂�܂��B\n",
		"COMMAND.DAT ��������...\n",
		"�G���[: ���Ή���\"COMMAND.DAT\"���t�@�C���̌`�����قȂ�܂��B\n",
		"�G���[: ��̃t�@�C���ł��B\n",
		"�G���[: ���������m�ۂł��܂���B\n",
		"�G���[: �t�@�C�������l�[���ł��܂���B\n",
		"�G���[: �o�̓t�@�C�����쐬�ł��܂���B\n",
		"\"%s\" �̃R�}���h���R�s�[��...\n",
		"�T�C�Y %dbytes �� %dbytes (%.1f%%��)\n",
#endif

		/* emulation core */
		"�������܂����B\n",
		"�G�~�����[�V�����̏I��",
		"���΂炭���҂����������B\n",

		/* inptport.c */
		"�R���g���[���[: �v���C���[1",
		"�R���g���[���[: �v���C���[%d",
#ifdef ADHOC
		"�����������܂����B\n",
		"%s�ɂ��|�[�Y",
		"�Q�[�����ĊJ",
		"�ؒf���ďI��",
#endif

		/* memintrf.c */
		"���[�h�� \"%s\"\n",
		"ROM�̃��[�h",
#if (EMU_SYSTEM != NCDZ)
		"ROM�����`�F�b�N��...\n",
		"���̃Q�[���̓T�|�[�g����Ă��܂���B\n",
		"ROM��������܂���B(zip�t�@�C�������������Ȃ�)\n",
		"\"%s\"�̃h���C�o������܂���B\n",
		"ROM�Z�b�g��\"%s\" (�e�Z�b�g��: %s)\n",
		"ROM�Z�b�g��\"%s\"\n",
#endif

#if (EMU_SYSTEM == CPS1)

		/* memintrf.c */
		"rominfo.cps1��������܂���B\n",
		"���������m�ۂł��܂���B(0x8000�o�C�g)",

#elif (EMU_SYSTEM == CPS2)

		/* cps2crpt.c */
		"�������������s�� %d%%\r",
		"�������������s�� 100%%\n",

		/* memintrf.c */
		"rominfo.cps2��������܂���B\n",

#ifdef PSP_SLIM
		/* vidhrdw.c */
		"�O���t�B�b�N�f�[�^��W�J��...\n",
#endif

#elif (EMU_SYSTEM == MVS)

		/* biosmenu.c */
		"BIOS�I�����j���[",
		"BIOS��������܂���B",
		"BIOS��I�����A" FONT_CIRCLE "�{�^���������Ă��������B",
		"NVRAM�t�@�C���͑S�č폜����܂����B\n",

		/* memintrf.c */
		"rominfo.mvs��������܂���B\n",
		"���[�h�� \"%s (%s)\"\n",
		"�����ς�GFX2 ROM�����[�h��...\n",
		"�����ς�SOUND1 ROM�����[�h��...\n",
		"ROM�����p�̃��������m�ۂł��܂���B\n",
		"�X�v���C�g�f�[�^�p�̃��������m�ۂł��܂���B\n",
		"�X�v���C�g�L���b�V���̎g�p�����݂܂��B\n",
		"BIOS���`�F�b�N��...\n",

#elif (EMU_SYSTEM == NCDZ)

		/* ncdz.c */
		"IPL.TXT�������ɃG���[���������܂����B",
		"�E�H�b�`�h�b�O�J�E���^�ɂ�郊�Z�b�g���������܂����B",

		/* cdrom.c */
		"�t�@�C��\"%s\"���I�[�v���ł��܂���B",
		"CD-ROM�̃X�e�[�g�f�[�^��ǉ��ł��܂���B",

		/* vidhrdw.c */
		"CD-ROM���x����: ����",
		"CD-ROM���x����: �L��",

		/* memintrf.c */
		"�Q�[��ID���`�F�b�N��...\n",
#endif
		"�`�[�g���j���[",
		"�`�[�g��I�����܂��B",

		"���������",
		"����������ݒ��ύX���܂��B",
		NULL
};
static const char *text_SPANISH[UI_TEXT_MAX] =
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
		"Boot NEO�GEO CDZ BIOS.",
		"NEO�GEO CDZ BIOS (neocd.bin) no encontrado.",
		"Encontrado NEO�GEO CDZ BIOS invalido.",
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
};


static int lang = 0;


void ui_text_init(void)
{
	int i;

	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &lang);

	if (lang == PSP_SYSTEMPARAM_LANGUAGE_JAPANESE)
	{
		lang = LANG_JAPANESE;
		for (i = 0; i < UI_TEXT_MAX; i++)
			ui_text[i] = text_JAPANESE[i];
	}
	else if (lang == PSP_SYSTEMPARAM_LANGUAGE_SPANISH)
	{
		lang = LANG_SPANISH;
		for (i = 0; i < UI_TEXT_MAX; i++)
			ui_text[i] = text_SPANISH[i];
	}
	else
	{
		lang = LANG_ENGLISH;
		for (i = 0; i < UI_TEXT_MAX; i++)
			ui_text[i] = text_ENGLISH[i];
	}

}

int ui_text_get_language(void)
{
	return lang;
}
