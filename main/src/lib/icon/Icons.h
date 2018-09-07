#pragma once

#include "icon.h"
#include "../common/general.h"
#include <vector>
#include <Windows.h>
#include <Commctrl.h>

class gdIcons {
private:
	HWND hwnd = NULL; // �w�肳�ꂽSysListView�̃E�B���h�E�n���h��
	HANDLE pid_h = NULL; // �w�肳�ꂽSysListView��PID�n���h��
	LPVOID ptr = nullptr; // �w�肳�ꂽSysListView�ɐ��������������̐擪�|�C���^
	unsigned int num_items; // �A�C�R������
	LVITEM lvi; // �ꎞ�o�b�t�@
	ULONG_PTR numRead; // �ꎞ�o�b�t�@

public:
	std::vector<gdIcon> icon; // �w�肳�ꂽSysListView���ɑ��݂���A�C�R��
	bool init(HWND set_hwnd){
		// �n���h�����
		hwnd = set_hwnd;
		// PID�擾
		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid);
		if (pid == NULL) return 1;
		// PID�n���h���擾
		pid_h = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, pid);
		if (pid_h == NULL) return 1;
		// PID�Ƀ������m��
		ptr = VirtualAllocEx(pid_h, NULL, sizeof(RECT) + sizeof(LVITEM), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (ptr == NULL) return 1;
		// �X�V
		update();
		// �����I��
		return 0;
	}
  bool update() {
		// �f�X�N�g�b�v�̃A�C�R�����擾
		num_items = ListView_GetItemCount(hwnd);

		// �A�C�R�����ɕύX������Δz�񂻂̂��̂��X�V
		if (icon.size() != num_items) {

      { // swap 
        std::vector<gdIcon> new_iconvec(num_items);
        icon.swap(new_iconvec);
      }

      gdIcon::index_type index = 0;
      for (auto& i : icon) {
        i.init(hwnd, pid_h, ptr, index++);
      }
		}

    // �S�A�C�R�����擾
		for (size_t i = 0; i < icon.size(); i++) {
			if(icon[i].update()) return 1;
		}

		return 0;
	}
	bool unselect() {
		// �A�C�e���̃X�e�[�^�X�ύX
		lvi.state = 0;
		lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
		if (WriteProcessMemory(pid_h, ptr, &lvi, sizeof(LVITEM), &numRead) == 0) return 1; // ptr��item���
    SendMessage(hwnd, LVM_SETITEMSTATE, static_cast<WPARAM>(int{ -1 }), (LPARAM)ptr);
		// �t�H�[�J�X��L��������ׂɃE�B���h�E���A�N�e�B�u�ɂ���
		SendMessage(hwnd, WM_ACTIVATE, WA_CLICKACTIVE, 0);
		return 0;
	}
	bool unhot() {
		// �U�̃}�E�X���W�𑗐M���ăz�b�g�A�C�e��������
		PostMessage(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(0, 0));
		return 0;
	}
	inline HWND getHwnd() { return hwnd; }
};