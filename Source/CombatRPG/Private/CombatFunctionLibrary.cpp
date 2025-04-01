// liclem97 All Rights Reserved.


#include "CombatFunctionLibrary.h"

#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UCombatAbilitySystemComponent* UCombatFunctionLibrary::NativeGetCombatASCFromActor(AActor* InActor)
{	
	check(InActor);

	return CastChecked<UCombatAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UCombatFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UCombatAbilitySystemComponent* ASC = NativeGetCombatASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UCombatFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UCombatAbilitySystemComponent* ASC = NativeGetCombatASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UCombatFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UCombatAbilitySystemComponent* ASC = NativeGetCombatASCFromActor(InActor);
	
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UCombatFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, ECombatConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? ECombatConfirmType::Yes : ECombatConfirmType::No;
}
