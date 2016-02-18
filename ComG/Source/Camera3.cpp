#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"
#include <GLFW/glfw3.h>

Application a;
extern GLFWwindow* m_window;

Camera3::Camera3() :
cameraRotationX(0),
cameraRotationY(0)
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	maxCameraX = 49.99f;
	cameraSpeed = 2.5f;

	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	camerarotation = Vector3(0, -180, 0);
	speed = 100;
	location = (0,0,0);
	location2 = (0, 0, 0);
	//delay = 0;
	delay2 = 0;
	//cd = 10;

	//mouseSpeed = 0.005f;
}

void Camera3::Update(double dt)
{
	static const float CAMERA_SPEED = 50.f;
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
	//std::cout << xpos << " " << ypos << std::endl;
	if (mouseControl == true)
	{
		xpos, ypos = a.Mouse(xpos, ypos);

		if (xpos < 390)
		{
			//rotation.SetToRotation(3, up.x, up.y, up.z);
			//view = rotation * view;
			//target = position + view;
			camerarotation.y += 3;
		}
		if (xpos > 410)
		{
			//rotation.SetToRotation(-3, up.x, up.y, up.z);
			//view = rotation * view;
			//target = position + view;
			camerarotation.y -= 3;
		}
		if (ypos < 290)
		{
			if (camerarotation.x > -45)
			{
				//rotation.SetToRotation(2, right.x, right.y, right.z);
				//view = rotation * view;
				//target = position + view;
				camerarotation.x -= 3;
			}
		}
		if (ypos > 310)
		{
			if (camerarotation.x < 45)
			{
				//rotation.SetToRotation(-2, right.x, right.y, right.z);
				//view = rotation * view;
				//target = position + view;
				camerarotation.x += 3;
			}
		}
	}
	else
	{
		if (Application::IsKeyPressed(VK_LEFT))
		{
			//rotation.SetToRotation(2, up.x, up.y, up.z);
			//view = rotation * view;
			//target = position + view;
			camerarotation.y += 3;
		}
		if (Application::IsKeyPressed(VK_RIGHT))
		{
			//rotation.SetToRotation(-2, up.x, up.y, up.z);
			//view = rotation * view;
			//target = position + view;
			camerarotation.y -= 3;
		}
		if (Application::IsKeyPressed(VK_UP))
		{

			/*if (view.y < 0.5)
			{
				rotation.SetToRotation(1.5, right.x, right.y, right.z);
				view = rotation * view;
				target = position + view;
			}*/
			if (camerarotation.x > -45)
			{
				camerarotation.x -= 3;
			}
			
			
		}
		if (Application::IsKeyPressed(VK_DOWN))
		{
			/*if (view.y > -0.5)
			{
				rotation.SetToRotation(-1.5, right.x, right.y, right.z);
				view = rotation * view;
				target = position + view;
			}*/
			
			if (camerarotation.x < 45)
			{
				camerarotation.x += 3;
			}
		}
	}
	/*if (Application::IsKeyPressed(VK_LEFT))
	{
		Vector3 view = (target - position).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(1, up.x, up.y, up.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		Vector3 view = (target - position).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(-1, up.x, up.y, up.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		if (view.y < 0.5)
		{
			Mtx44 rotation;
			rotation.SetToRotation(1, right.x, right.y, right.z);
			view = rotation * view;
			target = position + view;
		}
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		if (view.y > -0.5)
		{
			Mtx44 rotation;
			rotation.SetToRotation(-1, right.x, right.y, right.z);
			view = rotation * view;
			target = position + view;
		}
	}*/

	location2 = position + view*dt*speed;
	location = position;

	direction = (view*dt*speed)*2;

	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		if (position.x + (view.x * dt * speed) <= 495 && position.x + (view.x * dt * speed) >= -495 && hitbox(position.x * dt * speed) == false)
		{
			position.x += view.x * dt * speed;
			target.x += view.x * dt * speed;
		}
		if (position.z + (view.z * dt * speed) <= 495 && position.z + (view.z * dt * speed) >= -495 && hitbox(position.z * dt * speed) == false)
		{
			position.z += view.z * dt * speed;
			target.z += view.z * dt * speed;
		}	
		if (Application::IsKeyPressed(VK_SHIFT) && position.y + (view.y * dt * speed) <= 495 && position.y + (view.y * dt * speed) >= -45 && hitbox(position.y * dt * speed) == false)
		{
			position.y += view.y * dt * speed;
			target.y += view.y * dt * speed;
		}
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();
		if (position.x - (view.x * dt * speed) <= 495 && position.x - (view.x * dt * speed) >= -495 && hitbox(-(position.x * dt * speed)) == false)
		{
			position.x -= view.x * dt * speed;
			target.x -= view.x * dt * speed;
		}
		if (position.z - (view.z * dt * speed) <= 495 && position.z - (view.z * dt * speed) >= -495 && hitbox(-(view.z * dt * speed)) == false)
		{
			position.z -= view.z * dt * speed;
			target.z -= view.z * dt * speed;
		}
		if (Application::IsKeyPressed(VK_SHIFT) && position.y - (view.y * dt * speed) <= 495 && position.y - (view.y * dt * speed) >= -45 && hitbox(-(view.y * dt * speed)) == false)
		{
			position.y -= view.y * dt * speed;
			target.y -= view.y * dt * speed;
		}
	}
	if (Application::IsKeyPressed('D'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		if (position.x + (right.x * dt * speed) <= 495 && position.x + (right.x * dt * speed) >= -495 && hitbox(right.x * dt * speed) == false)
		{
			position.x += right.x * dt * speed;
			target.x += right.x * dt * speed;
		}
		if (position.z + (right.z * dt * speed) <= 495 && position.z + (right.z * dt * speed) >= -495 && hitbox(right.z * dt * speed) == false)
		{
			position.z += right.z * dt * speed;
			target.z += right.z * dt * speed;
		}
	}
	if (Application::IsKeyPressed('A'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		if (position.x - (right.x * dt * speed) <= 495 && position.x - (right.x * dt * speed) >= -495 && hitbox(-(right.x * dt * speed)) == false)
		{
			position.x -= right.x * dt * speed;
			target.x -= right.x * dt * speed;
		}
		if (position.z - (right.z * dt * speed) <= 495 && position.z - (right.z * dt * speed) >= -495 && hitbox(-(right.z * dt * speed)) == false)
		{
			position.z -= right.z * dt * speed;
			target.z -= right.z * dt * speed;
		}
	}

	//std::cout << direction.x - position.x << std::endl;
	//std::cout << direction.y - position.y << std::endl;
	//std::cout << direction.z - position.z << std::endl;

	if (Application::IsKeyPressed(VK_LBUTTON))
	{
		std::cout << position << "cam" << std::endl;
		std::cout << view << "view" << std::endl;
	}

	if (camerarotation.x > maxCameraX)
	{
		camerarotation.x = maxCameraX;
	}
	else if (camerarotation.x < -maxCameraX)
	{
		camerarotation.x = -maxCameraX;
	}

	

	/*if (Application::IsKeyPressed('R'))
	{
		Reset();
	}*/

	
	//Changing target
	target = Vector3(sin(DegreeToRadian(camerarotation.y))*cos(DegreeToRadian(camerarotation.x)) + this->position.x, -sin(DegreeToRadian(camerarotation.x)) + this->position.y,
		cos(DegreeToRadian(camerarotation.y))*cos(DegreeToRadian(camerarotation.x)) + this->position.z);

}

Vector3 Camera3::setPos()
{
	location2 = position;
	return location;
}

bool Camera3::testhitbox(const Vector3& lowest, const Vector3& highest, double move)
{
	/*if (position.x + 5 + move > lowest.x && position.x - 5 + move < highest.x && position.y + 5 + move > lowest.y && position.y - 5 + move < highest.y && position.z + 5 + move > lowest.z && position.z - 5 + move < highest.z)
	{
		return true;
	}
	else
	{*/
		return false;
	//}
}

bool Camera3::hitbox(double move)
{
	/*for (int i = 0; i <= 50; i++)
	{
		if (testhitbox(Vector3(treex[i] - 20, -150, treez[i] - 20), Vector3(treex[i] + 20, 100, treez[i] + 20), move) == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}*/
	return false;
}

bool Camera3::checkcollisionwithObject(Vector3& otherObjectposition, float sizex, float sizey, float sizez)
{
	//to do
	// Get all min and max (eg. minX, maxX)

	// Do SAT Test on all axis)
	// eg (if (position.x < min) return false; // (Outside!)
	//		Repeat for Max, Repeat Min + Max for Y and Z axis
	float minX = otherObjectposition.x - (sizex / 2);
	float maxX = otherObjectposition.x + (sizex / 2);

	float minY = otherObjectposition.y - (sizey / 2);
	float maxY = otherObjectposition.y + (sizey / 2);

	float minZ = otherObjectposition.z - (sizez / 2);
	float maxZ = otherObjectposition.z + (sizez / 2);
	if (position.x < minX || position.x > maxX)
	{
		return false;
	}
	if (position.y < minY || position.y > maxY)
	{
		return false;
	}
	if (position.z < minZ || position.z > maxZ)
	{
		return false;
	}
	//test2 = true;
	return true;
	//return false;
}

bool Camera3::checkcollisionwithTricker(Vector3& otherObjectposition, float sizex, float sizey, float sizez)
{
	//float minX = otherObjectposition.x - (sizex / 2);
	//float maxX = otherObjectposition.x + (sizex / 2);
	//float minZ = otherObjectposition.z - (sizez / 2);
	//float maxZ = otherObjectposition.z + (sizez / 2);
	//if (position.x < minX || position.x > maxX)
	//{
	//	return false;
	//}
	//if (position.z < minZ || position.z > maxZ)
	//{
	//	return false;
	//}
	//test = true;
	//return true;
	return false;
}


void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}