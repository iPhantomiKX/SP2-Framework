#ifndef ENEMY_H
#define ENEMY_H
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

struct target
{
	int hp = 10;
	bool isDead = false;
};

class enemy
{
public:
	enemy();
	enemy(int health, float x, float y, float z, int s);
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	//void spawnEnemy(float x, float y, float z);
	virtual ~enemy();
	bool isDead();
	void Update(double dt);
	void Render();
	Vector3 pos;
	int hp;
	bool isDieded;
	/*Camera3 camera;*/
	int speed;
};

#endif