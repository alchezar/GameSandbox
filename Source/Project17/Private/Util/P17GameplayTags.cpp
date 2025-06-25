// Copyright © 2025, Ivan Kinder

#include "Util/P17GameplayTags.h"

namespace P17::Tags
{
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "P17.Input.Move")
	UE_DEFINE_GAMEPLAY_TAG(Input_Look, "P17.Input.Look")
	UE_DEFINE_GAMEPLAY_TAG(Input_Jump, "P17.Input.Jump")
	UE_DEFINE_GAMEPLAY_TAG(Input_Equip_Axe, "P17.Input.Equip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Input_Unequip_Axe, "P17.Input.Unequip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Input_Attack_Light_Axe, "P17.Input.Attack.Light.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Input_Attack_Heavy_Axe, "P17.Input.Attack.Heavy.Axe")

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Axe, "P17.Player.Ability.Equip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Axe, "P17.Player.Ability.Unequip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Axe, "P17.Player.Ability.Attack.Light.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Axe, "P17.Player.Ability.Attack.Heavy.Axe")

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Axe, "P17.Player.Event.Equip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Axe, "P17.Player.Event.Unequip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Axe, "P17.Player.Weapon.Axe")

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_JumpToFinisher, "P17.Player.Status.JumpToFinisher")
}
