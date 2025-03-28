// liclem97 All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace CombatGameplayTags
{
	/** Input Tags **/
	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipAxe);
	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipRifle);
	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipAxe);
	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipRifle);

	/** Player Tags **/
	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);
	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Rifle);

	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe);
	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unquip_Axe);
	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Rifle);
	COMBATRPG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unquip_Rifle);
}