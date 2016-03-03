#include "Sp2Scene.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Utility.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include "enemy.h"
#include "player.h"
#include "objective.h"
#include "gun2.h"
#include <iostream>

using std::cout;

Camera3 c3;
bool Camera3::mouseControl = true;
bool Camera3::inBase = false;
bool Camera3::inMenu = true;
bool Camera3::knockback = false;
bool Camera3::scopedWep = false;
double Camera3::xpos = 0;
double Camera3::ypos = 0;
double Camera3::recoil = 0;
Vector3 Camera3::location = (0, 0, 0);
Vector3 Camera3::location2 = (0, 0, 0);
Vector3 Camera3::direction = (0, 0, 0);
Vector3 enemy::kB = (0, 0, 0);
int enemy::attackPow = 1;
int boss::attackPow = 5;
pistol pis;
rifle rif;
sniper sr;
shotgun sg;
target t;
enemy en;
player play;
objective obj;
int objective::chooseObj = 0;
std::vector<Vector3> Camera3::MineralVectors;
enemy thecube(10, -100.f, 0, -100.f, 50);
enemy thecube2(10, 200.f, 0, -300.f, 50);
enemy thecube3(10, -300.f, 0, 200.f, 50);
boss theboss(1000, -300.f, 0, 300.f, 50);


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
	atkCd = 0;
	buttonCd = 0;
	heals = 2;
	gameStates = states::menu;
	buttonCd = 0;
	ingame = false;


	//Initialize camera settings
	camera.Init(Vector3(0, 10, 40), Vector3(1, 10, 0), Vector3(0, 1, 0));

	
	myWeapon[PISTOL1].damage = 1;
	myWeapon[PISTOL1].RoF = 20;
	myWeapon[PISTOL1].ammo = 12;
	myWeapon[PISTOL1].maxAmmo = 12;
	myWeapon[PISTOL1].reloadSpd = 100;
	myWeapon[PISTOL1].inAccuracy = 1;

	myWeapon[RIFLE1].damage = 2;
	myWeapon[RIFLE1].RoF = 10;
	myWeapon[RIFLE1].ammo = 30;
	myWeapon[RIFLE1].maxAmmo = 30;
	myWeapon[RIFLE1].reloadSpd = 150;
	myWeapon[RIFLE1].inAccuracy = 2;

	myWeapon[SNIPER1].damage = 5;
	myWeapon[SNIPER1].RoF = 30;
	myWeapon[SNIPER1].ammo = 10;
	myWeapon[SNIPER1].maxAmmo = 10;
	myWeapon[SNIPER1].reloadSpd = 200;
	myWeapon[SNIPER1].inAccuracy = 3;

	myWeapon[SHOTGUN1].damage = 1;
	myWeapon[SHOTGUN1].RoF = 30;
	myWeapon[SHOTGUN1].ammo = 8;
	myWeapon[SHOTGUN1].maxAmmo = 8;
	myWeapon[SHOTGUN1].reloadSpd = 50;
	myWeapon[SHOTGUN1].inAccuracy = 2;

	CurrentWeapon = PISTOL1;
	myWeapon[CurrentWeapon];


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
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	//Crafting Ui image
	meshList[GEO_CRAFT_UI] = MeshBuilder::GenerateOBJ("craftin_UI","OBJ//CraftingUI.obj");
	meshList[GEO_CRAFT_UI]->textureID = LoadTGA("Image//CraftingUI.tga");

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

	meshList[GEO_HEALTHUI] = MeshBuilder::GenerateOBJ("healthui", "OBJ//healthUI.obj");
	meshList[GEO_HEALTHUI]->textureID = LoadTGA("Image//healthUV.tga");

	meshList[GEO_HUD] = MeshBuilder::GenerateOBJ("hud", "OBJ//HUDUV.obj");
	meshList[GEO_HUD]->textureID = LoadTGA("Image//HUDUV.tga");

	//Minerals Image
	meshList[GEO_MINERALS] = MeshBuilder::GenerateOBJ("gold_mineral", "OBJ//GoldMineral.obj");
	meshList[GEO_MINERALS]->textureID = LoadTGA("Image//GoldMineralsUV.tga");


	meshList[GEO_MENUPORTAL] = MeshBuilder::GenerateOBJ("menuportal", "OBJ//ScenePortal.obj");
	meshList[GEO_MENUPORTAL]->textureID = LoadTGA("Image//ScenePortalUV.tga");

	meshList[GEO_SPIRAL] = MeshBuilder::GenerateOBJ("spiral", "OBJ//spiral.obj");
	meshList[GEO_SPIRAL]->textureID = LoadTGA("Image//Spiral1UV.tga");
	
	meshList[GEO_SCOPE] = MeshBuilder::GenerateOBJ("scopemodel", "OBJ//Scope.obj");
	meshList[GEO_SCOPE]->textureID = LoadTGA("Image//Scope.tga");

	meshList[GEO_SHOT] = MeshBuilder::GenerateSphere("shot", Color(0.3,0.3,1), 10,20);

	meshList[GEO_PISTOL1] = MeshBuilder::GenerateOBJ("pistol1model", "OBJ//pistol1.obj");
	meshList[GEO_PISTOL1]->textureID = LoadTGA("Image//pistol1texture.tga");


	meshList[GEO_RIFLE1] = MeshBuilder::GenerateOBJ("rifle1model", "OBJ//AR.obj");
	meshList[GEO_RIFLE1]->textureID = LoadTGA("Image//AR_UV.tga");

	meshList[GEO_SNIPER1] = MeshBuilder::GenerateOBJ("sniper1model", "OBJ//AK47.obj");
	meshList[GEO_SNIPER1]->textureID = LoadTGA("Image//AK47UV.tga");

	meshList[GEO_SHOTGUN1] = MeshBuilder::GenerateOBJ("shotgun1model", "OBJ//Shotgun.obj");
	meshList[GEO_SHOTGUN1]->textureID = LoadTGA("Image//shotgunUV.tga");


	meshList[GEO_THECUBE] = MeshBuilder::GenerateOBJ("thecube", "OBJ//MaleRobot.obj");
	meshList[GEO_THECUBE]->textureID = LoadTGA("Image//RobotUV.tga");

	meshList[GEO_THEBOSS] = MeshBuilder::GenerateOBJ("theboss", "OBJ//Boss.obj");
	meshList[GEO_THEBOSS]->textureID = LoadTGA("Image//BossUV.tga");

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

