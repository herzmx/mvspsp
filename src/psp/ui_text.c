/******************************************************************************

	ui_text.c

	ユーザインタフェーステキスト管理

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
		"Boot NEO･GEO CDZ BIOS.",
		"NEO･GEO CDZ BIOS (neocd.bin) not found.",
		"Invalid NEO･GEO CDZ BIOS found.",
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
		"しばらくお待ちください。",
		"zipname.%sをオープンできません。",
#ifdef ADHOC
		"WLANスイッチをONにしてください。",
		"AdHocモジュールをロード出来ませんでした。",
#endif

		/* psp/sound.c */
		"オーディオチャネルを確保できません。",
		"サウンドスレッドを開始できません。",

		/* psp/ui.c */
		"警告: バッテリーの残容量がありません(%d%%)。充電してください！",

		FONT_CIRCLE " 起動      " FONT_CROSS " 中止",
		FONT_CIRCLE " 続行      " FONT_CROSS " 中止",
#ifdef SAVE_STATE
		FONT_CIRCLE " ゲームに戻る   " FONT_CROSS " メニューに戻る",
#endif
		"何かボタンを押してください。",
		"エミュレーションを開始します。",
#ifdef ADHOC
		"エミュレーションを開始します。(AdHoc)",
#endif
		"エミュレーションを終了します。",
		"エミュレーションをリセットします。",
		"エミュレータを再起動します。",
#if (EMU_SYSTEM != NCDZ)
		"このゲームは動作しません。",
		"このゲームは現状ではどうやっても正常に動作しませんが、",
		"気にしないでください。",
#endif
		"このディレクトリを初期ディレクトリに設定します。",
#ifdef PSP_SLIM
		"このプログラムはPSP-2000 + FW 3.71 M33以降専用です。",
#endif
#ifdef SAVE_STATE
		"ステートセーブを開始します。",
		"ステートロードを開始します。",
#endif
#if defined(SAVE_STATE) || defined(COMMAND_LIST)
		"完了しました。",
#endif
#ifdef SAVE_STATE
		"ステートデータを削除します。",
#endif
#if (EMU_SYSTEM == NCDZ)
		"MP3ファイルが無い為、CDDAは再生されません。",
		"IPL.TXTがありません。",
		"NEO･GEO CDZのBIOSを起動します。",
		"NEO･GEO CDZのBIOS(neocd.bin)がありません。",
		"NEO･GEO CDZのBIOSが無効です。",
		"ゲームを起動できません。",
#endif

		"ヘルプ - %s",
		"使用しません",
		"何かボタンを押すとメニューに戻ります",
		"ファイルブラウザ",
		"カーソ\ルを移動",
		"カーソ\ルを1ページ分移動",
		"ゲームを起動",
		"ゲームを起動 (AdHoc)",
		"エミュレータを終了",
		"BIOS選択メニューを表\示",
		"カラー設定メニューを開く",
		"このヘルプを開く",
		"BIOSを起動",
		"選択中のディレクトリを起動ディレクトリに設定",
		"値を変更",
		"選択",
		"スロットの選択",
		"処理を変更",
		"処理を実行",
		"項目の選択",
		"項目の選択/値の変更",
		"RGBの値を+5",
		"ファイルブラウザに戻る",
		"RGBの値を-5",
		"全ての値を標準に戻す",
		"リストをスクロール/項目を選択",
		"項目メニューを開く/閉じる",
		"ゲーム/メインメニューに戻る",

		/* psp/ui_menu.c */
		"オン",
		"オフ",
		"はい",
		"いいえ",
		"有効",
		"無効",
		"メインメニューに戻る",
		"ゲーム設定メニュー",
		"ラスタエフェクト",
		"画面拡大",
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		"360x270 (4:3)",
		"384x270 (24:17)",
		"480x270 (16:9)",
		"画面を回転",
#else
		"360x270 (4:3)",
		"420x270 (14:9)",
		"480x270 (16:9)",
#endif
		"VBLANK同期",
		"自動フレームスキップ",
		"フレームスキップ",
		"レベル1",
		"レベル2",
		"レベル3",
		"レベル4",
		"レベル5",
		"レベル6",
		"レベル7",
		"レベル8",
		"レベル9",
		"レベル10",
		"レベル11",
		"FPS表\示",
		"60fpsフレーム制限",
		"サウンド再生",
		"サンプルレート",
		"11025Hz",
		"22050Hz",
		"44100Hz",
		"サウンド音量",
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
		"コントローラ",
		"プレイヤー1",
		"プレイヤー2",
