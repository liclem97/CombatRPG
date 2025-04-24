// liclem97 All Rights Reserved.


#include "CombatFunctionLibrary.h"

#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "Interfaces/PawnCombatInterface.h"

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

UPawnCombatComponent* UCombatFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{	
	check(InActor);

	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}

	return nullptr;
}

UPawnCombatComponent* UCombatFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, ECombatValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = CombatComponent ? ECombatValidType::Valid : ECombatValidType::InValid;

	return CombatComponent;
}

bool UCombatFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{	
	check(QueryPawn && TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}

	return false;
}

float UCombatFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}
