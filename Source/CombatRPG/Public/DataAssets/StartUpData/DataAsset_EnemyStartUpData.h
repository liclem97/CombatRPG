// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "DataAsset_EnemyStartUpData.generated.h"

class UEnemyGameplayAbility;

/**
 * 
 */
UCLASS()
class COMBATRPG_API UDataAsset_EnemyStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UEnemyGameplayAbility>> EnemyCombatAbilties;
};
