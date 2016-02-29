#ifndef OBJECTIVE_H
#define OBJECTIVE_H

class objective
{
private:
	int enemiesDieded;
	int maxEnemiesDieded;
	int resourcesCollected;
	int maxResourcesCollected;
	int bossDefeated;
	int specialItemCollected;

public:
	static int chooseObj;
	objective();
	~objective();
	void getObjective();
	int objectiveProgress(int amt);
	void objComplete();
};

#endif