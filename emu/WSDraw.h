/*
$Date: 2010-05-11 09:13:27 -0400 (Tue, 11 May 2010) $
$Rev: 85 $
*/

#define BOOL int
#define HRESULT int

#ifndef WSDRAW_H_
#define WSDRAW_H_

extern int Kerorikan;

enum DRAWSIZE {
    DS_1 = 1,
    DS_2 = 2,
    DS_3 = 3,
    DS_4 = 4,
    DS_FULL = 255
};

HRESULT drawInitialize(BOOL isFullScreen);
void    drawFinalize(void);
HRESULT drawCreate(void);
void    drawDestroy(void);
void    drawDraw(void);
void    WsResize(void);
int     SetDrawMode(int Mode);
int     SetDrawSize(enum DRAWSIZE Size);

#endif
