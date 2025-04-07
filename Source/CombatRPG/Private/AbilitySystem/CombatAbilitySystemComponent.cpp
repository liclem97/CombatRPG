// liclem97 All Rights Reserved.


#include "AbilitySystem/CombatAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/HeroGameplayAbility.h"
#include "CombatGameplayTags.h"

// ������ Key�� ������ �����Ƽ�� �ߵ���Ű�� �Լ�
void UCombatAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	TArray<FGameplayAbilitySpecHandle> SpecHandles;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{	
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			SpecHandles.Add(AbilitySpec.Handle); // �����Ƽ�� �ش� �±װ� �ִ� ��� �ڵ鿡 ����
		}
	}

	// ����� �ڵ鿡 ���� �����Ƽ �ߵ�
	for (const FGameplayAbilitySpecHandle& Handle : SpecHandles)
	{
		TryActivateAbility(Handle);
	}
}

void UCombatAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	if (InInputTag == CombatGameplayTags::InputTag_Aim_Rifle)
	{
		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

// ���� �����Ƽ�� �ο��ϴ� �Լ�
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

// ���⿡ �ο��� �����Ƽ ���� �Լ�
void UCombatAbilitySystemComponent::RemoveGrantedHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty()) return;

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	InSpecHandlesToRemove.Empty();
}
