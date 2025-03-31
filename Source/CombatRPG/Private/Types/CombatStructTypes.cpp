// liclem97 All Rights Reserved.


#include "Types/CombatStructTypes.h"

#include "AbilitySystem/Abilities/CombatGameplayAbility.h"

bool FHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
