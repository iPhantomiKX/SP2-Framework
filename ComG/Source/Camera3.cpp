#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"
#include <GLFW/glfw3.h>
#include "player.h"

Application a;
player p;
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
	directionRotation = Vector3(0, -180, 0);
	gunRecoil = Vector3(0,0,0);
	camerarotation = Vector3(0,0,0);
	speed = 100;
	location = (0,0,0);
	location2 = (0, 0, 0);
	//direction2 = (0, 0, 0);
	//mouseControl = true;
	//delay = 0;
	delay2 = 0;
	//cd = 10;
	cameraStore = 0;
	cRecoilCd = 0;
	stamina = 300;
	staminaDelay = 0;

	///for hitboxes///
	/*minVectors.push_back(minPos(Vector3(400, 60, 0), 100, 20, 100));
	minVectors.push_back(minPos(Vector3(400, 60, 100), 100, 20, 100));
	minVectors.push_back(minPos(Vector3(300, 60, 0), 100, 20, 100));
	minVectors.push_back(minPos(Vector3(300, 60, 100), 100, 20, 100));
	minVectors.push_back(minPos(Vector3(450, 110, 0), 15, 100, 100));
	minVectors.push_back(minPos(Vector3(450, 110, 100), 15, 100, 100));
	minVectors.push_back(minPos(Vector3(250, 110, 0), 15, 100, 100));
	minVectors.push_back(minPos(Vector3(250, 110, 100), 15, 100, 100));
	minVectors.push_back(minPos(Vector3(400, 110, -50), 100, 100, 15));
	minVectors.push_back(minPos(Vector3(400, 110, 150), 100, 100, 15));
	minVectors.push_back(minPos(Vector3(300, 110, -50), 100, 100, 15));
	minVectors.push_back(minPos(Vector3(300, 110, 150), 100, 100, 15));
	minVectors.push_back(minPos(Vector3(400, 160, 0), 100, 15, 100));
	minVectors.push_back(minPos(Vector3(400, 160, 100), 100, 15, 100));
	minVectors.push_back(minPos(Vector3(300, 160, 0), 100, 15, 100));
	minVectors.push_back(minPos(Vector3(300, 160, 100), 100, 15, 100));*/
	minVectors.push_back(minPos(Vector3(-338, 0, 38), 20, 120, 90));
	minVectors.push_back(minPos(Vector3(-460, 0, 38), 20, 120, 90));
	minVectors.push_back(minPos(Vector3(-400, 0, 73), 120, 120, 20));
	minVectors.push_back(minPos(Vector3(-360, 0, 0), 70, 100, 30));
	minVectors.push_back(minPos(Vector3(-435, 0, 0), 58, 120, 20));

	/*maxVectors.push_back(maxPos(Vector3(400, 60, 0), 100, 20, 100));
	maxVectors.push_back(maxPos(Vector3(400, 60, 100), 100, 20, 100));
	maxVectors.push_back(maxPos(Vector3(300, 60, 0), 100, 20, 100));
	maxVectors.push_back(maxPos(Vector3(300, 60, 100), 100, 20, 100));
	maxVectors.push_back(maxPos(Vector3(450, 110, 0), 15, 100, 100));
	maxVectors.push_back(maxPos(Vector3(450, 110, 100), 15, 100, 100));
	maxVectors.push_back(maxPos(Vector3(250, 110, 0), 15, 100, 100));
	maxVectors.push_back(maxPos(Vector3(250, 110, 100), 15, 100, 100));
	maxVectors.push_back(maxPos(Vector3(400, 110, -50), 100, 100, 15));
	maxVectors.push_back(maxPos(Vector3(400, 110, 150), 100, 100, 15));
	maxVectors.push_back(maxPos(Vector3(300, 110, -50), 100, 100, 15));
	maxVectors.push_back(maxPos(Vector3(300, 110, 150), 100, 100, 15));
	maxVectors.push_back(maxPos(Vector3(400, 160, 0), 100, 15, 100));
	maxVectors.push_back(maxPos(Vector3(400, 160, 100), 100, 15, 100));
	maxVectors.push_back(maxPos(Vector3(300, 160, 0), 100, 15, 100));
	maxVectors.push_back(maxPos(Vector3(300, 160, 100), 100, 15, 100));*/
	maxVectors.push_back(maxPos(Vector3(-338, 0, 38), 20, 120, 90));
	maxVectors.push_back(maxPos(Vector3(-460, 0, 38), 20, 120, 90));
	maxVectors.push_back(maxPos(Vector3(-400, 0, 73), 120, 120, 20));
	maxVectors.push_back(maxPos(Vector3(-360, 0, 0), 70, 100, 30));
	maxVectors.push_back(maxPos(Vector3(-435, 0, 0), 52, 120, 20));	


	//mouseSpeed = 0.005f;
}