for (int a = 0; a < 10; ++a)
{
	elementsx[a] = rand() % 980 - 980;
	elementsz[a] = rand() % 980 - 880;
}

translateY = -60;
translateX = 0;
translateX = 0;

///for hitboxes///
minVectors.push_back(c3.minPos(Vector3(-338, 0, 38), 15, 120, 75));
minVectors.push_back(c3.minPos(Vector3(-460, 0, 38), 15, 120, 75));
minVectors.push_back(c3.minPos(Vector3(-400, 0, 73), 120, 120, 15));
minVectors.push_back(c3.minPos(Vector3(-360, 0, 0), 45, 100, 20));
minVectors.push_back(c3.minPos(Vector3(-435, 0, 0), 40, 120, 15));

maxVectors.push_back(c3.maxPos(Vector3(-338, 0, 38), 15, 120, 75));
maxVectors.push_back(c3.maxPos(Vector3(-460, 0, 38), 15, 120, 75));
maxVectors.push_back(c3.maxPos(Vector3(-400, 0, 73), 120, 120, 15));
maxVectors.push_back(c3.maxPos(Vector3(-360, 0, 0), 45, 100, 20));
maxVectors.push_back(c3.maxPos(Vector3(-435, 0, 0), 40, 120, 15));
}

static float LSPEED = 10.f;
static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void Sp2Scene::Update(double dt)
{
	camera.Update(dt);
	if (buttonCd > 0)
	{
		buttonCd--;
	}
	if (gameStates == states::menu)
	{
		thecube.~enemy();
		thecube2.~enemy();
		thecube3.~enemy();
		theboss.~boss();
		if (c3.startKillGM() == true)
		{
			PlaySound(TEXT("warp.wav"), NULL, SND_ASYNC);
			camera.Init(Vector3(-400, 10, 50), Vector3(1, 10, 0), Vector3(0, 1, 0));
			Camera3::inMenu = false;
			Camera3::inBase = false;
			objective::chooseObj = 1;
			rotateGunX = 0;
			rotateGunY = 0;
			equipPistol1 = true;
			equipRifle1 = false;
			equipSniper1 = false;
			equipShotgun1 = false;
			boughtRifle1 = false;
			boughtSniper1 = false;
			boughtShotgun1 = false;
			rifle1Avail = false;
			sniper1Avail = false;
			shotgun1Avail = false;
			obj.resetObjective();
			play.earnMinerals(-10000);
			thecube.enemyReset(-100.f, 0, -100.f);
			thecube2.enemyReset(200.f, 0, -300.f);
			thecube3.enemyReset(-300.f, 0, 200.f);
		}
		if (c3.startCollectGM() == true)
		{
			PlaySound(TEXT("warp.wav"), NULL, SND_ASYNC);
			camera.Init(Vector3(-400, 10, 50), Vector3(1, 10, 0), Vector3(0, 1, 0));
			Camera3::inMenu = false;
			Camera3::inBase = false;
			objective::chooseObj = 2;
			rotateGunX = 0;
			rotateGunY = 0;
			equipPistol1 = true;
			equipRifle1 = false;
			equipSniper1 = false;
			equipShotgun1 = false;
			boughtRifle1 = false;
			boughtSniper1 = false;
			boughtShotgun1 = false;
			rifle1Avail = false;
			sniper1Avail = false;
			shotgun1Avail = false;
			obj.resetObjective();
			play.earnMinerals(-10000);
			thecube.enemyReset(-100.f, 0, -100.f);
			thecube2.enemyReset(200.f, 0, -300.f);
			thecube3.enemyReset(-300.f, 0, 200.f);
		}
		if (c3.startBossGM() == true)
		{
			PlaySound(TEXT("warp.wav"), NULL, SND_ASYNC);
			camera.Init(Vector3(-400, 10, 50), Vector3(1, 10, 0), Vector3(0, 1, 0));
			Camera3::inMenu = false;
			Camera3::inBase = false;
			objective::chooseObj = 3;
			rotateGunX = 0;
			rotateGunY = 0;
			equipPistol1 = true;
			equipRifle1 = false;
			equipSniper1 = false;
			equipShotgun1 = false;
			boughtRifle1 = false;
			boughtSniper1 = false;
			boughtShotgun1 = false;
			rifle1Avail = false;
			sniper1Avail = false;
			shotgun1Avail = false;
			obj.resetObjective();
			play.earnMinerals(-10000);
			thecube.enemyReset(-100.f, 0, -100.f);
			thecube2.enemyReset(200.f, 0, -300.f);
			thecube3.enemyReset(-300.f, 0, 200.f);
			theboss.bossReset(-300, 0, 300);
		}
	}

	if (gameStates == states::base)
	{
		if (camera.craftUi() == true && Application::IsKeyPressed('E') && crafting == false && buttonCd == 0)
		{
			crafting = true;
			buttonCd = 30;
		}
		else if (Application::IsKeyPressed('E') && crafting == true && buttonCd == 0 || camera.craftUi() == false)
		{
			crafting = false;
			buttonCd = 30;
		}

		if (camera.craftUi() == true)
		{
			if (Application::IsKeyPressed('2') && crafting == true && play.getMinerals() >= 1000 && boughtRifle1 == false)
			{
				PlaySound(TEXT("pay.wav"), NULL, SND_ASYNC);
				boughtRifle1 = true;
				rifle1Avail = true;
				play.spendMinerals(1000);
			}
			if (Application::IsKeyPressed('3') && crafting == true && play.getMinerals() >= 2000 && boughtShotgun1 == false)
			{
				PlaySound(TEXT("pay.wav"), NULL, SND_ASYNC);
				boughtShotgun1 = true;
				shotgun1Avail = true;
				play.spendMinerals(2000);
			}
			if (Application::IsKeyPressed('4') && crafting == true && play.getMinerals() >= 3000 && boughtSniper1 == false)
			{
				PlaySound(TEXT("pay.wav"), NULL, SND_ASYNC);
				boughtSniper1 = true;
				sniper1Avail = true;
				play.spendMinerals(3000);
			}
		}
		if (Camera3::xpos < 399)
		{
			rotateGunY += 2;
		}
		if (Camera3::xpos > 401)
		{
			rotateGunY -= 2;
		}
		if (Camera3::ypos < 299)
		{
			gunDir += 2;
		}
		if (Camera3::ypos > 301)
		{
			gunDir -= 2;
		}

		if (gunDir > 30)
		{
			gunDir = 30;
		}
		else if (gunDir < -30)
		{
			gunDir = -30;
		}
	}

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

	rotateAngle += (float)(10 * dt);
	planet1RotAngle += (float)(5 * dt);
	planet1RevAngle += (float)(2 * dt);
	moon1RotAngle += (float)(50 * dt);


	framerate = 1 / dt;

	changeStates();
	if (gameStates == states::outside)
	{
		xDist = Camera3::location.x - (-400);
		zDist = Camera3::location.z - (50);
		totalTranslateDist = (pow(xDist, 2.0)) + (pow(zDist, 2.0));

		baseDist = sqrt(totalTranslateDist);
		if (equipSniper1 == true)
		{
			Camera3::scopedWep = true;
		}
		if (Application::IsKeyPressed('E') && c3.checkcollisionwithOres() == true && buttonCd == 0)
		{
			PlaySound(TEXT("pay.wav"), NULL, SND_ASYNC);
			play.earnMinerals(500);
			if (objective::chooseObj == 2)
			{
				obj.objectiveProgress(1);
			}
			buttonCd = 30;
		}
		if (Camera3::location.x > -410 && Camera3::location.x < -370 && Camera3::location.y > 0 && Camera3::location.y < 20 && Camera3::location.z > 20 && Camera3::location.z < 60 && Application::IsKeyPressed('E') && heals > 0 && play.getHp() < 100 && buttonCd == 0)
		{
			play.healHp(50);
			PlaySound(TEXT("heal.wav"), NULL, SND_ASYNC);
			heals--;
			buttonCd = 30;
		}
		if (play.getHp() > 0)
		{
			if (atkCd <= 0)
			{
				if (objective::chooseObj != 3)
				{
					EnemyAttack(thecube.pos);
					EnemyAttack(thecube2.pos);
					EnemyAttack(thecube3.pos);
				}
				else
				{
					bossAttack(theboss.pos);
				}

				atkCd = 30;
			}
			if (atkCd > 0)
			{
				atkCd--;
			}

			//If pressed '1', switch to Pistol1
			if (equipPistol1 == true)
			{
				if (Camera3::location.x < -330 && Camera3::location.x > -470 && Camera3::location.z > -10 && Camera3::location.z < 80)
				{

				}
				else
				{
					if (Application::IsKeyPressed(VK_LBUTTON) && gunCd <= 0 && pis.ammo > 0 && gunReload <= 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
					{
						PlaySound(TEXT("shoot.wav"), NULL, SND_ASYNC);
						bulletRNG(pis.inAccuracy);
						shotsFired.push_back(Camera3::location2);
						shotsDir.push_back(Camera3::direction);
						weaponDmg.push_back(pis.damage);
						gunCd = pis.RoF;
						pis.ammo--;
						Camera3::recoil += 1;
						upRecoil += 1;
					}
				}

				bulletPos();

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
				if (Camera3::location.x < -330 && Camera3::location.x > -470 && Camera3::location.z > -10 && Camera3::location.z < 80)
				{

				}
				else
				{
					if (Application::IsKeyPressed(VK_LBUTTON) && gunCd <= 0 && rif.ammo > 0 && gunReload <= 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
					{
						PlaySound(TEXT("shoot.wav"), NULL, SND_ASYNC);
						bulletRNG(rif.inAccuracy);
						shotsFired.push_back(Camera3::location2);
						shotsDir.push_back(Camera3::direction);
						weaponDmg.push_back(rif.damage);
						gunCd = rif.RoF;
						rif.ammo--;
						Camera3::recoil += 0.5;
						upRecoil += 0.5;
					}
				}

					bulletPos();

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
					if (Camera3::location.x < -330 && Camera3::location.x > -470 && Camera3::location.z > -10 && Camera3::location.z < 80)
					{

					}
					else
					{
						if (Application::IsKeyPressed(VK_LBUTTON) && gunCd <= 0 && sr.ammo > 0 && gunReload <= 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
						{
							PlaySound(TEXT("shoot.wav"), NULL, SND_ASYNC);
							if (Application::IsKeyPressed(VK_RBUTTON))
							{
								bulletRNG(1);
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
					}

					bulletPos();

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
						if (Camera3::location.x < -330 && Camera3::location.x > -470 && Camera3::location.z > -10 && Camera3::location.z < 80)
						{

						}
						else
						{
							if (Application::IsKeyPressed(VK_LBUTTON) && gunCd <= 0 && sg.ammo > 0 && gunReload <= 0 && reloaded == true && Application::IsKeyPressed(VK_SHIFT) == false)
							{
								PlaySound(TEXT("shoot.wav"), NULL, SND_ASYNC);
								for (int shots = 0; shots < 6; ++shots)
								{
									bulletRNG(sg.inAccuracy);
									shotsFired.push_back(Camera3::location2);
									shotsDir.push_back(Camera3::direction);
									weaponDmg.push_back(sg.damage);
								}
								gunCd = sg.RoF;
								sg.ammo--;
								Camera3::recoil += 2;
								upRecoil += 2;
							}
						}
					}

						bulletPos();

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
				if (Camera3::location.x < -330 && Camera3::location.x > -470 && Camera3::location.z > -10 && Camera3::location.z < 80)
				{
				}
				else
				{
					if (objective::chooseObj != 3)
					{
						if (thecube.isDead() == false && gameStates == states::outside)
						{
							thecube.Update(dt, camera);
						}
						else
						{
							if (objective::chooseObj == 1)
							{
								obj.objectiveProgress(1);
							}
							play.earnMinerals(100);
							thecube.respawnEnemy(rand() % 1960 - 980, 0, rand() % 1960 - 980);
						}
						if (thecube2.isDead() == false && gameStates == states::outside)
						{
							thecube2.Update(dt, camera);
						}
						else
						{
							if (objective::chooseObj == 1)
							{
								obj.objectiveProgress(1);
							}
							play.earnMinerals(100);
							thecube2.respawnEnemy(rand() % 1960 - 980, 0, rand() % 1960 - 980);
						}
						if (thecube3.isDead() == false && gameStates == states::outside)
						{
							thecube3.Update(dt, camera);
						}
						else
						{
							if (objective::chooseObj == 1)
							{
								obj.objectiveProgress(1);
							}
							play.earnMinerals(100);
							thecube3.respawnEnemy(rand() % 1960 - 980, 0, rand() % 1960 - 980);
						}
					}
					if (objective::chooseObj == 3)
					{
						if (theboss.hp < 500 && boss50 == false)
						{
							boss50 = true;
							theboss.upgradeboss();
						}
						else if (theboss.hp < 200 && boss20 == false)
						{
							boss20 = true;
							theboss.upgradeboss();
						}
						if (theboss.isDead() == false && gameStates == states::outside)
						{
							theboss.Update(dt, camera);
						}
						else if (theboss.isDead() == true)
						{
							obj.objectiveProgress(1);
						}
					}
					
				}
		}
		if (Camera3::mouseControl == true)
		{
			if (Application::IsKeyPressed(VK_RBUTTON))
			{
				if (Camera3::xpos < 399)
				{
					rotateGunY += 0.5;
				}
				if (Camera3::xpos > 401)
				{
					rotateGunY -= 0.5;
				}
				if (Camera3::ypos < 299)
				{
					gunDir += 0.5;
				}
				if (Camera3::ypos > 301)
				{
					gunDir -= 0.5;
				}
				if (gunDir > 30)
				{
					gunDir = 30;
				}
				else if (gunDir < -30)
				{
					gunDir = -30;
				}
			}
			else
			{
				if (Camera3::xpos < 399)
				{
					rotateGunY += 2;
				}
				if (Camera3::xpos > 401)
				{
					rotateGunY -= 2;
				}
				if (Camera3::ypos < 299)
				{
					gunDir += 2;
				}
				if (Camera3::ypos > 301)
				{
					gunDir -= 2;
				}

				if (gunDir > 30)
				{
					gunDir = 30;
				}
				else if (gunDir < -30)
				{
					gunDir = -30;
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
	}
	rotateGunX = gunDir + upRecoil;
	if (rotateGunX > 30)
	{
		rotateGunX == 30;
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
	if (equipPistol1 == true)
	{
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
}

bool Sp2Scene::AICheckCollisionObject(Vector3 AIposition)
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

		if (AIposition.x > minTemp.x && AIposition.x < maxTemp.x && AIposition.y > minTemp.y && AIposition.y < maxTemp.y && AIposition.z > minTemp.z && AIposition.z < maxTemp.z)
		{
			return true;
		}
		*count++;
		*count1++;
	}
	return false;
}
void Sp2Scene::bulletRNG(int spray)
{
	double temp = 0;
	int random = 0;
	if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
	{
		random = (rand() % (spray));
		temp = (2*(random+1)*(rand() % 2)) - (random+1);
		Camera3::direction.x += temp / 10;
		random = (rand() % (spray));
		temp = (2 * (random + 1)*(rand() % 2)) - (random + 1);
		Camera3::direction.y += temp / 10;
		random = (rand() % (spray));
		temp = (2 * (random + 1)*(rand() % 2)) - (random + 1);
		Camera3::direction.z += temp / 10;
	}
	else
	{
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
}

void Sp2Scene::bulletPos()
{

	std::vector<Vector3>::iterator count = shotsFired.begin();
	std::vector<Vector3>::iterator count1 = shotsDir.begin();
	std::vector<int>::iterator count2 = weaponDmg.begin();



		while (count != shotsFired.end())
		{
			Vector3 temp = *count;
			Vector3 temp1 = *count1;
			*count += *count1;
			if (bulletEnemyCollision(temp, thecube.pos) == true)
			{
				thecube.hp -= *count2;
				count = shotsFired.erase(count);
				count1 = shotsDir.erase(count1);
				count2 = weaponDmg.erase(count2);
			}
			else if (bulletEnemyCollision(temp, thecube2.pos) == true)
			{
				thecube2.hp -= *count2;
				count = shotsFired.erase(count);
				count1 = shotsDir.erase(count1);
				count2 = weaponDmg.erase(count2);
			}
			else if (bulletEnemyCollision(temp, thecube3.pos) == true)
			{
				thecube3.hp -= *count2;
				count = shotsFired.erase(count);
				count1 = shotsDir.erase(count1);
				count2 = weaponDmg.erase(count2);
			}
			else if (bulletBossCollision(temp, theboss.pos) == true)
			{
				theboss.hp -= *count2;
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
	if (Camera3::inMenu == true)
	{
		gameStates = states::menu;
	}
	else if (Camera3::inBase == true && Camera3::inMenu == false)
	{
		gameStates = states::base;
	}
	else if (Camera3::inBase == false && Camera3::inMenu == false)
	{
		gameStates = states::outside;
	}
	
}

bool Sp2Scene::bulletEnemyCollision(Vector3 bulletPos, Vector3 targetLocation)
{
	targetLocation.y += 10;
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

bool Sp2Scene::bulletBossCollision(Vector3 bulletPos, Vector3 targetLocation)
{
	targetLocation.y += 10;
	if (bulletPos.x > (targetLocation.x - (80 / 2)) && bulletPos.x < (targetLocation.x + (80 / 2)) &&
		bulletPos.y >(targetLocation.y - (120 / 2)) && bulletPos.y < (targetLocation.y + (120 / 2)) &&
		bulletPos.z >(targetLocation.z - (30 / 2)) && bulletPos.z < (targetLocation.z + (30 / 2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Sp2Scene::EnemyAttack(Vector3 targetLocation)
{
	if (Camera3::location.x > (targetLocation.x - (30 / 2)) && Camera3::location.x < (targetLocation.x + (30 / 2)) &&
		Camera3::location.y >(targetLocation.y - (30 / 2)) && Camera3::location.y < (targetLocation.y + (30 / 2)) &&
		Camera3::location.z >(targetLocation.z - (30 / 2)) && Camera3::location.z < (targetLocation.z + (30 / 2)))
	{
		PlaySound(TEXT("hurt.wav"), NULL, SND_ASYNC);
		play.getHit(enemy::attackPow);
		Camera3::knockback = true;
	}
}

void Sp2Scene::bossAttack(Vector3 targetLocation)
{
	if (Camera3::location.x >(targetLocation.x - (50 / 2)) && Camera3::location.x < (targetLocation.x + (50 / 2)) &&
		Camera3::location.y >(targetLocation.y - (50 / 2)) && Camera3::location.y < (targetLocation.y + (50 / 2)) &&
		Camera3::location.z >(targetLocation.z - (50 / 2)) && Camera3::location.z < (targetLocation.z + (50 / 2)))
	{
		PlaySound(TEXT("hurt.wav"), NULL, SND_ASYNC);
		play.getHit(boss::attackPow);
		//Camera3::knockback = true;
	}
}

void Sp2Scene::RenderSkybox()
{
	//bottom
	modelStack.PushMatrix();
	modelStack.Translate(0 + Camera3::location.x, -500 + Camera3::location.y, 0 + Camera3::location.z);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Scale(-1, 1, 1);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	//top
	modelStack.PushMatrix();
	modelStack.Translate(0 + Camera3::location.x, 500 + Camera3::location.y, 0 + Camera3::location.z);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Scale(1, -1, 1);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	//left
	modelStack.PushMatrix();
	modelStack.Translate(500 + Camera3::location.x, 0 + Camera3::location.y, 0 + Camera3::location.z);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Rotate(-90, 0, 0, 1);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	//right
	modelStack.PushMatrix();
	modelStack.Translate(-500 + Camera3::location.x, 0 + Camera3::location.y, 0 + Camera3::location.z);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	//back
	modelStack.PushMatrix();
	modelStack.Translate(0 + Camera3::location.x, 0 + Camera3::location.y, 500 + Camera3::location.z);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	//Front
	modelStack.PushMatrix();
	modelStack.Translate(0 + Camera3::location.x, 0 + Camera3::location.y, -500 + Camera3::location.z);
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
void Sp2Scene::RenderImageOnScreen2(Mesh * mesh, float size, float x, float y)
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
	modelStack.Translate(x, y, 1);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
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
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(100, 1000, 100);
	modelStack.Scale(-1, 1, 1);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], false);
	modelStack.PopMatrix();

	//top
	modelStack.PushMatrix();
	modelStack.Translate(0,50, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(100, 1000, 100);
	modelStack.Scale(1, -1, 1);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], false);
	modelStack.PopMatrix();

	//left
	modelStack.PushMatrix();
	modelStack.Translate(50,0,0);
	modelStack.Scale(100, 100, 100);
	modelStack.Rotate(-90, 0, 0, 1);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], false);
	modelStack.PopMatrix();

	//right
	modelStack.PushMatrix();
	modelStack.Translate(-50, 0,0);
	modelStack.Scale(100, 100, 100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], false);
	modelStack.PopMatrix();

	//back
	modelStack.PushMatrix();
	modelStack.Translate(0,0,50);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], false);
	modelStack.PopMatrix();

	//Front
	modelStack.PushMatrix();
	modelStack.Translate(0,0, -50);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_SPACESHIPQUAD], false);
	modelStack.PopMatrix();
};

void Sp2Scene::RenderCraftingPanel()
{
	//crafting panel 
		modelStack.PushMatrix();
		modelStack.Translate(0, 3, -49);
		modelStack.Scale(1.5, 1.5, 1);
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

};

void Sp2Scene::RenderPortal2()
{
	//exit portal in spaceship
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 40);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_PORTAL2], true);
	modelStack.PopMatrix();
}
void Sp2Scene::RenderMenuPortal()
{
	modelStack.PushMatrix();
	modelStack.Translate(-40, 8, 0);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_MENUPORTAL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(40, 8, 0);
	//modelStack.Rotate(90,0,1,0);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_MENUPORTAL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 8, -40);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_MENUPORTAL], true);
	modelStack.PopMatrix();
}
void Sp2Scene::RenderSpiral()
{
	modelStack.PushMatrix();
	modelStack.Translate(-38, 10, 0);
	modelStack.Rotate(rotateAngle, 1, 0, 0);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_SPIRAL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(38, 10, 0);
	modelStack.Rotate(rotateAngle, 1, 0, 0);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_SPIRAL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 10, -38);
	modelStack.Rotate(rotateAngle, 0, 0, 1);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(3, 3, 3);
	RenderMesh(meshList[GEO_SPIRAL], true);
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
	if (heals > 0)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-386, 6, 40);
		modelStack.Scale(2.5, 2.5, 2.5);
		RenderMesh(meshList[GEO_HEALTHPACK], true);
		modelStack.PopMatrix();
	}

	//healthpack 2
	if (heals > 1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-393, 6, 40);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Scale(2.5, 2.5, 2.5);
		RenderMesh(meshList[GEO_HEALTHPACK], true);
		modelStack.PopMatrix();
	}

};

