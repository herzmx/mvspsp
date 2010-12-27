/******************************************************************************

	ui_text.c

	e��[eUeCe�e^nt�F�[�X�e�L�X�g��o�

******************************************************************************/

#include "psp.h"
#include <psputility_sysparam.h>


const char *ui_text[UI_TEXT_MAX];

static const char *text[2][UI_TEXT_MAX] =
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
		NULL
	},
	
};


static int lang = 0;


void ui_text_init(void)
{
	int i;

	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &lang);

	if (lang == PSP_SYSTEMPARAM_LANGUAGE_SPANISH)
		lang = 1;
	else
		lang = 0;

	for (i = 0; i < UI_TEXT_MAX; i++)
		ui_text[i] = text[lang][i];
}

int ui_text_get_language(void)
{
	return lang;
}
