// liclem97 All Rights Reserved.


#include "AbilitySystem/CombatAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/HeroGameplayAbility.h"

// ������ ��ǲ�� ������ �����Ƽ�� �ߵ���Ŵ.
void UCombatAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UCombatAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
}

// ���� �����Ƽ�� �ο���.
void UCombatAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty()) return;

	for (const FHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec)); // �����Ƽ �ο� �� �迭�� ����
	}
}
