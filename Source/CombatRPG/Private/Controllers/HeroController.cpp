// liclem97 All Rights Reserved.


#include "Controllers/HeroController.h"

AHeroController::AHeroController()
{
	HeroTeamID = FGenericTeamId(0); // �÷��̾��� �� ID�� 0
}

FGenericTeamId AHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}