#if (EMU_SYSTEM == CPS1 || EMU_SYSTEM == CPS2)
		"プレイヤー3",
		"プレイヤー4",
#endif
		"PSP CPUクロック",
		"222MHz",
		"266MHz",
		"300MHz",
		"333MHz",
#if (EMU_SYSTEM == MVS)
		"BIOS標準",
#endif
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"システム地域設定",
		"日本",
		"アメリカ",
		"ヨーロッパ",
#endif
#if (EMU_SYSTEM == MVS)
		"システムモード設定",
		"家庭用 (AES)",
		"アーケード (MVS)",
#endif
#if (EMU_SYSTEM == NCDZ)
		"ロード画面エミュレーション",
		"CD-ROM速度制限",
		"CDDA再生",
		"CDDA音量",
#endif

		"入力ボタン設定メニュー",
		"使用しない",
		FONT_UPARROW,
		FONT_DOWNARROW,
		FONT_LEFTARROW,
		FONT_RIGHTARROW,
		FONT_CIRCLE,
		FONT_CROSS,
		FONT_SQUARE,
		FONT_TRIANGLE,
		"Lトリガ",
		"Rトリガ",
		"START",
		"SELECT",
		"低",
		"標準",
		"高",
#if (EMU_SYSTEM == CPS2)
		"プレイヤー1 スタート",
		"プレイヤー2 スタート",
#endif
		"%dフレーム",
		"%dフレーム",
		FONT_UPARROW,
		FONT_DOWNARROW,
		FONT_LEFTARROW,
		FONT_RIGHTARROW,
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"ボタンA",
		"ボタンB",
		"ボタンC",
		"ボタンD",
#else
		"ボタン1",
		"ボタン2",
		"ボタン3",
		"ボタン4",
		"ボタン5",
		"ボタン6",
#endif
		"スタート",
#if (EMU_SYSTEM == NCDZ)
		"セレクト",
#else
		"コイン",
		"サービスコイン",
#endif
#if (EMU_SYSTEM == MVS)
		"テストスイッチ",
#elif (EMU_SYSTEM != NCDZ)
		"サービススイッチ",
#endif
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"ボタンA 連射",
		"ボタンB 連射",
		"ボタンC 連射",
		"ボタンD 連射",
#else
		"ボタン1 連射",
		"ボタン2 連射",
		"ボタン3 連射",
		"ボタン4 連射",
		"ボタン5 連射",
		"ボタン6 連射",
#endif
		"連射間隔",
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"AB 同時押し",
		"AC 同時押し",
		"AD 同時押し",
		"BC 同時押し",
		"BD 同時押し",
		"CD 同時押し",
		"ABC 同時押し",
		"ABD 同時押し",
		"ACD 同時押し",
		"BCD 同時押し",
		"ABCD 同時押し",
#endif
#if (EMU_SYSTEM != NCDZ)
		"アナログ感度",
#endif
#if (EMU_SYSTEM == CPS1)
		"ダイアル (左回転)",
		"ダイアル (右回転)",
		"ポーズ",
#endif
#if (EMU_SYSTEM == CPS2)
		"パドル (左回転)",
		"パドル (右回転)",
#endif
#if (EMU_SYSTEM == MVS)
		"ビッグ",
		"スモール",
		"ダブルアップ",
		"スタート/集計",
		"払い戻し",
		"キャンセル",
		"ベット",
		"全てにベット/全てキャンセル",
		"オペレータメニュー",
		"クレジットをクリア",
		"ホッパー取り出し",
#endif
		"スクリーンショット保存",
		"プレイヤー切り替え",
#if (EMU_SYSTEM == MVS || EMU_SYSTEM == NCDZ)
		"ボタン配置",
		"タイプ1 (NEO･GEOパッド)",
		"タイプ2 (MVS)",
		"ユーザ定義",
#endif

		"ディップスイッチ設定メニュー",
		"このゲームにはディップスイッチはありません。",

		"セーブ/ロードステート",
		"スロット %d:",
		"データなし",
		"ロード",
		"削除",
		"セーブ",
		"プレイ日付",
		"セーブ時刻",
		"バージョン",
		"ファイル\"%s\"を削除できませんでした。",

		"メインメニュー",
		"ゲーム設定",
		"ゲームの動作設定を変更します。",
		"入力ボタン設定",
		"入力ボタンと連射/ホットキーの設定をします。",
