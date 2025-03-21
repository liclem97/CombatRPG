// liclem97 All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"

#include "AbilitySystem/Abilities/CombatGameplayAbility.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UCombatAbilitySystemComponent* InCombatASCToGive, int32 ApplyLevel)
{
	check(InCombatASCToGive);

	GrantAbilities(ActivateOnGivenAbilties, InCombatASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InCombatASCToGive, ApplyLevel);
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UCombatGameplayAbility>>& InAbilitiesToGive, UCombatAbilitySystemComponent* InCombatASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty()) return;

	for (const TSubclassOf<UCombatGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InCombatASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InCombatASCToGive->GiveAbility(AbilitySpec);
	}		
}
