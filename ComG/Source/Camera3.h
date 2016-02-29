#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "MyMath.h"
#include "Vector3.h"
#include <vector>
#include <iostream>

using Math::DegreeToRadian;

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 target, up, right, view;
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	Vector3 camerarotation;
	float cameraRotationX, cameraRotationY;
	float maxCameraX;
	float cameraSpeed;
	float walkSpeed;
	int screenMidX;
	int screenMidY;
	int speed;
	float upvariable;
	float rightvariable;
	double move;
	static bool tricker;
	static bool test;
	static bool test2;
	static bool test3;
	static bool mouseControl;
	int delay2;
	static double xpos;
	static double ypos;
	static Vector3 location;
	static Vector3 location2;
	static Vector3 direction;
	//Vector3 direction2;
	static double recoil;
	double cameraStore;
	int recoilResist;
	Vector3 gunRecoil;
	Vector3 directionRotation;
	int cRecoilCd;
	static bool inBase;


	Vector3 diffx;
	Vector3 diffz;
	std::vector<Vector3> minVectors;
	std::vector<Vector3> maxVectors;
	std::vector<Vector3> minMineralVectors;
	std::vector<Vector3> maxMineralVectors;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	bool testhitbox(const Vector3& lowest, const Vector3& highest, double move);
	bool hitbox(double move);
	bool checkcollisionwithObject(double move);
	bool checkcollisionwithOres(double move); 
	//bool checkBulletcollisionwithObject(Vector3 bulletPos);
	bool checkcollisionwithTricker(Vector3& otherObjectposition, float sizex, float sizey, float sizez);
	bool checkcollisionwithOtherObject(Vector3& otherObjectposition, float sizex, float sizey, float sizez);
	void teleport();
	bool mining();
	bool craftUi();
	Vector3 minPos(Vector3 center, double sizeX, double sizeY, double sizeZ);
	Vector3 maxPos(Vector3 center, double sizeX, double sizeY, double sizeZ);
	Vector3 setPos();
	virtual void Reset();

};

#endif