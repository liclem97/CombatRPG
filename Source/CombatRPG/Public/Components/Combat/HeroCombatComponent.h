// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

class AHeroWeapon;

/**
 * 
 */
UCLASS()
class COMBATRPG_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Hero|Combat")
	AHeroWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UFUNCTION(BlueprintPure, Category = "Hero|Combat")
	AHeroWeapon* GetHeroCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintPure, Category = "Hero|Combat")
	float GetHeroCurrentEquippedWeaponDamageAtLevel(float InLevel) const;

	UPROPERTY(BlueprintReadWrite, Category = "Hero|Combat")
	FGameplayTag RifleAimTag;

	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponFulledFromTargetActor(AActor* InteractedActor) override;
};
