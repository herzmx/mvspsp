/******************************************************************************

	eeprom.c

	CPS2 EEPROMインタフェース関数

******************************************************************************/

#ifndef CPS2_EEPROM_H
#define CPS2_EEPROM_H

void EEPROM_init(void);
void EEPROM_write_bit(int bit);
int  EEPROM_read_bit(void);
void EEPROM_set_cs_line(int state);
void EEPROM_set_clock_line(int state);
void EEPROM_load(FILE *file);
void EEPROM_save(FILE *file);
UINT8   EEPROM_read_data(UINT32 address);
void EEPROM_write_data(UINT32 address, UINT8 data);

#ifdef SAVE_STATE
STATE_SAVE( eeprom );
STATE_LOAD( eeprom );
#endif

#ifdef ADHOC
int adhoc_send_eeprom(void);
int adhoc_recv_eeprom(void);
#endif /* ADHOC */

#endif /* CPS2_EEPROM_H */
