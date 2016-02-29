#include "enemy.h"
//target t;
//enemy thecube;

enemy::enemy()
{
	pos = Vector3(150.f, 0.f, 100.f);
	hp = 10;
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
//	Vector3 direction = camera.position - pos;
//	direction.Normalized();
//	this->pos = defaultPosition = pos;
//	this->target = defaultTarget = target;
//
//	RotateAI = Vector3(0, 0, 0);
//}

bool enemy::isDead()
{
	if (hp <= 0)
	{
		isDieded = true;
		std::cout << "amirul dieded" << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}

//void enemy::AIRotation(Vector3 Player)
//{
//	Vector3 directionInit(0, 0, 1);
//	Vector3 wantdirection(Player - pos);
//	wantdirection.Normalized();
//	Vector3 normal(0, 1, 0);
//
//	float Degree = Math::RadianToDegree(acos(directionInit.Dot(wantdirection)));
//	Vector3 Crossed = directionInit.Cross(wantdirection);
//
//	if (Crossed.Dot(normal) < 0)
//	{
//		Degree *= -1;
//	}
//}

void enemy::Update(double dt, Camera3 camera)
{
	//positioning of AI
	Vector3 direction = camera.position - pos;
	direction = direction.Normalized();
	
	pos.x += (direction.x * dt * speed) / 5;
	pos.y += (direction.y * dt * speed) / 5;
	pos.z += (direction.z * dt * speed) / 5;
	if (pos == camera.position)
	{
		std::cout << "reached end point" << std::endl;
	}

	//Rotation of AI
	Vector3 directionInit(0, 0, 1);
	Vector3 normal(0, 1, 0);
	Degree = DegreeToRadian(acos(directionInit.Dot(direction)));
	Vector3 Crossed = directionInit.Cross(direction);
	if (Crossed.Dot(normal) < 0)
	{
		Degree *= -1;
	}
}

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
