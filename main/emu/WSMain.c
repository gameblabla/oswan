/*
$Date: 2010-09-29 04:51:41 -0400 (Wed, 29 Sep 2010) $
$Rev: 102 $
*/

#include "../resource.h"
#include "WS.h"
#include "WSConfig.h"
#include "WSInput.h"
#include "WSDraw.h"
#include "WSRender.h"
#include "WSApu.h"
#include "WSFileio.h"
#include "WSDialog.h"

BOOL InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AboutProc(HWND, UINT, WPARAM, LPARAM);
wchar_t* OpenWSFile(wchar_t*, DWORD);
void WsPause(void);
void SetRecentRoms(wchar_t* RomPath);
void SetStateInfo(void);

HINSTANCE hInst;
HWND hWnd;
static LPCTSTR szClassName = TEXT("OswanJ"); //クラス名
static wchar_t  RecentOfn0[512];
static wchar_t  RecentOfn1[512];
static wchar_t  RecentOfn2[512];
static wchar_t  RecentOfn3[512];
static wchar_t  RecentOfn4[512];
static wchar_t  RecentOfn5[512];
wchar_t* RecentOfn[] = {RecentOfn0, RecentOfn1, RecentOfn2, RecentOfn3, RecentOfn4, RecentOfn5};
extern wchar_t StateName[512];

