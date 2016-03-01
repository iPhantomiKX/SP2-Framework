#include "player.h"

player::player()
{
	hp = 100;
	minerals = 3000;
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
}
void player::spendMinerals(int amt)
{
	minerals -= amt;
}
void player::earnMinerals(int amt)
{
	minerals += amt;
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


