// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UCombatGameplayAbility;
class UCombatAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class COMBATRPG_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UCombatAbilitySystemComponent* InCombatASCToGive, int32 ApplyLevel = 1);
	
protected:
	void GrantAbilities(const TArray<TSubclassOf<UCombatGameplayAbility>>& InAbilitiesToGive, UCombatAbilitySystemComponent* InCombatASCToGive, int32 ApplyLevel = 1);

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UCombatGameplayAbility>> ActivateOnGivenAbilties;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UCombatGameplayAbility>> ReactiveAbilities;
};
