/******************************************************************************

	inptport.c

	NEOGEO CDZ 入力ポートエミュレーション

******************************************************************************/

#include "ncdz.h"


/******************************************************************************
	グローバル変数
******************************************************************************/

int option_controller;
UINT8 ALIGN_DATA neogeo_port_value[NCDZ_PORT_MAX];

int input_map[MAX_INPUTS];
int af_interval = 1;
int analog_sensitivity;	// not used


/******************************************************************************
	ローカル変数
******************************************************************************/

static const UINT8 hotkey_mask[11] =
{
//	0xef,	// A
//	0xdf,	// B
//	0xbf,	// C
//	0x7f,	// D
	0xcf,	// A+B
	0xaf,	// A+C
	0x6f,	// A+D
	0x9f,	// B+C
	0x5f,	// B+D
	0x3f,	// C+D
	0x8f,	// A+B+C
	0x4f,	// A+B+D
	0x2f,	// A+C+D
	0x1f,	// B+C+D
	0x0f	// A+B+C+D
};

static UINT8 ALIGN_DATA input_flag[MAX_INPUTS];
static int ALIGN_DATA af_map1[NCDZ_BUTTON_MAX];
static int ALIGN_DATA af_map2[NCDZ_BUTTON_MAX];
static int ALIGN_DATA af_counter[NCDZ_BUTTON_MAX];
static int input_ui_wait;

#ifdef ADHOC
typedef struct
{
	UINT32 buttons;
	int loop_flag;
	UINT16 port_value[3];
} ADHOC_DATA;

static ADHOC_DATA ALIGN_PSPDATA send_data;
static ADHOC_DATA ALIGN_PSPDATA recv_data;
static SceUID adhoc_thread;
static volatile int adhoc_active;
static volatile int adhoc_update;
static volatile UINT32 adhoc_paused;
#endif


/******************************************************************************
	ローカル関数
******************************************************************************/

/*------------------------------------------------------
	連射フラグを更新
------------------------------------------------------*/

static UINT32 update_autofire(UINT32 buttons)
{
	int i;

	for (i = 0; i < NCDZ_BUTTON_MAX; i++)
	{
		if (af_map1[i])
		{
			if (buttons & af_map1[i])
			{
				buttons &= ~af_map1[i];

				if (af_counter[i] == 0)
					buttons |= af_map2[i];
				else
					buttons &= ~af_map2[i];

				if (++af_counter[i] > af_interval)
					af_counter[i] = 0;
			}
			else
			{
				af_counter[i] = 0;
			}
		}
	}

	return buttons;
}


/*------------------------------------------------------
	ホットキーフラグを反映
------------------------------------------------------*/

static UINT8 apply_hotkey(UINT8 value)
{
	int i, button;

	button = P1_AB;
	for (i= 0; i < 11; i++)
	{
		if (input_flag[button]) value &= hotkey_mask[i];
		button++;
	}

	return value;
}


/*------------------------------------------------------
	NEOGEO CDZ コントローラ1
------------------------------------------------------*/

static void update_inputport0(void)
{
	UINT8 value = 0xff;

	if (!option_controller)
	{
		if (input_flag[P1_UP])      value &= ~0x01;
		if (input_flag[P1_DOWN])    value &= ~0x02;
		if (input_flag[P1_LEFT])    value &= ~0x04;
		if (input_flag[P1_RIGHT])   value &= ~0x08;
		if (input_flag[P1_BUTTONA]) value &= ~0x10;
		if (input_flag[P1_BUTTONB]) value &= ~0x20;
		if (input_flag[P1_BUTTONC]) value &= ~0x40;
		if (input_flag[P1_BUTTOND]) value &= ~0x80;

		value = apply_hotkey(value);
	}

#ifdef ADHOC
	if (adhoc_enable)
		send_data.port_value[0] = value;
	else
#endif
		neogeo_port_value[0] = value;
}


/*------------------------------------------------------
	NEOGEO CDZ コントローラ2
------------------------------------------------------*/

