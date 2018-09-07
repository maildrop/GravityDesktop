#pragma once

#include "../icon/Icons.h"
#include "../Box2D/World.h"

class gdImageIcon{
private:
	gdCapture *capture;
	gdIcon *icon;
	gdObj obj;
	gdBox image_ico;
	gdBox image_all;
	gdVec2 local_mouse;
	bool on_mouse;
	float origin = 0.0f;

public:
	bool init(gdWorld *world, gdIcon *set_icon, gdCapture *set_capture) {
		icon = set_icon;
		obj.init(world, set_icon->ico);
		image_ico = icon->ico;
		image_all = icon->all;
		capture = set_capture;
		return 0;
	}
	void update() {
		// obj�X�V
		obj.update();
		// image_ico�X�V
		image_ico = obj.getBox();
		image_ico.position(image_ico.position()*(1.0f-origin)+icon->ico.position()*(origin));
		image_ico.angle(image_ico.angle()*(1.0f - origin) + icon->ico.angle()*(origin));
		// image_all�p�x�X�V
		image_all.angle(image_ico.angle());
		// gap_vec����]�s��ɂ�����
		gdVec2 gap_vec;
		gap_vec.x =
			(icon->all.position() - icon->ico.position()).x * std::cos(image_ico.angle()) -
			(icon->all.position() - icon->ico.position()).y * std::sin(image_ico.angle());
		gap_vec.y =
			(icon->all.position() - icon->ico.position()).x * std::sin(image_ico.angle()) +
			(icon->all.position() - icon->ico.position()).y * std::cos(image_ico.angle());
		// �v�Z
		image_all.position(image_ico.position() + gap_vec);
		// ���΃J�[�\�����W�Z�o
		double len = gdVec2(
			(Mouse::getCursor().x - (image_all.position().x)),
			(Mouse::getCursor().y - (image_all.position().y))
		).Length();
		float angle = std::atan2(
			Mouse::getCursor().y - image_all.position().y,
			Mouse::getCursor().x - image_all.position().x
		) - image_all.angle();
    local_mouse.x = static_cast<decltype(local_mouse.x)>(std::cos(angle)*len);
		local_mouse.y = static_cast<decltype(local_mouse.y)>(std::sin(angle)*len);
		// �J�[�\�����A�C�R����ɂ��邩����
		on_mouse =
			std::abs(local_mouse.x) <= icon->all.size().x / 2.0f &&
			std::abs(local_mouse.y) <= icon->all.size().y / 2.0f;
	}
	bool render() {
		RECT rc = gdGeneral::gdBox2RECT(icon->all.rect());
		capture->getSpriteBatch()->Begin();
		capture->getSpriteBatch()->Draw(
			capture->getResourceView(), // �e�N�X�`��
			image_ico.position(), // �`��̒��S���W
			&rc, // �`��͈�
			DirectX::Colors::White, // �X�v���C�g�𒅐F����F
			image_ico.angle(), // ��]
			image_ico.size() / 2.0 // �摜�̒��S�����W
		);
		capture->getSpriteBatch()->End();
		return 0;
	}
	void release() {
		obj.release();
	}
	inline gdVec2 getLocalMouse() {
		return local_mouse;
	}
	inline bool getOnMouse() {
		return on_mouse;
	}
	inline gdIcon* getIcon() {
		return icon;
	}
	inline gdObj* getObj() {
		return &obj;
	}
	inline void setOrigin(float set_origin) {
		origin = set_origin;
	}
};