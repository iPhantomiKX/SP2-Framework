#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "MyMath.h"

using Math::DegreeToRadian;

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	Vector3 camerarotation;
	float cameraRotationX, cameraRotationY;
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
	static Vector3 direction;


	Vector3 diffx;
	Vector3 diffz;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	//bool testhitbox(const Vector3& lowest, const Vector3& highest, double move);
	//bool hitbox(double move);
	//bool checkcollisionwithObject(Vector3& otherObjectposition, float sizex, float sizey, float sizez);

	virtual void Update(double dt);
	virtual void Reset();

};

#endif