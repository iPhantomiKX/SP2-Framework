#include "Sp2Scene.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "gun.h"
#include "enemy.h"
#include <iostream>

using std::cout;

//test

//bool Camera3::test = false;
//bool Camera3::test2 = false;
//bool Camera3::test3 = false;
//bool Sp2Scene::test4 = false;
//bool Sp2Scene::test5 = false;

Camera3 c3;
bool Camera3::mouseControl = true;
bool Camera3::inBase = false;
double Camera3::xpos = 0;
double Camera3::ypos = 0;
double Camera3::recoil = 0;
Vector3 Camera3::location = (0, 0, 0);
Vector3 Camera3::location2 = (0, 0, 0);
Vector3 Camera3::direction = (0, 0, 0);
pistol pis;
rifle rif;
sniper sr;
shotgun sg;
target t;


Sp2Scene::Sp2Scene()
{
}

Sp2Scene::~Sp2Scene()
{
}

void Sp2Scene::Init()
{
	// Init VBO here

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glDisable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutOff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0, 20, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 2;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutOff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutOff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);



	//variable to rotate geometry
	rotateAngle = 0;
	planet1RotAngle = planet1RevAngle = moon1RotAngle = 0;
	rotateGunX = 0;
	rotateGunY = 0;
	test = (0, 0, 0);
	range = 0;
	gunCd = 0;
	reloaded = true;
	upRecoil = 0;
	gunDir = 0;
	storeRand = 0;
	storeRand2 = 0;
	gameStates = states::outside;

	//Initialize camera settings
	camera.Init(Vector3(1, 10, 0), Vector3(0, 10, 0), Vector3(0, 1, 0));


	meshList[GEO_HEAD] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 10, 40);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 10, 20);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad2("bottom", Color(.3, .3, .3), TexCoord(50, 50));
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//Moon_texture.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1000, 1000);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//purplenebula_up.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1000, 1000);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//purplenebula_dn.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1000, 1000);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//purplenebula_ft.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1000, 1000);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//purplenebula_bk.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1000, 1000);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//purplenebula_lf.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1000, 1000);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//purplenebula_rt.tga");
	/*meshList[GEO_MODEL1] = MeshBuilder::GenerateOBJ("model1", "OBJ//chair.obj");
	meshList[GEO_MODEL1]->textureID = LoadTGA("Image//chair.tga");
	meshList[GEO_AIR] = MeshBuilder::GenerateOBJ("air", "OBJ//airvehicle.obj");
	meshList[GEO_AIR]->textureID = LoadTGA("Image//uvairvehicletexture.tga");
	meshList[GEO_DEADTREE] = MeshBuilder::GenerateOBJ("deadtree", "OBJ//deadtree.obj");
	meshList[GEO_DEADTREE]->textureID = LoadTGA("Image//deadtree.tga");
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(1, 1, 0), 20);*/
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	//Crafting Ui image
	meshList[GEO_CRAFT_UI] = MeshBuilder::GenerateOBJ("craftin_UI","OBJ//CraftingUI.obj");
	meshList[GEO_CRAFT_UI]->textureID = LoadTGA("Image//CraftingUI.tga");

	/*meshList[GEO_OBJECT] = MeshBuilder::GenerateOBJ("tricker", "OBJ//Tricker.obj");
	meshList[GEO_OBJECT]->textureID = LoadTGA("Image//trickeruv.tga");*/
	meshList[GEO_TEST] = MeshBuilder::GenerateOBJ("test", "OBJ//test.obj");

	//Spaceship UI Image
	meshList[GEO_SPACESHIPQUAD] = MeshBuilder::GenerateQuad("spaceshipquad", Color(1, 1, 1), 100, 100);
	meshList[GEO_SPACESHIPQUAD]->textureID = LoadTGA("Image//spaceship_floor.tga");

	//Crafting Panel UI Image
	meshList[GEO_CRAFTINGPANEL] = MeshBuilder::GenerateOBJ("craftingpanelmodel", "OBJ//Crafting_Panel.obj");
	meshList[GEO_CRAFTINGPANEL]->textureID = LoadTGA("Image//craft_bench_UV.tga");

	//Space House UI Image
	meshList[GEO_SPACEHOUSE] = MeshBuilder::GenerateOBJ("spacehouse", "OBJ//SpaceHouse.obj");
	meshList[GEO_SPACEHOUSE]->textureID = LoadTGA("Image//SpaceHouseUV.tga");
	
	//Entrance Portal UI Image
	meshList[GEO_PORTAL1] = MeshBuilder::GenerateOBJ("EnterPortal", "OBJ//portal.obj");
	meshList[GEO_PORTAL1]->textureID = LoadTGA("Image//portal1UV.tga");

	//Exit Portal UI Image
	meshList[GEO_PORTAL2] = MeshBuilder::GenerateOBJ("ExitPortal", "OBJ//portal.obj");
	meshList[GEO_PORTAL2]->textureID = LoadTGA("Image//portal2UV.tga");

	//Table Image
	meshList[GEO_TABLE] = MeshBuilder::GenerateOBJ("table", "OBJ//Table.obj");
	meshList[GEO_TABLE]->textureID = LoadTGA("Image//TableUV.tga");

	//HealthPack Image
	meshList[GEO_HEALTHPACK] = MeshBuilder::GenerateOBJ("healthpack", "OBJ//HealthPack.obj");
	meshList[GEO_HEALTHPACK]->textureID = LoadTGA("Image//HealthPackUV.tga");

	//Minerals Image
	meshList[GEO_MINERALS] = MeshBuilder::GenerateOBJ("gold_mineral", "OBJ//GoldMineral.obj");
	meshList[GEO_MINERALS]->textureID = LoadTGA("Image//GoldMineralsUV.tga");

	
	meshList[GEO_SCOPE] = MeshBuilder::GenerateOBJ("scopemodel", "OBJ//Scope.obj");
	meshList[GEO_SCOPE]->textureID = LoadTGA("Image//Scope.tga");

	meshList[GEO_SHOT] = MeshBuilder::GenerateSphere("shot", Color(0.3,0.3,1), 10,20);

	meshList[GEO_PISTOL1] = MeshBuilder::GenerateOBJ("pistol1model", "OBJ//pistol1.obj");
	meshList[GEO_PISTOL1]->textureID = LoadTGA("Image//pistol1texture.tga");

	meshList[GEO_PISTOL2] = MeshBuilder::GenerateOBJ("pistol2model", "OBJ//pistol2.obj");
	meshList[GEO_PISTOL2]->textureID = LoadTGA("Image//pistol2UV.tga");

	meshList[GEO_RIFLE1] = MeshBuilder::GenerateOBJ("rifle1model", "OBJ//AR.obj");
	meshList[GEO_RIFLE1]->textureID = LoadTGA("Image//AR_UV.tga");

	meshList[GEO_SNIPER1] = MeshBuilder::GenerateOBJ("sniper1model", "OBJ//AK47.obj");
	meshList[GEO_SNIPER1]->textureID = LoadTGA("Image//AK47UV.tga");

	meshList[GEO_SHOTGUN1] = MeshBuilder::GenerateOBJ("shotgun1model", "OBJ//Shotgun.obj");
	meshList[GEO_SHOTGUN1]->textureID = LoadTGA("Image//shotgunUV.tga");

	meshList[GEO_TARGET] = MeshBuilder::GenerateCube("target", Color(1, 0, 0));

	meshList[GEO_TARGETHIT] = MeshBuilder::GenerateCube("target2", Color(0, 1, 0));

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 2000.0f);
	projectionStack.LoadMatrix(projection);

	for (int i = 0; i < 1000; i++)
	{
		rainpositionx[i] = rand() % 998 - 499;
		rainpositiony[i] = rand() % 998 - 499;
		rainpositionz[i] = rand() % 998 - 499;
	}

	for (int i = 0; i < 50; ++i)
	{
		treex[i] = rand() % 980 - 490;
		treez[i] = rand() % 980 - 490;
	}

	for (int a = 0; a < 25; ++a)
	{
		elementsx[a] = rand() % 980 - 980;
		elementsz[a] = rand() % 980 - 880;
	}

	translateY = -60;
	translateX = 0;
	translateX = 0;

	///for hitboxes///
	minVectors.push_back(c3.minPos(Vector3(400, 60, 0), 75, 15, 75));
	minVectors.push_back(c3.minPos(Vector3(400, 60, 100), 75, 15, 75));
	minVectors.push_back(c3.minPos(Vector3(300, 60, 0), 75, 15, 75));
	minVectors.push_back(c3.minPos(Vector3(300, 60, 100), 75, 15, 75));
	minVectors.push_back(c3.minPos(Vector3(450, 110, 0), 10, 75, 75));
	minVectors.push_back(c3.minPos(Vector3(450, 110, 100), 10, 75, 75));
	minVectors.push_back(c3.minPos(Vector3(250, 110, 0), 10, 75, 75));
	minVectors.push_back(c3.minPos(Vector3(250, 110, 100), 10, 75, 75));
	minVectors.push_back(c3.minPos(Vector3(400, 110, -50), 75, 75, 10));
	minVectors.push_back(c3.minPos(Vector3(400, 110, 150), 75, 75, 10));
	minVectors.push_back(c3.minPos(Vector3(300, 110, -50), 75, 75, 10));
	minVectors.push_back(c3.minPos(Vector3(300, 110, 150), 75, 75, 10));
	minVectors.push_back(c3.minPos(Vector3(400, 160, 0), 75, 10, 75));
	minVectors.push_back(c3.minPos(Vector3(400, 160, 100), 75, 10, 75));
	minVectors.push_back(c3.minPos(Vector3(300, 160, 0), 75, 10, 75));
	minVectors.push_back(c3.minPos(Vector3(300, 160, 100), 75, 10, 75));
	minVectors.push_back(c3.minPos(Vector3(-338, 0, 38), 15, 90, 60));
	minVectors.push_back(c3.minPos(Vector3(-460, 0, 38), 15, 90, 60));
	minVectors.push_back(c3.minPos(Vector3(-400, 0, 73), 90, 90, 15));
	minVectors.push_back(c3.minPos(Vector3(-360, 0, 0), 30, 90, 15));
	minVectors.push_back(c3.minPos(Vector3(-435, 0, 0), 30, 90, 15));

	maxVectors.push_back(c3.maxPos(Vector3(400, 60, 0), 75, 15, 75));
	maxVectors.push_back(c3.maxPos(Vector3(400, 60, 100), 75, 15, 75));
	maxVectors.push_back(c3.maxPos(Vector3(300, 60, 0), 75, 15, 75));
	maxVectors.push_back(c3.maxPos(Vector3(300, 60, 100), 75, 15, 75));
	maxVectors.push_back(c3.maxPos(Vector3(450, 110, 0), 10, 75, 75));
	maxVectors.push_back(c3.maxPos(Vector3(450, 110, 100), 10, 75, 75));
	maxVectors.push_back(c3.maxPos(Vector3(250, 110, 0), 10, 75, 75));
	maxVectors.push_back(c3.maxPos(Vector3(250, 110, 100), 10, 75, 75));
	maxVectors.push_back(c3.maxPos(Vector3(400, 110, -50), 75, 75, 10));
	maxVectors.push_back(c3.maxPos(Vector3(400, 110, 150), 75, 75, 10));
	maxVectors.push_back(c3.maxPos(Vector3(300, 110, -50), 75, 75, 10));
	maxVectors.push_back(c3.maxPos(Vector3(300, 110, 150), 75, 75, 10));
	maxVectors.push_back(c3.maxPos(Vector3(400, 160, 0), 75, 10, 75));
	maxVectors.push_back(c3.maxPos(Vector3(400, 160, 100), 75, 10, 75));
	maxVectors.push_back(c3.maxPos(Vector3(300, 160, 0), 75, 10, 75));
	maxVectors.push_back(c3.maxPos(Vector3(300, 160, 100), 75, 10, 75));
	maxVectors.push_back(c3.maxPos(Vector3(-338, 0, 38), 15, 90, 60));
	maxVectors.push_back(c3.maxPos(Vector3(-460, 0, 38), 15, 90, 60));
	maxVectors.push_back(c3.maxPos(Vector3(-400, 0, 73), 90, 90, 15));
	maxVectors.push_back(c3.maxPos(Vector3(-360, 0, 0), 30, 90, 15));
	maxVectors.push_back(c3.maxPos(Vector3(-435, 0, 0), 30, 90, 15));

	
}

