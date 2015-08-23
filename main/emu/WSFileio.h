/*
$Date: 2010-05-18 19:16:59 -0400 (Tue, 18 May 2010) $
$Rev: 97 $
*/

#ifndef WSFILEIO_H_
#define WSFILEIO_H_

#define wchar_t char*

int WsCreate(char *CartName);
void WsRelease(void);
void WsLoadEeprom(void);
void WsSaveEeprom(void);
int WsLoadState(const char *savename, int num);
int WsSaveState(const char *savename, int num);

#endif
