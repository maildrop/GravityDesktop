//
// Main.cpp
//
#include "pch.h"

#include "lib\dxtk\Game.h"

#include <string>
#include <sstream>

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
  __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace
{
  Game* g_game; // グローバル変数であるが、そのライフライムは entryPoint() の unique_ptr で管理される
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

static int entryPoint(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



static int entryPoint(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  /* game は、g_game の生存期間を決めるために、このスコープにおく
     これにより、entryPoint のリターンより前において Game オブジェクトは解放される
  */
  std::unique_ptr<Game> game = std::make_unique<Game>();
  g_game = game.get(); // グローバル変数として外に置く

  // Register class and create window
  {
    // Register class
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, L"IDI_ICON");
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"gravityWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, L"IDI_ICON");
    if (!RegisterClassEx(&wcex))
      return 1;

    // Create window
    int w{ 0 }, h{ 0 };
    g_game->GetDefaultSize(w, h);

    gdHwndManager::reload();


    RECT rc = { 0 };
    GetWindowRect(gdHwndManager::getDesk(), &rc);
    AdjustWindowRect(&rc, WS_POPUP, FALSE);

    // TODO: Change to CreateWindowEx(WS_EX_TOPMOST, L"gravity3WindowClass", L"gravity3", WS_POPUP,
    // to default to fullscreen.

    HWND hwnd = CreateWindowEx(0, L"gravityWindowClass", L"Gravity Desktop", WS_POPUP | WS_CHILD,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
      nullptr);

    if (!hwnd) {
      return 1;
    }

    gdHwndManager::mine = hwnd;

    ShowWindow(hwnd, nCmdShow);

    // TODO: Change nCmdShow to SW_SHOWMAXIMIZED to default to fullscreen.

    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_game));

    if (! GetClientRect(hwnd, &rc) ){
      rc = { 0,0,w,h }; // GetClientRect が失敗した時に備えての予備の初期値
    }
    g_game->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);
  }

  // Main message loop
  {
    MSG msg = { 0, WM_NULL , 0 }; // message が未定義の場合 偶然 WM_QUIT と同じ場合を想定しなければならない。
    while (WM_QUIT != msg.message)
    {
      if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
      {
        static_cast<void>(TranslateMessage(&msg)); // 今回は、WM_KEYDOWNを扱いたいので TranslateMessageの戻り値を使用しない。
        DispatchMessage(&msg);
      }
      else
      {
        g_game->Tick();
      }
    }
    return (int)msg.wParam;
  }
}

// Entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
  /*
    この部分では、CoInitializeEx のみに注目して行う

    なぜならば、処理の途中で、return を混ぜられると、（そしてそれは不注意でよく行われる）
    CoUnitialize() が呼ばれるまえに処理が終了するからである。
    同様の目的を達成するために RAII が用いられるかもしれないが、RAII もまた、
    スコープの不注意で、早すぎる解放をされる場合がある。このポカ避けのために
    特別に一つの関数中で行う。
  */

  if (!DirectX::XMVerifyCPUSupport())
    return 1;

  // COM の宣言は、最優先で行う。
  const HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
#if 1
  if (RPC_E_CHANGED_MODE == hr) {   // 問題外の状態である
    return 1;
  }
  else if( SUCCEEDED( hr ) ){       // 一応成功した
    assert(S_OK == hr || S_FALSE == hr); // hr は S_OK もしくはS_FALSE のいずれかである(はず)

    // 一応 SUCCEEDED な状態の場合もあるが、
    // これは、結果は失敗したという 呼び出しに成功した S_FALSE の状態があるので
    // 正しくハンドリングする。

    assert(S_OK == hr); // S_OK 以外が戻るのはほぼプログラミング上の問題なので assert で落とすべき

    int result = entryPoint(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    if (S_OK == hr) {
      CoUninitialize();
    }
    else if (S_FALSE == hr) {
      // S_FALSE が帰ってきた場合は、CoUnitialize() してはならない。
      // なぜならば、既に先に CoInitializeEx した部分があるということは、
      // それがCoUninitialize() するからである。
      ; // do nothing 
    }
    return result;
  }
  else {
    // それ以外の未知の状態である。
    return 1;
  }
#else
  if (FAILED(hr)) {
    return 1;
  }
#endif
}

// Windows procedure
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static bool s_in_sizemove = false;
  static bool s_in_suspend = false;
  static bool s_minimized = false;
  // TODO: Set s_fullscreen to true if defaulting to fullscreen.

  auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
  switch (message)
  {
  case WM_PAINT:
  {
#if 0
    PAINTSTRUCT ps;
    HDC hdc;
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
#else
    ValidateRect(hWnd, NULL);

#endif
  }
  break;

  case WM_SIZE:
    if (wParam == SIZE_MINIMIZED)
    {
      if (!s_minimized)
      {
        s_minimized = true;
        if (!s_in_suspend && game)
          game->OnSuspending();
        s_in_suspend = true;
      }
    }
    else if (s_minimized)
    {
      s_minimized = false;
      if (s_in_suspend && game)
        game->OnResuming();
      s_in_suspend = false;
    }
    else if (!s_in_sizemove && game)
    {
      game->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
    }
    break;

  case WM_ENTERSIZEMOVE:
    s_in_sizemove = true;
    break;

  case WM_EXITSIZEMOVE:
    s_in_sizemove = false;
    if (game)
    {
      RECT rc;
      GetClientRect(hWnd, &rc);

      game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
    }
    break;

  case WM_GETMINMAXINFO:
  {
    auto info = reinterpret_cast<MINMAXINFO*>(lParam);
    info->ptMinTrackSize.x = 320;
    info->ptMinTrackSize.y = 200;
  }
  break;

  case WM_ACTIVATEAPP:
    if (game)
    {
      if (wParam)
      {
        game->OnActivated();
      }
      else
      {
        game->OnDeactivated();
      }
    }
    break;

  case WM_POWERBROADCAST:
    switch (wParam)
    {
    case PBT_APMQUERYSUSPEND:
      if (!s_in_suspend && game)
        game->OnSuspending();
      s_in_suspend = true;
      return TRUE;

    case PBT_APMRESUMESUSPEND:
      if (!s_minimized)
      {
        if (s_in_suspend && game)
          game->OnResuming();
        s_in_suspend = false;
      }
      return TRUE;
    }
    break;

  case WM_DESTROY:
    game->Exit();
    PostQuitMessage(0);
    break;

  case WM_MENUCHAR:
    // A menu is active and the user presses a key that does not correspond
    // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
    return MAKELRESULT(0, MNC_CLOSE);

  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_LBUTTONDBLCLK:
  case WM_RBUTTONDBLCLK:
  case WM_MBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
  case WM_RBUTTONDOWN:
  case WM_MBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_RBUTTONUP:
  case WM_MBUTTONUP:
  case WM_MOUSEMOVE:
  case WM_MOUSEWHEEL:
    // 人間向けの入力処理は、後でまとめて処理するよ。
    Messages::add(hWnd, message, wParam, lParam);
    break;

  default:
    break;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}


// Exit helper
void ExitGame()
{
  PostQuitMessage(0);
}
