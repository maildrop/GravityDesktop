#pragma once
#if !defined( LOCAL_HOOK_HEADER_GUARD )
#define LOCAL_HOOK_HEADER_GUARD 1

#include <Windows.h>

/*
 このマクロは、__declspec( dllxport ) と __declspec( dllimport ) を切り替えるためにある
 このために、LocalHook のプロジェクト プロパティ
 構成プロパティ
    C/C++
        プリプロセッサ の プリプロセッサの定義 において
        EXPORT_
        を定義する必要がる。この定義によって、LocalHook.dll を作成するときは __declspec(dllexport) が使われる

 このヘッダを使う場面では、プリプロセッサ定義 EXPORT_ は必要無い
 この場合 __declspec( dllimport ) が選択される。
*/

#ifdef EXPORT_
#define EXPORT_API_ __declspec(dllexport)
#else
#define EXPORT_API_ __declspec(dllimport)
#endif 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


  EXPORT_API_ bool SetHook(HWND hwnd, HWND target);
  EXPORT_API_ bool ResetHook();

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* !defined( LOCAL_HOOK_HEADER_GUARD ) */
