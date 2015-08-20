/*
$Date: 2010-09-29 04:51:41 -0400 (Wed, 29 Sep 2010) $
$Rev: 102 $
*/

#include "../resource.h"
#include "WSDialog.h"
#include <commctrl.h>
#include <tchar.h>
#include "WSInput.h"
#include "keycode.h"

static WNDPROC OrgEditProc;
static HWND hTabCtrl, hTab1, hTab2, hTab3, hTab4;
static int TmpKeyboardH[13];
static int TmpKeyboardV[13];
static int TmpJoypadH[13];
static int TmpJoypadV[13];
static int SelectedTab;

void WsDlgConfInit(HWND hDlg)
{
    HINSTANCE hInst;
    TC_ITEM tc;
    RECT    rt;
    LPPOINT pt = (LPPOINT)&rt;

    memcpy(TmpKeyboardH, WsKeyboardH, sizeof(int) * 13);
    memcpy(TmpKeyboardV, WsKeyboardV, sizeof(int) * 13);
    memcpy(TmpJoypadH, WsJoypadH, sizeof(int) * 13);
    memcpy(TmpJoypadV, WsJoypadV, sizeof(int) * 13);
    hInst = (HINSTANCE)GetWindowLong(hDlg, GWLP_HINSTANCE);
    hTabCtrl = GetDlgItem(hDlg, IDC_TAB1);
    // タブコントロールにタブシートを挿入
    tc.mask = TCIF_TEXT;
    tc.pszText = TEXT("キー横");
    TabCtrl_InsertItem(hTabCtrl , 0, &tc);
    tc.mask = TCIF_TEXT;
    tc.pszText = TEXT("キー縦");
    TabCtrl_InsertItem(hTabCtrl , 1, &tc);
    tc.mask = TCIF_TEXT;
    tc.pszText = TEXT("コントローラー横");
    TabCtrl_InsertItem(hTabCtrl , 2, &tc);
    tc.mask = TCIF_TEXT;
    tc.pszText = TEXT("コントローラー縦");
    TabCtrl_InsertItem(hTabCtrl , 3, &tc);
    // タブに貼り付けるダイアログを生成
    hTab1 = CreateDialog(hInst, (LPCTSTR)IDD_CONFIG_TAB1, hDlg, (DLGPROC)TabProc1);
    hTab2 = CreateDialog(hInst, (LPCTSTR)IDD_CONFIG_TAB2, hDlg, (DLGPROC)TabProc2);
    hTab3 = CreateDialog(hInst, (LPCTSTR)IDD_CONFIG_TAB1, hDlg, (DLGPROC)TabProc3);
    hTab4 = CreateDialog(hInst, (LPCTSTR)IDD_CONFIG_TAB2, hDlg, (DLGPROC)TabProc4);
    // タブコントロールのクライアント領域の座標を取得
    GetClientRect(hTabCtrl, &rt);
    TabCtrl_AdjustRect(hTabCtrl, FALSE, &rt);
    // 親ウィンドウがhDlgなのでタブのマップが必要
    MapWindowPoints(hTabCtrl, hDlg, pt, 2);
    // タブのウィンドウの位置とサイズを変更する
    MoveWindow(hTab1, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, FALSE);
    MoveWindow(hTab2, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, FALSE);
    MoveWindow(hTab3, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, FALSE);
    MoveWindow(hTab4, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, FALSE);
    // デフォルトでタブ1を表示
    ShowWindow(hTab1, SW_SHOW);
    SetFocus(GetDlgItem(hTab1, IDC_EDIT_Y1));
    SendMessage(GetDlgItem(hTab1, IDC_EDIT_Y1), EM_SETSEL, 0, -1);
    IDirectInputDevice8_Acquire(lpKeyDevice);
}

