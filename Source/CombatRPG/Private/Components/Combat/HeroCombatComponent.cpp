// liclem97 All Rights Reserved.


#include "Components/Combat/HeroCombatComponent.h"

#include "Items/Weapons/HeroWeapon.h"

AHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag)
{   
    return Cast<AHeroWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}
