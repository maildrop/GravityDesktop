#pragma once

#include "pch.h"
#include "Messages.h"
#include "../capture/all_inc.h"

class Mouse {
public:
	using CURSOR = DirectX::SimpleMath::Vector2; // �J�[�\�����W
	struct CLICK {
		bool l; // ���N���b�N
		bool r; // �E�N���b�N
		bool w; // �z�C�[���N���b�N
		CLICK() {
			l = 0;
			r = 0;
			w = 0;
		}
	};
	using WHEEL = DirectX::SimpleMath::Vector2; // �z�C�[����]��

private:
	static CURSOR cursor; // ���݂̃}�E�X�J�[�\�����W
	static CURSOR cursor_buf; // 1�O��cursor�̏��
	static CURSOR cursor_move; // 1�t���[���Ԃ̃J�[�\���̈ړ���
	static CURSOR drag_from; // ���A�E�A�z�C�[���̂����ꂩ���N���b�N���ꂽ�u�Ԃ̃J�[�\�����W(�h���b�O�p)
	static CLICK click_a; // ������Ă���Ԃ�����1�̃N���b�N���
	static CLICK click_b; // �����ꂽ�t���[���̂�1�̃N���b�N���
	static CLICK click_c; // �����ꂽ�t���[���̂�1�̃N���b�N���
	static CLICK double_click; // �_�u���N���b�N�t���O
	static WHEEL wheel; // 1�t���[���Ԃɔ��������z�C�[����]��
	static bool init_flag;

	static void init();

public:
	static bool loop();

	static inline CURSOR getCursor() { return cursor; }
	static inline CURSOR getCursorMove() { return cursor_move; }
	static inline CURSOR getDragFrom() { return drag_from; }
	static inline CLICK getClickA() { return click_a; }
	static inline CLICK getClickB() { return click_b; }
	static inline CLICK getClickC() { return click_c; }
	static inline CLICK getDoubleClick() { return double_click; }
	static inline WHEEL getWheel() { return wheel; }
};