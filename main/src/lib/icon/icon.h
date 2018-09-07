#pragma once

#include "../common/box.h"
#include "../common/general.h"

#include <type_traits>

class gdIcon {
private:
	HWND hwnd = NULL;
	HANDLE pid_h = NULL; // 指定されたSysListViewのPIDハンドル
	LPVOID ptr = nullptr; // 指定されたSysListViewに生成したメモリの先頭ポインタ
	RECT rc; // 一時バッファ
	LVITEM lvi; // 一時バッファ
	ULONG_PTR numRead; // 一時バッファ
	int index = -1;
	bool select_var = 0;
	bool focus_var = 0;
	bool hot_var = 0;

public:
	gdBox ico;
	gdBox all;

public:
  typedef decltype(index) index_type;

public:
  gdIcon():
    hwnd(NULL), pid_h(NULL), ptr(nullptr),
    rc{}, lvi{}, numRead{}, index{ -1 }, select_var{ false }, focus_var{ false }, hot_var{ false },
    ico{}, all{}
  {
  }

	void init(HWND set_hwnd, HANDLE set_pid_h, LPVOID set_ptr, int set_index)
  {
    this->hwnd = set_hwnd;
		this->pid_h = set_pid_h;
		this->ptr = set_ptr;
		this->index = set_index; 
	}

	bool update() {
		DWORD t1 = timeGetTime(); //---------------------------------------------
    UNREFERENCED_PARAMETER(t1);

		// アイテムのアイコン範囲座標取得
		rc.left = LVIR_ICON; // アイコン部分とテキスト部分の両方の範囲取得
		if (WriteProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // ptrにRECT代入
     
    // 負数の取り扱いのため 算術拡張してやってから、WPARAM への変換が必要
    //static_cast<WPARAM>(static_cast<typename std::make_signed<WPARAM>::type>(index));

		SendMessage(hwnd, LVM_GETITEMRECT, 
      static_cast<WPARAM>(static_cast<typename std::make_signed<WPARAM>::type>(index)), (LPARAM)ptr); // 取得要求と先頭ポインタ送信
		if (ReadProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // 範囲座標取得
		ico = gdGeneral::RECT2gdBox(rc);
		// アイテムの全範囲座標取得
		rc.left = LVIR_BOUNDS; // アイコン部分とテキスト部分の両方の範囲取得
		if (WriteProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // ptrにitem代入
		SendMessage(hwnd, LVM_GETITEMRECT,
      static_cast<WPARAM>(static_cast<typename std::make_signed<WPARAM>::type>(index)) , (LPARAM)ptr); // 取得要求と先頭ポインタ送信
		if (ReadProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // 範囲座標取得
		all = gdGeneral::RECT2gdBox(rc);

    { // アイテムアイコンの選択情報等を収集
      UINT const ret = static_cast<UINT>(SendMessage(hwnd, LVM_GETITEMSTATE,
        static_cast<WPARAM>(static_cast<typename std::make_signed<WPARAM>::type>(index)), LVIS_SELECTED | LVIS_FOCUSED));
      // 選択情報
      select_var = ((ret&LVIS_SELECTED) == LVIS_SELECTED);
      // フォーカス情報
      focus_var = ((ret&LVIS_FOCUSED) == LVIS_FOCUSED);
      // ホットアイテム情報取得
      hot_var = (index == SendMessage(hwnd, LVM_GETHOTITEM, 0, 0));
    }

		DWORD t2 = timeGetTime(); //---------------------------------------------
    UNREFERENCED_PARAMETER(t2);

		//if (t2 - t1 > 4) Debug::cout(std::to_wstring(t2 - t1).c_str());

		// 処理終了
		return 0;
	}
	bool select(UINT state, UINT stateMast) {
		// アイテムのステータス変更
		lvi.state = state;
		lvi.stateMask = stateMast;
		if (WriteProcessMemory(pid_h, ptr, &lvi, sizeof(LVITEM), &numRead) == 0) return 1; // ptrにitem代入
		SendMessage(hwnd, LVM_SETITEMSTATE, index, (LPARAM)ptr);
		// フォーカスを有効化する為にウィンドウをアクティブにする
		SendMessage(hwnd, WM_ACTIVATE, WA_CLICKACTIVE, 0);
		// 代入
		select_var = 1;
		focus_var = 1;
		// 処理終了
		return 0;
	}
	bool hot() {
		// ホットアイテム設定
		SendMessage(hwnd, LVM_SETHOTITEM, index, 0);
		// 代入
		hot_var = 1;
		// 処理終了
		return 0;
	}
	inline bool getSelect() { return select_var; }
	inline bool getFocus() { return focus_var; }
	inline bool getHot() { return hot_var; }
};