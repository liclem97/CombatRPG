// liclem97 All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"

#include "AbilitySystem/Abilities/CombatGameplayAbility.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	GrantAbilities(ActivateOnGivenAbilties, InASCToGive, ApplyLevel); // OnGiven �����Ƽ�� ������
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel); // ���� ������ �����Ƽ�� ������
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UCombatGameplayAbility>>& InAbilitiesToGive, UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty()) return;

	for (const TSubclassOf<UCombatGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InASCToGive->GiveAbility(AbilitySpec);
	}		
}
