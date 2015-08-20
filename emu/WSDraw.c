/*
$Date: 2010-09-29 18:53:48 -0400 (Wed, 29 Sep 2010) $
$Rev: 103 $
*/

#include "WSDraw.h"
#include "WS.h"
#include "WSInput.h"
#include "WSRender.h"

extern HWND hWnd;
int Kerorikan = 0;
static enum DRAWSIZE DrawSize = DS_1;           // 描画サイズ フルスクリーン、x1 x2 x3
static int DrawMode = 0;                        // 縦横フラグ 0:横
static LPDIRECT3D9                 pD3D;        // IDirect3D9インターフェイスへのポインタ
static LPDIRECT3DDEVICE9           pD3DDevice;  // IDirect3DDevice9インターフェイスへのポインタ
static D3DPRESENT_PARAMETERS       D3DPP;       // デバイスのプレゼンテーションパラメータ
static LPDIRECT3DVERTEXBUFFER9     pMyVB;       // Vertex Buffer
static LPDIRECT3DTEXTURE9          pTexture;    // Texture
static LPDIRECT3DSURFACE9          pSurface;    // Surface
static LPDIRECT3DTEXTURE9          pSegTexture;
static LPDIRECT3DSURFACE9          pSegSurface;

// 頂点１つのデータ型
typedef struct {
    D3DVECTOR   p;      // 位置
    DWORD       color;  // 色
    D3DXVECTOR2 t;      // テクスチャーの画像の位置
} MY_VERTEX;
// MY_VERTEXのフォーマット設定
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE  | D3DFVF_TEX1)
#define SAFE_RELEASE(p) { if(p) { IDirect3D9_Release(p); (p)=NULL; } }

