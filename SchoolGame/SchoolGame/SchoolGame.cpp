// SchoolGame.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include <d3d9.h>
#include <d3dx9.h>

#include "SchoolGame.h"
#include "CSprite.h"
#include "CTexture.h"
#include "CInput.h"

#pragma comment( lib, "d3d9.lib")
#pragma comment( lib, "d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND&);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//변수선언
CInput* g_Input;

CSprite* m_SchoolSprite; //학교 스프라이트
CSprite* m_HomeSprite; //집 스프라이트
CSprite* m_HeroSprite; //주인공 스프라이트
CSprite* m_TeacherSprite; //선생님 스프라이트

CTexture* m_SchoolTexture; //학교 텍스처
CTexture* m_HomeTexture; //집 텍스쳐
CTexture* m_HeroTexture; //주인공 텍스쳐
CTexture* m_TeacherTexture; //선생님 텍스쳐

D3DXVECTOR3 keyDir;
D3DXVECTOR3 SchoolDir;
D3DXVECTOR3 HomeDir;
D3DXVECTOR3 HeroDir;
D3DXVECTOR3 TeacherDir;

D3DXVECTOR3 SchoolPos = {1495.f,240.f,0.f };
D3DXVECTOR3 HomePos = { 10.f,240.f,0.f };
D3DXVECTOR3 HeroPos = { 100.f, 440.f, 0.f };
D3DXVECTOR3 TeacherPos;

LPDIRECT3D9 g_pD3D;
LPDIRECT3DDEVICE9 g_pD3DDevice;
D3DCOLOR g_ClearColor = D3DCOLOR_XRGB(0, 0, 0);
DWORD g_dwPrevTime = 0L;
LPD3DXFONT g_Font;

//함수선언
bool InitDirect3D(HWND hwnd);
void Render();
void ReleaseDirect3D(); 

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SCHOOLGAME, szWindowClass, MAX_LOADSTRING);
    //윈도우 클래스를 등록
    MyRegisterClass(hInstance);

    HWND hWnd = NULL;

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow, hWnd))
    {
        return FALSE;
    }

    m_SchoolSprite = new CSprite(g_pD3DDevice);
    m_SchoolTexture = new CTexture(g_pD3DDevice, _T("School.bmp"));
   
    m_HomeSprite = new CSprite(g_pD3DDevice);
    m_HomeTexture = new CTexture(g_pD3DDevice, _T("Home.bmp"));
   
    m_HeroSprite = new CSprite(g_pD3DDevice);
    m_HeroTexture = new CTexture(g_pD3DDevice, _T("Hero.bmp"));

    g_Input = new CInput(hWnd, hInstance);
    keyDir = { 0.f,0.f,0.f };

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCHOOLGAME));

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    // 기본 메시지 루프입니다:
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else 
        {
            g_Input->ReadKeyboard();
            //esc키 눌리면 종료
            if (g_Input->IsEscapePressed())
                msg.message = WM_QUIT;

            g_Input->GetInputDir(keyDir); // 1) 기능 함수화	
            HeroPos = HeroPos + keyDir * 5.f;

            Render();
        }
    }

    ReleaseDirect3D();
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCHOOLGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//

//CreateWindow 함수를 호출하여 윈도우를 만든다
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND& hWnd)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       0, 0, 780, 480, nullptr, nullptr, hInstance, nullptr); //크기 640*480

   if (!hWnd)
   {
      return FALSE;
   }

   InitDirect3D(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool InitDirect3D(HWND hwnd) {
    //DX 오브젝트 생성
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
   
    if (g_pD3D == NULL)
        return false;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = TRUE;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = 1500;
    d3dpp.BackBufferHeight = 480;

    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT
        , D3DDEVTYPE_HAL
        , hwnd
        , D3DCREATE_HARDWARE_VERTEXPROCESSING
        , &d3dpp
        , &g_pD3DDevice) == E_FAIL)
        return false;

    return true;
}

void Render() {
    if (g_pD3DDevice == NULL)
        return;

    g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, g_ClearColor, 1.0f, 0);


    if (SUCCEEDED(g_pD3DDevice->BeginScene())) {
        m_HeroSprite->Draw(m_HeroTexture->GetTexture(), { 32.f, 32.f,0.f }, HeroPos);
        //m_TeacherSprite->Draw(m_TeacherTexture->GetTexture(), { 32.f, 32.f,0.f }, TeacherPos);
       
        m_SchoolSprite->Draw(m_SchoolTexture->GetTexture(), { 10 , 240.f, 0.f }, SchoolPos);
        m_HomeSprite->Draw(m_HomeTexture->GetTexture(), { 10.f, 240.f ,0.f }, HomePos);

        RECT rc;
        rc = { 0, 0, 640, 40 };

        g_pD3DDevice->EndScene();
    }

    g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void ReleaseDirect3D()
{
    if (g_pD3DDevice != NULL)
        g_pD3DDevice->Release();

    if (g_pD3D != NULL)
        g_pD3D->Release();

    g_pD3DDevice = NULL;
    g_pD3D = NULL;
}