static void update_inputport1(void)
{
	UINT8 value = 0xff;

	if (option_controller)
	{
		if (input_flag[P1_UP])      value &= ~0x01;
		if (input_flag[P1_DOWN])    value &= ~0x02;
		if (input_flag[P1_LEFT])    value &= ~0x04;
		if (input_flag[P1_RIGHT])   value &= ~0x08;
		if (input_flag[P1_BUTTONA]) value &= ~0x10;
		if (input_flag[P1_BUTTONB]) value &= ~0x20;
		if (input_flag[P1_BUTTONC]) value &= ~0x40;
		if (input_flag[P1_BUTTOND]) value &= ~0x80;

		value = apply_hotkey(value);
	}

#ifdef ADHOC
	if (adhoc_enable)
		send_data.port_value[1] = value;
	else
#endif
		neogeo_port_value[1] = value;
}


/*------------------------------------------------------
	NEOGEO CDZ START/SELECTボタン
------------------------------------------------------*/

static void update_inputport2(void)
{
	UINT8 value = 0x0f;

	if (option_controller)
	{
		if (input_flag[P1_START])  value &= ~0x04;
		if (input_flag[P1_SELECT]) value &= ~0x08;
	}
	else
	{
		if (input_flag[P1_START])  value &= ~0x01;
		if (input_flag[P1_SELECT]) value &= ~0x02;
	}

#ifdef ADHOC
	if (adhoc_enable)
		send_data.port_value[2] = value | 0xf0;
	else
#endif
		neogeo_port_value[2] = value;
}


#ifdef ADHOC
/*------------------------------------------------------
	入力ポートを更新
------------------------------------------------------*/

static int adhoc_update_inputport(SceSize args, void *argp)
{
	int i;
	UINT32 buttons;

	while (adhoc_active)
	{
		adhoc_update = 0;

		do
		{
			sceKernelDelayThread(166);
		} while (!adhoc_update && adhoc_active);

		if (!adhoc_paused)
		{
			buttons = poll_gamepad();

			if (buttons & PSP_CTRL_HOME)
			{
				buttons = 0;
				adhoc_paused = adhoc_server + 1;
			}

			buttons = update_autofire(buttons);

			for (i = 0; i < MAX_INPUTS; i++)
				input_flag[i] = (buttons & input_map[i]) != 0;

			update_inputport0();
			update_inputport1();
			update_inputport2();

			send_data.buttons = adhoc_paused;
			send_data.loop_flag = Loop;

			if (adhoc_server)
			{
				adhocSendBlocking(&send_data, sizeof(ADHOC_DATA));
				if (frames_displayed <= 1)
				{
					if (adhocRecvBlocking(&recv_data, sizeof(ADHOC_DATA)) == 0)
					{
						ui_popup(TEXT(LOST_SYNC));
						Loop = LOOP_BROWSER;
					}
				}
				else
				{
					if (adhocRecvBlockingTimeout(&recv_data, sizeof(ADHOC_DATA), (1000000/60)*10) == 0)
					{
						adhocSendBlocking(&send_data, sizeof(ADHOC_DATA));
						if (adhocRecvBlocking(&recv_data, sizeof(ADHOC_DATA)) == 0)
						{
							ui_popup(TEXT(LOST_SYNC));
							Loop = LOOP_BROWSER;
						}
					}
				}
			}
			else
			{
				if (adhocRecvBlocking(&recv_data, sizeof(ADHOC_DATA)) == 0)
				{
					ui_popup(TEXT(LOST_SYNC));
					Loop = LOOP_BROWSER;
				}
				adhocSendBlocking(&send_data, sizeof(ADHOC_DATA));
			}

			if (Loop == LOOP_EXEC)
				Loop = recv_data.loop_flag;

			if (recv_data.buttons > adhoc_paused)
				adhoc_paused = recv_data.buttons;
		}
		else
		{
			send_data.buttons = poll_gamepad();

			if (adhoc_server)
			{
				adhocSendBlocking(&send_data, sizeof(ADHOC_DATA));
				if (frames_displayed <= 1)
				{
					if (adhocRecvBlocking(&recv_data, sizeof(ADHOC_DATA)) == 0)
					{
						ui_popup(TEXT(LOST_SYNC));
						Loop = LOOP_BROWSER;
					}
				}
				else
				{
					if (adhocRecvBlockingTimeout(&recv_data, sizeof(ADHOC_DATA), (1000000/60)*10) == 0)
					{
						adhocSendBlocking(&send_data, sizeof(ADHOC_DATA));
						if (adhocRecvBlocking(&recv_data, sizeof(ADHOC_DATA)) == 0)
						{
							ui_popup(TEXT(LOST_SYNC));
							Loop = LOOP_BROWSER;
						}
					}
				}
			}
			else
			{
				if (adhocRecvBlocking(&recv_data, sizeof(ADHOC_DATA)) == 0)
				{
					ui_popup(TEXT(LOST_SYNC));
					Loop = LOOP_BROWSER;
				}
				adhocSendBlocking(&send_data, sizeof(ADHOC_DATA));
			}

		}
	}

	sceKernelExitThread(0);

	return 0;
}