LRESULT CALLBACK ConfProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    NMHDR *nm = (NMHDR *)lParam;

    switch (msg)
    {
    case WM_INITDIALOG:
        WsInputInit(hDlg);
        WsDlgConfInit(hDlg);
        return TRUE;
    case WM_NOTIFY:
        // タブコントロールの選択されているタブが変更されたことを通知
        switch (nm->code)
        {
        case TCN_SELCHANGE:
            if (nm->hwndFrom == hTabCtrl)
            {
                SelectedTab = TabCtrl_GetCurSel(hTabCtrl);
                switch (SelectedTab)
                {
                case 0:
                    ShowWindow(hTab1, SW_SHOW);
                    ShowWindow(hTab2, SW_HIDE);
                    ShowWindow(hTab3, SW_HIDE);
                    ShowWindow(hTab4, SW_HIDE);
                    SetFocus(GetDlgItem(hTab1, IDC_EDIT_Y1));
                    SendMessage(GetDlgItem(hTab1, IDC_EDIT_Y1), EM_SETSEL, 0, -1);
                    IDirectInputDevice8_Acquire(lpKeyDevice);
                    break;
                case 1:
                    ShowWindow(hTab1, SW_HIDE);
                    ShowWindow(hTab2, SW_SHOW);
                    ShowWindow(hTab3, SW_HIDE);
                    ShowWindow(hTab4, SW_HIDE);
                    SetFocus(GetDlgItem(hTab2, IDC_EDIT_Y1));
                    SendMessage(GetDlgItem(hTab2, IDC_EDIT_Y1), EM_SETSEL, 0, -1);
                    IDirectInputDevice8_Acquire(lpKeyDevice);
                    break;
                case 2:
                    ShowWindow(hTab1, SW_HIDE);
                    ShowWindow(hTab2, SW_HIDE);
                    ShowWindow(hTab3, SW_SHOW);
                    ShowWindow(hTab4, SW_HIDE);
                    SetFocus(GetDlgItem(hTab3, IDC_EDIT_Y1));
                    SendMessage(GetDlgItem(hTab3, IDC_EDIT_Y1), EM_SETSEL, 0, -1);
                    break;
                case 3:
                    ShowWindow(hTab1, SW_HIDE);
                    ShowWindow(hTab2, SW_HIDE);
                    ShowWindow(hTab3, SW_HIDE);
                    ShowWindow(hTab4, SW_SHOW);
                    SetFocus(GetDlgItem(hTab4, IDC_EDIT_Y1));
                    SendMessage(GetDlgItem(hTab4, IDC_EDIT_Y1), EM_SETSEL, 0, -1);
                    break;
                }
                return TRUE;
            }
            break;
        }
        break;
    case WM_COMMAND:
        if(HIWORD(wParam) == BN_CLICKED)
        {
            switch (LOWORD(wParam))
            {
            case IDOK:
                memcpy(WsKeyboardH, TmpKeyboardH, sizeof(int) * 13);
                memcpy(WsKeyboardV, TmpKeyboardV, sizeof(int) * 13);
                memcpy(WsJoypadH, TmpJoypadH, sizeof(int) * 13);
                memcpy(WsJoypadV, TmpJoypadV, sizeof(int) * 13);
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            case IDCANCEL:
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
        }
        break;
    case WM_DESTROY:
        WsInputRelease();
        break;
    }
    return FALSE;
}

LRESULT CALLBACK EditProcKey(HWND hEditWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HRESULT hRet;
    BYTE    diKeys[256];
    int key, i;
    HWND next;

    switch (msg)
    {
    case WM_GETDLGCODE:
        return DLGC_WANTALLKEYS;
    case WM_CHAR:
        return 0;
    case WM_KEYDOWN:
        key = GetDlgCtrlID(hEditWnd) - IDC_EDIT_B;
        wParam = 0;
        hRet = IDirectInputDevice8_Acquire(lpKeyDevice);
        if (hRet == DI_OK || hRet == S_FALSE)
        {
            hRet = IDirectInputDevice8_GetDeviceState(lpKeyDevice, 256, diKeys);
            if (hRet == DI_OK)
            {
                for (i = 0; i < 256; i++)
                {
                    if (diKeys[i] & 0x80)
                    {
                        SetWindowText(hEditWnd, keyName[i]);
                        if (SelectedTab == 0)
                        {
                            TmpKeyboardH[key] = i;
                        }
                        else
                        {
                            TmpKeyboardV[key] = i;
                        }
                        next = GetNextDlgTabItem(GetParent(hEditWnd), hEditWnd, FALSE);
                        SetFocus(next);
                        SendMessage(next, EM_SETSEL, 0, -1);
                        return 0;
                    }
                }
            }
        }
        break;
    }
    return CallWindowProc(OrgEditProc, hEditWnd, msg, wParam, lParam);
}

