/******************************************************************************

	inptport.c

	MVS 入力ポートエミュレーション

******************************************************************************/

#include "mvs.h"


/******************************************************************************
	グローバル変数
******************************************************************************/

int option_controller;
UINT8 ALIGN_DATA neogeo_port_value[MVS_PORT_MAX];

int input_map[MAX_INPUTS];
int analog_sensitivity;
int af_interval = 1;

int neogeo_dipswitch;
int neogeo_input_mode;
int input_analog_value[2];


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
static int ALIGN_DATA af_map1[MVS_BUTTON_MAX];
static int ALIGN_DATA af_map2[MVS_BUTTON_MAX];
static int ALIGN_DATA af_counter[MVS_BUTTON_MAX];
static int input_ui_wait;
static int service_switch;

#ifdef ADHOC
typedef struct
{
	UINT32 buttons;
	int loop_flag;
	UINT16 port_value[5];
} ADHOC_DATA;

static ADHOC_DATA ALIGN_PSPDATA send_data;
static ADHOC_DATA ALIGN_PSPDATA recv_data;
static SceUID adhoc_thread;
static volatile int adhoc_active;
static volatile int adhoc_update;
static volatile UINT32 adhoc_paused;
#endif

static UINT32 (*poll_pad)(void);


/******************************************************************************
	ローカル関数
******************************************************************************/

/*------------------------------------------------------
	入力ポートタイプのチェック
------------------------------------------------------*/

void check_input_mode(void)
{
	if (machine_init_type == INIT_ms5pcb
	||	machine_init_type == INIT_svcpcb
	||	machine_init_type == INIT_kf2k3pcb)
	{
		neogeo_input_mode = INPUT_MVS;
		return;
	}
	else if (!neogeo_machine_mode)
	{
		if (memory_region_user1[0x00400 >> 1] & 0x8000)
			neogeo_input_mode = INPUT_AES;
		else
			neogeo_input_mode = INPUT_MVS;
	}
	else
	{
		neogeo_input_mode = neogeo_machine_mode - 1;
	}

	if (neogeo_ngh == NGH_irrmaze)
		return;

	switch (neogeo_bios)
	{
	case UNI_V10:
	case UNI_V11:
	case UNI_V12old:
	case UNI_V12:
	case UNI_V13:
	case UNI_V20:
	case UNI_V21:
	case UNI_V22:
		neogeo_input_mode = (neogeo_sram16[0x02 >> 1] & 0x8000) != 0;
		break;

	case ASIA_AES:
	case DEBUG_BIOS:
		neogeo_input_mode = INPUT_AES;
		break;
	}
}


/*------------------------------------------------------
	連射フラグを更新
------------------------------------------------------*/

static UINT32 update_autofire(UINT32 buttons)
{
	int i;

	for (i = 0; i < MVS_BUTTON_MAX; i++)
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
	for (i = 0; i < 11; i++)
	{
		if (input_flag[button]) value &= hotkey_mask[i];
		button++;
	}

	return value;
}


/*------------------------------------------------------
	MVS コントローラ1
------------------------------------------------------*/

static void update_inputport0(void)
{
	UINT8 value = 0xff;

	switch (neogeo_ngh)
	{
	case NGH_popbounc:
		if (!option_controller)
		{
			if (input_flag[P1_UP])      value &= ~0x01;
			if (input_flag[P1_DOWN])    value &= ~0x02;
			if (input_flag[P1_LEFT])    value &= ~0x04;
			if (input_flag[P1_RIGHT])   value &= ~0x08;
			if (input_flag[P1_BUTTONA]) value &= ~(0x10|0x80);
			if (input_flag[P1_BUTTONB]) value &= ~0x20;
			if (input_flag[P1_BUTTONC]) value &= ~0x40;
		}
		break;

	default:
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
		break;
	}

#ifdef ADHOC
	if (adhoc_enable)
		send_data.port_value[0] = value;
	else
#endif
		neogeo_port_value[0] = value;
}


/*------------------------------------------------------
	MVS コントローラ2
------------------------------------------------------*/

