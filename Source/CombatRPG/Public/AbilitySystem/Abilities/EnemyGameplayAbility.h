// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CombatGameplayAbility.h"
#include "EnemyGameplayAbility.generated.h"

class AEnemyCharacter;
class UEnemyCombatComponent;

/**
 * 
 */
UCLASS()
class COMBATRPG_API UEnemyGameplayAbility : public UCombatGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category ="Combat|Ability")
	AEnemyCharacter* GetEnemyCharacterFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<AEnemyCharacter> CachedEnemyCharacter;
};
