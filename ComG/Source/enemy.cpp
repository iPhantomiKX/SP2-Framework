#include "enemy.h"
//target t;
//enemy thecube;

enemy::enemy()
{
	/*pos = Vector3();
	hp = 10;
	isDieded = false;
	speed = 1;
	attackPow = 1;
	enemyUpgrade = 0;*/
}
enemy::enemy(int health, float x, float y, float z, int s)
{
	pos = Vector3(x, y, z);
	hp = health;
	isDieded = false;
	attackPow = 1;
	speed = s;
}

void enemy::respawnEnemy(int x, int y, int z)
{
	upgradeCount++;
	if (upgradeCount == 5)
	{
		upgradeCount = 0;
		enemyUpgrade++;
	}
	if (x < -300 && x > -500 && z > -30 && z < 100)
	{
		x = 0;
		z = 0;
	}
	pos = Vector3(x,y,z);
	hp = 10 + (enemyUpgrade * 5);
	attackPow = 1 + (enemyUpgrade/2);
	//speed = speed + 20;
	isDieded = false;
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
	else if (pos.x + (direction.x * dt * speed) < Camera3::location.x + 5 && pos.x + (direction.x * dt *speed) > Camera3::location.x - 5 && pos.z + (direction.z * dt *speed) > Camera3::location.z -5 && pos.z + (direction.z * dt *speed) < Camera3::location.z + 5)
	{

	}
	else
	{
		pos.x += (direction.x * dt * speed) * (1 + (enemyUpgrade/4));
		//std::cout << (direction.x * dt * speed) * ((enemyUpgrade / 4) + 1) << std::endl;
		//pos.y += (direction.y * dt * speed);
		pos.z += (direction.z * dt * speed) * (1 + (enemyUpgrade / 4));
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

	//std::vector<Vector3>::iterator count = bulletPos.begin();
	//std::vector<Vector3>::iterator count1 = bulletDir.begin();

	//while (count != bulletPos.end())
	//{
	//	*count += *count1;
	//	*count++;
	//	*count1++;
	//}
}

//void enemy::shootBullet()
//{
//	bulletPos.push_back(pos);
//	bulletDir.push_back(aim);
//}

Vector3 enemy::returnPos()
{
	//std::cout << pos << std::endl;
	return pos;
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
