// liclem97 All Rights Reserved.


#include "Controllers/HeroController.h"

AHeroController::AHeroController()
{
	HeroTeamID = FGenericTeamId(0); // 플레이어의 팀 ID는 0
}

FGenericTeamId AHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}