LRESULT CALLBACK EditProcJoy(HWND hEditWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND next;

    switch (msg)
    {
    case WM_GETDLGCODE:
        return DLGC_WANTALLKEYS;
    case WM_CHAR:
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_TAB || wParam == VK_RETURN)
        {
            next = GetNextDlgTabItem(GetParent(hEditWnd), hEditWnd, FALSE);
            SetFocus(next);
            SendMessage(next, EM_SETSEL, 0, -1);
        }
        return 0;
    }
    return CallWindowProc(OrgEditProc, hEditWnd, msg, wParam, lParam);
}

LRESULT CALLBACK TabProc1(HWND hCtrl, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_INITDIALOG:
        OrgEditProc = (WNDPROC)GetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y1), GWLP_WNDPROC);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_NOWAIT), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y1), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y2), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y3), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y4), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X1), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X2), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X3), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X4), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_START), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_A), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_B), GWLP_WNDPROC, (LONG_PTR)EditProcKey);

        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_NOWAIT), keyName[TmpKeyboardH[12]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y1), keyName[TmpKeyboardH[11]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y2), keyName[TmpKeyboardH[10]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y3), keyName[TmpKeyboardH[9]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y4), keyName[TmpKeyboardH[8]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X1), keyName[TmpKeyboardH[7]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X2), keyName[TmpKeyboardH[6]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X3), keyName[TmpKeyboardH[5]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X4), keyName[TmpKeyboardH[4]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_START), keyName[TmpKeyboardH[2]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_A), keyName[TmpKeyboardH[1]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_B), keyName[TmpKeyboardH[0]]);
        return TRUE;
    }
    return FALSE;
}

LRESULT CALLBACK TabProc2(HWND hCtrl, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_INITDIALOG:
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_NOWAIT), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y1), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y2), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y3), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y4), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X1), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X2), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X3), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X4), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_START), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_A), GWLP_WNDPROC, (LONG_PTR)EditProcKey);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_B), GWLP_WNDPROC, (LONG_PTR)EditProcKey);

        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_NOWAIT), keyName[TmpKeyboardH[12]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y1), keyName[TmpKeyboardV[11]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y2), keyName[TmpKeyboardV[10]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y3), keyName[TmpKeyboardV[9]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y4), keyName[TmpKeyboardV[8]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X1), keyName[TmpKeyboardV[7]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X2), keyName[TmpKeyboardV[6]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X3), keyName[TmpKeyboardV[5]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X4), keyName[TmpKeyboardV[4]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_START), keyName[TmpKeyboardV[2]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_A), keyName[TmpKeyboardV[1]]);
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_B), keyName[TmpKeyboardV[0]]);
        return TRUE;
    }
    return FALSE;
}

