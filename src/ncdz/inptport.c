/******************************************************************************

	inptport.c

	NEOGEO CDZ ���̓|�[�g�G�~�����[�V����

******************************************************************************/

#include "ncdz.h"


/******************************************************************************
	�O���[�o���ϐ�
******************************************************************************/

int option_controller;
UINT8 ALIGN_DATA neogeo_port_value[NCDZ_PORT_MAX];

int input_map[MAX_INPUTS];
int af_interval = 1;
int analog_sensitivity;	// not used


/******************************************************************************
	���[�J���ϐ�
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


/******************************************************************************
	���[�J���֐�
******************************************************************************/

/*------------------------------------------------------
	�A�˃t���O���X�V
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
	�z�b�g�L�[�t���O�𔽉f
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
	NEOGEO CDZ �R���g���[��1
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

	neogeo_port_value[0] = value;
}


/*------------------------------------------------------
	NEOGEO CDZ �R���g���[��2
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

	neogeo_port_value[1] = value;
}


/*------------------------------------------------------
	NEOGEO CDZ START/SELECT�{�^��
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

	neogeo_port_value[2] = value;
}


/******************************************************************************
	���̓|�[�g�C���^�t�F�[�X�֐�
******************************************************************************/

/*------------------------------------------------------
	���̓|�[�g�̏�����
------------------------------------------------------*/

int input_init(void)
{
	input_ui_wait = 0;

	memset(neogeo_port_value, 0xff, sizeof(neogeo_port_value));
	memset(af_counter, 0, sizeof(af_counter));
	memset(input_flag, 0, sizeof(input_flag));

	return 1;
}


/*------------------------------------------------------
	���̓|�[�g�̏I��
------------------------------------------------------*/

void input_shutdown(void)
{
}


/*------------------------------------------------------
	���̓|�[�g�����Z�b�g
------------------------------------------------------*/

void input_reset(void)
{
	memset(neogeo_port_value, 0xff, sizeof(neogeo_port_value));
	neogeo_port_value[2] = 0x0f;
	setup_autofire();
}


/*------------------------------------------------------
	�A�˃t���O��ݒ�
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
	���̓|�[�g���X�V
------------------------------------------------------*/

void update_inputport(void)
{
	int i;
	UINT32 buttons;

	buttons = poll_gamepad();

	if (systembuttons_available ? readHomeButton() :
		(buttons & PSP_CTRL_START) && (buttons & PSP_CTRL_SELECT))
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



/*------------------------------------------------------
	�Z�[�u/���[�h �X�e�[�g
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
