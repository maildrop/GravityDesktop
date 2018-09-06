#pragma once

#include "../icon/Icons.h"
#include "../Box2D/world.h"
#include "imageIcon.h"

class gdImageIcons {
private:
	gdCapture capture;
	gdIcons icons;
	gdWorld world;
	std::vector<gdImageIcon> obj;
	std::vector<gdObj> wall;
	gdBox WorldRect;
	float mostNear = 0.0f;
	unsigned int mostNearIndex = -1;
	unsigned int drugIndex = -1;
	double start_time = 1.0; // �A�j���[�V�������J�n�����܂ł̎���(s)
	double time = 0.0; // �A�j���[�V�������J�n�����܂ł̊Ԃ̌o�ߎ���(s)
	bool exitFlag = 0; // �I���t���O
	double exitAnimation1 = 0.0; // �I���A�j���[�V����(0<=exitAnimation1<=1)(y=x)
	double exitAnimation2 = 0.0; // �I���A�j���[�V����(0<=exitAnimation2<=1)(y=1-(x-1)^2)
	DWORD exitTime = 450; // �I���A�j���[�V�����ɂ����鎞��(ms�P��)
	void mouseEvent() {
		// �J�[�\�����A�C�R����ɂ���Ƃ��A�}�E�X�����ԋ����̋߂��A�C�R���擾
		mostNearIndex = -1;
		mostNear = 0.0f;
		for (size_t i = 0; i < obj.size(); i++) {
			if (obj[i].getOnMouse()) {
				if (mostNearIndex == -1) {
					mostNearIndex = i;
					mostNear = obj[i].getLocalMouse().Length();
				}
				if (obj[i].getLocalMouse().Length() < mostNear) {
					mostNearIndex = i;
					mostNear = obj[i].getLocalMouse().Length();
				}
			}
		}
		if (mostNearIndex != -1) {
			// Hot
			obj[mostNearIndex].getIcon()->hot();
			// Select��Focus
			if (Mouse::getClickB().l) {
				if ((!Keyboard::getShift().a) && (!Keyboard::getCtrl().a)) icons.unselect();
				if (Keyboard::getCtrl().a) obj[mostNearIndex].getIcon()->select(NULL, LVIS_FOCUSED | LVIS_SELECTED);
				else obj[mostNearIndex].getIcon()->select(LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			}
			// ���N���b�N�Ǐ]�A�C�R���̃C���f�b�N�X�w��
			if (Mouse::getClickB().l) drugIndex = mostNearIndex;
		}
		else
		{
			// Unhot
			icons.unhot();
			// Unselect��Unfocus
			if (Mouse::getClickB().l && (!Keyboard::getShift().a) && (!Keyboard::getCtrl().a)) icons.unselect();
		}
		// ���N���b�N�Ǐ]�A�C�R���̉��
		if (Mouse::getClickC().l && (drugIndex != -1)) drugIndex = -1;
		// �d�̓��Z�b�g
		if (Mouse::getClickC().l && (drugIndex == -1)) world.resetGravity();
		// ���N���b�N�Ǐ]
		if (drugIndex != -1) obj[drugIndex].getObj()->come(Mouse::getCursor().x, Mouse::getCursor().y);
		// �d�͂���ʂ̒��S����J�[�\�������̌����ɂ���
		if ((drugIndex == -1) && Mouse::getClickA().l) {
			gdVec2 vec = gdVec2(
				Mouse::getCursor().x - (world.getBox().size().x / 2.0f),
				Mouse::getCursor().y - (world.getBox().size().y / 2.0f)
			);
			vec /= WorldRect.size().y / 2.0f;
			vec *= (vec.Length() > 1.0f) ? (1.0f / vec.Length()) : 1.0f;
			vec *= (world.getDefaultGravity().Length() / world.getRate());
			world.setGravity(
				vec.x,
				vec.y
			);
		}
		// �E�N���b�N�Ǐ]
		if (Mouse::getClickA().r) {
			for (size_t i = 0; i < obj.size(); i++) {
				if (obj[i].getIcon()->getSelect()) obj[i].getObj()->spring(Mouse::getCursor().x, Mouse::getCursor().y);
			}
		}
		// �A�C�R�����_�u���N���b�N��
		if (Mouse::getDoubleClick().l && (mostNearIndex != -1)) {
			// �t�@�C�����s
			PostMessage(
				icons.getHwnd(),
				WM_LBUTTONDBLCLK,
				0,
				MAKELPARAM(
					obj[mostNearIndex].getIcon()->ico.position().x,
					obj[mostNearIndex].getIcon()->ico.position().y
				)
			);
		}
	}
	bool keyEvent() {
		if (Keyboard::getEsc().b) exitFlag = 1;
		if (exitFlag) {
			if (exitAnimation1 == 1.0) return 1;
			exitAnimation1 += 1000.0 / (world.getFps() * (double)exitTime);
			if (exitAnimation1 > 1.0) {
				exitAnimation1 = 1.0;
			}
			exitAnimation2 = exitAnimation1*(2.0 - exitAnimation1);
			for (size_t i = 0; i < obj.size(); i++) {
				obj[i].setOrigin(exitAnimation2);
			}
		}
		return 0;
	}

public:
	~gdImageIcons() {
		for (size_t i = 0; i < obj.size(); i++) {
			obj[i].release();
		}
		for (size_t i = 0; i < wall.size(); i++) {
			wall[i].release();
		}
	}
	bool init(HWND hwnd, ID3D11DeviceContext* deviceContext, double set_start_time = 1.0) {
		start_time = set_start_time;
		RECT rc;
		GetWindowRect(hwnd, &rc);
		WorldRect = gdGeneral::RECT2gdBox(rc);
		obj.clear();
		capture.init(deviceContext, hwnd);
		icons.init(hwnd);
		world.init(hwnd);
		update();
		return 0;
	}
	void setWallObj(RECT rc) {
		// �I�u�W�F�N�g����
		wall.push_back(gdObj());
		wall.back().init(&world, gdGeneral::RECT2gdBox(rc), false);
	}
	void setWallObj(HWND objHwnd) {
		RECT rc;
		GetWindowRect(objHwnd, &rc);
		rc.left -= WorldRect.rect().x;
		rc.top -= WorldRect.rect().y;
		rc.right -= WorldRect.rect().x;
		rc.bottom -= WorldRect.rect().y;
		setWallObj(rc);
	}
	bool update(bool stop = false) {
		if (time <= start_time) {
			time += 1.0f / getWorld()->getFps();
			stop = true;
		}
		// �A�C�R�����X�V
		if (icons.update()) return 1;
		// �f�X�N�g�b�v�̃A�C�R�������������Ă����ꍇ
		if (obj.size() != icons.icon.size()) {
			// ���Z�b�g
			for (size_t i = 0; i < obj.size(); i++) {
				obj[i].release();
			}
			obj = std::vector<gdImageIcon>(icons.icon.size());
			for (size_t i = 0; i < obj.size(); i++) {
				obj[i].init(&world, &icons.icon[i], &capture);
			}
		}
		// Box2d�X�V
		world.update(stop);
		// imageIcon�X�V
		for (size_t i = 0; i < obj.size(); i++) {
			// imageIcon�X�V
			obj[i].update();
		}
		// �}�E�X�C�x���g����
		mouseEvent();
		// �L�[�C�x���g����
		if (keyEvent()) return 1;
		// �����I��
		return 0;
	}
	bool render() {
		for (int i = 0; i < obj.size(); i++) {
			if (obj[i].render()) return 1;
		}
		return 0;
	}
	inline gdWorld* getWorld() {
		return &world;
	}
};