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

	///for hitboxes///
	minVectors.push_back(minPos(Vector3(400, 60, 0), 100, 20, 100));
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
	minVectors.push_back(minPos(Vector3(300, 160, 100), 100, 15, 100));
	minVectors.push_back(minPos(Vector3(-338, 0, 38), 20, 120, 90));
	minVectors.push_back(minPos(Vector3(-460, 0, 38), 20, 120, 90));
	minVectors.push_back(minPos(Vector3(-400, 0, 73), 120, 120, 20));
	minVectors.push_back(minPos(Vector3(-360, 0, 0), 58, 120, 20));
	minVectors.push_back(minPos(Vector3(-435, 0, 0), 58, 120, 20));

	maxVectors.push_back(maxPos(Vector3(400, 60, 0), 100, 20, 100));
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
	maxVectors.push_back(maxPos(Vector3(300, 160, 100), 100, 15, 100));
	maxVectors.push_back(maxPos(Vector3(-338, 0, 38), 20, 120, 90));
	maxVectors.push_back(maxPos(Vector3(-460, 0, 38), 20, 120, 90));
	maxVectors.push_back(maxPos(Vector3(-400, 0, 73), 120, 120, 20));
	maxVectors.push_back(maxPos(Vector3(-360, 0, 0), 52, 120, 20));
	maxVectors.push_back(maxPos(Vector3(-435, 0, 0), 52, 120, 20));	

	//Minerals hitbox
	minMineralVectors.push_back(minPos(Vector3(-291, 10, -39), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-349, 10, 124), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-460, 10, -357), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-527, 10, -109), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-675, 10, -782), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-237, 10, -230), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-125, 10, -679), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-430, 10, -258), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-325, 10, 61), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(20, 10, -736), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-480, 10, -704), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-466, 10, -322), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-321, 10, -505), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-824, 10, -338), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-478, 10, -298), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-529, 10, -383), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(64, 10, -633), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-352, 10, -694), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-310, 10, -300), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-158, 10, -409), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-327, 10, -271), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-88, 10, -426), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(91, 10, -775), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-256, 10, -420), 10, 10, 10));
	minMineralVectors.push_back(minPos(Vector3(-554, 10, -504), 10, 10, 10));

	maxMineralVectors.push_back(maxPos(Vector3(-291, 10, -39), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-349, 10, 124), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-460, 10, -357), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-527, 10, -109), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-675, 10, -782), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-237, 10, -230), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-125, 10, -679), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-430, 10, -258), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-325, 10, 61), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(20, 10, -736), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-480, 10, -704), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-466, 10, -322), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-321, 10, -505), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-824, 10, -338), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-478, 10, -298), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-529, 10, -383), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(64, 10, -633), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-352, 10, -694), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-310, 10, -300), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-158, 10, -409), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-327, 10, -271), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-88, 10, -426), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(91, 10, -775), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-256, 10, -420), 14, 10, 10));
	maxMineralVectors.push_back(maxPos(Vector3(-554, 10, -504), 14, 10, 10));

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
	//std::cout << xpos << " " << ypos << std::endl;
	if (mouseControl == true && Application::IsKeyPressed(VK_RBUTTON))
	{
		xpos, ypos = a.Mouse(xpos, ypos);

		if (xpos < 395)
		{
			//rotation.SetToRotation(3, up.x, up.y, up.z);
			//view = rotation * view;
			//target = position + view;
			directionRotation.y += 1.5;
		}
		if (xpos > 405)
		{
			//rotation.SetToRotation(-3, up.x, up.y, up.z);
			//view = rotation * view;
			//target = position + view;
			directionRotation.y -= 1.5;
		}
		if (ypos < 295)
		{
			if (directionRotation.x > -45)
			{
				//rotation.SetToRotation(2, right.x, right.y, right.z);
				//view = rotation * view;
				//target = position + view;
				directionRotation.x -= 1.5;
			}
		}
		if (ypos > 305)
		{
			if (directionRotation.x < 45)
			{
				//rotation.SetToRotation(-2, right.x, right.y, right.z);
				//view = rotation * view;
				//target = position + view;
				directionRotation.x += 1.5;
			}
		}
	}
	else if (mouseControl == true)
	{
		xpos, ypos = a.Mouse(xpos, ypos);

		if (xpos < 395)
		{
			//rotation.SetToRotation(3, up.x, up.y, up.z);
			//view = rotation * view;
			//target = position + view;
			directionRotation.y += 3;
		}
		if (xpos > 405)
		{
			//rotation.SetToRotation(-3, up.x, up.y, up.z);
			//view = rotation * view;
			//target = position + view;
			directionRotation.y -= 3;
		}
		if (ypos < 295)
		{
			if (directionRotation.x > -45)
			{
				//rotation.SetToRotation(2, right.x, right.y, right.z);
				//view = rotation * view;
				//target = position + view;
				directionRotation.x -= 3;
			}
		}
		if (ypos > 305)
		{
			if (directionRotation.x < 45)
			{
				//rotation.SetToRotation(-2, right.x, right.y, right.z);
				//view = rotation * view;
				//target = position + view;
				directionRotation.x += 3;
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
			directionRotation.y += 3;
		}
		if (Application::IsKeyPressed(VK_RIGHT))
		{
			//rotation.SetToRotation(-2, up.x, up.y, up.z);
			//view = rotation * view;
			//target = position + view;
			directionRotation.y -= 3;
		}
		if (Application::IsKeyPressed(VK_UP))
		{

			/*if (view.y < 0.5)
			{
				rotation.SetToRotation(1.5, right.x, right.y, right.z);
				view = rotation * view;
				target = position + view;
			}*/
			if (directionRotation.x > -45)
			{
				directionRotation.x -= 3;
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
	//direction2 = view * dt * speed;
	direction = (view*dt*speed)*2;

	if (Application::IsKeyPressed(VK_SHIFT))
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
			if (Application::IsKeyPressed(VK_SPACE) && position.y + (view.y * dt * speed) <= 990 && position.y + (view.y * dt * speed) >= 10 && hitbox((position.y * dt * speed)) == false && checkcollisionwithObject((view.y * dt * speed)) == false)
			{
				position.y += (view.y * dt * speed);
				target.y += (view.y * dt * speed);
			}
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
			if (Application::IsKeyPressed(VK_SPACE) && position.y - (view.y * dt * speed) <= 990 && position.y - (view.y * dt * speed) >= 10 && hitbox(-(view.y * dt * speed)) == false && checkcollisionwithObject(-(view.y * dt * speed)) == false)
			{
				position.y -= (view.y * dt * speed);
				target.y -= (view.y * dt * speed);
			}
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
	}
	else if (Application::IsKeyPressed(VK_RBUTTON))
	{
		if (Application::IsKeyPressed('W'))
		{
			Vector3 view = (target - position).Normalized();
			if (position.x + (view.x * dt * speed) / 5 <= 990 && position.x + (view.x * dt * speed) / 5 >= -990 && hitbox((position.x * dt * speed) / 5) == false && checkcollisionwithObject((view.x * dt * speed) / 5) == false)

			if (position.x + (view.x * dt * speed) / 5 <= 900 && position.x + (view.x * dt * speed) / 5 >= -900 && hitbox((position.x * dt * speed) / 5) == false)

			{
				position.x += (view.x * dt * speed) / 5;
				target.x += (view.x * dt * speed) / 5;

			}

			if (position.z + (view.z * dt * speed) / 5 <= 990 && position.z + (view.z * dt * speed) / 5 >= -990 && hitbox((view.z * dt * speed) / 5) == false && checkcollisionwithObject((view.z * dt * speed) / 5) == false)

			if (position.z + (view.z * dt * speed) / 5 <= 900 && position.z + (view.z * dt * speed) / 5 >= -900 && hitbox((view.z * dt * speed) / 5) == false)

			{
				position.z += (view.z * dt * speed) / 5;
				target.z += (view.z * dt * speed) / 5;
			}

			if (Application::IsKeyPressed(VK_SPACE) && position.y + (view.y * dt * speed) / 5 <= 990 && position.y + (view.y * dt * speed) / 5 >= 10 && hitbox((position.y * dt * speed) / 5) == false && checkcollisionwithObject((view.y * dt * speed) / 5) == false)

			if (Application::IsKeyPressed(VK_SHIFT) && position.y + (view.y * dt * speed) / 5 <= 900 && position.y + (view.y * dt * speed) / 5 >= 10 && hitbox((position.y * dt * speed) / 5) == false)

			{
				position.y += (view.y * dt * speed) / 5;
				target.y += (view.y * dt * speed) / 5;
			}
		}
		if (Application::IsKeyPressed('S'))
		{
			Vector3 view = (target - position).Normalized();

			if (position.x - (view.x * dt * speed) / 5 <= 990 && position.x - (view.x * dt * speed) / 5 >= -990 && hitbox(-(view.x * dt * speed) / 5) == false && checkcollisionwithObject(-(view.x * dt * speed) / 5) == false)

			if (position.x - (view.x * dt * speed) / 5 <= 900 && position.x - (view.x * dt * speed) / 5 >= -900 && hitbox(-(view.x * dt * speed) / 5) == false)

			{
				position.x -= (view.x * dt * speed) / 5;
				target.x -= (view.x * dt * speed) / 5;
			}

			if (position.z - (view.z * dt * speed) / 5 <= 990 && position.z - (view.z * dt * speed) / 5 >= -990 && hitbox(-(view.z * dt * speed) / 5) == false && checkcollisionwithObject(-(view.z * dt * speed) / 5) == false)

			if (position.z - (view.z * dt * speed) / 5 <= 900 && position.z - (view.z * dt * speed) / 5 >= -900 && hitbox(-(view.z * dt * speed) / 5) == false)

			{
				position.z -= (view.z * dt * speed) / 5;
				target.z -= (view.z * dt * speed) / 5;
			}

			if (Application::IsKeyPressed(VK_SPACE) && position.y - (view.y * dt * speed) / 5 <= 990 && position.y - (view.y * dt * speed) / 5 >= 10 && hitbox(-(view.y * dt * speed) / 5) == false && checkcollisionwithObject(-(view.y * dt * speed) / 5) == false)

			if (Application::IsKeyPressed(VK_SHIFT) && position.y - (view.y * dt * speed) / 5 <= 900 && position.y - (view.y * dt * speed) / 5 >= 10 && hitbox(-(view.y * dt * speed) / 5) == false)

			{
				position.y -= (view.y * dt * speed) / 5;
				target.y -= (view.y * dt * speed) / 5;
			}
		}
		if (Application::IsKeyPressed('D'))
		{
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up);

			if (position.x + (right.x * dt * speed) / 5 <= 990 && position.x + (right.x * dt * speed) / 5 >= -990 && hitbox((right.x * dt * speed) / 5) == false && checkcollisionwithObject((right.x * dt * speed) / 5) == false)

			if (position.x + (right.x * dt * speed) / 5 <= 900 && position.x + (right.x * dt * speed) / 5 >= -900 && hitbox((right.x * dt * speed) / 5) == false)
			{
				position.x += (right.x * dt * speed) / 5;
				target.x += (right.x * dt * speed) / 5;
			}

			if (position.z + (right.z * dt * speed) / 5 <= 990 && position.z + (right.z * dt * speed) / 5 >= -990 && hitbox((right.z * dt * speed) / 5) == false && checkcollisionwithObject((right.z * dt * speed) / 5) == false)

			if (position.z + (right.z * dt * speed) / 5 <= 900 && position.z + (right.z * dt * speed) / 5 >= -900 && hitbox((right.z * dt * speed) / 5) == false)
			{
				position.z += (right.z * dt * speed) / 5;
				target.z += (right.z * dt * speed) / 5;
			}

			if (position.y - (right.y * dt * speed) / 5 <= 990 && position.y - (right.y * dt * speed) / 5 >= -990 && hitbox(-(right.y * dt * speed) / 5) == false && checkcollisionwithObject((right.y * dt * speed) / 5) == false)

			if (position.y - (right.y * dt * speed) / 5 <= 900 && position.y - (right.y * dt * speed) / 5 >= -900 && hitbox(-(right.y * dt * speed) / 5) == false)

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

			if (position.x - (right.x * dt * speed) / 5 <= 900 && position.x - (right.x * dt * speed) / 5 >= -900 && hitbox(-(right.x * dt * speed) / 5) == false)
			{
				position.x -= (right.x * dt * speed) / 5;
				target.x -= (right.x * dt * speed) / 5;
			}
			if (position.z - (right.z * dt * speed) / 5 <= 990 && position.z - (right.z * dt * speed) / 5 >= -990 && hitbox(-(right.z * dt * speed) / 5) == false && checkcollisionwithObject(-(right.z * dt * speed) / 5) == false)

			if (position.z - (right.z * dt * speed) / 5 <= 900 && position.z - (right.z * dt * speed) / 5 >= -900 && hitbox(-(right.z * dt * speed) / 5) == false)
			{
				position.z -= (right.z * dt * speed) / 5;
				target.z -= (right.z * dt * speed) / 5;
			}
			if (position.y - (right.y * dt * speed) / 5 <= 990 && position.y - (right.y * dt * speed) / 5 >= -990 && hitbox(-(right.y * dt * speed) / 5) == false && checkcollisionwithObject(-(right.y * dt * speed) / 5) == false)

			if (position.y - (right.y * dt * speed) / 5 <= 900 && position.y - (right.y * dt * speed) / 5 >= -900 && hitbox(-(right.y * dt * speed) / 5) == false)
			{
				position.z -= (right.y * dt * speed) / 5;
				target.z -= (right.y * dt * speed) / 5;
			}
		}
	}
	if (Application::IsKeyPressed(VK_SHIFT))
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
			if (Application::IsKeyPressed(VK_SPACE) && position.y + (view.y * dt * speed)  <= 990 && position.y + (view.y * dt * speed)  >= 10 && hitbox((position.y * dt * speed)) == false && checkcollisionwithObject((view.y * dt * speed)) == false)
			{
				position.y += (view.y * dt * speed);
				target.y += (view.y * dt * speed);
			}
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
			if (Application::IsKeyPressed(VK_SPACE) && position.y - (view.y * dt * speed) <= 990 && position.y - (view.y * dt * speed) >= 10 && hitbox(-(view.y * dt * speed)) == false && checkcollisionwithObject(-(view.y * dt * speed)) == false)
			{
				position.y -= (view.y * dt * speed);
				target.y -= (view.y * dt * speed);
			}
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
			if (position.y - (right.y * dt * speed)<= 990 && position.y - (right.y * dt * speed) >= -990 && hitbox(-(right.y * dt * speed)) == false && checkcollisionwithObject((right.y * dt * speed)) == false)
			{
				position.z -= (right.y * dt * speed);
				target.z -= (right.y * dt * speed);
			}
		}
	}
	else
	{
	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		if (position.x + (view.x * dt * speed) / 2 <= 990 && position.x + (view.x * dt * speed) / 2 >= -990 && hitbox((position.x * dt * speed) / 2) == false && checkcollisionwithObject((view.x * dt * speed) / 2) == false)

		if (position.x + (view.x * dt * speed)/2 <= 900 && position.x + (view.x * dt * speed)/2 >= -900 && hitbox((position.x * dt * speed)/2) == false)
		{
			position.x += (view.x * dt * speed)/2;
			target.x += (view.x * dt * speed)/2;
		}
		if (position.z + (view.z * dt * speed) / 2 <= 990 && position.z + (view.z * dt * speed) / 2 >= -990 && hitbox((view.z * dt * speed) / 2) == false && checkcollisionwithObject((view.z * dt * speed) / 2) == false)

		if (position.z + (view.z * dt * speed) / 2 <= 900 && position.z + (view.z * dt * speed) / 2 >= -900 && hitbox((view.z * dt * speed) / 2) == false)
		{
			position.z += (view.z * dt * speed) / 2;
			target.z += (view.z * dt * speed) / 2;
		}	
		if (Application::IsKeyPressed(VK_SPACE) && position.y + (view.y * dt * speed) / 2 <= 990 && position.y + (view.y * dt * speed) / 2 >= 10 && hitbox((position.y * dt * speed) / 2) == false && checkcollisionwithObject((view.y * dt * speed) / 2) == false)

		if (Application::IsKeyPressed(VK_SHIFT) && position.y + (view.y * dt * speed)/2 <= 900 && position.y + (view.y * dt * speed)/2 >= 10 && hitbox((position.y * dt * speed)/2) == false)
		{
			position.y += (view.y * dt * speed) / 2;
			target.y += (view.y * dt * speed) / 2;
		}
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();

		if (position.x - (view.x * dt * speed) / 2 <= 990 && position.x - (view.x * dt * speed) / 2 >= -990 && hitbox(-(view.x * dt * speed) / 2) == false && checkcollisionwithObject(-(view.x * dt * speed) / 2) == false)

		if (position.x - (view.x * dt * speed) / 2 <= 900 && position.x - (view.x * dt * speed) / 2 >= -900 && hitbox(-(view.x * dt * speed) / 2) == false)

		{
			position.x -= (view.x * dt * speed) / 2;
			target.x -= (view.x * dt * speed) / 2;
		}

		if (position.z - (view.z * dt * speed) / 2 <= 990 && position.z - (view.z * dt * speed) / 2 >= -990 && hitbox(-(view.z * dt * speed) / 2) == false && checkcollisionwithObject(-(view.z * dt * speed) / 2) == false)

		if (position.z - (view.z * dt * speed) / 2 <= 900 && position.z - (view.z * dt * speed) / 2 >= -900 && hitbox(-(view.z * dt * speed) / 2) == false)

		{
			position.z -= (view.z * dt * speed) / 2;
			target.z -= (view.z * dt * speed) / 2;
		}

		if (Application::IsKeyPressed(VK_SPACE) && position.y - (view.y * dt * speed) / 2 <= 990 && position.y - (view.y * dt * speed) / 2 >= 10 && hitbox(-(view.y * dt * speed) / 2) == false && checkcollisionwithObject(-(view.y * dt * speed) / 2) == false)

		if (Application::IsKeyPressed(VK_SHIFT) && position.y - (view.y * dt * speed) / 2 <= 900 && position.y - (view.y * dt * speed) / 2 >= 10 && hitbox(-(view.y * dt * speed) / 2) == false)

		{
			position.y -= (view.y * dt * speed) / 2;
			target.y -= (view.y * dt * speed) / 2;
		}
	}
	if (Application::IsKeyPressed('D'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);

		if (position.x + (right.x * dt * speed) / 2 <= 990 && position.x + (right.x * dt * speed) / 2 >= -990 && hitbox((right.x * dt * speed) / 2) == false && checkcollisionwithObject((right.x * dt * speed) / 2) == false)

		if (position.x + (right.x * dt * speed) / 2 <= 900 && position.x + (right.x * dt * speed) / 2 >= -900 && hitbox((right.x * dt * speed) / 2) == false)

		{
			position.x += (right.x * dt * speed) / 2;
			target.x += (right.x * dt * speed) / 2;
		}

		if (position.z + (right.z * dt * speed) / 2 <= 990 && position.z + (right.z * dt * speed) / 2 >= -990 && hitbox((right.z * dt * speed) / 2) == false && checkcollisionwithObject((right.z * dt * speed) / 2) == false)

		if (position.z + (right.z * dt * speed)/2 <= 900 && position.z + (right.z * dt * speed)/2 >= -900 && hitbox((right.z * dt * speed)/2) == false)

		{
			position.z += (right.z * dt * speed) / 2;
			target.z += (right.z * dt * speed) / 2;
		}

		if (position.y - (right.y * dt * speed) / 2 <= 990 && position.y - (right.y * dt * speed) / 2 >= -990 && hitbox(-(right.y * dt * speed) / 2) == false && checkcollisionwithObject((right.y * dt * speed) / 2) == false)

		if (position.y - (right.y * dt * speed) / 2 <= 900 && position.y - (right.y * dt * speed) / 2 >= -900 && hitbox(-(right.y * dt * speed) / 2) == false)

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

		if (position.x - (right.x * dt * speed)/2 <= 900 && position.x - (right.x * dt * speed)/2 >= -900 && hitbox(-(right.x * dt * speed)/2) == false)

		{
			position.x -= (right.x * dt * speed)/2;
			target.x -= (right.x * dt * speed)/2;
		}

		if (position.z - (right.z * dt * speed) / 2 <= 990 && position.z - (right.z * dt * speed) / 2 >= -990 && hitbox(-(right.z * dt * speed) / 2) == false && checkcollisionwithObject(-(right.z * dt * speed) / 2) == false)

		if (position.z - (right.z * dt * speed)/2 <= 900 && position.z - (right.z * dt * speed)/2 >= -900 && hitbox(-(right.z * dt * speed)/2) == false)

		{
			position.z -= (right.z * dt * speed)/2;
			target.z -= (right.z * dt * speed)/2;
		}

		if (position.y - (right.y * dt * speed) / 2 <= 990 && position.y - (right.y * dt * speed) / 2 >= -990 && hitbox(-(right.y * dt * speed) / 2) == false && checkcollisionwithObject((right.y * dt * speed) / 2) == false)

		if (position.y - (right.y * dt * speed) / 2 <= 900 && position.y - (right.y * dt * speed) / 2 >= -900 && hitbox(-(right.y * dt * speed) / 2) == false)

		{
			position.z -= (right.y * dt * speed) / 2;
			target.z -= (right.y * dt * speed) / 2;
		}
	}
	}

	teleport();

	//std::cout << direction.x - position.x << std::endl;
	//std::cout << direction.y - position.y << std::endl;
	//std::cout << direction.z - position.z << std::endl;

	//if (Application::IsKeyPressed(VK_LBUTTON))
	//{
	//	std::cout << position << "cam" << std::endl;
	//	std::cout << view << "view" << std::endl;
	//}

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