LRESULT CALLBACK TabProc3(HWND hCtrl, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditWnd;
    int key, joy;

    switch (msg) {
    case WM_INITDIALOG:
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_NOWAIT), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y1), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y2), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y3), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y4), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X1), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X2), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X3), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X4), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_START), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_A), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_B), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);

        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_NOWAIT), GetJoyName(TmpJoypadH[12]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y1), GetJoyName(TmpJoypadH[11]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y2), GetJoyName(TmpJoypadH[10]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y3), GetJoyName(TmpJoypadH[9]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y4), GetJoyName(TmpJoypadH[8]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X1), GetJoyName(TmpJoypadH[7]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X2), GetJoyName(TmpJoypadH[6]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X3), GetJoyName(TmpJoypadH[5]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X4), GetJoyName(TmpJoypadH[4]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_START), GetJoyName(TmpJoypadH[2]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_A), GetJoyName(TmpJoypadH[1]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_B), GetJoyName(TmpJoypadH[0]));

        SetTimer(hCtrl, 0, 30, NULL);
        return TRUE;
    case WM_TIMER:
        if (SelectedTab != 2 || wParam != 0)
        {
            break;
        }
        hEditWnd = GetFocus();
        key = GetDlgCtrlID(hEditWnd) - IDC_EDIT_B;
        joy = GetJoyState();
        if (joy < 0)
        {
            break;
        }
        SetWindowText(hEditWnd, GetJoyName(joy));
        TmpJoypadH[key] = joy;
        return TRUE;
    }
    return FALSE;
}

LRESULT CALLBACK TabProc4(HWND hCtrl, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditWnd;
    int key, joy;

    switch (msg) {
    case WM_INITDIALOG:
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_NOWAIT), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y1), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y2), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y3), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_Y4), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X1), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X2), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X3), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_X4), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_START), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_A), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);
        SetWindowLongPtr(GetDlgItem(hCtrl, IDC_EDIT_B), GWLP_WNDPROC, (LONG_PTR)EditProcJoy);

        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_NOWAIT), GetJoyName(TmpJoypadH[12]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y1), GetJoyName(TmpJoypadV[11]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y2), GetJoyName(TmpJoypadV[10]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y3), GetJoyName(TmpJoypadV[9]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_Y4), GetJoyName(TmpJoypadV[8]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X1), GetJoyName(TmpJoypadV[7]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X2), GetJoyName(TmpJoypadV[6]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X3), GetJoyName(TmpJoypadV[5]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_X4), GetJoyName(TmpJoypadV[4]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_START), GetJoyName(TmpJoypadV[2]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_A), GetJoyName(TmpJoypadV[1]));
        SetWindowText(GetDlgItem(hCtrl, IDC_EDIT_B), GetJoyName(TmpJoypadV[0]));

        SetTimer(hCtrl, 1, 30, NULL);
        return TRUE;
    case WM_TIMER:
        if (SelectedTab != 3 || wParam != 1)
        {
            break;
        }
        hEditWnd = GetFocus();
        key = GetDlgCtrlID(hEditWnd) - IDC_EDIT_B;
        joy = GetJoyState();
        if (joy < 0)
        {
            break;
        }
        SetWindowText(hEditWnd, GetJoyName(joy));
        TmpJoypadV[key] = joy;
        return TRUE;
    }
    return FALSE;
}

