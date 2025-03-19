// liclem97 All Rights Reserved.


#include "Characters/HeroCharacter.h"

#include "CombatDebugHelper.h"

AHeroCharacter::AHeroCharacter()
{

}

void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	Debug::Print("HeroCharacter BeginPlay");
}