#if (EMU_SYSTEM != NCDZ)
		"ディップスイッチ設定",
		"ディップスイッチの設定を変更します。",
#endif
		"エミュレーションをリセット",
		SYSTEM_NAME "のエミュレーションをリセットします。",
		"ファイルブラウザに戻る",
		"エミュレーションを終了し、ファイルブラウザに戻ります。",
		"ゲームに戻る",
		"ゲーム画面に戻ります。",
		"エミュレータを終了",
		"このソ\フトウェアを終了し、PSPのメニューに戻ります。",
#ifdef COMMAND_LIST
		"コマンドリスト表\示",
		"このゲームのコマンド一覧を表\示します。",
#endif

#if PSP_VIDEO_32BPP
		"カラー設定メニュー",
		"背景画像の種類",
		"背景画像の明るさ",
		"タイトルバーの文字",
		"選択された文字",
		"標準の文字",
		"情報メッセージの文字",
		"警告メッセージの文字",
		"ファイル選択バー(開始)",
		"ファイル選択バー(終了)",
		"タイトルバー/メッセージボックス",
		"タイトルバー/メッセージボックスの枠",
		"背景色(背景画像未使用時)",
		"赤輝度",
		"緑輝度",
		"青輝度",
		"標準の画像",
		"ユーザの画像",
		"logo.pngのみ",
		"ファイル選択バー",
#endif

		"コマンドリスト",

		/* psp/mp3.c */
#if (EMU_SYSTEM == NCDZ)
		"MP3ファイル\"%s\"を再オープンできません。",
		"MP3デコードエラーが発生しました。",
		"MP3再生用オーディオチャネルを確保できません。",
		"MP3スレッドを開始できません。",
#endif

		/* psp/adhoc.c*/
#ifdef ADHOC
		"ロビー",
		"サーバー",
		"クライアント",
		"他のPSPの接続を待機中。\n",
		"%sに接続中...",
		"接続しました。",
		"%sから切断中...",
		"切断しました。",
		"接続失敗",
		"接続するサーバーを選択。" FONT_TRIANGLE "を押すと中止します。\n",
		"%sの接続許可を待っています。\n",
		FONT_CROSS "を押すと中止します。\n",
		"%sが接続許可を要求しています。\n",
		FONT_CIRCLE "で接続を許可、" FONT_CROSS "で接続を拒否します。\n",
		"同期を待っています。",
#endif

		/* psp/png.c */
		"PNG画像用のメモリを確保できません。",
		"PNG画像を作成できません。",
#if PSP_VIDEO_32BPP || (EMU_SYSTEM == NCDZ)
		"PNG画像を展開できません。",
#endif
		"%dbitカラーのPNG画像には対応していません。",

		/* emumain.c */
		"致命的なエラー",
		"スクリーンショットを\"%s_%02d.png\"に保存しました。",

#if USE_CACHE
		/* common/cache.c */
		"キャッシュファイルをオープンできません。\n",
		"メモリが足りません。\n",
		"キャッシュメモリを確保できません。\n",
		"%dKBのキャッシュメモリを確保しました。\n",
		"キャッシュロード中にエラーが発生しました。\n",
		"キャッシュ情報データをロード中...\n",
		"サポートされていないバージョン\"V%c%c\"のキャッシュファイルです。\n",
		"現在要求されるキャッシュファイルのバージョンは\"" CACHE_VERSION "\"です。\n",
		"キャッシュファイルを作成しなおしてください。\n",
#if (EMU_SYSTEM == CPS2)
		"スプライトブロック%03xをオープンできません。\n",
#elif (EMU_SYSTEM == MVS)
		"PCMキャッシュを使用します。\n",
#endif
#endif

		/* common/loadrom.c */
		"何かボタンを押してください。\n",
		"メモリを確保できません。(REGION_%s)\n",
		"CRC32が正しくありません。\"%s\"\n",
		"ファイルが見つかりません。\"%s\"\n",

#ifdef SAVE_STATE
		/* common/state.c */
		"ステート用のバッファを確保できません。",
		"ファイル\"%s.sv%d\"を作成できません。",
		"ファイル\"%s.sv%d\"をオープンできません。",
		"\"%s.sv%d\"を保存中",
		"\"%s.sv%d\"をロード中",