void Camera3::Update(double dt)
{

	static const float CAMERA_SPEED = 500.f;
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
	//std::s << xpos << " " << ypos << std::endl;
		if (mouseControl == true && Application::IsKeyPressed(VK_RBUTTON))
		{
			xpos, ypos = a.Mouse(xpos, ypos);

			if (xpos < 397)
			{
				directionRotation.y += 1;
			}
			if (xpos > 403)
			{
				directionRotation.y -= 1;
			}
			if (ypos < 297)
			{
				if (directionRotation.x > -40)
				{
					directionRotation.x -= 1;
				}
			}
			if (ypos > 303)
			{
				if (directionRotation.x < 40)
				{
					directionRotation.x += 1;
				}
			}
		}
		else if (mouseControl == true)
		{
			xpos, ypos = a.Mouse(xpos, ypos);

			if (xpos < 397)
			{
				directionRotation.y += 2;
			}
			if (xpos > 403)
			{
				directionRotation.y -= 2;
			}
			if (ypos < 297)
			{
				if (directionRotation.x > -40)
				{
					directionRotation.x -= 2;
				}
			}
			if (ypos > 303)
			{
				if (directionRotation.x < 40)
				{
					directionRotation.x += 2;
				}
			}
		}
		else
		{
			if (Application::IsKeyPressed(VK_LEFT))
			{
				directionRotation.y += 3;
			}
			if (Application::IsKeyPressed(VK_RIGHT))
			{
				directionRotation.y -= 3;
			}
			if (Application::IsKeyPressed(VK_UP))
			{
				if (directionRotation.x > -45)
				{
					directionRotation.x -= 3;
				}
			}
			if (Application::IsKeyPressed(VK_DOWN))
			{
				if (directionRotation.x < 45)
				{
					directionRotation.x += 3;
				}
			}
		}
		if (recoil > 20)
		{
			recoil = 20;
		}
		gunRecoil.x = recoil;
		if (Application::IsKeyPressed(VK_LBUTTON))
		{
			cRecoilCd = 20;
		}
		if (cRecoilCd > 0)
		{
			cRecoilCd--;
		}
		if (gunRecoil.x > 0 && cRecoilCd == 0)
		{
			gunRecoil.x -= 0.2;
		}
		if (recoil > 0 && cRecoilCd == 0)
		{
			recoil -= 0.2;
		}

		camerarotation = directionRotation - gunRecoil;

		if (stamina < 100 && staminaDelay == 0)
		{
			stamina++;
		}

		if (staminaDelay > 0)
		{
			staminaDelay--;
		}

		location2 = position + view*dt*speed;
		location = position;
		//direction2 = view * dt * speed;
		direction = (view*dt*speed) * 2;
		if (inBase == true)
		{
			if (Application::IsKeyPressed('W'))
			{
				Vector3 view = (target - position).Normalized();
				if (position.x + (view.x * dt * speed) / 2 <= 48 && position.x + (view.x * dt * speed) / 2 >= -48 && hitbox((position.x * dt * speed) / 2) == false && checkcollisionwithObject((view.x * dt * speed) / 2) == false)
				{
					position.x += (view.x * dt * speed) / 2;
					target.x += (view.x * dt * speed) / 2;
				}
				if (position.z + (view.z * dt * speed) / 2 <= 48 && position.z + (view.z * dt * speed) / 2 >= -48 && hitbox((view.z * dt * speed) / 2) == false && checkcollisionwithObject((view.z * dt * speed) / 2) == false)
				{
					position.z += (view.z * dt * speed) / 2;
					target.z += (view.z * dt * speed) / 2;
				}
				if (Application::IsKeyPressed(VK_SPACE) && position.y + (view.y * dt * speed) / 2 <= 48 && position.y + (view.y * dt * speed) / 2 >= 10 && hitbox((position.y * dt * speed) / 2) == false && checkcollisionwithObject((view.y * dt * speed) / 2) == false)
				{
					position.y += (view.y * dt * speed) / 2;
					target.y += (view.y * dt * speed) / 2;
				}
			}
			if (Application::IsKeyPressed('S'))
			{
				Vector3 view = (target - position).Normalized();

				if (position.x - (view.x * dt * speed) / 2 <= 48 && position.x - (view.x * dt * speed) / 2 >= -48 && hitbox(-(view.x * dt * speed) / 2) == false && checkcollisionwithObject(-(view.x * dt * speed) / 2) == false)
				{
					position.x -= (view.x * dt * speed) / 2;
					target.x -= (view.x * dt * speed) / 2;
				}

				if (position.z - (view.z * dt * speed) / 2 <= 48 && position.z - (view.z * dt * speed) / 2 >= -48 && hitbox(-(view.z * dt * speed) / 2) == false && checkcollisionwithObject(-(view.z * dt * speed) / 2) == false)
				{
					position.z -= (view.z * dt * speed) / 2;
					target.z -= (view.z * dt * speed) / 2;
				}

				if (Application::IsKeyPressed(VK_SPACE) && position.y - (view.y * dt * speed) / 2 <= 48 && position.y - (view.y * dt * speed) / 2 >= 10 && hitbox(-(view.y * dt * speed) / 2) == false && checkcollisionwithObject(-(view.y * dt * speed) / 2) == false)
				{
					position.y -= (view.y * dt * speed) / 2;
					target.y -= (view.y * dt * speed) / 2;
				}
			}
			if (Application::IsKeyPressed('D'))
			{
				Vector3 view = (target - position).Normalized();
				Vector3 right = view.Cross(up);

				if (position.x + (right.x * dt * speed) / 2 <= 48 && position.x + (right.x * dt * speed) / 2 >= -48 && hitbox((right.x * dt * speed) / 2) == false && checkcollisionwithObject((right.x * dt * speed) / 2) == false)
				{
					position.x += (right.x * dt * speed) / 2;
					target.x += (right.x * dt * speed) / 2;
				}

				if (position.z + (right.z * dt * speed) / 2 <= 48 && position.z + (right.z * dt * speed) / 2 >= -48 && hitbox((right.z * dt * speed) / 2) == false && checkcollisionwithObject((right.z * dt * speed) / 2) == false)
				{
					position.z += (right.z * dt * speed) / 2;
					target.z += (right.z * dt * speed) / 2;
				}

				if (position.y - (right.y * dt * speed) / 2 <= 48 && position.y - (right.y * dt * speed) / 2 >= -48 && hitbox(-(right.y * dt * speed) / 2) == false && checkcollisionwithObject((right.y * dt * speed) / 2) == false)
				{
					position.z -= (right.y * dt * speed) / 2;
					target.z -= (right.y * dt * speed) / 2;
				}
			}
			if (Application::IsKeyPressed('A'))
			{
				Vector3 view = (target - position).Normalized();
				Vector3 right = view.Cross(up);

				if (position.x - (right.x * dt * speed) / 2 <= 48 && position.x - (right.x * dt * speed) / 2 >= -48 && hitbox(-(right.x * dt * speed) / 2) == false && checkcollisionwithObject(-(right.x * dt * speed) / 2) == false)
				{
					position.x -= (right.x * dt * speed) / 2;
					target.x -= (right.x * dt * speed) / 2;
				}

				if (position.z - (right.z * dt * speed) / 2 <= 48 && position.z - (right.z * dt * speed) / 2 >= -48 && hitbox(-(right.z * dt * speed) / 2) == false && checkcollisionwithObject(-(right.z * dt * speed) / 2) == false)
				{
					position.z -= (right.z * dt * speed) / 2;
					target.z -= (right.z * dt * speed) / 2;
				}


				//if (position.y - (right.y * dt * speed) / 2 <= 900 && position.y - (right.y * dt * speed) / 2 >= -900 && hitbox(-(right.y * dt * speed) / 2) == false)
				if (position.y - (right.y * dt * speed) / 2 <= 48 && position.y - (right.y * dt * speed) / 2 >= -48 && hitbox(-(right.y * dt * speed) / 2) == false && checkcollisionwithObject((right.y * dt * speed) / 2) == false)
				{
					position.z -= (right.y * dt * speed) / 2;
					target.z -= (right.y * dt * speed) / 2;
				}
			}
		}
		else if (inBase == false)
		{

			if (Application::IsKeyPressed(VK_RBUTTON))
			{
				if (Application::IsKeyPressed('W'))
				{
					Vector3 view = (target - position).Normalized();
					if (position.x + (view.x * dt * speed) / 5 <= 990 && position.x + (view.x * dt * speed) / 5 >= -990 && hitbox((position.x * dt * speed) / 5) == false && checkcollisionwithObject((view.x * dt * speed) / 5) == false)
					{
						position.x += (view.x * dt * speed) / 5;
						target.x += (view.x * dt * speed) / 5;

					}

					if (position.z + (view.z * dt * speed) / 5 <= 990 && position.z + (view.z * dt * speed) / 5 >= -990 && hitbox((view.z * dt * speed) / 5) == false && checkcollisionwithObject((view.z * dt * speed) / 5) == false)
					{
						position.z += (view.z * dt * speed) / 5;
						target.z += (view.z * dt * speed) / 5;
					}

					/*if (Application::IsKeyPressed(VK_SPACE) && position.y + (view.y * dt * speed) / 5 <= 990 && position.y + (view.y * dt * speed) / 5 >= 10 && hitbox((position.y * dt * speed) / 5) == false && checkcollisionwithObject((view.y * dt * speed) / 5) == false && stamina > 0)
					{
						position.y += (view.y * dt * speed) / 5;
						target.y += (view.y * dt * speed) / 5;
						stamina--;
						staminaDelay = 100;
					}*/
				}
				if (Application::IsKeyPressed('S'))
				{
					Vector3 view = (target - position).Normalized();

					if (position.x - (view.x * dt * speed) / 5 <= 990 && position.x - (view.x * dt * speed) / 5 >= -990 && hitbox(-(view.x * dt * speed) / 5) == false && checkcollisionwithObject(-(view.x * dt * speed) / 5) == false)
					{
						position.x -= (view.x * dt * speed) / 5;
						target.x -= (view.x * dt * speed) / 5;
					}

					if (position.z - (view.z * dt * speed) / 5 <= 990 && position.z - (view.z * dt * speed) / 5 >= -990 && hitbox(-(view.z * dt * speed) / 5) == false && checkcollisionwithObject(-(view.z * dt * speed) / 5) == false)
					{
						position.z -= (view.z * dt * speed) / 5;
						target.z -= (view.z * dt * speed) / 5;
					}

					/*if (Application::IsKeyPressed(VK_SPACE) && position.y - (view.y * dt * speed) / 5 <= 990 && position.y - (view.y * dt * speed) / 5 >= 10 && hitbox(-(view.y * dt * speed) / 5) == false && checkcollisionwithObject(-(view.y * dt * speed) / 5) == false && stamina > 0)
					{
						position.y -= (view.y * dt * speed) / 5;
						target.y -= (view.y * dt * speed) / 5;
						stamina--;
						staminaDelay = 100;
					}*/
				}
				if (Application::IsKeyPressed('D'))
				{
					Vector3 view = (target - position).Normalized();
					Vector3 right = view.Cross(up);

					if (position.x + (right.x * dt * speed) / 5 <= 990 && position.x + (right.x * dt * speed) / 5 >= -990 && hitbox((right.x * dt * speed) / 5) == false && checkcollisionwithObject((right.x * dt * speed) / 5) == false)
					{
						position.x += (right.x * dt * speed) / 5;
						target.x += (right.x * dt * speed) / 5;
					}

					if (position.z + (right.z * dt * speed) / 5 <= 990 && position.z + (right.z * dt * speed) / 5 >= -990 && hitbox((right.z * dt * speed) / 5) == false && checkcollisionwithObject((right.z * dt * speed) / 5) == false)
					{
						position.z += (right.z * dt * speed) / 5;
						target.z += (right.z * dt * speed) / 5;
					}

					if (position.y - (right.y * dt * speed) / 5 <= 990 && position.y - (right.y * dt * speed) / 5 >= -990 && hitbox(-(right.y * dt * speed) / 5) == false && checkcollisionwithObject((right.y * dt * speed) / 5) == false)
					{
						position.z -= (right.y * dt * speed) / 5;
						target.z -= (right.y * dt * speed) / 5;
					}
				}
				if (Application::IsKeyPressed('A'))
				{
					Vector3 view = (target - position).Normalized();
					Vector3 right = view.Cross(up);

					if (position.x - (right.x * dt * speed) / 5 <= 990 && position.x - (right.x * dt * speed) / 5 >= -990 && hitbox(-(right.x * dt * speed) / 5) == false && checkcollisionwithObject(-(right.x * dt * speed) / 5) == false)
					{
						position.x -= (right.x * dt * speed) / 5;
						target.x -= (right.x * dt * speed) / 5;
					}
					if (position.z - (right.z * dt * speed) / 5 <= 990 && position.z - (right.z * dt * speed) / 5 >= -990 && hitbox(-(right.z * dt * speed) / 5) == false && checkcollisionwithObject(-(right.z * dt * speed) / 5) == false)
					{
						position.z -= (right.z * dt * speed) / 5;
						target.z -= (right.z * dt * speed) / 5;
					}
					if (position.y - (right.y * dt * speed) / 5 <= 990 && position.y - (right.y * dt * speed) / 5 >= -990 && hitbox(-(right.y * dt * speed) / 5) == false && checkcollisionwithObject(-(right.y * dt * speed) / 5) == false)
					{
						position.z -= (right.y * dt * speed) / 5;
						target.z -= (right.y * dt * speed) / 5;
					}
				}
			}
			else if (Application::IsKeyPressed(VK_SHIFT) && stamina > 0)
			{
				if (Application::IsKeyPressed('W'))
				{
					Vector3 view = (target - position).Normalized();
					if (position.x + (view.x * dt * speed) <= 990 && position.x + (view.x * dt * speed) >= -990 && hitbox((position.x * dt * speed)) == false && checkcollisionwithObject((view.x * dt * speed)) == false)
					{
						position.x += (view.x * dt * speed);
						target.x += (view.x * dt * speed);
					}
					if (position.z + (view.z * dt * speed) <= 990 && position.z + (view.z * dt * speed) >= -990 && hitbox((view.z * dt * speed)) == false && checkcollisionwithObject((view.z * dt * speed)) == false)
					{
						position.z += (view.z * dt * speed);
						target.z += (view.z * dt * speed);
					}
					/*if (Application::IsKeyPressed(VK_SPACE) && position.y + (view.y * dt * speed) <= 990 && position.y + (view.y * dt * speed) >= 10 && hitbox((position.y * dt * speed)) == false && checkcollisionwithObject((view.y * dt * speed)) == false)
					{
						position.y += (view.y * dt * speed);
						target.y += (view.y * dt * speed);
					}*/
				}
				if (Application::IsKeyPressed('S'))
				{
					Vector3 view = (target - position).Normalized();
					if (position.x - (view.x * dt * speed) <= 990 && position.x - (view.x * dt * speed) >= -990 && hitbox(-(view.x * dt * speed)) == false && checkcollisionwithObject(-(view.x * dt * speed)) == false)
					{
						position.x -= (view.x * dt * speed);
						target.x -= (view.x * dt * speed);
					}
					if (position.z - (view.z * dt * speed) <= 990 && position.z - (view.z * dt * speed) >= -990 && hitbox(-(view.z * dt * speed)) == false && checkcollisionwithObject(-(view.z * dt * speed)) == false)
					{
						position.z -= (view.z * dt * speed);
						target.z -= (view.z * dt * speed);
					}
					/*if (Application::IsKeyPressed(VK_SPACE) && position.y - (view.y * dt * speed) <= 990 && position.y - (view.y * dt * speed) >= 10 && hitbox(-(view.y * dt * speed)) == false && checkcollisionwithObject(-(view.y * dt * speed)) == false)
					{
						position.y -= (view.y * dt * speed);
						target.y -= (view.y * dt * speed);
					}*/
				}
				if (Application::IsKeyPressed('D'))
				{
					Vector3 view = (target - position).Normalized();
					Vector3 right = view.Cross(up);
					if (position.x + (right.x * dt * speed) <= 990 && position.x + (right.x * dt * speed) >= -990 && hitbox((right.x * dt * speed)) == false && checkcollisionwithObject((right.x * dt * speed)) == false)
					{
						position.x += (right.x * dt * speed);
						target.x += (right.x * dt * speed);
					}
					if (position.z + (right.z * dt * speed) <= 990 && position.z + (right.z * dt * speed) >= -990 && hitbox((right.z * dt * speed)) == false && checkcollisionwithObject((right.z * dt * speed)) == false)
					{
						position.z += (right.z * dt * speed);
						target.z += (right.z * dt * speed);
					}
					if (position.y - (right.y * dt * speed) <= 990 && position.y - (right.y * dt * speed) >= -990 && hitbox(-(right.y * dt * speed)) == false && checkcollisionwithObject((right.y * dt * speed)) == false)
					{
						position.z -= (right.y * dt * speed);
						target.z -= (right.y * dt * speed);
					}
				}
				if (Application::IsKeyPressed('A'))
				{
					Vector3 view = (target - position).Normalized();
					Vector3 right = view.Cross(up);
					if (position.x - (right.x * dt * speed) <= 990 && position.x - (right.x * dt * speed) >= -990 && hitbox(-(right.x * dt * speed)) == false && checkcollisionwithObject(-(right.x * dt * speed)) == false)
					{
						position.x -= (right.x * dt * speed);
						target.x -= (right.x * dt * speed);
					}
					if (position.z - (right.z * dt * speed) <= 990 && position.z - (right.z * dt * speed) >= -990 && hitbox(-(right.z * dt * speed)) == false && checkcollisionwithObject(-(right.z * dt * speed)) == false)
					{
						position.z -= (right.z * dt * speed);
						target.z -= (right.z * dt * speed);
					}
					if (position.y - (right.y * dt * speed) <= 990 && position.y - (right.y * dt * speed) >= -990 && hitbox(-(right.y * dt * speed)) == false && checkcollisionwithObject((right.y * dt * speed)) == false)
					{
						position.z -= (right.y * dt * speed);
						target.z -= (right.y * dt * speed);
					}
				}
				stamina--;
				staminaDelay = 100;
			}
			else
			{
				if (Application::IsKeyPressed('W'))
				{
					Vector3 view = (target - position).Normalized();
					if (position.x + (view.x * dt * speed) / 2 <= 990 && position.x + (view.x * dt * speed) / 2 >= -990 && hitbox((position.x * dt * speed) / 2) == false && checkcollisionwithObject((view.x * dt * speed) / 2) == false)
					{
						position.x += (view.x * dt * speed) / 2;
						target.x += (view.x * dt * speed) / 2;
					}
					if (position.z + (view.z * dt * speed) / 2 <= 990 && position.z + (view.z * dt * speed) / 2 >= -990 && hitbox((view.z * dt * speed) / 2) == false && checkcollisionwithObject((view.z * dt * speed) / 2) == false)
					{
						position.z += (view.z * dt * speed) / 2;
						target.z += (view.z * dt * speed) / 2;
					}
					/*if (Application::IsKeyPressed(VK_SPACE) && position.y + (view.y * dt * speed) / 2 <= 990 && position.y + (view.y * dt * speed) / 2 >= 10 && hitbox((position.y * dt * speed) / 2) == false && checkcollisionwithObject((view.y * dt * speed) / 2) == false && stamina > 0)
					{
						position.y += (view.y * dt * speed) / 2;
						target.y += (view.y * dt * speed) / 2;
						stamina--;
						staminaDelay = 100;
					}*/
				}
				if (Application::IsKeyPressed('S'))
				{
					Vector3 view = (target - position).Normalized();

					if (position.x - (view.x * dt * speed) / 2 <= 990 && position.x - (view.x * dt * speed) / 2 >= -990 && hitbox(-(view.x * dt * speed) / 2) == false && checkcollisionwithObject(-(view.x * dt * speed) / 2) == false)
					{
						position.x -= (view.x * dt * speed) / 2;
						target.x -= (view.x * dt * speed) / 2;
					}

					if (position.z - (view.z * dt * speed) / 2 <= 990 && position.z - (view.z * dt * speed) / 2 >= -990 && hitbox(-(view.z * dt * speed) / 2) == false && checkcollisionwithObject(-(view.z * dt * speed) / 2) == false)
					{
						position.z -= (view.z * dt * speed) / 2;
						target.z -= (view.z * dt * speed) / 2;
					}

					/*if (Application::IsKeyPressed(VK_SPACE) && position.y - (view.y * dt * speed) / 2 <= 990 && position.y - (view.y * dt * speed) / 2 >= 10 && hitbox(-(view.y * dt * speed) / 2) == false && checkcollisionwithObject(-(view.y * dt * speed) / 2) == false && stamina > 0)
					{
						position.y -= (view.y * dt * speed) / 2;
						target.y -= (view.y * dt * speed) / 2;
						stamina--;
						staminaDelay = 100;
					}*/
				}
				if (Application::IsKeyPressed('D'))
				{
					Vector3 view = (target - position).Normalized();
					Vector3 right = view.Cross(up);

					if (position.x + (right.x * dt * speed) / 2 <= 990 && position.x + (right.x * dt * speed) / 2 >= -990 && hitbox((right.x * dt * speed) / 2) == false && checkcollisionwithObject((right.x * dt * speed) / 2) == false)
					{
						position.x += (right.x * dt * speed) / 2;
						target.x += (right.x * dt * speed) / 2;
					}

					if (position.z + (right.z * dt * speed) / 2 <= 990 && position.z + (right.z * dt * speed) / 2 >= -990 && hitbox((right.z * dt * speed) / 2) == false && checkcollisionwithObject((right.z * dt * speed) / 2) == false)
					{
						position.z += (right.z * dt * speed) / 2;
						target.z += (right.z * dt * speed) / 2;
					}

					if (position.y - (right.y * dt * speed) / 2 <= 990 && position.y - (right.y * dt * speed) / 2 >= -990 && hitbox(-(right.y * dt * speed) / 2) == false && checkcollisionwithObject((right.y * dt * speed) / 2) == false)
					{
						position.z -= (right.y * dt * speed) / 2;
						target.z -= (right.y * dt * speed) / 2;
					}
				}
				if (Application::IsKeyPressed('A'))
				{
					Vector3 view = (target - position).Normalized();
					Vector3 right = view.Cross(up);

					if (position.x - (right.x * dt * speed) / 2 <= 990 && position.x - (right.x * dt * speed) / 2 >= -990 && hitbox(-(right.x * dt * speed) / 2) == false && checkcollisionwithObject(-(right.x * dt * speed) / 2) == false)
					{
						position.x -= (right.x * dt * speed) / 2;
						target.x -= (right.x * dt * speed) / 2;
					}

					if (position.z - (right.z * dt * speed) / 2 <= 990 && position.z - (right.z * dt * speed) / 2 >= -990 && hitbox(-(right.z * dt * speed) / 2) == false && checkcollisionwithObject(-(right.z * dt * speed) / 2) == false)
					{
						position.z -= (right.z * dt * speed) / 2;
						target.z -= (right.z * dt * speed) / 2;
					}

					if (position.y - (right.y * dt * speed) / 2 <= 990 && position.y - (right.y * dt * speed) / 2 >= -990 && hitbox(-(right.y * dt * speed) / 2) == false && checkcollisionwithObject((right.y * dt * speed) / 2) == false)
					{
						position.z -= (right.y * dt * speed) / 2;
						target.z -= (right.y * dt * speed) / 2;
					}
				}
			}
		}
		if (knockback == true)
		{
			knockback = false;
			if (position.x - (view.x * dt * speed)*10 <= 990 && position.x - (view.x * dt * speed)*10 >= -990 && hitbox(-(view.x * dt * speed)*10) == false && checkcollisionwithObject(-(view.x * dt * speed)*10) == false)
			{
				position.x -= (view.x * dt * speed) * 10;
			}
			if (position.z - (view.z * dt * speed) * 10 <= 990 && position.z - (view.z * dt * speed) * 10 >= -990 && hitbox(-(view.z * dt * speed) * 10) == false && checkcollisionwithObject(-(view.z * dt * speed) * 10) == false)
			{
				position.z -= (view.z * dt * speed) * 10;
			}
		}
	teleport();
	kb = (right * dt * speed);
	if (camerarotation.x > maxCameraX)
	{
		camerarotation.x = maxCameraX;
	}
	else if (camerarotation.x < -maxCameraX)
	{
		camerarotation.x = -maxCameraX;
	}

	//Changing target
	target = Vector3(sin(DegreeToRadian(camerarotation.y))*cos(DegreeToRadian(camerarotation.x)) + this->position.x, -sin(DegreeToRadian(camerarotation.x)) + this->position.y,
		cos(DegreeToRadian(camerarotation.y))*cos(DegreeToRadian(camerarotation.x)) + this->position.z);

}