static void update_inputport1(void)
{
	UINT8 value = 0xff;

	switch (neogeo_ngh)
	{
	case NGH_irrmaze:
		{
			if (input_flag[P1_UP])      value &= ~0x01;
			if (input_flag[P1_DOWN])    value &= ~0x02;
			if (input_flag[P1_LEFT])    value &= ~0x04;
			if (input_flag[P1_RIGHT])   value &= ~0x08;
			if (input_flag[P1_BUTTONA]) value &= ~0x10;
			if (input_flag[P1_BUTTONB]) value &= ~0x20;
		}
		break;

	case NGH_popbounc:
		if (option_controller)
		{
			if (input_flag[P1_UP])      value &= ~0x01;
			if (input_flag[P1_DOWN])    value &= ~0x02;
			if (input_flag[P1_LEFT])    value &= ~0x04;
			if (input_flag[P1_RIGHT])   value &= ~0x08;
			if (input_flag[P1_BUTTONA]) value &= ~(0x10|0x80);
			if (input_flag[P1_BUTTONB]) value &= ~0x20;
			if (input_flag[P1_BUTTONC]) value &= ~0x40;
		}
		break;

	default:
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
		break;
	}

#ifdef ADHOC
	if (adhoc_enable)
		send_data.port_value[1] = value;
	else
#endif
		neogeo_port_value[1] = value;
}


/*------------------------------------------------------
	MVS スタートボタン
------------------------------------------------------*/

static void update_inputport2(void)
{
	UINT8 value = 0xff;

	switch (neogeo_ngh)
	{
	case NGH_vliner:
		if (input_flag[P1_START]) value &= ~0x01;
		break;

	case NGH_jockeygp:
		break;

	default:
		if (option_controller)
		{
			if (input_flag[P1_START]) value &= ~0x04;
			if (!neogeo_input_mode)
			{
				if (input_flag[P1_COIN]) value &= ~0x08;
			}
		}
		else
		{
			if (input_flag[P1_START]) value &= ~0x01;
			if (!neogeo_input_mode)
			{
				if (input_flag[P1_COIN]) value &= ~0x02;
			}
		}
		break;
	}

#ifdef ADHOC
	if (adhoc_enable)
		send_data.port_value[2] = value;
	else
#endif
		neogeo_port_value[2] = value;
}


/*------------------------------------------------------
	MVS コイン/サービススイッチ
------------------------------------------------------*/

static void update_inputport4(void)
{
	UINT8 value;

	switch (neogeo_ngh)
	{
	case NGH_vliner:
		{
			static int coin_wait = 0;

			value = 0xff;
			if (coin_wait == 0)
			{
				if (input_flag[P1_COIN])
				{
					value &= ~0x01;
					coin_wait = 12;	// コイン投入ウェイト
				}
			}
			else if (coin_wait)
			{
				// コイン投入ウェイト処理
				if (coin_wait > 4) value &= ~0x01;
				coin_wait--;
			}
			if (input_flag[OTHER1]) value &= ~0x10;
			if (input_flag[OTHER2]) value &= ~0x20;
			if (input_flag[OTHER3]) value &= ~0x80;
		}
		break;

	default:
		value = 0x3f;
		if (neogeo_input_mode)
		{
			if (option_controller)
			{
				if (input_flag[P1_COIN]) value &= ~0x02;
			}
			else
			{
				if (input_flag[P1_COIN]) value &= ~0x01;
				if (input_flag[SERV_COIN]) value &= ~0x04;
			}
		}
		break;
	}

#ifdef ADHOC
	if (adhoc_enable)
		send_data.port_value[4] = value;
	else
#endif
		neogeo_port_value[4] = value;
}


/*------------------------------------------------------
	MVS テストスイッチ
------------------------------------------------------*/

static void update_inputport5(void)
{
	UINT8 value = 0xc0;

	if (neogeo_input_mode)
	{
		if (input_flag[TEST_SWITCH] || service_switch) value &= ~0x80;
	}

#ifdef ADHOC
	if (adhoc_enable)
		send_data.port_value[5] = value;
	else
#endif
		neogeo_port_value[5] = value;
}


/*------------------------------------------------------
	irrmaze アナログ入力ポート
------------------------------------------------------*/