void Sp2Scene::RenderElements()
{
	std::vector<Vector3>::iterator count = Camera3::MineralVectors.begin();

	while (count != Camera3::MineralVectors.end())
	{
		Vector3 temp = *count;
		modelStack.PushMatrix();
		modelStack.Translate(temp.x, 0, temp.z);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_MINERALS], true);
		modelStack.PopMatrix();
		*count++;
	}
	
};

void Sp2Scene::RenderEnemy()
{

		modelStack.PushMatrix();
		modelStack.Translate(thecube.pos.x, 0, thecube.pos.z);
		modelStack.Rotate(thecube.Degree, 0, 1, 0);
		modelStack.PushMatrix();
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_THECUBE], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(thecube2.pos.x, 0, thecube2.pos.z);
		modelStack.Rotate(thecube2.Degree, 0, 1, 0);
		modelStack.PushMatrix();
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_THECUBE], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(thecube3.pos.x, 0, thecube3.pos.z);
		modelStack.Rotate(thecube3.Degree, 0, 1, 0);
		modelStack.PushMatrix();
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_THECUBE], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
}

void Sp2Scene::RenderBoss()
{
	modelStack.PushMatrix();
	modelStack.Translate(theboss.pos.x, -5, theboss.pos.z);
	modelStack.Rotate(theboss.Degree, 0, 1, 0);
	modelStack.PushMatrix();
	modelStack.Scale(theboss.sizeIncrease, theboss.sizeIncrease, theboss.sizeIncrease);
	RenderMesh(meshList[GEO_THEBOSS], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

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

	//RenderMesh(meshList[GEO_AXES], false);

		if (thecube.isDead() == false && gameStates == states::outside && objective::chooseObj != 3)
		{
			RenderEnemy();
		}
		if (theboss.isDead() == false && gameStates == states::outside && objective::chooseObj == 3)
		{
			RenderBoss();
		}
		modelStack.PushMatrix();

		//Starting game State
		//gameStates = states::outside;

		if (gameStates == states::menu)
		{

			RenderSpaceshipQuad();
			RenderMenuPortal();
			RenderSpiral();

			RenderTextOnScreen(meshList[GEO_TEXT], "SPACE SIMULATOR", Color(0.8, 0.8, 0), 4, 4, 11);

			modelStack.PushMatrix();
			modelStack.Translate(40, 25, -12);
			modelStack.Rotate(-90, 0, 1, 0);
			modelStack.Scale(2, 2, 2);
			RenderText(meshList[GEO_TEXT], "EXTERMINATION", Color(1, 1, 1));
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-40, 25, 12);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(2, 2, 2);
			RenderText(meshList[GEO_TEXT], "FIGHT DR OP", Color(1, 1, 1));
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-10, 25, -40);
			modelStack.Scale(2, 2, 2);
			RenderText(meshList[GEO_TEXT], "EXCAVATION", Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		if (gameStates == states::outside)
		{
			RenderSkybox();
			RenderPortal1();
			RenderSpaceHouse();
			RenderTable();
			RenderHealthPack();
			RenderElements();

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

			//Render HUD before anything else
			RenderImageOnScreen(meshList[GEO_HUD], 6, 6.9, 5);

			if (Application::IsKeyPressed(VK_RBUTTON) && equipSniper1 == true || reloaded == false || Application::IsKeyPressed(VK_SHIFT))
			{
			}
			else
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "+", Color(0.3, 0.8, 0.3), 5, 8.28, 6);
			}
			if (Camera3::location.x > -410 && Camera3::location.x < -370 && Camera3::location.y > 0 && Camera3::location.y < 20 && Camera3::location.z > 20 && Camera3::location.z < 60 && heals > 0 && play.getHp() < 100)
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "E TO HEAL", Color(0.3, 0.8, 0.3), 5, 5, 5);
			}
			else if (Camera3::location.x > -410 && Camera3::location.x < -370 && Camera3::location.y > 0 && Camera3::location.y < 20 && Camera3::location.z > 20 && Camera3::location.z < 60 && heals > 0 && play.getHp() == 100)
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "FULL HP", Color(0.8, 0.3, 0.3), 5, 5, 5);
			}
		}
		if (gameStates == states::base)
		{
			RenderSpaceshipQuad();
			RenderCraftingPanel();
			RenderPortal1();
			RenderPortal2();

			if (crafting == true)
			{
				RenderImageOnScreen(meshList[GEO_CRAFT_UI], 4, 10, 5);
			}
			if (camera.craftUi() == true && crafting == false)
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "E TO CRAFT", Color(0.3, 0.8, 0.3), 5, 4, 5);
			}
		}
		if (gameStates == states::outside || gameStates == states::base)
		{
			if (Application::IsKeyPressed('1') && equipPistol1 == false && reloaded == true)
			{
				CurrentWeapon = PISTOL1;
				equipPistol1 = true;
				equipRifle1 = false;
				equipShotgun1 = false;
				equipSniper1 = false;
			}
			if (Application::IsKeyPressed('2') && equipRifle1 == false && reloaded == true && rifle1Avail == true)
			{
				CurrentWeapon = RIFLE1;
				equipPistol1 = false;
				equipRifle1 = true;
				equipShotgun1 = false;
				equipSniper1 = false;
			}
			if (Application::IsKeyPressed('3') && equipShotgun1 == false && reloaded == true && shotgun1Avail == true)
			{
				CurrentWeapon = SHOTGUN1;
				equipPistol1 = false;
				equipRifle1 = false;
				equipShotgun1 = true;
				equipSniper1 = false;
			}
			if (Application::IsKeyPressed('4') && equipSniper1 == false && reloaded == true && sniper1Avail == true)
			{
				CurrentWeapon = SNIPER1;
				equipPistol1 = false;
				equipRifle1 = false;
				equipShotgun1 = false;
				equipSniper1 = true;
			}
			if (equipPistol1 == true)
			{
				RenderPistol1();
				RenderImageOnScreen(meshList[GEO_PISTOL1], 0.5, 130, 15);
			}
			else if (equipRifle1 == true)
			{
				RenderRifle1();
				RenderImageOnScreen(meshList[GEO_RIFLE1], 4, 16, 2.5);
			}
			else if (equipSniper1 == true)
			{
				RenderSniper1();
				RenderImageOnScreen(meshList[GEO_SNIPER1], 1, 65, 10);

			}
			else if (equipShotgun1 == true)
			{
				RenderShotgun1();
				RenderImageOnScreen(meshList[GEO_SHOTGUN1], 0.5, 130, 18);
			}

	if (gameStates == states::base)
	{
		if (crafting == true)
		{
			RenderImageOnScreen(meshList[GEO_CRAFT_UI], 4, 10, 5);
		}
		if (camera.craftUi() == true && crafting == false)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "E TO CRAFT", Color(0.3, 0.8, 0.3), 5, 4, 5);
		}
	}


			RenderImageOnScreen2(meshList[GEO_MINERALS], 2, 3, 2);
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(play.getMinerals()), Color(0.7, 0.7, 0.3), 3, 4, 1.2);

			RenderTextOnScreen(meshList[GEO_TEXT], "Base Dist: " + std::to_string(baseDist), Color(0.7, 0.7, 0.3), 2, 1, 28);

			if (play.getHp() <= 30)
			{
				RenderImageOnScreen2(meshList[GEO_HEALTHUI], 1, 35, 6);
				RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(play.getHp()), Color(0.8, 0.3, 0.3), 3, 14, 2);
			}
			else
			{
				RenderImageOnScreen2(meshList[GEO_HEALTHUI], 1, 35, 6);
				RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(play.getHp()), Color(0.3, 0.8, 0.3), 3, 14, 2);
			}

			if (objective::chooseObj == 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Objective: " + std::to_string(obj.getProgress()) + "/" + std::to_string(obj.getObjective()), Color(0.7, 0.7, 0.3), 2, 25, 29);
				RenderTextOnScreen(meshList[GEO_TEXT], "Enemy Kills", Color(0.7, 0.7, 0.3), 2, 25, 28);
			}
			if (objective::chooseObj == 2)
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Objective: " + std::to_string(obj.getProgress()) + "/" + std::to_string(obj.getObjective()), Color(0.7, 0.7, 0.3), 2, 25, 29);
				RenderTextOnScreen(meshList[GEO_TEXT], "Mineral Deposits", Color(0.7, 0.7, 0.3), 2, 25, 28);
			}
			if (objective::chooseObj == 3)
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Objective: " + std::to_string(theboss.hp) + "/1000" , Color(0.7, 0.7, 0.3), 2, 20, 29);
				RenderTextOnScreen(meshList[GEO_TEXT], "Boss HP", Color(0.7, 0.7, 0.3), 2, 26, 28);
			}
		}

		if (obj.objComplete() == true)
		{
			PlaySound(TEXT("win.wav"), NULL, SND_ASYNC);
			Camera3::inMenu = true;
			showWin = 300;
			camera.Init(Vector3(1, 10, 1), Vector3(1, 10, 0), Vector3(0, 1, 0));
			play.healHp(110);
			objective::chooseObj = 0;
		}
		else if (play.isDead() == true)
		{
			PlaySound(TEXT("lose.wav"), NULL, SND_ASYNC);
			Camera3::inMenu = true;
			showLose = 300;
			camera.Init(Vector3(1, 10, 1), Vector3(1, 10, 0), Vector3(0, 1, 0));
			play.healHp(110);
		}
		else if (Application::IsKeyPressed('P') && Camera3::inMenu == false)
		{
			PlaySound(TEXT("lose.wav"), NULL, SND_ASYNC);
			Camera3::inMenu = true;
			showRQ = 300;
			camera.Init(Vector3(1, 10, 1), Vector3(1, 10, 0), Vector3(0, 1, 0));
			play.healHp(110);
		}

		if (showLose > 0)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "U DIEDED", Color(0.8, 0.2, 0.2), 5, 5, 7);
			RenderTextOnScreen(meshList[GEO_TEXT], "Score: " + std::to_string(obj.getProgress()), Color(0.8, 0.2, 0.2), 5, 5, 5);
			showLose--;
		}

		if (showRQ > 0)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "U RAGEQUIT", Color(0.8, 0.2, 0.2), 5, 5, 7);
			RenderTextOnScreen(meshList[GEO_TEXT], "Score: " + std::to_string(obj.getProgress()), Color(0.8, 0.2, 0.2), 5, 5, 5);
			showRQ--;
		}

		if (showWin > 0)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "U WIN", Color(0.2, 0.8, 0.2), 5, 5, 5);
			showWin--;
		}

		//Render camera position on screen
		/*std::ostringstream oss;
		oss << "X: " << camera.position.x << " Y: " << camera.position.y << "Z: " << camera.position.z;
		string var = oss.str();
		RenderTextOnScreen(meshList[GEO_TEXT], oss.str(), Color(0, 1, 0), 2, 2, 29);*/

		//RenderTextOnScreen(meshList[GEO_TEXT], "FPS: " + std::to_string(framerate), Color(1, 0, 0), 2, 32, 29);

		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(0, 499, 0);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_LIGHTBALL], false);
		modelStack.PopMatrix();
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

		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base || Camera3::location.x < -330 && Camera3::location.x > -470 && Camera3::location.z > -10 && Camera3::location.z  < 80)
		{
			modelStack.Translate(0, -10, -10);
			modelStack.Rotate(-60, 0, 1, 0);
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
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(pis.ammo), Color(0.5, 0.2, 0.2), 3, 19, 1);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(pis.ammo), Color(0.2, 0.5,0.2), 3, 19, 1);
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
		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base || Camera3::location.x  < -330 && Camera3::location.x  > -470 && Camera3::location.z > -10 && Camera3::location.z < 80)
		{
			modelStack.Translate(0, -10, -10);
		}
		else
		{
			modelStack.Translate(5, -10, -15);
		}
		modelStack.Translate(0, 5, 0);
		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base || Camera3::location.x  < -330 && Camera3::location.x  > -470 && Camera3::location.z > -10 && Camera3::location.z < 80)
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
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(rif.ammo), Color(0.5, 0.2, 0.2), 3, 18, 1);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(rif.ammo), Color(0.2, 0.5, 0.2), 3, 18, 1);
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
		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base || Camera3::location.x  < -330 && Camera3::location.x  > -470 && Camera3::location.z > -10 && Camera3::location.z < 80)
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
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(sr.ammo), (0.5, 0.2, 0.2), 3, 18, 1);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(sr.ammo), Color(0.2, 0.5, 0.2), 3, 18, 1);
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
		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base || Camera3::location.x  < -330 && Camera3::location.x  > -470 && Camera3::location.z > -10 && Camera3::location.z < 80)
		{
			modelStack.Translate(0, -10, -15);
		}
		else
		{
			modelStack.Translate(5, -10, -15);
		}
		modelStack.Translate(0.3, 4.1, 0);
		if (Application::IsKeyPressed(VK_SHIFT) == true || gameStates == states::base || Camera3::location.x  < -330 && Camera3::location.x  > -470 && Camera3::location.z > -10 && Camera3::location.z < 80)
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
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(sg.ammo), Color(0.5, 0.2, 0.2), 3, 18, 1);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "Ammo: " + std::to_string(sg.ammo), Color(0.2, 0.5, 0.2), 3, 18, 1);
		modelStack.PopMatrix();
	}
}

void Sp2Scene::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}