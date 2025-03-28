// liclem97 All Rights Reserved.


#include "CombatGameplayTags.h"

namespace CombatGameplayTags
{
	/** Input Tags **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipAxe, "InputTag.EquipAxe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipRifle, "InputTag.EquipRifle");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipAxe, "InputTag.UnequipAxe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipRifle, "InputTag.UnequipRifle");

	/** Player Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Axe, "Player.Weapon.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Rifle, "Player.Weapon.Rifle");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Axe, "Player.Event.Equip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unquip_Axe, "Player.Event.Unquip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Rifle, "Player.Event.Equip.Rifle");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unquip_Rifle, "Player.Event.Unquip.Rifle");
}