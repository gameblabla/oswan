/*
$Date: 2010-09-29 04:51:41 -0400 (Wed, 29 Sep 2010) $
$Rev: 102 $
*/

#include <windows.h>
#include <stdio.h>
#include "WSconfig.h"
#include "WSInput.h"
#include "WSFileio.h"

extern wchar_t* RecentOfn[];
extern int WsWaveVol;
static LPCWSTR KeyName[] = {
    L"B", L"A", L"START", L"OPTION", 
    L"X4", L"X3", L"X2", L"X1", 
    L"Y4", L"Y3", L"Y2", L"Y1",
    L"NOWAIT"
};

void ConfigCreate(void)
{
    int i;

    /* default values */
    WsKeyboardH[0] = DIK_Z; // B
    WsKeyboardH[1] = DIK_X; // A
    WsKeyboardH[2] = DIK_RETURN; // START
    WsKeyboardH[3] = DIK_LCONTROL; // OPTION
    WsKeyboardH[4] = DIK_LEFT; // X4
    WsKeyboardH[5] = DIK_DOWN; // X3
    WsKeyboardH[6] = DIK_RIGHT; // X2
    WsKeyboardH[7] = DIK_UP; // X1
    WsKeyboardH[8] = DIK_A; // Y4
    WsKeyboardH[9] = DIK_S; // Y3
    WsKeyboardH[10] = DIK_D; // Y2
    WsKeyboardH[11] = DIK_W; // Y1
    WsKeyboardH[12] = DIK_SPACE; // NO WAIT

    WsKeyboardV[0] = DIK_Z; // B
    WsKeyboardV[1] = DIK_X; // A
    WsKeyboardV[2] = DIK_RETURN; // START
    WsKeyboardV[3] = DIK_LCONTROL; // OPTION
    WsKeyboardV[4] = DIK_S; // X4
    WsKeyboardV[5] = DIK_D; // X3
    WsKeyboardV[6] = DIK_W; // X2
    WsKeyboardV[7] = DIK_A; // X1
    WsKeyboardV[8] = DIK_DOWN; // Y4
    WsKeyboardV[9] = DIK_RIGHT; // Y3
    WsKeyboardV[10] = DIK_UP; // Y2
    WsKeyboardV[11] = DIK_LEFT; // Y1
    WsKeyboardV[12] = DIK_SPACE; // NO WAIT
    
    WsJoypadH[0] = 1; // B
    WsJoypadH[1] = 2; // A
    WsJoypadH[2] = 8; // START
    WsJoypadH[3] = 0; // OPTION
    WsJoypadH[4] = WS_JOY_AXIS_X_M; // X4
    WsJoypadH[5] = WS_JOY_AXIS_Y_P; // X3
    WsJoypadH[6] = WS_JOY_AXIS_X_P; // X2
    WsJoypadH[7] = WS_JOY_AXIS_Y_M; // X1
    WsJoypadH[8] = WS_JOY_POV1_LEFT; // Y4
    WsJoypadH[9] = WS_JOY_POV1_DOWN; // Y3
    WsJoypadH[10] = WS_JOY_POV1_RIGHT; // Y2
    WsJoypadH[11] = WS_JOY_POV1_UP; // Y1
    WsJoypadH[12] = 7; // NO WAIT
    
    WsJoypadV[0] = 5; // B
    WsJoypadV[1] = 6; // A
    WsJoypadV[2] = 8; // START
    WsJoypadV[3] = 0; // OPTION
    WsJoypadV[4] = 1; // X4
    WsJoypadV[5] = 2; // X3
    WsJoypadV[6] = 4; // X2
    WsJoypadV[7] = 3; // X1
    WsJoypadV[8] = WS_JOY_AXIS_Y_P; // Y4
    WsJoypadV[9] = WS_JOY_AXIS_X_P; // Y3
    WsJoypadV[10] = WS_JOY_AXIS_Y_M; // Y2
    WsJoypadV[11] = WS_JOY_AXIS_X_M; // Y1
    WsJoypadV[12] = 7; // NO WAIT
    
    for (i = 12; i >= 0; i--)
    {
         WsKeyboardH[i] = GetPrivateProfileIntW(L"KEY_H", KeyName[i], WsKeyboardH[i], IniPath);
    }
    for (i = 12; i >= 0; i--)
    {
        WsKeyboardV[i] = GetPrivateProfileIntW(L"KEY_V", KeyName[i], WsKeyboardV[i], IniPath);
    }
    for (i = 12; i >= 0; i--)
    {
        WsJoypadH[i] = GetPrivateProfileIntW(L"JOY_H", KeyName[i], WsJoypadH[i], IniPath);
    }
    for (i = 12; i >= 0; i--)
    {
        WsJoypadV[i] = GetPrivateProfileIntW(L"JOY_V", KeyName[i], WsJoypadV[i], IniPath);
    }
    GetPrivateProfileStringW(L"RECENT_ROMS", L"0", L"", RecentOfn[0], 512, IniPath);
    GetPrivateProfileStringW(L"RECENT_ROMS", L"1", L"", RecentOfn[1], 512, IniPath);
    GetPrivateProfileStringW(L"RECENT_ROMS", L"2", L"", RecentOfn[2], 512, IniPath);
    GetPrivateProfileStringW(L"RECENT_ROMS", L"3", L"", RecentOfn[3], 512, IniPath);
    GetPrivateProfileStringW(L"RECENT_ROMS", L"4", L"", RecentOfn[4], 512, IniPath);
    GetPrivateProfileStringW(L"RECENT_ROMS", L"5", L"", RecentOfn[5], 512, IniPath);
    WsWaveVol = GetPrivateProfileIntW(L"VOLUME", L"VOL", 4, IniPath);
}

static void WritePrivateProfileIntW(LPCWSTR lpAppName, LPCWSTR lpKeyName, int nInt, LPCWSTR lpFileName)
{
    wchar_t s[32];

    swprintf_s(s, 32, L"%d", nInt);
    WritePrivateProfileStringW(lpAppName, lpKeyName, s, lpFileName);
}

void ConfigRelease(void)
{
    int i;

    for (i = 12; i >= 0; i--)
    {
        WritePrivateProfileIntW(L"KEY_H", KeyName[i], WsKeyboardH[i], IniPath);
    }
    for (i = 12; i >= 0; i--)
    {
        WritePrivateProfileIntW(L"KEY_V", KeyName[i], WsKeyboardV[i], IniPath);
    }
    for (i = 12; i >= 0; i--)
    {
        WritePrivateProfileIntW(L"JOY_H", KeyName[i], WsJoypadH[i], IniPath);
    }
    for (i = 12; i >= 0; i--)
    {
        WritePrivateProfileIntW(L"JOY_V", KeyName[i], WsJoypadV[i], IniPath);
    }
    WritePrivateProfileStringW(L"RECENT_ROMS", L"0", RecentOfn[0], IniPath);
    WritePrivateProfileStringW(L"RECENT_ROMS", L"1", RecentOfn[1], IniPath);
    WritePrivateProfileStringW(L"RECENT_ROMS", L"2", RecentOfn[2], IniPath);
    WritePrivateProfileStringW(L"RECENT_ROMS", L"3", RecentOfn[3], IniPath);
    WritePrivateProfileStringW(L"RECENT_ROMS", L"4", RecentOfn[4], IniPath);
    WritePrivateProfileStringW(L"RECENT_ROMS", L"5", RecentOfn[5], IniPath);
    WritePrivateProfileIntW(L"VOLUME", L"VOL", WsWaveVol, IniPath);
}
