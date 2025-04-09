// liclem97 All Rights Reserved.


#include "Components/Combat/HeroCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CombatGameplayTags.h"
#include "Items/Weapons/HeroWeapon.h"

#include "CombatDebugHelper.h"

AHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag)
{   
    return Cast<AHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

void UHeroCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    if (OverlappedActors.Contains(HitActor)) return;

    OverlappedActors.AddUnique(HitActor); // 히트 데미지를 주기 위한 액터 수집

    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = HitActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwningPawn(),
        CombatGameplayTags::Shared_Event_MeleeHit,
        Data
    );
}

void UHeroCombatComponent::OnWeaponFulledFromTargetActor(AActor* InteractedActor)
{    
}