//void Camera3::recoil()
//{
//	upRecoil += 10;
//	std::cout << upRecoil << std::endl;
//}

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

bool Camera3::checkcollisionwithOres()
{
	std::vector<Vector3>::iterator count = minMineralVectors.begin();
	std::vector<Vector3>::iterator count1 = maxMineralVectors.begin();

	while (count != minMineralVectors.end())
	{
		Vector3 minTemp = *count;
		Vector3 maxTemp = *count1;

		if (position.x > minTemp.x && position.x  < maxTemp.x && position.y  > minTemp.y && position.y  < maxTemp.y && position.z  > minTemp.z && position.z < maxTemp.z)
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

	Vector3 p2Min = minPos(Vector3(400.088, 71, 0.0353), 10, 15, 10);
	Vector3 p2Max = maxPos(Vector3(400.088, 71, 0.0353), 10, 15, 10);

	if (position.x > p1Min.x && position.y > p1Min.y && position.z > p1Min.z
		&& position.x < p1Max.x && position.y  < p1Max.y && position.z < p1Max.z)
	{
		position.x = 399.526;
		position.y = 80.0759; 
		position.z = 100.455;
		inBase = true;
	}

	if (position.x > p2Min.x && position.y > p2Min.y && position.z > p2Min.z
		&& position.x < p2Max.x && position.y < p2Max.y && position.z < p2Max.z)
	{
		position.x = -400; 
		position.y = 20; 
		position.z = 1;
		inBase = false;
	}
}

bool Camera3::mining()
{
	Vector3 p1Min = minPos(Vector3(-291, 10, -39), 10, 10, 10);
	Vector3 p1Max = maxPos(Vector3(-291, 10, -39), 10, 10, 10);

	/*Vector3 p2Min = minPos(Vector3(-349, 10, 124), 10, 10, 10);
	Vector3 p2Max = maxPos(Vector3(-349, 10, 124), 10, 10, 10);

	Vector3 p3Min = minPos(Vector3(-460, 10, -357), 10, 10, 10);
	Vector3 p3Max = maxPos(Vector3(-460, 10, -357), 10, 10, 10);

	Vector3 p4Min = minPos(Vector3(-527, 10, -109), 10, 10, 10);
	Vector3 p4Max = maxPos(Vector3(-527, 10, -109), 10, 10, 10);

	Vector3 p5Min = minPos(Vector3(-675, 10, -782), 10, 10, 10);
	Vector3 p5Max = maxPos(Vector3(-675, 10, -782), 10, 10, 10);

	Vector3 p6Min = minPos(Vector3(-237, 10, -230), 10, 10, 10);
	Vector3 p6Max = maxPos(Vector3(-237, 10, -230), 10, 10, 10);

	Vector3 p7Min = minPos(Vector3(-125, 10, -679), 10, 10, 10);
	Vector3 p7Max = maxPos(Vector3(-125, 10, -679), 10, 10, 10);

	Vector3 p8Min = minPos(Vector3(-430, 10, -258), 10, 10, 10);
	Vector3 p8Max = maxPos(Vector3(-430, 10, -258), 10, 10, 10);

	Vector3 p9Min = minPos(Vector3(-325, 10, 61), 10, 10, 10);
	Vector3 p9Max = maxPos(Vector3(-325, 10, 61), 10, 10, 10);

	Vector3 p10Min = minPos(Vector3(20, 10, -736), 10, 10, 10);
	Vector3 p10Max = maxPos(Vector3(20, 10, -736), 10, 10, 10);

	Vector3 p11Min = minPos(Vector3(-480, 10, -704), 10, 10, 10);
	Vector3 p11Max = maxPos(Vector3(-480, 10, -704), 10, 10, 10);

	Vector3 p12Min = minPos(Vector3(-466, 10, -322), 10, 10, 10);
	Vector3 p12Max = maxPos(Vector3(-466, 10, -322), 10, 10, 10);

	Vector3 p13Min = minPos(Vector3(-321, 10, -505), 10, 10, 10);
	Vector3 p13Max = maxPos(Vector3(-321, 10, -505), 10, 10, 10);

	Vector3 p14Min = minPos(Vector3(-824, 10, -338), 10, 10, 10);
	Vector3 p14Max = maxPos(Vector3(-824, 10, -338), 10, 10, 10);

	Vector3 p15Min = minPos(Vector3(-478, 10, -298), 10, 10, 10);
	Vector3 p15Max = maxPos(Vector3(-478, 10, -298), 10, 10, 10);

	Vector3 p16Min = minPos(Vector3(-529, 10, -383), 10, 10, 10);
	Vector3 p16Max = maxPos(Vector3(-529, 10, -383), 10, 10, 10);

	Vector3 p17Min = minPos(Vector3(64, 10, -633), 10, 10, 10);
	Vector3 p17Max = maxPos(Vector3(64, 10, -633), 10, 10, 10);

	Vector3 p18Min = minPos(Vector3(-352, 10, -694), 10, 10, 10);
	Vector3 p18Max = maxPos(Vector3(-352, 10, -694), 10, 10, 10);

	Vector3 p19Min = minPos(Vector3(-310, 10, -300), 10, 10, 10);
	Vector3 p19Max = maxPos(Vector3(-310, 10, -300), 10, 10, 10);

	Vector3 p20Min = minPos(Vector3(-158, 10, -409), 10, 10, 10);
	Vector3 p20Max = maxPos(Vector3(-158, 10, -409), 10, 10, 10);
	
	Vector3 p21Min = minPos(Vector3(-327, 10, -271), 10, 10, 10);
	Vector3 p21Max = maxPos(Vector3(-327, 10, -271), 10, 10, 10);

	Vector3 p22Min = minPos(Vector3(-88, 10, -426), 10, 10, 10);
	Vector3 p22Max = maxPos(Vector3(-88, 10, -426), 10, 10, 10);

	Vector3 p23Min = minPos(Vector3(91, 10, -775), 10, 10, 10);
	Vector3 p23Max = maxPos(Vector3(91, 10, -775), 10, 10, 10);

	Vector3 p24Min = minPos(Vector3(-256, 10, -420), 10, 10, 10);
	Vector3 p24Max = maxPos(Vector3(-256, 10, -420), 10, 10, 10);

	Vector3 p25Min = minPos(Vector3(-554, 10, -504), 10, 10, 10);
	Vector3 p25Max = maxPos(Vector3(-554, 10, -504), 10, 10, 10);*/

	if (position.x > p1Min.x && position.y > p1Min.y && position.z > p1Min.z && position.x < p1Max.x && position.y < p1Max.y && position.z < p1Max.z
		/*&& position.x > p2Min.x && position.y > p2Min.y && position.z > p2Min.z && position.x < p2Max.x && position.y < p2Max.y && position.z < p2Max.z*/)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Camera3::craftUi()
{
	Vector3 min = minPos(Vector3(399.667, 80, -38), 10, 15, 10);
	Vector3 max = maxPos(Vector3(399.667, 80, -38), 10, 15, 10);

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