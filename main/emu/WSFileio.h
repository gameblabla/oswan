/*
$Date: 2010-05-18 19:16:59 -0400 (Tue, 18 May 2010) $
$Rev: 97 $
*/

#ifndef WSFILEIO_H_
#define WSFILEIO_H_

uint32_t WsCreate(char *CartName);
void WsRelease(void);
void WsLoadEeprom(void);
void WsSaveEeprom(void);
uint32_t WsLoadState(const char *savename, uint32_t num);
uint32_t WsSaveState(const char *savename, uint32_t num);

#endif