//-------------------------------------------------------------
// レンダラーの初期化
// 引数
//      isFullScreen : フルスクリーンの場合TRUE
// 戻り値
//      成功したらS_OK 
//-------------------------------------------------------------
HRESULT drawInitialize(BOOL isFullScreen)
{
    D3DDISPLAYMODE d3ddm;

    // Direct3D9オブジェクトの作成
    if((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
    {
        return E_FAIL;  // 取得失敗
    }
    // 現在のディスプレイモードを取得
    if(FAILED(IDirect3D9_GetAdapterDisplayMode(pD3D, D3DADAPTER_DEFAULT, &d3ddm)))
    {
        return E_FAIL;
    }
    // デバイスのプレゼンテーションパラメータを初期化
    ZeroMemory(&D3DPP, sizeof(D3DPRESENT_PARAMETERS));
    if(isFullScreen) {                                  // フルスクリーンの場合
        D3DPP.Windowed              = FALSE;            // フルスクリーン表示の指定
        D3DPP.BackBufferWidth       = 800;              // フルスクリーン時の横幅
        D3DPP.BackBufferHeight      = 600;              // フルスクリーン時の縦幅
    }
    else {
        D3DPP.Windowed              = TRUE;             // ウインドウ内表示の指定
        D3DPP.BackBufferWidth       = 2016;             // 224と144の最小公倍数
        D3DPP.BackBufferHeight      = 2016;
    }
    D3DPP.BackBufferCount           = 1;
    D3DPP.BackBufferFormat          = d3ddm.Format;                     // カラーモードの指定
    D3DPP.SwapEffect                = D3DSWAPEFFECT_DISCARD;            // 
    D3DPP.EnableAutoDepthStencil    = TRUE;                             // エラー対策
    D3DPP.AutoDepthStencilFormat    = D3DFMT_D16;                       // エラー対策
    D3DPP.PresentationInterval      = D3DPRESENT_INTERVAL_IMMEDIATE;    // 垂直同期しない
    // ディスプレイアダプタを表すためのデバイスを作成
    // 描画と頂点処理をハードウェアで行なう
    if(FAILED(IDirect3D9_CreateDevice(pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DPP, &pD3DDevice)))
    {
        // 上記の設定が失敗したら
        // 描画をハードウェアで行い、頂点処理はCPUで行なう
        if(FAILED(IDirect3D9_CreateDevice(pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPP, &pD3DDevice)))
        {
            // 初期化失敗
            MessageBox(hWnd, TEXT(" グラフィックチップが未対応です "), TEXT("Direct3D Error"), MB_OK);
            return E_FAIL;
        }
    }
    return S_OK;
}

//-------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------
void drawFinalize(void)
{
    // デバイスオブジェクトの解放
    SAFE_RELEASE(pD3DDevice);
    // DirectXGraphicsの解放
    SAFE_RELEASE(pD3D);
}

//-------------------------------------------------------------
// シーンを生成
// 戻り値
//      成功したらS_OK
//-------------------------------------------------------------
HRESULT drawCreate(void)
{
    if(pD3DDevice == 0)
    {
        return E_FAIL;
    }
    drawDestroy();
    //--------------------------------------
    // オブジェクトの頂点バッファを生成
    //--------------------------------------
    // 8つの頂点からなる頂点バッファを作る(メイン4個　セグメント4個)
    if(FAILED(IDirect3DDevice9_CreateVertexBuffer(pD3DDevice, 8 * sizeof(MY_VERTEX), D3DUSAGE_WRITEONLY, MY_VERTEX_FVF, D3DPOOL_MANAGED, &pMyVB, NULL)))
    {
        return E_FAIL;
    }
    IDirect3DDevice9_CreateTexture(pD3DDevice, 256, 256, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &pTexture, NULL);
    IDirect3DTexture9_GetSurfaceLevel(pTexture, 0, &pSurface);
    IDirect3DDevice9_CreateTexture(pD3DDevice, 32, 1024, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &pSegTexture, NULL);
    IDirect3DTexture9_GetSurfaceLevel(pSegTexture, 0, &pSegSurface);
    //--------------------------------------
    // テクスチャステージの設定
    //--------------------------------------
    IDirect3DDevice9_SetTextureStageState(pD3DDevice, 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    IDirect3DDevice9_SetTextureStageState(pD3DDevice, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    //--------------------------------------
    // レンダリングステートパラメータの設定
    //--------------------------------------
    // 両面描画モードの指定
    IDirect3DDevice9_SetRenderState(pD3DDevice, D3DRS_CULLMODE, D3DCULL_NONE);
    // ディザリングを行なう（高品質描画）
    IDirect3DDevice9_SetRenderState(pD3DDevice, D3DRS_DITHERENABLE, TRUE);
    // ノーライティングモード   
    IDirect3DDevice9_SetRenderState(pD3DDevice, D3DRS_LIGHTING, FALSE);
    return S_OK;
}

//-------------------------------------------------------------
// シーンの破棄
//-------------------------------------------------------------
void drawDestroy(void)
{
    SAFE_RELEASE(pSurface);
    SAFE_RELEASE(pTexture);
    SAFE_RELEASE(pSegSurface);
    SAFE_RELEASE(pSegTexture);
    SAFE_RELEASE(pMyVB);
}

//-------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------
#define MAIN_X (224.0f/256.0f)
#define MAIN_Y (144.0f/256.0f)
#define MAIN_W (2 * 224.0f/234.0f - 1.0f)
#define SEG_X (4 * 8.0f/32.0f)
#define SEG_Y (4 * 144.0f/1024.0f)
#define SEG_W (1.0f - 2 * 8.0f/234.0f)
#define KERO 0.0075f
void drawDraw()
{
    D3DLOCKED_RECT lockRect;
    int x, y, pitch;
    WORD *p;
    MY_VERTEX* v;
    D3DXMATRIX mat;
    D3DXMatrixIdentity(&mat);

    if(pD3DDevice == 0)
    {
        return;
    }
    if(pMyVB == 0)
    {
        return;
    }
    // 画像データをテクスチャに転送
    IDirect3DSurface9_LockRect(pSurface, &lockRect, NULL, D3DLOCK_DISCARD);
    p = FrameBuffer[0] + 8;
    pitch = lockRect.Pitch / 2;
    for (y = 0; y < 144; y++)
    {
        for (x = 0; x < 224; x++)
        {
            *((WORD*)lockRect.pBits + pitch * y + x) = *p++;
        }
        p += 32;
    }
    IDirect3DSurface9_UnlockRect(pSurface);
    // 液晶セグメントデータをテクスチャに転送
    RenderSegment();
    IDirect3DSurface9_LockRect(pSegSurface, &lockRect, NULL, D3DLOCK_DISCARD);
    p = SegmentBuffer;
    pitch = lockRect.Pitch / 2;
    for (y = 0; y < 144 * 4; y++)
    {
        for (x = 0; x < 32; x++)
        {
            *((WORD*)lockRect.pBits + pitch * y + x) = *p++;
        }
    }
    IDirect3DSurface9_UnlockRect(pSegSurface);
    //頂点バッファの中身を埋める
    IDirect3DVertexBuffer9_Lock( pMyVB, 0, 0, (void**)&v, 0 );
    if (Kerorikan)
    {
        // 頂点座標の設定
        D3DVECTOR p[8] = {
            -112*KERO,  72*KERO, 0.0f,
             112*KERO,  72*KERO, 0.0f,
            -112*KERO, -72*KERO, 0.0f,
             112*KERO, -72*KERO, 0.0f,
             114*KERO,  72*KERO, 0.0f,
             122*KERO,  72*KERO, 0.0f,
             114*KERO, -72*KERO, 0.0f,
             122*KERO, -72*KERO, 0.0f
        };
        v[0].p = p[0];
        v[1].p = p[1];
        v[2].p = p[2];
        v[3].p = p[3];
        v[4].p = p[4];
        v[5].p = p[5];
        v[6].p = p[6];
        v[7].p = p[7];
    }
    else
    {
        // 頂点座標の設定
        D3DVECTOR p[8] = {
            -1.0f,  1.0f, 0.0f,
            MAIN_W, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            MAIN_W,-1.0f, 0.0f,
            SEG_W,  1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
            SEG_W, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f
        };
        v[0].p = p[0];
        v[1].p = p[1];
        v[2].p = p[2];
        v[3].p = p[3];
        v[4].p = p[4];
        v[5].p = p[5];
        v[6].p = p[6];
        v[7].p = p[7];
    }
    {
        // テクスチャ座標の設定
        D3DXVECTOR2 t[8] = {
            0.0f, 0.0f,
            MAIN_X, 0.0f,
            0.0f, MAIN_Y,
            MAIN_X, MAIN_Y,
            0.0f, 0.0f,
            SEG_X, 0.0f,
            0.0f, SEG_Y,
            SEG_X, SEG_Y
        };
        v[0].t = t[0];
        v[1].t = t[1];
        v[2].t = t[2];
        v[3].t = t[3];
        v[4].t = t[4];
        v[5].t = t[5];
        v[6].t = t[6];
        v[7].t = t[7];
    }
    {
    // 頂点カラーの設定
    v[0].color = v[1].color = v[2].color = v[3].color = 0xFFFFFFFFU;
    v[4].color = v[5].color = v[6].color = v[7].color = 0xFFFFFFFFU;
    }
    IDirect3DVertexBuffer9_Unlock(pMyVB);
    // 回転処理
    // 斜め（左atan(0.5)回転）
    if (Kerorikan)
    {
        D3DXMatrixRotationZ(&mat, 0.4636476f); // atanf(0.5f)だと最適化されないかもしれないので
    }
    // 縦（左90度回転）
    else if (DrawMode & 0x01)
    {
        D3DXMatrixRotationZ(&mat, D3DXToRadian(90));
    }
    // 描画開始宣言
    if(SUCCEEDED(IDirect3DDevice9_BeginScene(pD3DDevice)))
    {
        IDirect3DDevice9_Clear(pD3DDevice, 0, NULL, D3DCLEAR_TARGET, 0xFF666666, 0.0f, 0);
        IDirect3DDevice9_SetStreamSource(pD3DDevice, 0, pMyVB, 0, sizeof(MY_VERTEX));
        IDirect3DDevice9_SetFVF(pD3DDevice, MY_VERTEX_FVF);
        IDirect3DDevice9_SetTexture(pD3DDevice, 0, (IDirect3DBaseTexture9 *)pTexture);
        IDirect3DDevice9_DrawPrimitive(pD3DDevice, D3DPT_TRIANGLESTRIP, 0, 2); // 0番の頂点から三角形を2個
        IDirect3DDevice9_SetTexture(pD3DDevice, 0, (IDirect3DBaseTexture9 *)pSegTexture);
        IDirect3DDevice9_DrawPrimitive(pD3DDevice, D3DPT_TRIANGLESTRIP, 4, 2); // 4番の頂点から三角形を2個
        IDirect3DDevice9_SetTransform(pD3DDevice, D3DTS_VIEW, &mat);
        // 描画終了宣言
        IDirect3DDevice9_EndScene(pD3DDevice);
    }
    // 描画結果の転送
    if(FAILED(IDirect3DDevice9_Present(pD3DDevice, NULL, NULL, NULL, NULL)))
    {
        // デバイス消失から復帰
        IDirect3DDevice9_Reset(pD3DDevice, &D3DPP);
    }
}

//-------------------------------------------------------------
void WsResize(void)
{
    RECT wind;
    RECT wind2;
    int  lcdHeight = 144;
    int  lcdWidth  = 224 + 10; // +10はセグメント分
    int  client_width;
    int  client_height;

    if (Kerorikan)
    {
        client_width  = 256 * DrawSize;
        client_height = 256 * DrawSize;
    }
    else if (DrawMode & 0x01)
    {
        client_width  = lcdHeight * DrawSize;
        client_height = lcdWidth  * DrawSize;
    }
    else
    {
        client_width  = lcdWidth  * DrawSize;
        client_height = lcdHeight * DrawSize;
    }
    wind.top    = 0;
    wind.left   = 0;
    wind.right  = client_width;
    wind.bottom = client_height;
    AdjustWindowRectEx(&wind, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        TRUE, WS_EX_APPWINDOW | WS_EX_ACCEPTFILES);
    wind2 = wind;
    SendMessage(hWnd, WM_NCCALCSIZE, FALSE, (LPARAM) &wind2);
    SetWindowPos(hWnd, NULL, 0, 0,
        (wind.right  - wind.left) + client_width  - (wind2.right  - wind2.left),
        (wind.bottom - wind.top ) + client_height - (wind2.bottom - wind2.top ),
        SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
}

//-------------------------------------------------------------
int SetDrawMode(int Mode)
{
    if(DrawMode != Mode)
    {
        DrawMode = Mode;
        WsInputSetKeyMap(Mode);
        WsResize();
    }
    return 0;
}

//-------------------------------------------------------------
int SetDrawSize(enum DRAWSIZE Size)
{
    DrawSize = Size;
    WsResize();
    return 0;
}

