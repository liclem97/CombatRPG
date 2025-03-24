// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CombatGameplayAbility.generated.h"

class UPawnCombatComponent;
class UCombatAbilitySystemComponent;

UENUM(BlueprintType)
enum class ECombatAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class COMBATRPG_API UCombatGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface.

	UPROPERTY(EditDefaultsOnly, Category = "CombatAbility")
	ECombatAbilityActivationPolicy AbilityActivationPolicy = ECombatAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Ability")
	UCombatAbilitySystemComponent* GetCombatAbilitySystemComponentFromActorInfo() const;
};