int WINAPI wWinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPWSTR lpsCmdLine, int nCmdShow)
{
    MSG msg;
    HACCEL accel;

    if (!hPrevInst)
    {
        if (!InitApp(hCurInst)) return FALSE;
    }
    if (!InitInstance(hCurInst))
    {
        return FALSE;
    }
    hInst = hCurInst;
    accel = LoadAccelerators(hCurInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));
    WsSetDir();
    WsLoadEeprom();
    ConfigCreate();
    SetRecentRoms(NULL);
    apuInit();
    apuLoadSound();
    drawInitialize(FALSE);
    drawCreate();
    SetDrawSize(DS_2);
    WsInputInit(hWnd);
    WsInputSetKeyMap(0);
    WsSplash();
    if (__argc > 1)
    {
        if (WsCreate(__wargv[1]) != -1)
        {
            Sleep(500);
            Run = 1;
            SetRecentRoms(__wargv[1]);
            SetStateInfo();
        }
    }
    while (1) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0))
            {
                break;
            }
            if (!TranslateAccelerator(hWnd, accel, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        if (Run)
        {
            WsRun();
        }
        else
        {
            Sleep(2);
        }
    }
    WsSaveEeprom();
    WsRelease();
    drawDestroy();
    drawFinalize();
    apuEnd();
    ConfigRelease();
    return 0;
}
//ウィンドウ・クラスの登録
BOOL InitApp(HINSTANCE hInst)
{
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    wc.lpszClassName = szClassName;
    return (RegisterClass(&wc));
}
//ウィンドウの生成
BOOL InitInstance(HINSTANCE hInst)
{
    hWnd = CreateWindow(szClassName, TEXT("OswanJ"),//タイトルバーにこの名前が表示されます
        WS_OVERLAPPEDWINDOW, //ウィンドウの種類
        CW_USEDEFAULT, //Ｘ座標
        CW_USEDEFAULT, //Ｙ座標
        CW_USEDEFAULT, //幅
        CW_USEDEFAULT, //高さ
        NULL, //親ウィンドウのハンドル、親を作るときはNULL
        NULL, //メニューハンドル、クラスメニューを使うときはNUL
        hInst, //インスタンスハンドル
        NULL);
    if (!hWnd) return FALSE;
    return TRUE;
}
//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    wchar_t RomPath[512] = {0};
    HMENU menu;
    WORD id;

    switch (msg)
    {
    case WM_CREATE:
        break;
    case WM_COMMAND:
        switch (LOWORD(wp))
        {
        case ID_FILE_OPENROM:
            if (OpenWSFile(RomPath, 512))
            {
                apuWaveClear();
                WsRelease();
                Run = 1;
                WsCreate(RomPath);
                SetRecentRoms(RomPath);
                SetStateInfo();
            }
            return 0L;
        case ID_FILE_RECENT_0:
        case ID_FILE_RECENT_1:
        case ID_FILE_RECENT_2:
        case ID_FILE_RECENT_3:
        case ID_FILE_RECENT_4:
        case ID_FILE_RECENT_5:
            id = LOWORD(wp) - ID_FILE_RECENT_0;
            if (*RecentOfn[id])
            {
                apuWaveClear();
                WsRelease();
                Run = 1;
                if (WsCreate(RecentOfn[id]) < 0)
                {
                    wcscpy(RecentOfn[id], L"");
                    Run = 0;
                    WsSplash();
                }
                SetRecentRoms(RecentOfn[id]);
                SetStateInfo();
            }
            return 0L;
        case ID_PDATA_SET:
            apuWaveClear();
            WsRelease();
            Run = 1;
            WsCreate(NULL);
            return 0L;
        case ID_STATE_SAVE_0:
        case ID_STATE_SAVE_1:
        case ID_STATE_SAVE_2:
        case ID_STATE_SAVE_3:
            id = LOWORD(wp) - ID_STATE_SAVE_0;
            WsSaveState(id);
            SetStateInfo();
            return 0L;
        case ID_STATE_LOAD_0:
        case ID_STATE_LOAD_1:
        case ID_STATE_LOAD_2:
        case ID_STATE_LOAD_3:
            id = LOWORD(wp) - ID_STATE_LOAD_0;
            WsLoadState(id);
            return 0L;
        case ID_PAUSE:
            WsPause();
            return 0L;
        case ID_RESET:
            WsReset();
            return 0L;
        case ID_SIZE_1:
            menu = GetMenu(hWnd);
            CheckMenuItem(menu, ID_SIZE_1, MF_CHECKED);
            CheckMenuItem(menu, ID_SIZE_2, MF_UNCHECKED);
            CheckMenuItem(menu, ID_SIZE_3, MF_UNCHECKED);
            CheckMenuItem(menu, ID_SIZE_4, MF_UNCHECKED);
            SetDrawSize(DS_1);
            return 0L;
        case ID_SIZE_2:
            menu = GetMenu(hWnd);
            CheckMenuItem(menu, ID_SIZE_1, MF_UNCHECKED);
            CheckMenuItem(menu, ID_SIZE_2, MF_CHECKED);
            CheckMenuItem(menu, ID_SIZE_3, MF_UNCHECKED);
            CheckMenuItem(menu, ID_SIZE_4, MF_UNCHECKED);
            SetDrawSize(DS_2);
            return 0L;
        case ID_SIZE_3:
            menu = GetMenu(hWnd);
            CheckMenuItem(menu, ID_SIZE_1, MF_UNCHECKED);
            CheckMenuItem(menu, ID_SIZE_2, MF_UNCHECKED);
            CheckMenuItem(menu, ID_SIZE_3, MF_CHECKED);
            CheckMenuItem(menu, ID_SIZE_4, MF_UNCHECKED);
            SetDrawSize(DS_3);
            return 0L;
        case ID_SIZE_4:
            menu = GetMenu(hWnd);
            CheckMenuItem(menu, ID_SIZE_1, MF_UNCHECKED);
            CheckMenuItem(menu, ID_SIZE_2, MF_UNCHECKED);
            CheckMenuItem(menu, ID_SIZE_3, MF_UNCHECKED);
            CheckMenuItem(menu, ID_SIZE_4, MF_CHECKED);
            SetDrawSize(DS_4);
            return 0L;
        case ID_LAYER_1:
        case ID_LAYER_2:
        case ID_LAYER_S:
            id = LOWORD(wp) - ID_LAYER_1;
            menu = GetMenu(hWnd);
            if (Layer[id])
            {
                Layer[id] = 0;
                CheckMenuItem(menu, LOWORD(wp), MF_UNCHECKED);
            }
            else
            {
                Layer[id] = 1;
                CheckMenuItem(menu, LOWORD(wp), MF_CHECKED);
            }
            return 0L;
        case ID_KERORIKAN:
            menu = GetMenu(hWnd);
            if (Kerorikan)
            {
                Kerorikan = 0;
                CheckMenuItem(menu, LOWORD(wp), MF_UNCHECKED);
                WsResize();
                drawDraw();
            }
            else
            {
                Kerorikan = 1;
                CheckMenuItem(menu, LOWORD(wp), MF_CHECKED);
                WsResize();
                drawDraw();
            }
            return 0L;
        case ID_SOUND_1:
        case ID_SOUND_2:
        case ID_SOUND_3:
        case ID_SOUND_4:
        case ID_SOUND_5:
        case ID_SOUND_6:
        case ID_SOUND_7:
            id = LOWORD(wp) - ID_SOUND_1;
            menu = GetMenu(hWnd);
            if (Sound[id])
            {
                Sound[id] = 0;
                CheckMenuItem(menu, LOWORD(wp), MF_UNCHECKED);
            }
            else
            {
                Sound[id] = 1;
                CheckMenuItem(menu, LOWORD(wp), MF_CHECKED);
            }
            return 0L;
        case ID_SOUND_UP:
            if (WsWaveVol < 100)
            {
                WsWaveVol++;
            }
            return 0L;
        case ID_SOUND_DOWN:
            if (WsWaveVol >= 1)
            {
                WsWaveVol--;
            }
            return 0L;
        case ID_CONF_INPUT:
            WsInputRelease();
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_CONFIG), hWnd, (DLGPROC)ConfProc);
            WsInputInit(hWnd);
            return 0L;
        case ID_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_ABOUT), hWnd, (DLGPROC)AboutProc);
            break;
        }
        break;
    case WM_PAINT:
        drawDraw();
        break;
    case WM_DESTROY:  
        PostQuitMessage(0);
        return 0L; 
    default: 
        break;
    } 
    return (DefWindowProc(hWnd, msg, wp, lp)); 
} 

