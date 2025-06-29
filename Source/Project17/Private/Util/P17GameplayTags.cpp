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
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Hit_Pause, "P17.Player.Ability.Hit.Pause")

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Axe, "P17.Player.Event.Equip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Axe, "P17.Player.Event.Unequip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Hit_Pause, "P17.Player.Event.Hit.Pause")

	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Axe, "P17.Player.Weapon.Axe")

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_JumpToFinisher, "P17.Player.Status.JumpToFinisher")

	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Light, "P17.Player.SetByCaller.AttackType.Light")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Heavy, "P17.Player.SetByCaller.AttackType.Heavy")

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "P17.Enemy.Ability.Melee")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Range, "P17.Enemy.Ability.Range")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "P17.Enemy.Weapon")

	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Hit_React, "P17.Shared.Ability.Hit.React")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Hit_Melee, "P17.Shared.Event.Hit.Melee")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Hit_React, "P17.Shared.Event.Hit.React")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Damage_Base, "P17.Shared.SetByCaller.Damage.Base")
}