static void irrmaze_update_analog_port(UINT16 value)
{
	int axis, delta;
	int current, pad_value[2];

	pad_value[0] = value & 0xff;
	pad_value[1] = value >> 8;

	for (axis = 0; axis < 2; axis++)
	{
		current = pad_value[axis];

		delta = 0;
		if (axis)
		{
			if (input_flag[P1_UP]) delta = -1;
			if (input_flag[P1_DOWN]) delta = 1;
		}
		else
		{
			if (input_flag[P1_LEFT]) delta = -1;
			if (input_flag[P1_RIGHT]) delta = 1;
		}
		switch (analog_sensitivity)
		{
		case 0:
			if (current > 0x80)
			{
				if (current >= 0xe0) delta = 2;
				else if (current >= 0xa0) delta = 1;
			}
			else
			{
				if (current <= 0x1f) delta = -3;
				else if (current <= 0x5f) delta = -1;
			}
			break;

		case 1:
			if (current > 0x80)
			{
				if (current >= 0xf0) delta = 3;
				else if (current >= 0xd0) delta = 2;
				else if (current >= 0xa0) delta = 1;
			}
			else
			{
				if (current <= 0x0f) delta = -3;
				else if (current <= 0x2f) delta = -2;
				else if (current <= 0x5f) delta = -1;
			}
			break;

		case 2:
			if (current > 0x80)
			{
				if (current >= 0xf8) delta = 4;
				else if (current >= 0xe8) delta = 3;
				else if (current >= 0xd0) delta = 2;
				else if (current >= 0x98) delta = 1;
			}
			else
			{
				if (current <= 0x07) delta = -4;
				else if (current <= 0x17) delta = -3;
				else if (current <= 0x2f) delta = -2;
				else if (current <= 0x67) delta = -1;
			}
			break;
		}

		// reverse
		delta = -delta;

		input_analog_value[axis] += delta;
		input_analog_value[axis] &= 0xff;
	}
}


/*------------------------------------------------------
	popbounc アナログ入力ポート
------------------------------------------------------*/

static void popbounc_update_analog_port(UINT16 value)
{
	int delta, current;

	delta = 0;
	current = value & 0xff;

	switch (analog_sensitivity)
	{
	case 0:
		if (current > 0x80)
		{
			if (current >= 0xf0) delta = 3;
			else if (current >= 0xd0) delta = 2;
			else if (current >= 0xa0) delta = 1;
		}
		else
		{
			if (current <= 0x0f) delta = -3;
			else if (current <= 0x2f) delta = -2;
			else if (current <= 0x5f) delta = -1;
		}
		break;

	case 1:
		if (current > 0x80)
		{
			if (current >= 0xf8) delta = 4;
			else if (current >= 0xe8) delta = 3;
			else if (current >= 0xd0) delta = 2;
			else if (current >= 0x98) delta = 1;
		}
		else
		{
			if (current <= 0x07) delta = -4;
			else if (current <= 0x17) delta = -3;
			else if (current <= 0x2f) delta = -2;
			else if (current <= 0x67) delta = -1;
		}
		break;

	case 2:
		if (current > 0x80)
		{
			if (current >= 0xf8) delta = 5;
			else if (current >= 0xe8) delta = 4;
			else if (current >= 0xd8) delta = 3;
			else if (current >= 0xc0) delta = 2;
			else if (current >= 0x98) delta = 1;
		}
		else
		{
			if (current <= 0x07) delta = -5;
			else if (current <= 0x17) delta = -4;
			else if (current <= 0x27) delta = -3;
			else if (current <= 0x3f) delta = -2;
			else if (current <= 0x67) delta = -1;
		}
		break;
	}

	input_analog_value[option_controller] += delta;
	if (input_analog_value[option_controller] < 0)
		input_analog_value[option_controller] = 0;
	if (input_analog_value[option_controller] > 0xff)
		input_analog_value[option_controller] = 0xff;
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
			service_switch = 0;

			buttons = (*poll_pad)();

			if (buttons & PSP_CTRL_HOME)
			{
				buttons = 0;
				adhoc_paused = adhoc_server + 1;
			}
			else if ((buttons & PSP_CTRL_LTRIGGER) && (buttons & PSP_CTRL_RTRIGGER))
			{
				if (buttons & PSP_CTRL_SELECT)
				{
					buttons &= ~(PSP_CTRL_SELECT | PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER);
					service_switch = 1;
				}
			}

			buttons = update_autofire(buttons);

			for (i = 0; i < MAX_INPUTS; i++)
				input_flag[i] = (buttons & input_map[i]) != 0;

			update_inputport0();
			update_inputport1();
			update_inputport2();
			update_inputport4();
			update_inputport5();

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
	service_switch = 0;

	memset(neogeo_port_value, 0xff, sizeof(neogeo_port_value));
	memset(af_counter, 0, sizeof(af_counter));
	memset(input_flag, 0, sizeof(input_flag));

	input_analog_value[0] = 0x7f;
	input_analog_value[1] = 0x7f;

	neogeo_dipswitch = 0xff;

	if (neogeo_ngh == NGH_irrmaze || neogeo_ngh == NGH_popbounc)
	{
#ifdef ADHOC
		if (adhoc_enable)
			poll_pad = poll_gamepad;
		else
#endif
			poll_pad = poll_gamepad_analog;
	}
	else if (!strcmp(game_name, "fatfursp"))
	{
		poll_pad = poll_gamepad_fatfursp;
	}
	else
	{
		poll_pad = poll_gamepad;
	}
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
	input_analog_value[0] = 0x7f;
	input_analog_value[1] = 0x7f;
	service_switch = 0;

	check_input_mode();

	setup_autofire();

	if (neogeo_input_mode)
		neogeo_port_value[3] = neogeo_dipswitch & 0xff;

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

	for (i = 0; i < MVS_BUTTON_MAX; i++)
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
			neogeo_port_value[4] = send_data.port_value[4] & recv_data.port_value[4];
			neogeo_port_value[5] = send_data.port_value[5] & recv_data.port_value[5];
			adhoc_update = 1;

			if (adhoc_paused) adhoc_pause();
		}
	}
	else
