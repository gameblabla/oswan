/*
$Date: 2009-10-30 05:26:46 +0100 (ven., 30 oct. 2009) $
$Rev: 71 $
*/

#ifndef WSFILEIO_H_
#define WSFILEIO_H_

void WsSetDir(char* path);
int WsCreate(char *CartName);
void WsRelease(void);
void WsLoadIEep(void);
void WsSaveIEep(void);
void WsLoadState(const char *savename, int num);
void WsSaveState(const char *savename, int num);

#endif
