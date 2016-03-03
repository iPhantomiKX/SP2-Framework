#include "player.h"

player::player()
{
	hp = 100;
	minerals = 0;
}
player::~player()
{

}

void player::getHit(int damage)
{
	if (hp >= 0)
	{
		hp -= damage;
	}
}
void player::healHp(int heal)
{
	hp += heal;
	if (hp > 100)
	{
		hp = 100;
	}
}
void player::spendMinerals(int amt)
{
	minerals -= amt;
}
void player::earnMinerals(int amt)
{
	minerals += amt;
	if (minerals < 0)
	{
		minerals = 0;
	}
}
bool player::isDead()
{
	if (hp <= 0)
	{
		return true;
	}
}

int player::getHp()
{
	return hp;
}

int player::getMinerals()
{
	return minerals;
}


