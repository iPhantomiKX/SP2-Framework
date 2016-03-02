#ifndef SP2SCENE_H
#define SP2SCENE_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "enemy.h"
#include "gun.h"
#include "gun2.h"
#include "player.h"
#include <vector>
#include <sstream>
#include <string>

using std::string;
using std::vector;



class Sp2Scene : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_RING,
		GEO_SPHERE,
		GEO_PLANETS,
		GEO_MOONIA,
		GEO_PLANET1,
		GEO_MOON1A,
		GEO_RING1,
		GEO_LIGHTBALL,
		GEO_MERCURY,
		GEO_URANUS,
		GEO_HEMISHERE,
		GEO_CONE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_MODEL1,
		GEO_LAND,
		GEO_AIR,
		GEO_TEXT,
		GEO_DEADTREE,
		GEO_HEAD,
		GEO_ICY_LEFT,
		GEO_ICY_RIGHT,
		GEO_ICY_TOP,
		GEO_ICY_BOTTOM,
		GEO_ICY_FRONT,
		GEO_ICY_BACK,
		GEO_ICYHELL_LEFT,
		GEO_ICYHELL_RIGHT,
		GEO_ICYHELL_TOP,
		GEO_ICYHELL_BOTTOM,
		GEO_ICYHELL_FRONT,
		GEO_ICYHELL_BACK,
		GEO_BLOODY_LEFT,
		GEO_BLOODY_RIGHT,
		GEO_BLOODY_TOP,
		GEO_BLOODY_BOTTOM,
		GEO_BLOODY_FRONT,
		GEO_OBJECT,
		GEO_TEST,

		GEO_MENU,
		GEO_MENUPORTAL,
		GEO_SPIRAL,
		GEO_TARGET,
		GEO_TARGETHIT,

		GEO_SHOT,
		GEO_SPACESHIPQUAD,
		GEO_CRAFTINGPANEL,
		GEO_SPACEHOUSE,
		GEO_PORTAL1,
		GEO_PORTAL2,
		GEO_TABLE,
		GEO_HEALTHPACK,
		GEO_MINERALS,

		GEO_PISTOL1,
		GEO_PISTOL2,
		GEO_RIFLE1,
		GEO_SNIPER1,
		GEO_SHOTGUN1,

		GEO_CRAFT_UI,

		GEO_SCOPE,
		GEO_AMMO,
		GEO_THECUBE,
		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
	enum class states
	{
		menu,
		base,
		outside,
	};

public:
	Sp2Scene();
	~Sp2Scene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	void RenderMesh(Mesh *mesh, bool enablelight);
private:
	
	void RenderSkybox();
	void RenderText(Mesh * mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh * mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh * mesh, float sizex, float x, float y);
	void RenderImageOnScreen(Mesh * mesh, float sizex, float sizey, float sizez, float x, float y);
	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngle;
	float planet1RevAngle, planet1RotAngle, moon1RotAngle;
	int framerate;

	float rainpositionx[1000];
	float rainpositiony[1000];
	float rainpositionz[1000];

	float elementsx[25];
	float elementsz[25];

	WEAPON_TYPE CurrentWeapon;
	Weapon myWeapon[MAX_COUNT];

	int treex[50];
	int treez[50];

	int tempx[50];
	int tempz[50];

	float translateY;
	float translateX;
	float translateZ;

	float rotateGunX;
	float rotateGunY;

	float Degree;

	float spawnPositionx[10];
	float spawnPositionz[10];

	double range;
	double gunCd;
	double gunReload;
	float upRecoil;
	float gunDir;
	int sRecoilCd;
	bool reloaded = false;
	void bulletRNG(int spray);
	void aimBulletRNG(int spray);
	bool bulletObjectCollision(Vector3 bulletPos);
	bool AICheckCollisionObject(Vector3 AIposition);
	bool ingame;

	double storeRand;
	double storeRand2;

	//boolean for crafting UI to pop out
	bool crafting = false;

	//boolean for equipping weapons
	bool equipPistol1 = true;
	//bool equipPistol2 = true;
	bool equipRifle1 = false;
	bool equipSniper1 = false;
	bool equipShotgun1 = false;

	//check if weapon is bought
	//bool boughtPistol1 = false;
	bool boughtRifle1 = false;
	bool boughtSniper1 = false;
	bool boughtShotgun1 = false;

	//check if weapon is available
	//bool pistol2Avail = false;
	bool rifle1Avail = false;
	bool sniper1Avail = false;
	bool shotgun1Avail = false;

	bool testPortal = false;
	bool testPortalsign = false;

	int buttonCd;
	int heals;

	bool testHB = false;

	void RenderMenu();
	void RenderPistol1();
	void RenderPistol2();
	void RenderRifle1();
	void RenderSniper1();
	void RenderShotgun1();
	void RenderElements();
	void RenderEnemy();

	bool bulletEnemyCollision(Vector3 bulletPos, Vector3 targetLocation);
	void Sp2Scene::EnemyAttack(Vector3 targetLocation);
	int atkCd;
	/*static bool test4;
	static bool test5;*/

	void RenderSpaceshipQuad();
	void RenderCraftingPanel();
	void bulletPos();
	void RenderSpaceHouse();
	void RenderPortal1();
	void RenderPortal2();
	void RenderTable();
	void RenderHealthPack();

	

	void changeStates();

	Vector3 treeposition[150];
	Vector3 test;

	std::vector<Vector3> minVectors;
	std::vector<Vector3> maxVectors;

	std::vector<Vector3> shotsFired;
	std::vector<Vector3> shotsDir;
	std::vector<int> weaponDmg;
	//vector<> shotsFired;

	double targetReg;

	Camera3 camera;

	MS modelStack, viewStack, projectionStack;

	Light light[1];

	states gameStates;
};

#endif