//Aboutダイアログプロシージャ
LRESULT CALLBACK AboutProc(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    HDC         hDC;
    HFONT       hFont;

    switch (msg)
    {
    case WM_INITDIALOG:
        SetWindowText(GetDlgItem(hDlgWnd, IDC_TEXT_VERSION), TEXT("バージョン 0.1"));
        break;
    case WM_CTLCOLORSTATIC:
        if((HWND)lp == GetDlgItem(hDlgWnd, IDC_TEXT_VERSION))
        {
            hDC = (HDC)wp;
            hFont = CreateFont(
                18,                         /* フォント高さ */
                0,                          /* 文字幅 */
                0,                          /* テキストの角度 */
                0,                          /* ベースラインとｘ軸との角度 */
                FW_BOLD,                    /* フォントの重さ（太さ） */
                FALSE,                      /* イタリック体 */
                FALSE,                      /* アンダーライン */
                FALSE,                      /* 打ち消し線 */
                SHIFTJIS_CHARSET,           /* 文字セット */
                OUT_DEFAULT_PRECIS,         /* 出力精度 */
                CLIP_DEFAULT_PRECIS,        /* クリッピング精度 */
                PROOF_QUALITY,              /* 出力品質 */
                FIXED_PITCH | FF_MODERN,    /* ピッチとファミリー */
                (LPCTSTR)"ＭＳ Ｐゴシック"  /* 書体名 */
            );
            SelectObject(hDC, hFont);
            SetTextColor(hDC, RGB(0, 0, 255));
            SetBkMode(hDC, TRANSPARENT);
            return (BOOL)(HBRUSH)GetStockObject(NULL_BRUSH);
        }
        return DefWindowProc(hDlgWnd, msg, wp, lp);
    case WM_COMMAND:
        if(HIWORD(wp) == BN_CLICKED)
        {
            switch (LOWORD(wp))
            {
            case IDOK:
            case IDCANCEL:
                EndDialog(hDlgWnd, LOWORD(wp));
                return 1;
            }
        }
        break;
    }
    return 0;
}

wchar_t* OpenWSFile(wchar_t* path, DWORD max)
{
    OPENFILENAMEW ofn;

    ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize     = sizeof(OPENFILENAMEW);
    ofn.hwndOwner       = hWnd;
    ofn.lpstrFile       = path;
    ofn.nMaxFile        = max;
    ofn.lpstrFilter     = L"Wonderswan(*.ws,*.wsc)\0*.ws;*.wsc\0Wonderswan mono(*.ws)\0*.ws\0Wonderswan color(*.wsc)\0*.wsc\0\0";
    ofn.nFilterIndex    = 1;
    ofn.lpstrFileTitle  = NULL;
    ofn.nMaxFileTitle   = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.hInstance       = NULL;
    if (GetOpenFileNameW(&ofn) == FALSE)
    {
        return NULL;
    }
    return path;
}

