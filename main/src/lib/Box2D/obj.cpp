#include "pch.h"
#include "obj.h"
#include "../Win32/Debug.h"

void gdObj::init(gdWorld *set_world, gdBox set_size, bool dynamic) {
	// ������
	gd_world = set_world;
	world = gd_world->getWorld();
	rate = gd_world->getRate();
	size = set_size;
	// ��`
	bodyDef.type = dynamic ? b2_dynamicBody : b2_staticBody;
	bodyDef.allowSleep = false;
	bodyDef.linearDamping = 0.0f;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.8f;
	fixtureDef.restitution = 0.45f;
	bodyDef.position.Set(
		size.position().x * rate,
		size.position().y * rate
	);
	bodySize.SetAsBox(
		size.size().x * rate / 2.0f,
		size.size().y * rate / 2.0f
	);
	fixtureDef.shape = &bodySize;
	// ����
	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	// �X�V
	update();
}
gdObj::~gdObj() {}
void gdObj::come(float x, float y) {
	// (x,y)�ɓ����ŋ߂Â�
	body->SetLinearVelocity(b2Vec2(
		(float32)(x - size.position().x) * rate * 10.0f,
		(float32)(y - size.position().y) * rate * 10.0f
	));
}
void gdObj::spring(float x, float y) {
	// (x,y)�Ƀo�l�̂悤�ɋ߂Â�
	// �ϐ�������
	b2Vec2 move_ivec;
	float move_length;
	// �v�Z
	move_ivec.x = (x - size.position().x) * rate;
	move_ivec.y = (y - size.position().y) * rate;
	move_length = move_ivec.Length();
	move_ivec *= (1.0f / move_length);
	move_ivec = 100.0f * move_length * body->GetMass() * move_ivec;
	// �d�͖�����
	move_ivec -= (body->GetMass() * world->GetGravity());
	// ���x�������ύX
	body->SetLinearDamping(5.0f);
	// �͂̓K��
	body->ApplyForce(move_ivec, body->GetPosition(), 1);
}
void gdObj::update() {
	// ���x���������Z�b�g
	body->SetLinearDamping(0.0f);
	// Box2D���W�A�p�x�擾
	size.b2position((1 / rate)*(body->GetPosition()));
	size.angle(body->GetAngle());
	size.angle(size.angle() - (gdVec1)gdGeneral::pi2*(gdVec1)std::floor((size.angle() + gdGeneral::pi) / gdGeneral::pi2)); // (-pi<=body[i].agl<pi)�ɕϊ�
}
void gdObj::release() {
	if (body != nullptr) world->DestroyBody(body);
}