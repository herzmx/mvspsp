/******************************************************************************

	ticker.c

	���Ԍv���p�J�E���^�֐� (64bit)

******************************************************************************/

#include "psp.h"


/******************************************************************************
	�O���[�o���ϐ�
******************************************************************************/

//TICKER TICKS_PER_SEC;


/******************************************************************************
	�O���[�o���֐�
******************************************************************************/

/*--------------------------------------------------------
	RTC�̕���\���擾
--------------------------------------------------------*/

#if 0
void ticker_init(void)
{
	TICKS_PER_SEC = (UINT64)sceRtcGetTickResolution();		// 1000000
}
#endif


/*--------------------------------------------------------
	���݂̃J�E���^���擾
--------------------------------------------------------*/

TICKER ticker(void)
{
	UINT64 current_ticks;

	sceRtcGetCurrentTick(&current_ticks);
	return current_ticks;
}