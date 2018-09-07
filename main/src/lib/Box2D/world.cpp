#include "pch.h"
#include "world.h"


bool gdWorld::init(float size_x, float size_y) 
{
	// ���
	size.size(size_x, size_y);
	rate = max_size / size.size().y; // Box2D�̋�Ԃ̏c���ő�l��max_size�ɐݒ�					
	gravity = b2Vec2(0.0, 9.8f); // �d�͐ݒ�
	// ��Ԑ���
	world = new b2World(gravity);
	// �ǐ���
	left_wall = std::unique_ptr<gdObj>(new gdObj);
	top_wall = std::unique_ptr<gdObj>(new gdObj);
	right_wall = std::unique_ptr<gdObj>(new gdObj);
	bottom_wall = std::unique_ptr<gdObj>(new gdObj);
	left_wall->init(this, gdBox({ 10.0f,size.size().y }, { 0.0f - 5.0f, size.size().y / 2.0f }), false);
	top_wall->init(this, gdBox({ size.size().x,10.0f }, { size.size().x / 2.0f, 0.0f - 5.0f }), false);
	right_wall->init(this, gdBox({ 10.0f,size.size().y }, { size.size().x + 5.0f, size.size().y / 2.0f }), false);
	bottom_wall->init(this, gdBox({ size.size().x,10.0f }, { size.size().x / 2.0f, size.size().y + 5.0f }), false);
	return 0;
}

/**
  init( float , float) �Ɛ��x�Ⴂ�̏������p
  �{���̌v�Z���x��float�ł��邪�AHWND �̐��x�� LONG �Ȃ̂ŁA���̃M���b�v�𖄂߂邽�߂�
  �R���r�j�G���X��init() �P���ȃ��b�p
*/
bool gdWorld::init(LONG size_x, LONG size_y)
{
  return init(static_cast<float>(size_x), static_cast<float>(size_y));
}

bool gdWorld::init(HWND hwnd) {
	RECT rc;
	GetWindowRect(hwnd, &rc);
	return init(rc.right - rc.left, rc.bottom - rc.top );
}
void gdWorld::update(bool stop) {
	// ���݂̃t���[�����[�g�擾
	now_time = timeGetTime();
	if (before_time == 0) fps = 120.0;
	else fps = 1000.0 / (double)(now_time - before_time);
	before_time = now_time;
	// Box2D�X�V
	if (!stop) {
		world->Step(
			(float32)(1.0 / fps),
			(int32)(fps),
			2
		);
	}
}
void gdWorld::setGravity(float x, float y) {
	world->SetGravity(rate * b2Vec2(x, y));
}
void gdWorld::resetGravity() {
	world->SetGravity(gravity);
}