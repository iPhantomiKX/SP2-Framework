#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"
#include "MyMath.h"

Camera3::Camera3()
{

}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	maxCameraX = 49.99f;
	cameraSpeed = 2.5f;

	//Normal Bounds coordinates
	//maxX = 1048;
	//minX = -830;
	//maxZ = 240.7;
	//minZ = -886;

	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	//Default camera rotation
	cameraRotate = Vector3(0, -180, 0);

}

void Camera3::Update(double dt)
{
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}

	//Changing view (target)
	if (Application::IsKeyPressed(VK_LEFT))
	{
		cameraRotate.y += 2;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		cameraRotate.y -= 2;
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		cameraRotate.x -= 2;
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		cameraRotate.x += 2;
	}

	//Bounds checking based on maximum and minimum
	/*if (position.x > maxX)
	{
		position.x = maxX;
	}
	if (position.x < minX)
	{
		position.x = minX;
	}
	if (position.z > maxZ)
	{
		position.z = maxZ;
	}
	if (position.z < minZ)
	{
		position.z = minZ;
	}
	*/

	//Moving the camera
	if (Application::IsKeyPressed('W'))
	{
		position.x += sin(DegreeToRadian(cameraRotate.y)) * cameraSpeed;
		position.z += cos(DegreeToRadian(cameraRotate.y)) * cameraSpeed;
	}

	if (Application::IsKeyPressed('S'))
	{
		position.x += sin(DegreeToRadian(cameraRotate.y + 180)) * cameraSpeed;
		position.z += cos(DegreeToRadian(cameraRotate.y + 180)) * cameraSpeed;
	}

	if (Application::IsKeyPressed('A'))
	{
		position.x += sin(DegreeToRadian(cameraRotate.y + 90)) * cameraSpeed;
		position.z += cos(DegreeToRadian(cameraRotate.y + 90)) * cameraSpeed;
	}

	if (Application::IsKeyPressed('D'))
	{
		position.x += sin(DegreeToRadian(cameraRotate.y + 270)) * cameraSpeed;
		position.z += cos(DegreeToRadian(cameraRotate.y + 270)) * cameraSpeed;
	}
	if (Application::IsKeyPressed('E') && position.x == 335)
	{
		position.x = 247;
	}

	if (cameraRotate.x > maxCameraX)
	{
		cameraRotate.x = maxCameraX;
	}
	else if (cameraRotate.x < -maxCameraX)
	{
		cameraRotate.x = -maxCameraX;
	}

	//zoom in and out
	if (Application::IsKeyPressed('N'))
	{
		Vector3 direction = target - position;
		if (direction.Length() > 5)
		{
			Vector3 view = (target - position).Normalized();
			position += view * (float)(10.f * dt);
		}
	}
	if (Application::IsKeyPressed('M'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * (float)(10.f * dt);
	}

	//Changing target
	target = Vector3(sin(DegreeToRadian(cameraRotate.y))*cos(DegreeToRadian(cameraRotate.x)) + this->position.x, -sin(DegreeToRadian(cameraRotate.x)) + this->position.y,
		cos(DegreeToRadian(cameraRotate.y))*cos(DegreeToRadian(cameraRotate.x)) + this->position.z);
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(defaultUp);
	right.y = 0;
	up = right.Cross(view);

}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}