Vector3 Camera3::setPos()
{
	location2 = position;
	return location;
}

bool Camera3::hitbox(double move)
{
	/*for (int i = 0; i <= 50; i++)
	{
		if (testhitargetLocation(Vector3(treex[i] - 20, -150, treez[i] - 20), Vector3(treex[i] + 20, 100, treez[i] + 20), move) == true)
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

bool Camera3::checkcollisionwithObject(double move)
{
	//to do
	// Get all min and max (eg. minX, maxX)

	// Do SAT Test on all axis)
	// eg (if (position.x < min) return false; // (Outside!)
	//		Repeat for Max, Repeat Min + Max for Y and Z axis

	std::vector<Vector3>::iterator count = minVectors.begin();
	std::vector<Vector3>::iterator count1 = maxVectors.begin();

	while (count != minVectors.end())
	{
		Vector3 minTemp = *count;
		Vector3 maxTemp = *count1;

		if (position.x + move > minTemp.x && position.x + move < maxTemp.x && position.y + move > minTemp.y && position.y + move < maxTemp.y && position.z + move > minTemp.z && position.z + move < maxTemp.z)
		{
			return true;
		}
		*count++;
		*count1++;
	}
	return false;
}

void Camera3::teleport()
{
	Vector3 p1Min = minPos(Vector3(-435.088, 10, 50.0353), 10, 15, 10);
	Vector3 p1Max = maxPos(Vector3(-435.088, 10, 50.0353), 10, 15, 10);

	Vector3 p2Min = minPos(Vector3(0,10,40), 10, 15, 10);
	Vector3 p2Max = maxPos(Vector3(0,10, 40), 10, 15, 10);

	if (position.x > p1Min.x && position.y > p1Min.y && position.z > p1Min.z
		&& position.x < p1Max.x && position.y  < p1Max.y && position.z < p1Max.z)
	{
		position.x = 0;
		position.y = 10; 
		position.z = 0;
		inBase = true;
	}

	if (position.x > p2Min.x && position.y > p2Min.y && position.z > p2Min.z
		&& position.x < p2Max.x && position.y < p2Max.y && position.z < p2Max.z)
	{
		position.x = -400; 
		position.y = 10; 
		position.z = 1;
		inBase = false;
	}
}

bool Camera3::craftUi()
{
	Vector3 min = minPos(Vector3(0,10, -35), 15, 15, 15);
	Vector3 max = maxPos(Vector3(0,10,-35), 15, 15, 15);

	if (position.x > min.x && position.y > min.y && position.z > min.z
		&& position.x < max.x && position.y < max.y && position.z < max.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vector3 Camera3::minPos(Vector3 center, double sizeX, double sizeY, double sizeZ)
{
	return Vector3(center.x - (sizeX / 2), center.y - (sizeY / 2), center.z - (sizeZ / 2));
}

Vector3 Camera3::maxPos(Vector3 center, double sizeX, double sizeY, double sizeZ)
{
	return Vector3(center.x + (sizeX / 2), center.y + (sizeY / 2), center.z + (sizeZ / 2));
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}