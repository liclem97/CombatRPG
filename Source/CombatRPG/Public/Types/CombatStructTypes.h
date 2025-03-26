// liclem97 All Rights Reserved.

#pragma once

#include "CombatStructTypes.generated.h"

class UHeroLinkedAnimLayer;

USTRUCT(BlueprintType)
struct FHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UHeroLinkedAnimLayer> WeaponAnimLayerToLink;
};