// liclem97 All Rights Reserved.


#include "AbilitySystem/Abilities/HeroGameplayAbility.h"

#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "Characters/HeroCharacter.h"
#include "CombatGameplayTags.h"
#include "Controllers/HeroController.h"


AHeroCharacter* UHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{	
	if (!CachedHeroCharacter.IsValid())
	{
		CachedHeroCharacter = Cast<AHeroCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CachedHeroCharacter.IsValid() ? CachedHeroCharacter.Get() : nullptr;
}

AHeroController* UHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedHeroController.IsValid())
	{
		CachedHeroController = Cast<AHeroController>(CurrentActorInfo->PlayerController);
	}

	return CachedHeroController.IsValid() ? CachedHeroController.Get() : nullptr;
}

UHeroCombatComponent* UHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

FGameplayEffectSpecHandle UHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount)
{	
	check(EffectClass);

	// GameplayEffect�� ������/��ó �� ���ؽ�Ʈ ���� ����
	FGameplayEffectContextHandle ContextHandle = GetCombatAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// EffectClass�� ������� SpecHandle ����
	FGameplayEffectSpecHandle EffectSpecHandle = GetCombatAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		CombatGameplayTags::Shared_SetByCaller_BaseDamage,
		InWeaponBaseDamage
	);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InCurrentComboCount);
	}

	return EffectSpecHandle;
}
