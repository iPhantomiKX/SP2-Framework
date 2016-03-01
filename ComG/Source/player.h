#ifndef PLAYER_H
#define PLAYER_H


class player
{
private:
	int hp;
	int minerals;
public:
	player();
	~player();
	void getHit(int damage);
	void healHp(int heal);
	bool isDead();
	int getHp();
	int getMinerals();
	void spendMinerals(int amt);
	void earnMinerals(int amt);
	void reset();
};

#endif