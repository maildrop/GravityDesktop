//
// pch.h
// Header for standard system include files.
//

#pragma once

#if !defined( PCH_H_HEADER_GUARD )
#define PCH_H_HEADER_GUARD 1

#include <WinSDKVer.h>
#if !defined( _WIN32_WINNT )
/*
  _WIN32_WINNT は、Windows の 動作バージョンを指定するマクロ定義なのではあるが、
  一つ目はコンパイルスイッチで定義する場合がある。
  二つ目はコンパイルスイッチで定義されなかった場合はここで定義する必要がある
  これを意図するために _WIN32_WINNT が定義されていない場合のみここで定義することにする。
*/
#define _WIN32_WINNT 0x0601
#endif /* !defined( _WIN32_WINNT ) */
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <mmsystem.h>
#include <Commctrl.h>

#include <wrl/client.h>

#include <dxgi1_2.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <iostream>
#include <sstream>
#include <locale> 
#include <codecvt> 
#include <type_traits>
#include <algorithm>
#include <exception>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cassert>

#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include <SimpleMath.h>


#ifdef _DEBUG
#pragma comment(lib, "DirectXTK_d.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#endif

namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DirectX API errors
            throw std::exception();
        }
    }
}

#endif /* !defined( PCH_H_HEADER_GUARD ) */
