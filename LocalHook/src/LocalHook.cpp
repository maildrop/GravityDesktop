#include <cstdio>
#include <tchar.h>
#include <string>
#include <sstream>
#include "LocalHook.h"

#pragma data_seg(".shareddata")
HHOOK hKeyHook = 0;
HHOOK hMouseHook = 0;
HWND hwnd = NULL;
HWND target = NULL;
DWORD tid = NULL;
RECT targetRect;
#pragma data_seg()

HINSTANCE hInst;

EXPORT_API_ bool CALLBACK SetHook(HWND set_hwnd, HWND set_target)
{
	hwnd = set_hwnd;
	target = set_target;
	GetWindowRect(target, &targetRect);
	// �^�[�Q�b�g�̃E�B���h�E�n���h������X���b�hID�擾
	tid = GetWindowThreadProcessId(target, NULL);
	// �^�[�Q�b�g�̃E�B���h�E�n���h���ɃL�[�t�b�N�ǉ�
	hKeyHook = SetWindowsHookEx(WH_KEYBOARD, KeyHookProc, hInst, tid);
	// �^�[�Q�b�g�̃E�B���h�E�n���h���Ƀ}�E�X�t�b�N�ǉ�
	hMouseHook = SetWindowsHookEx(WH_MOUSE, MouseHookProc, hInst, tid);
	// �G���[�`�F�b�N
	if ((hKeyHook != NULL) && (hMouseHook != NULL)) return 1;
	return 0;
}

EXPORT_API_ bool CALLBACK ResetHook()
{
	if (UnhookWindowsHookEx(hKeyHook) == 0) return 1;
	return 0;
}

EXPORT_API_ LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wp, LPARAM lp)
{
	if (nCode < 0)    //���܂莖
		return CallNextHookEx(hKeyHook, nCode, wp, lp);
	if ((nCode == HC_ACTION) && (hwnd != NULL)) {
		//�ړI�̃E�C���h�E�ɃL�[�{�[�h���b�Z�[�W�ƁA�L�[�R�[�h�̓]��

		// �ǂ��ŉ����Ă�HOOK����
		//�{�^���������ꂽ��Ԃ̎�����
		PostMessage(
			hwnd,
			(lp & 0b10000000000000000000000000000000) ? WM_KEYUP : WM_KEYDOWN,
			wp, 
			lp
		);
		return 0;
	}

	return CallNextHookEx(hKeyHook, nCode, wp, lp);
}

EXPORT_API_ LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wp, LPARAM lp)
{
	MOUSEHOOKSTRUCT *msg;
	if (nCode < 0)    //���܂莖
		return CallNextHookEx(hKeyHook, nCode, wp, lp);
	if ((nCode == HC_ACTION) && (hwnd != NULL)) {
		//�ړI�̃E�C���h�E�ɃL�[�{�[�h���b�Z�[�W�ƁA�L�[�R�[�h�̓]��
		msg = (MOUSEHOOKSTRUCT*)lp;
		// �ǂ��ŉ����Ă�HOOK����
		//�{�^���������ꂽ��Ԃ̎�����
		ScreenToClient(target, &msg->pt);
		PostMessage(
			hwnd,
			(UINT)wp,
			(WPARAM)msg->wHitTestCode,
			MAKELPARAM(msg->pt.x, msg->pt.y)
		);
		return 0;
	}

	return CallNextHookEx(hKeyHook, nCode, wp, lp);
}

// �G���g���|�C���g
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// �A�^�b�`
		hInst = hModule;
		//bSetHook = FALSE;
		break;
	case DLL_PROCESS_DETACH:
		// �f�^�b�`
		break;
	}
	return TRUE;
}