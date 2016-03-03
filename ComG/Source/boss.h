#ifndef BOSS_H
#define BOSS_H
#include "Vector3.h"
#include "Application.h"
#include "Camera3.h"
#include "SP2Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <string>
#include <iostream>

using std::string;

class boss
{
public:
	boss();
	boss(int health, float x, float y, float z, int s);
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual ~boss();
	bool isDead();
	void Update(double dt, Camera3 camera);
	void Render();
	void upgradeboss();
	void bossReset(float x, float y, float z);
	Vector3 returnPos();
	static Vector3 kB;
	//void AIRotation(Vector3 Player);

	Vector3 pos;
	int hp;
	bool isDieded;
	int speed;
	float Degree;
	static int attackPow;
	float bossUpgrade = 0;
	int upgradeCount = 0;
	Vector3 aim;
	static std::vector<Vector3> bulletPos;
	static std::vector<Vector3> bulletDir;
	float sizeIncrease;
};

#endif