#pragma once

#include "../common/box.h"
#include "../common/general.h"

class gdIcon {
private:
	HWND hwnd = NULL;
	HANDLE pid_h = NULL; // �w�肳�ꂽSysListView��PID�n���h��
	LPVOID ptr = nullptr; // �w�肳�ꂽSysListView�ɐ��������������̐擪�|�C���^
	RECT rc; // �ꎞ�o�b�t�@
	LVITEM lvi; // �ꎞ�o�b�t�@
	ULONG_PTR numRead; // �ꎞ�o�b�t�@
	unsigned int index = -1;
	bool select_var = 0;
	bool focus_var = 0;
	bool hot_var = 0;

public:
	gdBox ico;
	gdBox all;
	void init(HWND set_hwnd, HANDLE set_pid_h, LPVOID set_ptr, unsigned int set_index) {
		hwnd = set_hwnd;
		pid_h = set_pid_h;
		ptr = set_ptr;
		index = set_index;
	}
	bool update() {
		DWORD t1 = timeGetTime(); //---------------------------------------------

		// �A�C�e���̃A�C�R���͈͍��W�擾
		rc.left = LVIR_ICON; // �A�C�R�������ƃe�L�X�g�����̗����͈͎̔擾
		if (WriteProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // ptr��RECT���
		SendMessage(hwnd, LVM_GETITEMRECT, index, (LPARAM)ptr); // �擾�v���Ɛ擪�|�C���^���M
		if (ReadProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // �͈͍��W�擾
		ico = gdGeneral::RECT2gdBox(rc);
		// �A�C�e���̑S�͈͍��W�擾
		rc.left = LVIR_BOUNDS; // �A�C�R�������ƃe�L�X�g�����̗����͈͎̔擾
		if (WriteProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // ptr��item���
		SendMessage(hwnd, LVM_GETITEMRECT, index, (LPARAM)ptr); // �擾�v���Ɛ擪�|�C���^���M
		if (ReadProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // �͈͍��W�擾
		all = gdGeneral::RECT2gdBox(rc);
		// �I���A�t�H�[�J�X���擾
		UINT ret = SendMessage(hwnd, LVM_GETITEMSTATE, index, LVIS_SELECTED | LVIS_FOCUSED);
		select_var = ((ret&LVIS_SELECTED) == LVIS_SELECTED);
		focus_var = ((ret&LVIS_FOCUSED) == LVIS_FOCUSED);
		// �z�b�g�A�C�e�����擾
		hot_var = (index == SendMessage(hwnd, LVM_GETHOTITEM, 0, 0));

		DWORD t2 = timeGetTime(); //---------------------------------------------
		//if (t2 - t1 > 4) Debug::cout(std::to_wstring(t2 - t1).c_str());

		// �����I��
		return 0;
	}
	bool select(UINT state, UINT stateMast) {
		// �A�C�e���̃X�e�[�^�X�ύX
		lvi.state = state;
		lvi.stateMask = stateMast;
		if (WriteProcessMemory(pid_h, ptr, &lvi, sizeof(LVITEM), &numRead) == 0) return 1; // ptr��item���
		SendMessage(hwnd, LVM_SETITEMSTATE, index, (LPARAM)ptr);
		// �t�H�[�J�X��L��������ׂɃE�B���h�E���A�N�e�B�u�ɂ���
		SendMessage(hwnd, WM_ACTIVATE, WA_CLICKACTIVE, 0);
		// ���
		select_var = 1;
		focus_var = 1;
		// �����I��
		return 0;
	}
	bool hot() {
		// �z�b�g�A�C�e���ݒ�
		SendMessage(hwnd, LVM_SETHOTITEM, index, 0);
		// ���
		hot_var = 1;
		// �����I��
		return 0;
	}
	inline bool getSelect() { return select_var; }
	inline bool getFocus() { return focus_var; }
	inline bool getHot() { return hot_var; }
};