#include "pch.h"
#include "hwnd_manager.h"

HWND gdHwndManager::desk;
HWND gdHwndManager::wallPaper;
std::vector<HWND> gdHwndManager::taskBar;
HWND gdHwndManager::mine;

void gdHwndManager::reload() {
	// �f�X�N�g�b�v�̃n���h���擾
	{
		HWND buf_1; // �n���h���o�b�t�@1
		HWND buf_2; // �n���h���o�b�t�@2
		{
			// �f�X�N�g�b�v��Progman���g�p���Ă����ꍇ�AWorkerW�ɕύX
			buf_1 = NULL;
			buf_2 = NULL;
			while (true) {
				buf_1 = FindWindowEx(NULL, buf_1, L"Progman", L"Program Manager");
				if (buf_1 == NULL) {
					buf_2 = NULL;
					break;
				}
				buf_2 = FindWindowEx(buf_1, NULL, L"SHELLDLL_DefView", L"");
				if (buf_2 != NULL) {
					if (SendMessageTimeout(buf_1, 0x052C, 0, 0, 0x0, 7000, NULL) == NULL) desk = NULL;
					break;
				}
			}
			// WorkerW��SysListView32�n���h���擾
			buf_1 = NULL;
			buf_2 = NULL;
			while (true) {
				buf_1 = FindWindowEx(NULL, buf_1, L"WorkerW", L"");
				if (buf_1 == NULL) {
					buf_2 = NULL;
					break;
				}
				buf_2 = FindWindowEx(buf_1, NULL, L"SHELLDLL_DefView", L"");
				if (buf_2 != NULL) {
					break;
				}
			}
		}
		if (buf_2 == NULL) desk = NULL;
		// �f�X�N�g�b�v�̃��X�g�̃n���h���擾
		desk = FindWindowEx(buf_2, NULL, L"SysListView32", NULL);
	}
	// �ǎ��n���h���̎擾
	{
		// WorkerW�̃n���h���擾
		wallPaper = NULL;
		while (true) {
			wallPaper = FindWindowEx(NULL, wallPaper, L"WorkerW", L"");
			if (wallPaper == NULL) {
				break;
			}
			if (GetParent(wallPaper) == FindWindowEx(NULL, NULL, L"Progman", L"Program Manager")) break;
		}
	}
	// �^�X�N�o�[�n���h����
	{
		taskBar.clear();
		HWND buf;
		// ���C���̃^�X�N�o�[�擾
		buf = NULL;
		while (true) {
			buf = FindWindowEx(NULL, buf, L"Shell_TrayWnd", NULL);
			if (buf == NULL) break;
			taskBar.push_back(buf);
		}
		// �T�u�̃^�X�N�o�[�擾
		buf = NULL;
		while (true) {
			buf = FindWindowEx(NULL, buf, L"Shell_SecondaryTrayWnd", NULL);
			if (buf == NULL) break;
			taskBar.push_back(buf);
		}
	}
}

void gdHwndManager::cover(HWND hwnd) {
	HWND h; // �n���h���ꎞ����p�o�b�t�@
// 	LONG style; // �X�^�C���ꎞ�ێ��o�b�t�@

	// �ǎ��E�B���h�E�̓����x��100%�ɂ���
	// �ǎ��E�B���h�E��L��������
	{
		// �ǎ�
		h = wallPaper;
		show(h);
		EnableWindow(h, TRUE);
		SetActiveWindow(h);
		// �ǎ��̐e
		h = GetParent(h);
		show(h);
		EnableWindow(h, TRUE);
		SetActiveWindow(h);
	}

	// �e�E�B���h�E�̐ݒ�
	SetParent(hwnd, wallPaper); // �e�E�B���h�E��ǎ��ɐݒ�

	RECT rc;
	GetWindowRect(wallPaper, &rc);
	SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0); // �E�B���h�E���W�𑊑΍��W�ɕύX

	// �f�X�N�g�b�v�̑c��E�B���h�E�𓧖��x��0%�ɂ���
	// (�f�X�N�g�b�v�̓����x��100%�̂܂܂ɂ���)
	{
		h = GetAncestor(desk, GA_ROOTOWNER);
		hide(h);
	}
	SetHook(hwnd, GetParent(wallPaper));
}

void gdHwndManager::hide(HWND hwnd) {
	LONG style; // �X�^�C���ꎞ�ێ��o�b�t�@
	style = GetWindowLong(hwnd, GWL_EXSTYLE);
	SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, 0, LWA_ALPHA);
}

void gdHwndManager::half(HWND hwnd) {
	LONG style; // �X�^�C���ꎞ�ێ��o�b�t�@
	style = GetWindowLong(hwnd, GWL_EXSTYLE);
	SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, 128, LWA_ALPHA);
}

void gdHwndManager::show(HWND hwnd) {
	LONG style; // �X�^�C���ꎞ�ێ��o�b�t�@
	style = GetWindowLong(hwnd, GWL_EXSTYLE);
	SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
}

void gdHwndManager::exit(HWND hwnd) {
	HWND h; // �n���h���ꎞ����p�o�b�t�@
	//LONG style; // �X�^�C���ꎞ�ێ��o�b�t�@
  UNREFERENCED_PARAMETER(hwnd);

	ResetHook();

	// �f�X�N�g�b�v�̑c��E�B���h�E�𓧖��x��0%�ɂ���
	// (�f�X�N�g�b�v�̓����x��100%�̂܂܂ɂ���)
	{
		h = GetAncestor(desk, GA_ROOTOWNER);
		show(h);
	}
}