/*------------------------------------------------------
	ポーズ
------------------------------------------------------*/

static void adhoc_pause(void)
{
	int control, sel = 0;
	UINT32 buttons, frame = frames_displayed;
	char buf[64];
	RECT rect = { 140-8, 96-8, 340+8, 176+8 };

	if ((adhoc_server && adhoc_paused == 2) || (!adhoc_server && adhoc_paused == 1))
		control = 1;
	else
		control = 0;

	sound_thread_enable(0);

	video_copy_rect(show_frame, work_frame, &rect, &rect);

	do
	{
		video_copy_rect(work_frame, draw_frame, &rect, &rect);

		draw_dialog(140, 96, 340, 176);

		sprintf(buf, TEXT(PAUSED_BY_x), (adhoc_paused == 2) ? TEXT(SERVER) : TEXT(CLIENT));
		uifont_print_center(106, UI_COLOR(UI_PAL_INFO), buf);

		if (sel == 0)
		{
			uifont_print_center(132, COLOR_WHITE, TEXT(RETURN_TO_GAME));
			uifont_print_center(150, COLOR_GRAY, TEXT(DISCONNECT2));
		}
		else
		{
			uifont_print_center(132, COLOR_GRAY, TEXT(RETURN_TO_GAME));
			uifont_print_center(150, COLOR_WHITE, TEXT(DISCONNECT2));
		}

		sceKernelDelayThread(1000000/60/2);
		video_wait_vsync();
		video_copy_rect(draw_frame, show_frame, &rect, &rect);
		frame++;

		if (frame & 1)
		{
			while (adhoc_update)
			{
				sceKernelDelayThread(50);
			}

			if (control)
				buttons = send_data.buttons;
			else
				buttons = recv_data.buttons;
			send_data.buttons = recv_data.buttons = 0;

			adhoc_update = 1;

			if (buttons & PSP_CTRL_UP)
			{
				sel = 0;
			}
			else if (buttons & PSP_CTRL_DOWN)
			{
				sel = 1;
			}
			else if (buttons & PSP_CTRL_CIRCLE)
			{
				adhoc_paused = 0;
				if (sel == 1) Loop = LOOP_BROWSER;
			}
		}
	} while (adhoc_paused);

	autoframeskip_reset();
	sound_thread_enable(1);
}

#endif


/******************************************************************************
	入力ポートインタフェース関数
******************************************************************************/

/*------------------------------------------------------
	入力ポートの初期化(AdHoc)
------------------------------------------------------*/

#ifdef ADHOC
void adhoc_input_init(void)
{
	adhoc_thread = -1;
	adhoc_update = 0;
	adhoc_active = 0;
	adhoc_paused = 0;

	if (adhoc_enable)
	{
		adhoc_thread = sceKernelCreateThread("Input thread", adhoc_update_inputport, 0x11, 0x2000, 0, NULL);
		if (adhoc_thread < 0)
		{
			adhocTerm();
			adhoc_enable = 0;
		}
	}
}
#endif


/*------------------------------------------------------
	入力ポートの初期化
------------------------------------------------------*/