LPCTSTR GetJoyName(int joy)
{
    static LPCTSTR JoyStr[] = {
        TEXT("POV1 UP"), TEXT("POV1 RIGHT"), TEXT("POV1 DOWN"), TEXT("POV1 LEFT"),
        TEXT("POV2 UP"), TEXT("POV2 RIGHT"), TEXT("POV2 DOWN"), TEXT("POV2 LEFT"),
        TEXT("POV3 UP"), TEXT("POV3 RIGHT"), TEXT("POV3 DOWN"), TEXT("POV3 LEFT"),
        TEXT("POV4 UP"), TEXT("POV4 RIGHT"), TEXT("POV4 DOWN"), TEXT("POV4 LEFT"),
        TEXT("Y -"), TEXT("X +"), TEXT("Y +"), TEXT("X -"), TEXT("Z +"), TEXT("Z -"),
        TEXT("RY -"), TEXT("RX +"), TEXT("RY +"), TEXT("RX -"), TEXT("RZ +"), TEXT("RZ -"),
        TEXT("Slider1 +"), TEXT("Slider1 -"), TEXT("Slider2 +"), TEXT("Slider2 -")
    };
    static const int JoyInt[] = {
        WS_JOY_POV1_UP, WS_JOY_POV1_RIGHT, WS_JOY_POV1_DOWN, WS_JOY_POV1_LEFT,
        WS_JOY_POV2_UP, WS_JOY_POV2_RIGHT, WS_JOY_POV2_DOWN, WS_JOY_POV2_LEFT,
        WS_JOY_POV3_UP, WS_JOY_POV3_RIGHT, WS_JOY_POV3_DOWN, WS_JOY_POV3_LEFT,
        WS_JOY_POV4_UP, WS_JOY_POV4_RIGHT, WS_JOY_POV4_DOWN, WS_JOY_POV4_LEFT,
        WS_JOY_AXIS_Y_M, WS_JOY_AXIS_X_P, WS_JOY_AXIS_Y_P, WS_JOY_AXIS_X_M, WS_JOY_AXIS_Z_P, WS_JOY_AXIS_Z_M,
        WS_JOY_AXIS_RY_M, WS_JOY_AXIS_RX_P, WS_JOY_AXIS_RY_P, WS_JOY_AXIS_RX_M, WS_JOY_AXIS_RZ_P, WS_JOY_AXIS_RZ_M,
        WS_JOY_SLIDER1_P, WS_JOY_SLIDER1_M, WS_JOY_SLIDER2_P, WS_JOY_SLIDER2_M
    };
    static TCHAR buf[8];
    int i;

    if (joy < 0x80)
    {
        _stprintf_s(buf, 8, TEXT("%d"), joy);
        return buf;
    }
    for (i = 0; i < 32; i++)
    {
        if (joy == JoyInt[i])
        {
            return JoyStr[i];
        }
    }
    buf[0] = 0;
    return buf;
}

