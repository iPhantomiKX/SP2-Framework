#ifndef OBJECTIVE_H
#define OBJECTIVE_H

class objective
{
private:
	int enemiesDieded;
	int maxEnemiesDieded;
	int resourcesCollected;
	int maxResourcesCollected;
	bool bossDefeated;

public:
	static int chooseObj;
	objective();
	~objective();
	int getObjective();
	int getProgress();
	void resetObjective();
	void objectiveProgress(int amt);
	bool objComplete();
};

#endif