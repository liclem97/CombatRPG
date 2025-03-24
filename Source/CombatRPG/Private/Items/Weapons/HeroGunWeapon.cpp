// liclem97 All Rights Reserved.


#include "Items/Weapons/HeroGunWeapon.h"

#include "Components/SkeletalMeshComponent.h"

AHeroGunWeapon::AHeroGunWeapon()
{
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(GetRootComponent());
}
