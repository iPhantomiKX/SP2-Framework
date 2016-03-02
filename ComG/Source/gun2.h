#ifndef GUN2_H
#define GUN2_H

#include "Scene.h"

struct Weapon
{
	// weapon characteristics
	int damage;
	double RoF;
	int ammo;
	int maxAmmo;
	double reloadSpd;
	int inAccuracy;

	// weapon render data



};

enum WEAPON_TYPE
{
	PISTOL1,
	PISTOL2,
	RIFLE1,
	SHOTGUN1,
	SNIPER1,

	MAX_COUNT,
};


#endif