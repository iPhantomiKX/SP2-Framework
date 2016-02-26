#include "enemy.h"
//target t;
//enemy thecube;

enemy::enemy()
{
	pos = Vector3(150.f, 0.f, 100.f);
	hp = 100;
	isDieded = false;
	speed = 1;
}
enemy::enemy(int health, float x, float y, float z, int s)
{
	pos = Vector3(x, y, z);
	hp = health;
	isDieded = false;
	speed = s;
}

//void enemy::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
//{
//	maxRotationX = 49.99f;
//	Vector3 view = camera.position - pos;
//	view.Normalized();
//	this->pos = defaultPosition = pos;
//	this->target = defaultTarget = target;
//
//	RotateAI = Vector3(0, 0, 0);
//}

bool enemy::isDead()
{
	return isDieded;
}

//void enemy::AIRotation(Vector3 Player)
//{
//	Vector3 viewInit(0, 0, 1);
//	Vector3 wantView(Player - pos);
//	wantView.Normalized();
//	Vector3 normal(0, 1, 0);
//
//	float Degree = Math::RadianToDegree(acos(viewInit.Dot(wantView)));
//	Vector3 Crossed = viewInit.Cross(wantView);
//
//	if (Crossed.Dot(normal) < 0)
//	{
//		Degree *= -1;
//	}
//}

//void enemy::Update(double dt)
//{
//	//camera.Update(dt);
//	Vector3 view = camera.position - pos;
//	view.Normalized();
//	
//	pos.x += (view.x * dt * speed) / 5;
//	pos.y += (view.y * dt * speed) / 5;
//	pos.z += (view.z * dt * speed) / 5;
//	if (pos == camera.position)
//	{
//		std::cout << "reached end point" << std::endl;
//	}
//}

//void enemy::Render()
//{
//	modelStack.PushMatrix();
//	modelStack.Scale(4, 4, 4);
//	modelStack.Translate(pos.x, pos.y, pos.z);
//	modelStack.PopMatrix();
//}

enemy::~enemy()
{

}
