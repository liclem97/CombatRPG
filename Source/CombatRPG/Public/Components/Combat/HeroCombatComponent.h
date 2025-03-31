// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

class AHeroWeapon;

/**
 * 
 */
UCLASS()
class COMBATRPG_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Hero|Combat")
	AHeroWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag);
};
