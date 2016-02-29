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
	int getObjective();
	int getProgress();
	void objectiveProgress(int amt);
	bool objComplete();
};

#endif