#include "boss.h"
//target t;
//boss thecube;

boss::boss()
{
	/*pos = Vector3();
	hp = 10;
	isDieded = false;
	speed = 1;
	attackPow = 1;
	bossUpgrade = 0;*/
}
boss::boss(int health, float x, float y, float z, int s)
{
	pos = Vector3(x, y, z);
	hp = health;
	isDieded = false;
	attackPow = 5;
	speed = s;
	sizeIncrease = 3;
}

void boss::upgradeboss()
{
	bossUpgrade++;
	attackPow = 5 + (bossUpgrade*5);
}

void boss::bossReset(float x, float y, float z)
{
	bossUpgrade = 0;
	hp = 1000;
	attackPow = 5;
	pos = Vector3(x, y, z);
	sizeIncrease = 3;
}

bool boss::isDead()
{
	if (hp <= 0)
	{
		isDieded = true;
		std::cout << "BOSS HAS BEEN SLAIN" << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}

void boss::Update(double dt, Camera3 camera)
{
	//positioning of AI
	Vector3 direction = camera.position - pos;
	direction = direction.Normalized();

	if (pos.x + (direction.x * dt * speed) < -330 && pos.x + (direction.x * dt *speed) > -470 && pos.z + (direction.z * dt *speed) > -10 && pos.z + (direction.z * dt *speed) < 80)
	{
		if (pos.x + (direction.x * dt * speed) < -330 && pos.x + (direction.x * dt *speed) > -470)
		{
			pos.z--;
		}
		if (pos.z + (direction.z * dt *speed) > -10 && pos.z + (direction.z * dt *speed) < 80)
		{
			pos.x++;
		}
	}
	else if (pos.x + (direction.x * dt * speed) < Camera3::location.x + 5 && pos.x + (direction.x * dt *speed) > Camera3::location.x - 5 && pos.z + (direction.z * dt *speed) > Camera3::location.z - 5 && pos.z + (direction.z * dt *speed) < Camera3::location.z + 5)
	{

	}
	else
	{
		pos.x += (direction.x * dt * speed) * (0.5 + (bossUpgrade/2));
		//std::cout << (direction.x * dt * speed) * ((bossUpgrade / 4) + 1) << std::endl;
		//pos.y += (direction.y * dt * speed);
		pos.z += (direction.z * dt * speed) * (0.5 + (bossUpgrade / 2));
	}

	//Rotation of AI
	Vector3 directionInit(0, 0, 1);
	Vector3 normal(0, 1, 0);
	Degree = Math::RadianToDegree(acos(directionInit.Dot(direction)));
	Vector3 Crossed = directionInit.Cross(direction);
	if (Crossed.Dot(normal) < 0)
	{
		Degree *= -1;
	}

}

Vector3 boss::returnPos()
{
	return pos;
}

boss::~boss()
{

}
