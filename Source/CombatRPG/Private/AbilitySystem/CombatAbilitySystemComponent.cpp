// liclem97 All Rights Reserved.


#include "AbilitySystem/CombatAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/HeroGameplayAbility.h"
#include "CombatGameplayTags.h"

// 설정한 Key를 누르면 어빌리티를 발동시키는 함수
void UCombatAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	TArray<FGameplayAbilitySpecHandle> SpecHandles;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{	
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
		{
			SpecHandles.Add(AbilitySpec.Handle); // 어빌리티에 해당 태그가 있는 경우 핸들에 저장
		}
	}

	// 저장된 핸들에 대해 어빌리티 발동
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

// 무기 어빌리티를 부여하는 함수
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

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec)); // 어빌리티 부여 후 배열에 저장
	}
}

// 무기에 부여된 어빌리티 제거 함수
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
