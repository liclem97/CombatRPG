// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/CombatWeaponBase.h"
#include "Types/CombatStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "HeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COMBATRPG_API AHeroWeapon : public ACombatWeaponBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FHeroWeaponData HeroWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
