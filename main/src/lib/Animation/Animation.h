/*


*/
#pragma once

#if !defined( ANIMATION_H_HEADER_GUARD )
#define ANIMATION_H_HEADER_GUARD 1

#include "..\capture\all_inc.h"
#include "..\Win32\all_inc.h"
#include "..\imageIcon\imageIcons.h"
#include "..\common\general.h"

class Anime {
private:
	HWND hwnd;
	gdImageIcons icons;
	bool exit_step; // TODO: 条件式がよくわからないフラグ 

public:
  /*
    コンストラクタ 
    コンストラクタで構築後に init で初期化する二段階初期化法を使っている
  */
  Anime() : hwnd{ NULL }, icons{}, exit_step{ false } {
  }

	inline bool init(ID3D11DeviceContext1* context, HWND set_hwnd) {
		hwnd = set_hwnd;
		// Box2D初期化
		if (icons.init(hwnd, context)) return 1;
		return 0;
	}

	inline void setWallObj(HWND set_hwnd) {
		icons.setWallObj(set_hwnd);
	}

	inline bool render() {
		// 描画
    do {

      if (exit_step) {
        PostMessage(gdHwndManager::mine, WM_CLOSE, NULL, NULL);
      }
      if (icons.update()) {
        exit_step = true;
      }

      if (! exit_step ) {
        icons.render();
      }

    } while (false);

		// 処理の終了
		return 0;
	}
};

#endif /* !defined( ANIMATION_H_HEADER_GUARD ) */
