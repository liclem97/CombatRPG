// liclem97 All Rights Reserved.


#include "AbilitySystem/Abilities/CombatGameplayAbility.h"

#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"

// 어빌리티 시스템 컴포넌트에 어빌리티가 부여된 직후 호출.
void UCombatGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == ECombatAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UCombatGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == ECombatAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UCombatGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{	
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UCombatAbilitySystemComponent* UCombatGameplayAbility::GetCombatAbilitySystemComponentFromActorInfo() const
{	
	return Cast<UCombatAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}
