#ifndef PLAYER_H
#define PLAYER_H

class player
{
private:
	int hp;
public:
	player();
	~player();
	void getHit(int damage);
	void isDead();
};

#endif