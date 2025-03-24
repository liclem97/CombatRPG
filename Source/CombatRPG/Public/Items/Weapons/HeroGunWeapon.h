// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/HeroWeapon.h"
#include "HeroGunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COMBATRPG_API AHeroGunWeapon : public AHeroWeapon
{
	GENERATED_BODY()

public:
	AHeroGunWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	TSubclassOf<AActor> ProjectileClass;
};