#if (EMU_SYSTEM == MVS)
		"BIOSをロードできません。何かボタンを押すと終了します。",
#elif (EMU_SYSTEM == NCDZ)
		"ステートデータを圧縮できません。",
		"ステートデータを展開できません。",
#endif
#endif

#ifdef COMMAND_LIST
		/* common/cmdlist.c */
		"このゲーム用のコマンドリストがありません。",
		"コマンドリスト - %s",
		"%d/%d 項目",
		"COMMAND.DAT サイズ縮小処理",
		"このエミュレータで対応していないゲームのコマンドリストを削除し、\n",
		"COMMAND.DATのファイルサイズを縮小します。\n",
		"処理を行う場合は、" FONT_CIRCLE "ボタンを押してください。\n",
		FONT_CROSS "ボタンを押すと処理を中止してファイルブラウザに戻ります。\n",
		"COMMAND.DAT を検査中...\n",
		"エラー: 未対応の\"COMMAND.DAT\"かファイルの形式が異なります。\n",
		"エラー: 空のファイルです。\n",
		"エラー: メモリを確保できません。\n",
		"エラー: ファイルをリネームできません。\n",
		"エラー: 出力ファイルを作成できません。\n",
		"\"%s\" のコマンドをコピー中...\n",
		"サイズ %dbytes → %dbytes (%.1f%%減)\n",
#endif

		/* emulation core */
		"完了しました。\n",
		"エミュレーションの終了",
		"しばらくお待ちください。\n",

		/* inptport.c */
		"コントローラー: プレイヤー1",
		"コントローラー: プレイヤー%d",
#ifdef ADHOC
		"同期を失いました。\n",
		"%sによるポーズ",
		"ゲームを再開",
		"切断して終了",
#endif

		/* memintrf.c */
		"ロード中 \"%s\"\n",
		"ROMのロード",
#if (EMU_SYSTEM != NCDZ)
		"ROM情報をチェック中...\n",
		"このゲームはサポートされていません。\n",
		"ROMが見つかりません。(zipファイル名が正しくない)\n",
		"\"%s\"のドライバがありません。\n",
		"ROMセット名\"%s\" (親セット名: %s)\n",
		"ROMセット名\"%s\"\n",
#endif

#if (EMU_SYSTEM == CPS1)

		/* memintrf.c */
		"rominfo.cps1が見つかりません。\n",
		"メモリを確保できません。(0x8000バイト)",

#elif (EMU_SYSTEM == CPS2)

		/* cps2crpt.c */
		"復号処理を実行中 %d%%\r",
		"復号処理を実行中 100%%\n",

		/* memintrf.c */
		"rominfo.cps2が見つかりません。\n",

#ifdef PSP_SLIM
		/* vidhrdw.c */
		"グラフィックデータを展開中...\n",
#endif

#elif (EMU_SYSTEM == MVS)

		/* biosmenu.c */
		"BIOS選択メニュー",
		"BIOSが見つかりません。",
		"BIOSを選択し、" FONT_CIRCLE "ボタンを押してください。",
		"NVRAMファイルは全て削除されました。\n",

		/* memintrf.c */
		"rominfo.mvsが見つかりません。\n",
		"ロード中 \"%s (%s)\"\n",
		"復号済みGFX2 ROMをロード中...\n",
		"復号済みSOUND1 ROMをロード中...\n",
		"ROM復号用のメモリを確保できません。\n",
		"スプライトデータ用のメモリを確保できません。\n",
		"スプライトキャッシュの使用を試みます。\n",
		"BIOSをチェック中...\n",

#elif (EMU_SYSTEM == NCDZ)

		/* ncdz.c */
		"IPL.TXT処理中にエラーが発生しました。",
		"ウォッチドッグカウンタによるリセットが発生しました。",

		/* cdrom.c */
		"ファイル\"%s\"をオープンできません。",
		"CD-ROMのステートデータを追加できません。",

		/* vidhrdw.c */
		"CD-ROM速度制限: 無効",
		"CD-ROM速度制限: 有効",

		/* memintrf.c */
		"ゲームIDをチェック中...\n",
#endif
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
	else
		lang = 0;

	for (i = 0; i < UI_TEXT_MAX; i++)
		ui_text[i] = text[lang][i];
}

int ui_text_get_language(void)
{
	return lang;
}
