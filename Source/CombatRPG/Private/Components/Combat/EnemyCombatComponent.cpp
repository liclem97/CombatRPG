// liclem97 All Rights Reserved.


#include "Components/Combat/EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CombatFunctionLibrary.h"
#include "CombatGameplayTags.h"

#include "CombatDebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);

	// TODO : block 체크 구현
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UCombatFunctionLibrary::NativeDoesActorHaveTag(HitActor, CombatGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = false;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		bIsValidBlock = UCombatFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	if (bIsValidBlock)
	{
		// TODO : 블록 성공
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			CombatGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}
