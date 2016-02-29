#include "objective.h"

objective::objective()
{
	enemiesDieded = 0;
	maxEnemiesDieded = 10;
}


objective::~objective()
{

}
int objective::getProgress()
{
	if (chooseObj == 1)
	{
		return enemiesDieded;
	}
}
int objective::getObjective()
{
	if (chooseObj == 1)
	{
		return maxEnemiesDieded;
	}
}
void objective::objectiveProgress(int amt)
{
	if (chooseObj == 1)
	{
		enemiesDieded++;
	}
}
bool objective::objComplete()
{
	if (chooseObj == 1)
	{
		if (enemiesDieded == maxEnemiesDieded)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}