void WsPause(void)
{
    static int pause = 0;
    HMENU   menu = GetMenu(hWnd);

    if (Run)
    {
        pause = 1;
        Run = 0;
        CheckMenuItem(menu, ID_PAUSE, MF_CHECKED);
        apuWaveClear();
    }
    else if (pause)
    {
        Run = 1;
        pause = 0;
        CheckMenuItem(menu, ID_PAUSE, MF_UNCHECKED);
    }
}

void SetRecentRoms(wchar_t* RomPath)
{
    int         i;
    wchar_t*    temp;
    wchar_t*    filename;
    wchar_t     buf[256];
    MENUITEMINFOW minfo;
    HMENU       menu = GetMenu(hWnd);

    minfo.cbSize     = sizeof(MENUITEMINFOW);
    minfo.fMask      = MIIM_STATE | MIIM_TYPE;
    minfo.fType      = MFT_STRING;
    minfo.fState     = MFS_ENABLED;
    minfo.dwTypeData = buf;
    if (RomPath && *RomPath)
    {
        for (i = 0; i < 6; i++)
        {
            if (wcscmp(RomPath, RecentOfn[i]) == 0)
            {
                temp = RecentOfn[i];
                while (i) {
                    RecentOfn[i] = RecentOfn[i - 1];
                    i--;
                }
                RecentOfn[0] = temp;
                break;
            }
        }
        if (i)
        {
            temp         = RecentOfn[5];
            RecentOfn[5] = RecentOfn[4];
            RecentOfn[4] = RecentOfn[3];
            RecentOfn[3] = RecentOfn[2];
            RecentOfn[2] = RecentOfn[1];
            RecentOfn[1] = RecentOfn[0];
            RecentOfn[0] = temp;
            wcscpy(RecentOfn[0], RomPath);
        }
    }
    for (i = 0; i < 6; i++)
    {
        if (*RecentOfn[i])
        {
            filename = wcsrchr(RecentOfn[i], '\\');
            wsprintf(buf, L"&%d %s", i + 1, ++filename);
        }
        else
        {
            wsprintf(buf, L"%d", i + 1);
        }
        SetMenuItemInfo(menu, ID_FILE_RECENT_0 + i, FALSE, &minfo);
    }
}

void SetStateInfo(void)
{
    int         i;
    wchar_t     buf[512];
    MENUITEMINFOW minfo;
    HMENU       menu = GetMenu(hWnd);
    HANDLE      file;
    FILETIME    ft, lt;
    SYSTEMTIME  st;

    minfo.cbSize     = sizeof(MENUITEMINFOW);
    minfo.fMask      = MIIM_STATE | MIIM_TYPE;
    minfo.fType      = MFT_STRING;
    minfo.fState     = MFS_ENABLED;
    minfo.dwTypeData = buf;
    if (StateName[0])
    {
        for (i = 0; i < 4; i++)
        {
            wsprintf(buf, L"%s.%03d", StateName, i);
            file = CreateFileW(buf, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (file != INVALID_HANDLE_VALUE)
            {
                GetFileTime(file, NULL, NULL, &ft);
                FileTimeToLocalFileTime(&ft, &lt);
                FileTimeToSystemTime(&lt, &st);
                wsprintf(buf, L"&%d %04d/%02d/%02d %02d:%02d:%02d\tShift+F%d",
                    i + 1, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, i + 1);
                SetMenuItemInfo(menu, ID_STATE_SAVE_0 + i, FALSE, &minfo);
                wsprintf(buf, L"&%d %04d/%02d/%02d %02d:%02d:%02d\tF%d",
                    i + 1, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, i + 1);
                SetMenuItemInfo(menu, ID_STATE_LOAD_0 + i, FALSE, &minfo);
                CloseHandle(file);
            }
            else
            {
                wsprintf(buf, L"%d", i + 1);
                SetMenuItemInfo(menu, ID_STATE_SAVE_0 + i, FALSE, &minfo);
                SetMenuItemInfo(menu, ID_STATE_LOAD_0 + i, FALSE, &minfo);
            }
        }
    }
}
