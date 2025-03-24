// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatGameplayAbility.h"
#include "HeroGameplayAbility.generated.h"

class AHeroCharacter;
class AHeroController;
class UHeroCombatComponent;

/**
 * 
 */
UCLASS()
class COMBATRPG_API UHeroGameplayAbility : public UCombatGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	AHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	AHeroController* GetHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<AHeroCharacter> CachedHeroCharacter;
	TWeakObjectPtr<AHeroController> CachedHeroController;
};
