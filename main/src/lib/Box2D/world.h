#pragma once

#include <vector>
#include <chrono>
#include "../common/box.h"
#include "Box2D/Box2D.h"

#include "obj.h"

#include "../Win32/Debug.h"

class gdObj;

class gdWorld {
private:
	float max_size = 5.0f; // Box2D�̍ő��ԃT�C�Y
	float rate = 1.0f; // Box2D��ԒP��/���[�U�[��ԒP��
	DWORD now_time = 0; // �t���[�����[�g�擾�p�̌��ݎ���(ms�P��)
	DWORD before_time = 0; // �t���[�����[�g�擾�p��1�t���[���O����(ms�P��)
	double fps = 60.0; // ���݂̃t���[�����[�g
	b2World *world = nullptr; // Box2D���
	gdBox size; // ���[�U�[��ԃT�C�Y
	b2Vec2 gravity = b2Vec2(0.0f, 9.8f); // �d�͐ݒ�
	std::unique_ptr<gdObj> left_wall; // ����
	std::unique_ptr<gdObj> top_wall; // ���
	std::unique_ptr<gdObj> right_wall; // �E��
	std::unique_ptr<gdObj> bottom_wall; // ����

public:
	bool init(float size_x, float size_y);
  bool init(LONG size_x, LONG size_y);
	bool init(HWND hwnd);
	void update(bool stop = 0);
	void setGravity(float x, float y);
	void resetGravity();
	inline b2World* getWorld() {
		return world;
	}
	inline float getRate() { return rate; }
	inline double getFps() { return fps; }
	inline gdBox getBox() { return size; }
	inline b2Vec2 getDefaultGravity() { return gravity; };
};