int GetJoyState(void)
{
    const long joyCenter = 0x7fff;
    static int xFlag  = 0;
    static int yFlag  = 0;
    static int zFlag  = 0;
    static int rxFlag = 0;
    static int ryFlag = 0;
    static int rzFlag = 0;
    static int s1Flag = 0;
    static int s2Flag = 0;
    HRESULT      hRet;
    int          joy;
    unsigned int i;
    DIJOYSTATE2  js;
    DIDEVCAPS    diDevCaps;

    diDevCaps.dwSize = sizeof(DIDEVCAPS);
    if (lpJoyDevice == NULL)
    {
        return -1;
    }
    hRet = IDirectInputDevice8_Poll(lpJoyDevice);
    if (FAILED(hRet))
    {
        hRet = IDirectInputDevice8_Acquire(lpJoyDevice);
        while (hRet == DIERR_INPUTLOST)
        {
            hRet = IDirectInputDevice8_Acquire(lpJoyDevice);
        }
        return -1;
    }
    IDirectInputDevice8_GetDeviceState(lpJoyDevice, sizeof(DIJOYSTATE2), &js);
    IDirectInputDevice8_GetCapabilities(lpJoyDevice, &diDevCaps);
    for (i = 0; i < diDevCaps.dwButtons; i++)
    {
        if (js.rgbButtons[i] & 0x80)
        {
            return i + 1;
        }
    }
    for (i = 0; i < diDevCaps.dwPOVs; i++)
    {
        joy = WS_JOY_POV1_UP + (i << 4);
        if (js.rgdwPOV[i] == JOY_POVFORWARD)
        {
            return joy;
        }
        else if (js.rgdwPOV[i] == JOY_POVRIGHT)
        {
            return joy + 1;
        }
        else if (js.rgdwPOV[i] == JOY_POVBACKWARD)
        {
            return joy + 3;
        }
        else if (js.rgdwPOV[i] == JOY_POVLEFT)
        {
            return joy + 7;
        }
    }
    if ((js.lX > joyCenter - 0x1000) && (js.lX < joyCenter + 0x1000))
    {
        xFlag = 1;
    }
    if ((js.lX > (joyCenter + 0x4000)) && xFlag)
    {
        xFlag = 0;
        return WS_JOY_AXIS_X_P;
    }
    else if ((js.lX < (joyCenter - 0x4000)) && xFlag)
    {
        xFlag = 0;
        return WS_JOY_AXIS_X_M;
    }
    if ((js.lY > joyCenter - 0x1000) && (js.lY < joyCenter + 0x1000))
    {
        yFlag = 1;
    }
    if ((js.lY > (joyCenter + 0x4000)) && yFlag)
    {
        yFlag = 0;
        return WS_JOY_AXIS_Y_P;
    }
    else if ((js.lY < (joyCenter - 0x4000)) && yFlag)
    {
        yFlag = 0;
        return WS_JOY_AXIS_Y_M;
    }
    if ((js.lZ > joyCenter - 0x1000) && (js.lZ < joyCenter + 0x1000))
    {
        zFlag = 1;
    }
    if ((js.lZ > (joyCenter + 0x4000)) && zFlag)
    {
        zFlag = 0;
        return WS_JOY_AXIS_Z_P;
    }
    else if ((js.lZ < (joyCenter - 0x4000)) && zFlag)
    {
        zFlag = 0;
        return WS_JOY_AXIS_Z_M;
    }
    if ((js.lRx > joyCenter - 0x1000) && (js.lRx < joyCenter + 0x1000))
    {
        rxFlag = 1;
    }
    if ((js.lRx > (joyCenter + 0x4000)) && rxFlag)
    {
        rxFlag = 0;
        return WS_JOY_AXIS_RX_P;
    }
    else if ((js.lRx < (joyCenter - 0x4000)) && rxFlag)
    {
        rxFlag = 0;
        return WS_JOY_AXIS_RX_M;
    }
    if ((js.lRy > joyCenter - 0x1000) && (js.lRy < joyCenter + 0x1000))
    {
        ryFlag = 1;
    }
    if ((js.lRy > (joyCenter + 0x4000)) && ryFlag)
    {
        ryFlag = 0;
        return WS_JOY_AXIS_RY_P;
    }
    else if ((js.lRy < (joyCenter - 0x4000)) && ryFlag)
    {
        ryFlag = 0;
        return WS_JOY_AXIS_RY_M;
    }
    if ((js.lRz > joyCenter - 0x1000) && (js.lRz < joyCenter + 0x1000))
    {
        rzFlag = 1;
    }
    if ((js.lRz > (joyCenter + 0x4000)) && rzFlag)
    {
        rzFlag = 0;
        return WS_JOY_AXIS_RZ_P;
    }
    else if ((js.lRz < (joyCenter - 0x4000)) && rzFlag)
    {
        rzFlag = 0;
        return WS_JOY_AXIS_RZ_M;
    }
    if ((js.rglSlider[0] > joyCenter - 0x1000) && (js.rglSlider[0] < joyCenter + 0x1000))
    {
        s1Flag = 1;
    }
    if ((js.rglSlider[0] > (joyCenter + 0x4000)) && s1Flag)
    {
        s1Flag = 0;
        return WS_JOY_SLIDER1_P;
    }
    else if ((js.rglSlider[0] < (joyCenter - 0x4000)) && s1Flag)
    {
        s1Flag = 0;
        return WS_JOY_SLIDER1_M;
    }
    if ((js.rglSlider[1] > joyCenter - 0x1000) && (js.rglSlider[0] < joyCenter + 0x1000))
    {
        s2Flag = 1;
    }
    if ((js.rglSlider[1] > (joyCenter + 0x4000)) && s2Flag)
    {
        s2Flag = 0;
        return WS_JOY_SLIDER2_P;
    }
    else if ((js.rglSlider[1] < (joyCenter - 0x4000)) && s2Flag)
    {
        s2Flag = 0;
        return WS_JOY_SLIDER2_M;
    }
    return -1;
}
