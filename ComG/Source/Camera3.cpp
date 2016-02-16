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
	static const float CAMERA_SPEED = 50.f;
	direction = target;
	prevPosition = position;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	if (Application::IsKeyPressed('Q') && mouseControl == false && delay2 == 0)
	{
		mouseControl = true;
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		delay2 = 10;
	}
	else if (Application::IsKeyPressed('Q') && mouseControl == true && delay2 == 0)
	{
		mouseControl = false;
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		delay2 = 10;
	}
	if (delay2 > 0)
	{
		delay2--;
	}
	Mtx44 rotation;
	if (mouseControl == true)
	{
		xpos, ypos = a.Mouse(xpos, ypos);

		if (xpos < 390)
		{
			rotation.SetToRotation(3, up.x, up.y, up.z);
			view = rotation * view;
			target = position + view;
		}
		if (xpos > 410)
		{
			rotation.SetToRotation(-3, up.x, up.y, up.z);
			view = rotation * view;
			target = position + view;
		}
		if (ypos < 290)
		{
			if (view.y < 0.5)
			{
				rotation.SetToRotation(2, right.x, right.y, right.z);
				view = rotation * view;
				target = position + view;
			}
		}
		if (ypos > 310)
		{
			if (view.y > -0.5)
			{
				rotation.SetToRotation(-2, right.x, right.y, right.z);
				view = rotation * view;
				target = position + view;
			}
		}
	}
	else
	{
		if (Application::IsKeyPressed(VK_LEFT))
		{
			rotation.SetToRotation(2, up.x, up.y, up.z);
			view = rotation * view;
			target = position + view;
		}
		if (Application::IsKeyPressed(VK_RIGHT))
		{
			rotation.SetToRotation(-2, up.x, up.y, up.z);
			view = rotation * view;
			target = position + view;
		}
		if (Application::IsKeyPressed(VK_UP))
		{

			if (view.y < 0.5)
			{
				rotation.SetToRotation(1.5, right.x, right.y, right.z);
				view = rotation * view;
				target = position + view;
			}
		}
		if (Application::IsKeyPressed(VK_DOWN))
		{
			if (view.y > -0.5)
			{
				rotation.SetToRotation(-1.5, right.x, right.y, right.z);
				view = rotation * view;
				target = position + view;
			}
		}
	}
	/*if (Application::IsKeyPressed(VK_LEFT))
>>>>>>> origin/master
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

	location = position;

	if (Application::IsKeyPressed('R'))
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