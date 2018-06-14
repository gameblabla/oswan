/*
$Date: 2010-05-18 19:16:59 -0400 (Tue, 18 May 2010) $
$Rev: 97 $
*/

#ifndef WSFILEIO_H_
#define WSFILEIO_H_

int32_t WsCreate(int8_t *CartName);
void WsRelease(void);
void WsLoadEeprom(void);
void WsSaveEeprom(void);
int32_t WsLoadState(const int8_t *savename, int32_t num);
int32_t WsSaveState(const int8_t *savename, int32_t num);

#endif