static float LSPEED = 10.f;
static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void Sp2Scene::Update(double dt)
{
	camera.Update(dt);

	if (gameStates == states::base)
	{
		if (camera.craftUi() == true && Application::IsKeyPressed('E'))
		{
			crafting = true;
		}
		else if (camera.craftUi() == false)
		{
			crafting = false;
		}

		if (camera.craftUi() == true)
		{
			if (Application::IsKeyPressed('2') && crafting == true)
			{
				boughtRifle1 = true;
				rifle1Avail = true;


				/*if (boughtShotgun1 == false)
				{
				shotgun1Avail = false;
				}

			if (boughtSniper1 == false)
			{
			sniper1Avail = false;
			}*/
		}
		if (Application::IsKeyPressed('3') && crafting == true)
		{
			/*		if (boughtRifle1 == false)
			{
			rifle1Avail = false;
			}*/
			boughtShotgun1 = true;
			shotgun1Avail = true;

				if (boughtSniper1 == false)
				{
				sniper1Avail = false;
				}
			}
			if (Application::IsKeyPressed('3') && crafting == true)
			{

				/*		if (boughtRifle1 == false)
				{
				rifle1Avail = false;
				}*/
				boughtShotgun1 = true;
				shotgun1Avail = true;

				//if (boughtSniper1 == false)
				//{
				//	sniper1Avail = false;
				//}
			}
			if (Application::IsKeyPressed('4') && crafting == true)
			{
				/*if (boughtRifle1 == false)
				{
				rifle1Avail = false;
				}

				if (boughtShotgun1 == false)
				{
				shotgun1Avail = false;
				}*/
				cout << "3" << std::endl;
				boughtSniper1 = true;
				sniper1Avail = true;
			}
		}
	}


	//If pressed '1', switch to Pistol1



	//if (Application::IsKeyPressed('E'))
	//{
	//	if (camera.checkcollisionwithObject(Vector3(399.667, 80, -38), 10, 15, 10))
	//	{
	//		crafting = true;
	//	}

	//}
	//else if (!camera.checkcollisionwithObject(Vector3(399.667, 80, -38), 10, 15, 10))
	//{
	//	crafting = false;
	//}



	//for (int i = 0; i < 50; ++i)
	//{
	//	if (camera.checkcollisionwithObject(Vector3(treex[i], 0, treez[i]), 20, 40, 20) == true)
	//	{
	//		camera.position = camera.prevPosition;
	//	}

	//	//collision for spaceship floor 2
	//	if (camera.checkcollisionwithObject(Vector3(400, 60, 100), 100, 15, 100))
	//	{
	//		camera.position = camera.prevPosition;
	//	}

	//	//collision for spaceship wall length 1
	//	if (camera.checkcollisionwithObject(Vector3(450, 110, 0), 15, 100, 100))
	//	{
	//		camera.position = camera.prevPosition;
	//		meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1000, 1000);
	//		meshList[GEO_TOP]->textureID = LoadTGA("Image//blood_up.tga");
	//		meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1000, 1000);
	//		meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//blood_dn.tga");
	//		meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1000, 1000);
	//		meshList[GEO_FRONT]->textureID = LoadTGA("Image//blood_ft.tga");
	//		meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1000, 1000);
	//		meshList[GEO_BACK]->textureID = LoadTGA("Image//blood_bk.tga");
	//		meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1000, 1000);
	//		meshList[GEO_LEFT]->textureID = LoadTGA("Image//blood_lf.tga");
	//		meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1000, 1000);
	//		meshList[GEO_RIGHT]->textureID = LoadTGA("Image//blood_rt.tga");

	//		meshList[GEO_HEAD] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 10, 40);
	//		meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 0, 0), 10, 20);

	//		meshList[GEO_OBJECT] = MeshBuilder::GenerateOBJ("tricker", "OBJ//Rock.obj");
	//		//meshList[GEO_OBJECT]->textureID = LoadTGA("Image//chair.tga");

	//		light[0].color.Set(1, 0, 0);
	//		for (int i = 0; i < 1000; i++)
	//		{
	//			rainpositiony[i] -= (float)(100 * dt);
	//		}
	//		/*Camera3::test2 = false;
	//		Camera3::test = false;
	//		Camera3::test3 = true;
	//		}*/
			
			//collision for spaceship floor 1
			/*if (camera.checkcollisionwithObject(Vector3(400, 60, 0), 100, 15, 100))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spaceship floor 2
		/*	if (camera.checkcollisionwithObject(Vector3(400, 60, 100), 100, 15, 100))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spaceship floor 3
			/*if (camera.checkcollisionwithObject(Vector3(300, 60, 0), 100, 15, 100))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spaceship floor 4
			//if (camera.checkcollisionwithObject(Vector3(300, 60, 100), 100, 15, 100))
			//{
			//	camera.position = camera.prevPosition;
			//}

			//collision for spaceship wall length 1
			//if (camera.checkcollisionwithObject(Vector3(450, 110, 0), 15, 100, 100))
			//{
			//	camera.position = camera.prevPosition;
			//}

			//collision for spaceship wall length 2
			/*if (camera.checkcollisionwithObject(Vector3(450, 110, 100), 15, 100, 100))
			{
				camera.position = camera.prevPosition;
			}*/

			
			//collision for spaceship wall length 3
			/*if (camera.checkcollisionwithObject(Vector3(250, 110, 0), 15, 100, 100))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spaceship wall length 4
			/*if (camera.checkcollisionwithObject(Vector3(250, 110, 100), 15, 100, 100))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spaceship wall width 1
			//if (camera.checkcollisionwithObject(Vector3(400, 110, -50), 100, 100, 15))
			//{
			//	camera.position = camera.prevPosition;
			//}

			//collision for spaceship wall width 2
			/*if (camera.checkcollisionwithObject(Vector3(400, 110, 150), 100, 100, 15))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spaceship wall width 3
			/*if (camera.checkcollisionwithObject(Vector3(300, 110, -50), 100, 100, 15))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spaceship wall width 4
			/*if (camera.checkcollisionwithObject(Vector3(300, 110, 150), 100, 100, 15))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spaceship roof 1
			/*if (camera.checkcollisionwithObject(Vector3(400, 160, 0), 100, 15, 100))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spaceship roof 2
			/*if (camera.checkcollisionwithObject(Vector3(400, 160, 100), 100, 15, 100))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spaceship roof 3
			//if (camera.checkcollisionwithObject(Vector3(300, 160, 0), 100, 15, 100))
			//{
			//	camera.position = camera.prevPosition;
			//}

			//collision for spaceship roof 4
			/*if (camera.checkcollisionwithObject(Vector3(300, 160, 100), 100, 15, 100))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spacehouse width 1
			/*if (camera.checkcollisionwithObject(Vector3(-338, 0, 38), 18, 120, 120))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spacehouse width 2
			/*if (camera.checkcollisionwithObject(Vector3(-460, 0, 38), 18, 120, 120))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for spacehouse back wall 
			/*if (camera.checkcollisionwithObject(Vector3(-400, 0, 73), 120, 120, 18))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for front wall 1
			/*if (camera.checkcollisionwithObject(Vector3(-360, 0, 0), 52, 120, 18))
			{
				camera.position = camera.prevPosition;
			}*/

			//collision for front wall 2
			/*if (camera.checkcollisionwithObject(Vector3(-435, 0, 0), 52, 120, 18))
			{
				camera.position = camera.prevPosition;
			}*/

			//portal1 interaction
			//if (camera.checkcollisionwithObject(Vector3(-435.088, 10, 50.0353), 10, 15, 10))
			//{
			//	camera.position = camera.prevPosition;
			//	testPortalsign = true;
			//	testPortal = true;
			//	if (Application::IsKeyPressed('E') && testPortal == true)
			//	{
			//		camera.position.x = 399.526;
			//		camera.position.y = 80.0759;
			//		camera.position.z = 100.455;
			//		testPortal = false;
			//		testPortalsign = false;
			//	}
			//}
			//else
			//{
			//	testPortal = false;
			//	testPortalsign = false;
			//}


			//portal2 interaction
			//if (camera.checkcollisionwithObject(Vector3(400.088, 71, 0.0353), 10, 15, 10))
			//{
				//camera.position = camera.prevPosition;
			//	testPortalsign = true;
			//	testPortal = true;
			//	if (Application::IsKeyPressed('E') && testPortal == true)
			//	{
			//		camera.position.x = -400;
			//		camera.position.y = 20;
			//		camera.position.z = 1;
			//		testPortal = false;
			//		testPortalsign = false;
	/*			}
			}
			else
			{
				testPortal = false;
				testPortalsign = false;
			}*/

			////Crafting interaction
			//if (camera.checkcollisionwithObject(Vector3(399.667, 80, -38), 10, 15, 10))
			//{
			//	camera.position = camera.prevPosition;

			//	if (Application::IsKeyPressed('E'))
			//	{
			//		
			//		RenderImageOnScreen(meshList[GEO_CRAFT_UI], 4, 10, 5);
			//		cout << "I am running";
			//	}
			//}
			//else
			//{

			//}


			//if (Application::IsKeyPressed('1')) //enable back face culling
			//glDisable(GL_CULL_FACE);
			//if (Application::IsKeyPressed('2')) //disable back face culling
			//glEnable(GL_CULL_FACE);
			if (Application::IsKeyPressed('0'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
			if (Application::IsKeyPressed('5'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
			

			if (Application::IsKeyPressed('I'))
				light[0].position.z -= (float)(LSPEED * dt);
			if (Application::IsKeyPressed('K'))
				light[0].position.z += (float)(LSPEED * dt);
			if (Application::IsKeyPressed('J'))
				light[0].position.x -= (float)(LSPEED * dt);
			if (Application::IsKeyPressed('L'))
				light[0].position.x += (float)(LSPEED * dt);
			if (Application::IsKeyPressed('O'))
				light[0].position.y -= (float)(LSPEED * dt);
			if (Application::IsKeyPressed('P'))
				light[0].position.y += (float)(LSPEED * dt);

			if (Application::IsKeyPressed('7'))
			{
				light[0].type = Light::LIGHT_DIRECTIONAL;
			}
			if (Application::IsKeyPressed('8'))
			{
				light[0].type = Light::LIGHT_SPOT;
			}


			/*if (Application::IsKeyPressed('5'))
			{
				meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_TOP]->textureID = LoadTGA("Image//snow_up.tga");
				meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//snow_dn.tga");
				meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_FRONT]->textureID = LoadTGA("Image//snow_ft.tga");
				meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_BACK]->textureID = LoadTGA("Image//snow_bk.tga");
				meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_LEFT]->textureID = LoadTGA("Image//snow_lf.tga");
				meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_RIGHT]->textureID = LoadTGA("Image//snow_rt.tga");
				meshList[GEO_HEAD] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 10, 40);
				meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 10, 20);
				light[0].color.Set(1, 1, 1);
			}

			if (Application::IsKeyPressed('6'))
			{
				meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_TOP]->textureID = LoadTGA("Image//icyhell_up.tga");
				meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//icyhell_dn.tga");
				meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_FRONT]->textureID = LoadTGA("Image//icyhell_ft.tga");
				meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_BACK]->textureID = LoadTGA("Image//icyhell_bk.tga");
				meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_LEFT]->textureID = LoadTGA("Image//icyhell_lf.tga");
				meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_RIGHT]->textureID = LoadTGA("Image//icyhell_rt.tga");
				meshList[GEO_HEAD] = MeshBuilder::GenerateSphere("sphere", Color(0.5, 0.5, 0.5), 10, 40);
				meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(0.5, 0.5, 0.5), 10, 20);
				light[0].color.Set(0.5, 0.5, 0.5);
			}

			if (Application::IsKeyPressed('9'))
			{
				meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_TOP]->textureID = LoadTGA("Image//blood_up.tga");
				meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//blood_dn.tga");
				meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_FRONT]->textureID = LoadTGA("Image//blood_ft.tga");
				meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_BACK]->textureID = LoadTGA("Image//blood_bk.tga");
				meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_LEFT]->textureID = LoadTGA("Image//blood_lf.tga");
				meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1000, 1000);
				meshList[GEO_RIGHT]->textureID = LoadTGA("Image//blood_rt.tga");

				meshList[GEO_HEAD] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 10, 40);
				meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 0, 0), 10, 20);
				light[0].color.Set(1, 0, 0);
				for (int i = 0; i < 1000; i++)
				{
					rainpositiony[i] -= (float)(100 * dt);
				}
			}

			rotateAngle += (float)(10 * dt);
			planet1RotAngle += (float)(5 * dt);
			planet1RevAngle += (float)(2 * dt);
			moon1RotAngle += (float)(50 * dt);*/

			framerate = 1 / dt;

		/*	for (int i = 0; i < 1000; i++)
			{
				rainpositiony[i] -= (float)(50 * dt);
				if (rainpositiony[i] < -499.0f)
				{
					rainpositiony[i] = 499.0f;
					rainpositionx[i] = rand() % 998 - 499;
					rainpositionz[i] = rand() % 998 - 499;
				}
				if (Application::IsKeyPressed(VK_SPACE))
				{
					rainpositiony[i] -= (float)(150 * dt);
				}
				if (Application::IsKeyPressed('Q'))
				{
					rainpositiony[i] += (float)(150 * dt);
				}
				if (Application::IsKeyPressed('E'))
				{
					rainpositiony[i] += (float)(50 * dt);
				}
				if (Application::IsKeyPressed('G'))
				{
					rainpositionx[i] -= (float)(150 * dt);
				}
				if (Application::IsKeyPressed('H'))
				{
					rainpositionx[i] += (float)(150 * dt);
				}
			}*/

			if (gameStates == states::outside)
			{
				
				//If pressed '1', switch to Pistol1
				if (equipPistol1 == true)
				{

					if (Application::IsKeyPressed(VK_LBUTTON) && gunCd <= 0 && pis.ammo > 0 && gunReload <= 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
					{
						if (Application::IsKeyPressed(VK_RBUTTON))
						{
							if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
							{
								aimBulletRNG(pis.inAccuracy);
							}
						}
						else
						{
							bulletRNG(pis.inAccuracy);
						}
						shotsFired.push_back(Camera3::location2);
						shotsDir.push_back(Camera3::direction);
						weaponDmg.push_back(pis.damage);
						gunCd = pis.RoF;
						pis.ammo--;
						Camera3::recoil += 1;
						upRecoil += 1;
					}

					bulletPos();
					gunCd--;

					if (Application::IsKeyPressed('R') && pis.ammo < pis.maxAmmo && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false || pis.ammo == 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
					{
						gunReload = pis.reloadSpd;
						reloaded = false;
						Camera3::recoil = 0;
						upRecoil = 0;
					}

					if (gunReload > 0)
					{
						gunReload--;
						if (gunReload <= 0)
						{
							pis.ammo = pis.maxAmmo;
							reloaded = true;
						}
					}
				}

				if (equipRifle1 == true)
				{
					if (Application::IsKeyPressed(VK_LBUTTON) && gunCd <= 0 && rif.ammo > 0 && gunReload <= 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
					{
						if (Application::IsKeyPressed(VK_RBUTTON))
						{
							if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
							{
								aimBulletRNG(rif.inAccuracy);
							}
						}
						else
						{
							bulletRNG(rif.inAccuracy);
						}
						shotsFired.push_back(Camera3::location2);
						shotsDir.push_back(Camera3::direction);
						weaponDmg.push_back(rif.damage);
						gunCd = rif.RoF;
						rif.ammo--;
						Camera3::recoil += 0.5;
						upRecoil += 0.5;
					}

					bulletPos();
					gunCd--;

					if (Application::IsKeyPressed('R') && rif.ammo < rif.maxAmmo && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false || rif.ammo == 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
					{
						gunReload = rif.reloadSpd;
						reloaded = false;
						Camera3::recoil = 0;
						upRecoil = 0;
					}

					if (gunReload > 0)
					{
						gunReload--;
						if (gunReload <= 0)
						{
							rif.ammo = rif.maxAmmo;
							reloaded = true;
						}
					}
				}
				if (equipSniper1 == true)
				{
					if (Application::IsKeyPressed(VK_LBUTTON) && gunCd <= 0 && sr.ammo > 0 && gunReload <= 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
					{
						if (Application::IsKeyPressed(VK_RBUTTON))
						{
							if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
							{
								aimBulletRNG(sr.inAccuracy);
							}
						}
						else
						{
							bulletRNG(sr.inAccuracy);
						}
						shotsFired.push_back(Camera3::location2);
						shotsDir.push_back(Camera3::direction);
						weaponDmg.push_back(sr.damage);
						gunCd = sr.RoF;
						sr.ammo--;
						Camera3::recoil += 2;
						upRecoil += 2;
					}

					bulletPos();
					gunCd--;

					if (Application::IsKeyPressed('R') && sr.ammo < sr.maxAmmo && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false || sr.ammo == 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
					{
						gunReload = sr.reloadSpd;
						reloaded = false;
						Camera3::recoil = 0;
						upRecoil = 0;
					}

					if (gunReload > 0)
					{
						gunReload--;
						if (gunReload <= 0)
						{
							sr.ammo = sr.maxAmmo;
							reloaded = true;
						}
					}
				}

				if (equipShotgun1 == true)
				{
					if (Application::IsKeyPressed(VK_LBUTTON) && gunCd <= 0 && sg.ammo > 0 && gunReload <= 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
					{
						for (int shots = 0; shots < 6; ++shots)
						{
							if (Application::IsKeyPressed(VK_RBUTTON))
							{
								bulletRNG(sg.inAccuracy - 1);
							}
							else
							{
								bulletRNG(sg.inAccuracy);
							}
							shotsFired.push_back(Camera3::location2);
							shotsDir.push_back(Camera3::direction);
							weaponDmg.push_back(sg.damage);
						}
						gunCd = sg.RoF;
						sg.ammo--;
						Camera3::recoil += 2;
						upRecoil += 2;
					}

					bulletPos();
					gunCd--;

					if (Application::IsKeyPressed('R') && sg.ammo < sg.maxAmmo && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false || sg.ammo == 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
					{
						gunReload = sg.reloadSpd;
						reloaded = false;
						Camera3::recoil = 0;
						upRecoil = 0;

					}

					if (gunReload > 0)
					{
						gunReload--;
						if (Application::IsKeyPressed(VK_LBUTTON))
						{
							reloaded = true;
							gunReload = 0;
						}
						else
						{
							if (gunReload <= 0)
							{
								if (sg.ammo < sg.maxAmmo)
								{
									sg.ammo++;
									gunReload = sg.reloadSpd;
								}
								else
								{
									reloaded = true;
								}
							}
						}
					}
				}

				if (testHB == true)
				{
					targetReg = 100;
					testHB = false;
				}
				else
				{
					if (targetReg > 0)
					{
						targetReg--;
					}
				}

				if (t.hp <= 0)
				{
					t.isDead = true;
				}
				if (Application::IsKeyPressed('P'))
				{
					t.hp = 10;
					t.isDead = false;
				}
			}
			if (Camera3::mouseControl == true)
			{
				if (Application::IsKeyPressed(VK_RBUTTON))
				{
					if (Camera3::xpos < 395)
					{
						rotateGunY += 1.5;
					}
					if (Camera3::xpos > 405)
					{
						rotateGunY -= 1.5;
					}
					if (Camera3::ypos < 295)
					{
						gunDir += 1.5;
					}
					if (Camera3::ypos > 305)
					{
						gunDir -= 1.5;
					}
					if (gunDir > 45)
					{
						gunDir = 45;
					}
					else if (gunDir < -45)
					{
						gunDir = -45;
					}
				}
				else
				{
					if (Camera3::xpos < 395)
					{
						rotateGunY += 3;
					}
					if (Camera3::xpos > 405)
					{
						rotateGunY -= 3;
					}
					if (Camera3::ypos < 295)
					{
						gunDir += 3;
					}
					if (Camera3::ypos > 305)
					{
						gunDir -= 3;
					}

					if (gunDir > 45)
					{
						gunDir = 45;
					}
					else if (gunDir < -45)
					{
						gunDir = -45;
					}
				}
			}
			else if (Camera3::mouseControl == false)
			{
				if (Application::IsKeyPressed(VK_LEFT))
				{
					rotateGunY += 3;
				}
				if (Application::IsKeyPressed(VK_RIGHT))
				{
					rotateGunY -= 3;
				}
				if (Application::IsKeyPressed(VK_UP))
				{
					gunDir += 3;
				}
				if (Application::IsKeyPressed(VK_DOWN))
				{
					gunDir -= 3;
				}

				//Boundaries for gun rotation
				if (gunDir > 45)
				{
					gunDir = 45;
				}
				else if (gunDir < -45)
				{
					gunDir = -45;
				}
			}

			if (upRecoil > 20)
			{
				upRecoil = 20;
			}

			rotateGunX = gunDir + upRecoil;
			if (Application::IsKeyPressed(VK_LBUTTON))
			{
				sRecoilCd = 20;
			}
			else if (sRecoilCd > 0)
			{
				sRecoilCd--;
			}


			if (upRecoil > 0 && sRecoilCd == 0)
			{
				upRecoil -= 0.2;
			}

			if (rotateGunX > 45)
			{
				rotateGunX = 45;
			}
			if (rotateGunX < -45)
			{
				rotateGunX = -45;
			}
			if (Application::IsKeyPressed('1') && equipPistol1 == false && reloaded == true)
			{
				equipPistol1 = true;
				equipRifle1 = false;
				equipSniper1 = false;
				equipShotgun1 = false;
			}

			if (boughtRifle1 == true)
			{
				//If pressed '2', switch to Rifle1
				if (Application::IsKeyPressed('2') && equipRifle1 == false && reloaded == true && rifle1Avail == true)
				{
					equipPistol1 = false;
					equipRifle1 = true;
					if (sniper1Avail == true)
					{
						equipSniper1 = false;
					}
					if (shotgun1Avail = true)
					{
						equipShotgun1 = false;
					}
				}
			}

			if (boughtShotgun1 == true)
			{
				//If pressed '3', switch to Sniper1
				if (Application::IsKeyPressed('3') && equipShotgun1 == false && reloaded == true && shotgun1Avail == true)
				{
					equipPistol1 = false;
					if (rifle1Avail == true)
					{
						equipRifle1 = false;
					}

					equipShotgun1 = true;

					if (sniper1Avail == true)
					{
						equipSniper1 = false;
					}
				}
			}
			if (boughtSniper1 == true)
			{
				if (Application::IsKeyPressed('4') && equipSniper1 == false && reloaded == true && sniper1Avail == true)
				{
					equipPistol1 = false;
					if (rifle1Avail == true)
					{
						equipRifle1 = false;
					}
					if (shotgun1Avail == true)
					{
						equipShotgun1 = false;
					}
					equipSniper1 = true;
				}
			}
		changeStates();
}


	
void Sp2Scene::bulletRNG(int spray)
{

	/*if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
	{
		storeRand = rand() % ((spray + 2));
	}
	else
	{
		storeRand = rand() % (spray + 1);
	}
	storeRand2 = rand() % 2;
	if (storeRand2 == 1)
	{
		storeRand -= (storeRand + storeRand);
	}
	if (storeRand == 0)
	{

	}
	else
	{
		Camera3::direction.x += storeRand / 10;
	}
	if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
	{
		storeRand = rand() % ((spray + 2));
	}
	else
	{
		storeRand = rand() % (spray + 1);
	}
	storeRand2 = rand() % 2;
	if (storeRand2 == 1)
	{
		storeRand -= (storeRand + storeRand);
	}
	if (storeRand == 0)
	{

	}
	else
	{
		Camera3::direction.y += storeRand / 10;
	}
	if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
	{
		storeRand = rand() % ((spray + 2));
	}
	else
	{
		storeRand = rand() % (spray + 1);
	}
	storeRand2 = rand() % 2;
	if (storeRand2 == 1)
	{
		storeRand -= (storeRand + storeRand);
	}
	if (storeRand == 0)
	{

	}
	else
	{
		Camera3::direction.z += storeRand / 10;
	}*/ 
	double temp = 0;
	int random = 0;
	if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
	{
		random = (rand() % (spray));
		temp = (2*(random+2)*(rand() % 2)) - (random+2);
		Camera3::direction.x += temp / 10;
		random = (rand() % (spray));
		temp = (2 * (random + 2)*(rand() % 2)) - (random + 2);
		Camera3::direction.y += temp / 10;
		random = (rand() % (spray));
		temp = (2 * (random + 2)*(rand() % 2)) - (random + 2);
		Camera3::direction.z += temp / 10;
	}
	else
	{
		random = (rand() % (spray));
		temp = (2 * (random + 1)*(rand() % 2)) - (random + 1);
		std::cout << temp << std::endl;
		Camera3::direction.x += temp / 10;
		random = (rand() % (spray));
		temp = (2 * (random + 1)*(rand() % 2)) - (random + 1);
		Camera3::direction.y += temp / 10;
		random = (rand() % (spray));
		temp = (2 * (random + 1)*(rand() % 2)) - (random + 1);
		Camera3::direction.z += temp / 10;
	}
}
void Sp2Scene::aimBulletRNG(int spray)
{
	/*if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
	{
		storeRand = rand() % (spray);
		storeRand2 = rand() % 2;
		if (storeRand2 == 1)
		{
			storeRand -= (storeRand + storeRand);
		}
		if (storeRand == 0)
		{

		}
		else
		{
			Camera3::direction.x += storeRand / 10;
		}
	}
	if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
	{
		storeRand = rand() % (spray);
		storeRand2 = rand() % 2;
		if (storeRand2 == 1)
		{
			storeRand -= (storeRand + storeRand);
		}
		if (storeRand == 0)
		{

		}
		else
		{
			Camera3::direction.y += storeRand / 10;
		}
		if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
		{
			storeRand = rand() % (spray);
			storeRand2 = rand() % 2;
			if (storeRand2 == 1)
			{
				storeRand -= (storeRand + storeRand);
			}
			if (storeRand == 0)
			{

			}
			else
			{
				Camera3::direction.z += storeRand / 10;
			}
		}
	}*/
	double temp = 0;
	int random = 0;
	random = (rand() % (spray));
	temp = (2 * (random)*(rand() % 2)) - (random);
	Camera3::direction.x += temp / 10;
	random = (rand() % (spray));
	temp = (2 * (random)*(rand() % 2)) - (random);
	Camera3::direction.y += temp / 10;
	random = (rand() % (spray));
	temp = (2 * (random)*(rand() % 2)) - (random);
	Camera3::direction.z += temp / 10;
}

void Sp2Scene::bulletPos()
{

	std::vector<Vector3>::iterator count = shotsFired.begin();
	std::vector<Vector3>::iterator count1 = shotsDir.begin();
	std::vector<int>::iterator count2 = weaponDmg.begin();


	//testHB = c3.bulletEnemyCollision(store, (50, 1, 0));

		while (count != shotsFired.end())
		{
			Vector3 temp = *count;
			Vector3 temp1 = *count1;
			*count += *count1;
			//std::cout << temp << "look at me" << std::endl;
			if (bulletEnemyCollision(temp, Vector3(50, 10, 0)) == true && t.isDead == false )
			{
				t.hp -= *count2;
				testHB = true;
				std::cout << "hit" << std::endl;
				count = shotsFired.erase(count);
				count1 = shotsDir.erase(count1);
				count2 = weaponDmg.erase(count2);
			}
			else if (temp.y <= 0 || temp.x >= 1000 || temp.z >= 1000 || temp.y >= 1000 || temp.x <= -1000 || temp.z <= -1000 || temp.y <= -1000 || bulletObjectCollision(temp) == true)
			{
				count = shotsFired.erase(count);
				count1 = shotsDir.erase(count1);
				count2 = weaponDmg.erase(count2);
			}
			else
			{
				*count++;
				*count1++;
				*count2++;
			}
		}
}

bool Sp2Scene::bulletObjectCollision(Vector3 bulletPos) 
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
		if (bulletPos.x > minTemp.x && bulletPos.x < maxTemp.x && bulletPos.y > minTemp.y && bulletPos.y < maxTemp.y && bulletPos.z > minTemp.z && bulletPos.z < maxTemp.z)
		{
			return true;
		}
		*count++;
		*count1++;
	}
	return false;
}

void Sp2Scene::changeStates()
{
	if (Camera3::inBase == true)
	{
		gameStates = states::base;
	}
	else if (Camera3::inBase == false)
	{
		gameStates = states::outside;
	}
}

bool Sp2Scene::bulletEnemyCollision(Vector3 bulletPos, Vector3 targetLocation)
{
	if (bulletPos.x > (targetLocation.x - (6 / 2)) && bulletPos.x < (targetLocation.x + (6 / 2)) &&
		bulletPos.y >(targetLocation.y - (6 / 2)) && bulletPos.y < (targetLocation.y + (6 / 2)) &&
		bulletPos.z >(targetLocation.z - (6 / 2)) && bulletPos.z < (targetLocation.z + (6 / 2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Sp2Scene::RenderSkybox()
{
	//bottom
	modelStack.PushMatrix();
	modelStack.Translate(0 + Camera3::location.x, -499 + Camera3::location.y, 0 + Camera3::location.z);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Scale(-1, 1, 1);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	//top
	modelStack.PushMatrix();
	modelStack.Translate(0 + Camera3::location.x, 499 + Camera3::location.y, 0 + Camera3::location.z);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Scale(1, -1, 1);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	//left
	modelStack.PushMatrix();
	modelStack.Translate(499 + Camera3::location.x, 0 + Camera3::location.y, 0 + Camera3::location.z);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Rotate(-90, 0, 0, 1);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	//right
	modelStack.PushMatrix();
	modelStack.Translate(-499 + Camera3::location.x, 0 + Camera3::location.y, 0 + Camera3::location.z);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	//back
	modelStack.PushMatrix();
	modelStack.Translate(0 + Camera3::location.x, 0 + Camera3::location.y, 499 + Camera3::location.z);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	//Front
	modelStack.PushMatrix();
	modelStack.Translate(0 + Camera3::location.x, 0 + Camera3::location.y, -499 + Camera3::location.z);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
}

void Sp2Scene::RenderText(Mesh * mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);

	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);


}

void Sp2Scene::RenderTextOnScreen(Mesh * mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);

	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Sp2Scene::RenderImageOnScreen(Mesh * mesh, float size, float x, float y)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	//modelStack.Rotate(20, 1, 0, 0);
	modelStack.Translate(x, y, 1);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Sp2Scene::RenderMesh(Mesh *mesh, bool enablelight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enablelight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}

	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


void Sp2Scene::RenderSpaceshipQuad()
{
	//spaceship floor 1
	modelStack.PushMatrix();
	modelStack.Translate(400, 60, 0);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship floor 2
	modelStack.PushMatrix();
	modelStack.Translate(400, 60, 100);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship floor 3
	modelStack.PushMatrix();
	modelStack.Translate(300, 60, 0);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship floor 4
	modelStack.PushMatrix();
	modelStack.Translate(300, 60, 100);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship wall length 1
	modelStack.PushMatrix();
	modelStack.Translate(450, 110, 0);
	modelStack.Rotate(90,0, 0, 1);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();
	
	//spaceship wall length 2
	modelStack.PushMatrix();
	modelStack.Translate(450, 110, 100);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship wall length 3
	modelStack.PushMatrix();
	modelStack.Translate(250, 110, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship wall length 4
	modelStack.PushMatrix();
	modelStack.Translate(250, 110, 100);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship wall width 1
	modelStack.PushMatrix();
	modelStack.Translate(400, 110, -50);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship wall width 2
	modelStack.PushMatrix();
	modelStack.Translate(400, 110, 150);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship wall width 3 
	modelStack.PushMatrix();
	modelStack.Translate(300, 110, -50);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship wall width 4
	modelStack.PushMatrix();
	modelStack.Translate(300, 110, 150);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship roof 1
	modelStack.PushMatrix();
	modelStack.Translate(400, 160, 0);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();


	//spaceship roof 2
	modelStack.PushMatrix();
	modelStack.Translate(400, 160, 100);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship roof 3
	modelStack.PushMatrix();
	modelStack.Translate(300, 160, 0);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();

	//spaceship roof 4
	modelStack.PushMatrix();
	modelStack.Translate(300, 160, 100);
	modelStack.Scale(100, 1, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], true);
	modelStack.PopMatrix();
};

void Sp2Scene::RenderCraftingPanel()
{
	//crafting panel 
		modelStack.PushMatrix();
		modelStack.Translate(400, 65, -49);
		modelStack.Scale(2, 2, 1);
		RenderMesh(meshList[GEO_CRAFTINGPANEL], true);
		modelStack.PopMatrix();

};

void Sp2Scene::RenderSpaceHouse()
{
	//space house
	modelStack.PushMatrix();
	modelStack.Translate(-400, 0, 0);
	modelStack.Scale(15, 15, 15);
	RenderMesh(meshList[GEO_SPACEHOUSE], true);
	modelStack.PopMatrix();
};

void Sp2Scene::RenderPortal1()
{
	if (gameStates == states::outside)
	{
		//entrance portal in house
		modelStack.PushMatrix();
		modelStack.Translate(-435, 0, 50);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_PORTAL1], true);
		modelStack.PopMatrix();
	}

	if (gameStates == states::base)
	{
		//entrance portal in spaceship
		modelStack.PushMatrix();
		modelStack.Translate(400, 61, 100);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_PORTAL1], true);
		modelStack.PopMatrix();
	}

};

void Sp2Scene::RenderPortal2()
{
	//exit portal in spaceship
	modelStack.PushMatrix();
	modelStack.Translate(400, 61, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_PORTAL2], true);
	modelStack.PopMatrix();
}

void Sp2Scene::RenderTable()
{
	//table in spacehouse
	modelStack.PushMatrix();
	modelStack.Translate(-390, 0, 40);
	modelStack.Scale(2.4, 2.4, 2.4);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();
};

void Sp2Scene::RenderHealthPack()
{
	//healthpack 1
	modelStack.PushMatrix();
	modelStack.Translate(-386, 6, 40);
	modelStack.Scale(2.5, 2.5, 2.5);
	RenderMesh(meshList[GEO_HEALTHPACK], true);
	modelStack.PopMatrix();

	//healthpack 2
	modelStack.PushMatrix();
	modelStack.Translate(-393, 6, 40);
	modelStack.Rotate(-180, 0, 1, 0);
	modelStack.Scale(2.5, 2.5, 2.5);
	RenderMesh(meshList[GEO_HEALTHPACK], true);
	modelStack.PopMatrix();

};

void Sp2Scene::RenderElements()
{
	for (int a = 0; a < 25; ++a)
	{
		modelStack.PushMatrix();
		modelStack.Translate(elementsx[a], -5, elementsz[a]);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_MINERALS], true);
		modelStack.PopMatrix();
	}
	
};

void Sp2Scene::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Mtx44 perspective;
	//perspective.SetToPerspective()
	//Set view matrix using camera settings
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);

	modelStack.LoadIdentity();

	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_CameraSpace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_CameraSpace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else // POINT LIGHT
	{
		Position lightposition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightposition_cameraspace.x);
	}

	RenderMesh(meshList[GEO_AXES], false);

	if (targetReg > 0 && t.isDead == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(50, 10, 0);
		modelStack.Scale(6, 6, 6);
		RenderMesh(meshList[GEO_TARGET], false);
		modelStack.PopMatrix();
	}
	else if (t.isDead == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(50, 10, 0);
		modelStack.Scale(6, 6, 6);
		RenderMesh(meshList[GEO_TARGETHIT], false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	if (gameStates == states::outside)
	{
		RenderSkybox();
		RenderPortal1();
		RenderSpaceHouse();
		RenderTable();
		RenderHealthPack();
		RenderElements();
	}
	if (gameStates == states::base)
	{
		RenderSpaceshipQuad();
		RenderCraftingPanel();
		RenderPortal1();
		RenderPortal2();
	}
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(0, 499, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();


	/*if (Sp2Scene::test5 == true)
	{
		for (int i = 0; i < 1000; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(rainpositionx[i], rainpositiony[i], rainpositionz[i]);
			modelStack.Scale(1, 6, 1);
			RenderMesh(meshList[GEO_HEAD], false);
			modelStack.PopMatrix();
		}
	}*/

	/*for (int i = 0; i < 50; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(treex[i], translateY, treez[i]);
		modelStack.Scale(10, 30, 10);
		RenderMesh(meshList[GEO_DEADTREE], true);
		modelStack.PopMatrix();
	}*/

	/*modelStack.PushMatrix();
	modelStack.Translate(0, -50, 0);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Scale(-1, 1, 1);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();*/

	//Moon texture 
	
	if (gameStates == states::outside)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0);
		modelStack.Scale(2000, 2000, 2000);
		RenderMesh(meshList[GEO_QUAD], true);
		modelStack.PopMatrix();
		for (std::vector<Vector3>::iterator count = shotsFired.begin(); count != shotsFired.end(); ++count)
		{
			test = *count;
			modelStack.PushMatrix();
			modelStack.Translate(test.x, test.y, test.z);
			modelStack.Scale(0.3, 0.3, 0.3);
			RenderMesh(meshList[GEO_SHOT], false);
			modelStack.PopMatrix();
		}

		if (reloaded == false)
		{
			modelStack.PushMatrix();
			RenderTextOnScreen(meshList[GEO_TEXT], "RELOADING", Color(0, 1, 0), 5, 4, 8);
			modelStack.PopMatrix();
		}

		/*if (testPortalsign == true)
		{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "PRESS E TO TELEPORT", Color(0, 1, 0), 3, 4, 15);
		}*/

		if (Application::IsKeyPressed(VK_RBUTTON) || reloaded == false || Application::IsKeyPressed(VK_SHIFT))
		{
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "+", Color(0.3, 0.8, 0.3), 5, 8.28, 6);
		}
	}

		if (equipPistol1 == true)
		{
			RenderPistol1();
			RenderImageOnScreen(meshList[GEO_PISTOL1], 0.5, 25, 15);
			//RenderImageOnScreen(meshList[GEO_CRAFT_UI], 4, 10, 5);
		}
		if (equipPistol2 == true)
		{
			RenderPistol2();
			RenderImageOnScreen(meshList[GEO_PISTOL2], 0.5, 25, 15);
		//RenderImageOnScreen(meshList[GEO_CRAFT_UI], 4, 10, 5);
	}
		else if (equipRifle1 == true)
		{
			RenderRifle1();
			RenderImageOnScreen(meshList[GEO_RIFLE1], 4, 4, 2.5);
		}
		else if (equipSniper1 == true)
		{
			RenderSniper1();
			RenderImageOnScreen(meshList[GEO_SNIPER1], 1, 15, 10);

		}
		else if (equipShotgun1 == true)
		{
			RenderShotgun1();
			RenderImageOnScreen(meshList[GEO_SHOTGUN1], 0.5, 26, 18);
		}

	if (gameStates == states::base)
	{
		if (crafting == true)
		{
			RenderImageOnScreen(meshList[GEO_CRAFT_UI], 4, 10, 5);
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(-150, 50, -200);
	modelStack.Scale(10, 10, 10);
	RenderText(meshList[GEO_TEXT], "Welcome To The Moon", Color(1, 0, 0));
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(0,5,0);
	//modelStack.Scale(1, 1, 1);
	//RenderMesh(meshList[GEO_TEST], true);
	//modelStack.PopMatrix();

	//Check if button has pressed
	
	
	
	//Render Frame rate on screen
	RenderTextOnScreen(meshList[GEO_TEXT], "FPS: " + std::to_string(framerate), Color(1, 0, 0), 2, 1, 25);

	//Render camera position on screen
	std::ostringstream oss;
	oss << "X: " << camera.position.x << " Y: " << camera.position.y << "Z: " << camera.position.z;
	string var = oss.str();
	RenderTextOnScreen(meshList[GEO_TEXT], oss.str(), Color(0, 1, 0), 2, 2, 28);


}
void Sp2Scene::RenderPistol1()
{
	if (gunReload <= 0)
	{
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
		modelStack.Rotate(rotateGunY, 0, 1, 0);
		modelStack.Rotate(rotateGunX, 1, 0, 0);
		modelStack.Scale(0.5, 0.5, 0.5);


		modelStack.PushMatrix();
		
		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base)
		{
			modelStack.Translate(0, -10, -10);
			modelStack.Rotate(-60, 0, 1, 0);
		}
		else if (Application::IsKeyPressed(VK_RBUTTON) == true)
		{
			modelStack.Translate(0, -7.1, -10);
			modelStack.Rotate(-90, 0, 1, 0);
		}
		else
		{
			modelStack.Translate(5, -10, -15);
			modelStack.Rotate(-90, 0, 1, 0);
		}
		RenderMesh(meshList[GEO_PISTOL1], true);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}
	if (pis.ammo < 5)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(pis.ammo), Color(.7, 0, 0), 3, 18, 1);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(pis.ammo), Color(0, .7, 0), 3, 18, 1);
		modelStack.PopMatrix();
	}
}
	else
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "+", Color(0.3, 0.8, 0.3), 5, 8.28, 6);
	}

}
void Sp2Scene::RenderPistol2()
{
	if (gunReload <= 0)
	{
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
		modelStack.Rotate(rotateGunY, 0, 1, 0);
		modelStack.Rotate(rotateGunX, 1, 0, 0);
		modelStack.Scale(0.5, 0.5, 0.5);


		modelStack.PushMatrix();

		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base)
		{
			modelStack.Translate(0, -10, -10);
			modelStack.Rotate(-60, 0, 1, 0);
		}
		else if (Application::IsKeyPressed(VK_RBUTTON) == true)
		{
			modelStack.Translate(0, -7.1, -10);
			modelStack.Rotate(-90, 0, 1, 0);
		}
		else
		{
			modelStack.Translate(5, -10, -15);
			modelStack.Rotate(-90, 0, 1, 0);
		}
		RenderMesh(meshList[GEO_PISTOL2], true);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}
	if (pis.ammo < 5)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(pis.ammo), Color(.7, 0, 0), 3, 18, 1);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(pis.ammo), Color(0, .7, 0), 3, 18, 1);
		modelStack.PopMatrix();
	}
}
void Sp2Scene::RenderRifle1()
{
	if (gunReload <= 0)
	{
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
		modelStack.Rotate(rotateGunY, 0, 1, 0);
		modelStack.Rotate(rotateGunX, 1, 0, 0);
	
		modelStack.PushMatrix();
		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base)
		{
			modelStack.Translate(0, -10, -10);
		}
		else if (Application::IsKeyPressed(VK_RBUTTON) == true)
		{
			modelStack.Translate(0, -8.1, -10);
		}
		else
		{
			modelStack.Translate(5, -10, -15);
		}
		modelStack.Translate(0, 5, 0);
		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base)
		{
			modelStack.Rotate(-60, 0, 1, 0);
		}
		else
		{
			modelStack.Rotate(-90, 0, 1, 0);
		}
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_RIFLE1], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
	if (rif.ammo < 11)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(rif.ammo), Color(.7, 0, 0), 3, 18, 1);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(rif.ammo), Color(0, .7, 0), 3, 18, 1);
		modelStack.PopMatrix();
	}
}
void Sp2Scene::RenderSniper1()
{
	if (gunReload <= 0)
	{
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
		modelStack.Rotate(rotateGunY, 0, 1, 0);
		modelStack.Rotate(rotateGunX, 1, 0, 0);

		modelStack.PushMatrix();
		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base)
		{
			modelStack.Translate(0, -10, -15);
			modelStack.Translate(0, 1.5, 0);
			modelStack.Rotate(-60, 0, 1, 0);
			modelStack.Scale(2, 2, 2);
			RenderMesh(meshList[GEO_SNIPER1], true);
		}
		else if (Application::IsKeyPressed(VK_RBUTTON) == true)
		{

			modelStack.Translate(0, -7.1, -10);
			//modelStack.Translate(0, -7, -10);
			RenderImageOnScreen(meshList[GEO_SCOPE], 10, 4, 3);
		}
		else
		{
			modelStack.Translate(5, -10, -15);
			modelStack.Translate(0, 1.5, 0);
			modelStack.Rotate(-90, 0, 1, 0);
			modelStack.Scale(2, 2, 2);
			RenderMesh(meshList[GEO_SNIPER1], true);
			
		}
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}
	if (sr.ammo < 4)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(sr.ammo), Color(0.7, 0, 0), 3, 18, 1);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(sr.ammo), Color(0, 0.7, 0), 3, 18, 1);
		modelStack.PopMatrix();
	}
}
void Sp2Scene::RenderShotgun1()
{
	if (reloaded == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
		modelStack.Rotate(rotateGunY, 0, 1, 0);
		modelStack.Rotate(rotateGunX, 1, 0, 0);

		modelStack.PushMatrix();
		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base)
		{
			modelStack.Translate(0, -10, -15);
		}
		else if (Application::IsKeyPressed(VK_RBUTTON) == true)
		{
			modelStack.Translate(0, -7, -10);
		}
		else
		{
			modelStack.Translate(5, -10, -15);
		}
		modelStack.Translate(0.3, 4.1, 0);
		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base)
		{
			modelStack.Rotate(-60, 0, 1, 0);
		}
		else
		{
			modelStack.Rotate(-90, 0, 1, 0);
		}
		
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_SHOTGUN1], true);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}
	if (sg.ammo < 3)
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(sg.ammo), Color(0.7, 0, 0), 3, 18, 1);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(sg.ammo), Color(0, 0.7, 0), 3, 18, 1);
		modelStack.PopMatrix();
	}
}
//void Sp2Scene::RenderShotgun1()
//{
//	if (gunReload <= 0)
//	{
//		modelStack.PushMatrix();
//		modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
//		modelStack.Rotate(rotateGunY, 0, 1, 0);
//		modelStack.Rotate(rotateGunX, 1, 0, 0);
//
//		modelStack.PushMatrix();
//		if (Application::IsKeyPressed(VK_RBUTTON) == true)
//		{
//			modelStack.Translate(0, -7, -10);
//		}
//		else
//		{
//			modelStack.Translate(5, -10, -15);
//		}
//		modelStack.Translate(0, 1.5, 0);
//		modelStack.Rotate(-90, 0, 1, 0);
//		modelStack.Scale(2, 2, 2);
//		RenderMesh(meshList[GEO_SHOTGUN1], true);
//		modelStack.PopMatrix();
//
//		modelStack.PopMatrix();
//	}
//	modelStack.PushMatrix();
//	RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(sr.ammo), Color(0, 0.8, 0), 2, 30, 1);
//	modelStack.PopMatrix();
//}
void Sp2Scene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}