#endif
	{
		int i;
		UINT32 buttons;

		service_switch = 0;

		buttons = (*poll_pad)();

#ifdef KERNEL_MODE
		if (buttons & PSP_CTRL_HOME)
#else
		if ((buttons & PSP_CTRL_START) && (buttons & PSP_CTRL_SELECT))
#endif
		{
			showmenu();
			setup_autofire();

			if (neogeo_input_mode)
				neogeo_port_value[3] = neogeo_dipswitch & 0xff;
			else
				neogeo_port_value[3] = 0xff;

			buttons = (*poll_pad)();
		}
		else if ((buttons & PSP_CTRL_LTRIGGER) && (buttons & PSP_CTRL_RTRIGGER))
		{
			if (buttons & PSP_CTRL_SELECT)
			{
				buttons &= ~(PSP_CTRL_SELECT | PSP_CTRL_LTRIGGER | PSP_CTRL_RTRIGGER);
				service_switch = 1;
			}
		}

		if (neogeo_ngh == NGH_irrmaze)
		{
			irrmaze_update_analog_port(buttons >> 16);
			buttons &= 0xffff;
		}
		else if (neogeo_ngh == NGH_popbounc)
		{
			popbounc_update_analog_port(buttons >> 16);
			buttons &= 0xffff;
		}

		buttons = update_autofire(buttons);

		for (i = 0; i < MAX_INPUTS; i++)
			input_flag[i] = (buttons & input_map[i]) != 0;

		update_inputport0();
		update_inputport1();
		update_inputport2();
		update_inputport4();
		update_inputport5();

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


/******************************************************************************
	セーブ/ロード ステート
******************************************************************************/

#ifdef SAVE_STATE

STATE_SAVE( input )
{
	state_save_long(&option_controller, 1);
	state_save_long(&neogeo_dipswitch, 1);
	state_save_long(&input_analog_value[0], 1);
	state_save_long(&input_analog_value[1], 1);
}

STATE_LOAD( input )
{
	state_load_long(&option_controller, 1);
	state_load_long(&neogeo_dipswitch, 1);
	state_load_long(&input_analog_value[0], 1);
	state_load_long(&input_analog_value[1], 1);

	check_input_mode();
}

#endif /* SAVE_STATE */
