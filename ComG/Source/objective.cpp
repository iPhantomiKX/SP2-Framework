#include "objective.h"

objective::objective()
{
	enemiesDieded = 0;
	maxEnemiesDieded = 30;
	resourcesCollected = 0;
	maxResourcesCollected = 10;
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
	else if (chooseObj == 2)
	{
		return resourcesCollected;
	}
}
int objective::getObjective()
{
	if (chooseObj == 1)
	{
		return maxEnemiesDieded;
	}
	else if (chooseObj == 2)
	{
		return maxResourcesCollected;
	}
}

void objective::resetObjective()
{
	enemiesDieded = 0;
	resourcesCollected = 0;
}

void objective::objectiveProgress(int amt)
{
	if (chooseObj == 1)
	{
		enemiesDieded += amt;
	}
	else if (chooseObj == 2)
	{
		resourcesCollected += amt;
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
	if (chooseObj == 2)
	{
		if (resourcesCollected == maxResourcesCollected)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}