void input_init(void)
{
	input_ui_wait = 0;

	memset(neogeo_port_value, 0xff, sizeof(neogeo_port_value));
	memset(af_counter, 0, sizeof(af_counter));
	memset(input_flag, 0, sizeof(input_flag));
}


/*------------------------------------------------------
	入力ポートの終了
------------------------------------------------------*/

void input_shutdown(void)
{
#ifdef ADHOC
	if (adhoc_enable)
	{
		if (adhoc_thread >= 0)
		{
			adhoc_active = 0;
			sceKernelWaitThreadEnd(adhoc_thread, NULL);

			sceKernelDeleteThread(adhoc_thread);
			adhoc_thread = -1;
		}
		adhocTerm();
	}
#endif
}


/*------------------------------------------------------
	入力ポートをリセット
------------------------------------------------------*/

void input_reset(void)
{
	memset(neogeo_port_value, 0xff, sizeof(neogeo_port_value));
#ifdef ADHOC
	if (!adhoc_enable);
#endif
		neogeo_port_value[2] = 0x0f;
	setup_autofire();

#ifdef ADHOC
	if (adhoc_enable)
	{
		memset(send_data.port_value, 0xff, sizeof(send_data.port_value));
		memset(recv_data.port_value, 0xff, sizeof(recv_data.port_value));

		send_data.buttons = recv_data.buttons = 0;
		send_data.loop_flag = recv_data.loop_flag = LOOP_EXEC;

		if (!adhoc_active)
		{
			adhoc_active = 1;
			sceKernelStartThread(adhoc_thread, 0, 0);
		}
	}
#endif
}


/*------------------------------------------------------
	連射フラグを設定
------------------------------------------------------*/

void setup_autofire(void)
{
	int i;

	for (i = 0; i < NCDZ_BUTTON_MAX; i++)
	{
		af_map1[i] = input_map[P1_AF_A + i];
		af_map2[i] = input_map[P1_BUTTONA + i];
	}
}


/*------------------------------------------------------
	入力ポートを更新
------------------------------------------------------*/

void update_inputport(void)
{
#ifdef ADHOC
	if (adhoc_enable)
	{
		if (frames_displayed & 1)
		{
			while (adhoc_update && Loop == LOOP_EXEC)
			{
				sceKernelDelayThread(50);
			}
			neogeo_port_value[0] = send_data.port_value[0] & recv_data.port_value[0];
			neogeo_port_value[1] = send_data.port_value[1] & recv_data.port_value[1];
			neogeo_port_value[2] = send_data.port_value[2] & recv_data.port_value[2];
			adhoc_update = 1;

			if (adhoc_paused) adhoc_pause();
		}
	}
	else
#endif
	{
		int i;
		UINT32 buttons;

		buttons = poll_gamepad();

#ifdef KERNEL_MODE
		if (buttons & PSP_CTRL_HOME)
#else
		if ((buttons & PSP_CTRL_START) && (buttons & PSP_CTRL_SELECT))
#endif
		{
			showmenu();
			setup_autofire();
			buttons = poll_gamepad();
		}
		else if ((buttons & PSP_CTRL_RTRIGGER) && (buttons & PSP_CTRL_SELECT))
		{
			commandlist(1);
			buttons = poll_gamepad();
		}

		buttons = update_autofire(buttons);

		for (i = 0; i < MAX_INPUTS; i++)
			input_flag[i] = (buttons & input_map[i]) != 0;

		update_inputport0();
		update_inputport1();
		update_inputport2();

		if (input_flag[SNAPSHOT])
		{
			save_snapshot();
		}
		if (input_flag[SWPLAYER])
		{
			if (!input_ui_wait)
			{
				option_controller ^= 1;
				ui_popup(TEXT(CONTROLLER_PLAYERx), option_controller + 1);
				input_ui_wait = 30;
			}
		}
		if (input_ui_wait > 0) input_ui_wait--;
	}
}



/*------------------------------------------------------
	セーブ/ロード ステート
------------------------------------------------------*/

#ifdef SAVE_STATE

STATE_SAVE( input )
{
	state_save_long(&option_controller, 1);
}

STATE_LOAD( input )
{
	state_load_long(&option_controller, 1);
}

#endif /* SAVE_STATE */
