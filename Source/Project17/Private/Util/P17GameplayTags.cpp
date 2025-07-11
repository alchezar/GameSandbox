// Copyright © 2025, Ivan Kinder

#include "Util/P17GameplayTags.h"

namespace P17::Tags
{
	/// @par Input tags --------------------------------------------------------
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "P17.Input.Move")
	UE_DEFINE_GAMEPLAY_TAG(Input_Look, "P17.Input.Look")
	UE_DEFINE_GAMEPLAY_TAG(Input_Jump, "P17.Input.Jump")
	UE_DEFINE_GAMEPLAY_TAG(Input_Roll, "P17.Input.Roll")
	UE_DEFINE_GAMEPLAY_TAG(Input_Equip_Axe, "P17.Input.Equip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Input_Unequip_Axe, "P17.Input.Unequip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Input_Attack_Light_Axe, "P17.Input.Attack.Light.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Input_Attack_Heavy_Axe, "P17.Input.Attack.Heavy.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold, "P17.Input.Hold")
	UE_DEFINE_GAMEPLAY_TAG(Input_Hold_Block, "P17.Input.Hold.Block")
	UE_DEFINE_GAMEPLAY_TAG(Input_Toggle, "P17.Input.Toggle")
	UE_DEFINE_GAMEPLAY_TAG(Input_Toggle_TargetLock, "P17.Input.Toggle.TargetLock")
	UE_DEFINE_GAMEPLAY_TAG(Input_SwitchTarget, "P17.Input.SwitchTarget")

	/// @par Player tags -------------------------------------------------------
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Axe, "P17.Player.Ability.Equip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Axe, "P17.Player.Ability.Unequip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Axe, "P17.Player.Ability.Attack.Light.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Axe, "P17.Player.Ability.Attack.Heavy.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Hit_Pause, "P17.Player.Ability.Hit.Pause")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Roll, "P17.Player.Ability.Roll")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Block, "P17.Player.Ability.Block")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_TargetLock, "P17.Player.Ability.TargetLock")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Axe, "P17.Player.Event.Equip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Axe, "P17.Player.Event.Unequip.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Hit_Pause, "P17.Player.Event.Hit.Pause")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Hit_SuccessfulBlock, "P17.Player.Event.Hit.SuccessfulBlock")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Left, "P17.Player.Event.SwitchTarget.Left")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Right, "P17.Player.Event.SwitchTarget.Right")
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Axe, "P17.Player.Weapon.Axe")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_JumpToFinisher, "P17.Player.Status.JumpToFinisher")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rolling, "P17.Player.Status.Rolling")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Blocking, "P17.Player.Status.Blocking")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_TargetLock, "P17.Player.Status.TargetLock")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Light, "P17.Player.SetByCaller.AttackType.Light")
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Heavy, "P17.Player.SetByCaller.AttackType.Heavy")

	/// @par Enemy tags --------------------------------------------------------
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "P17.Enemy.Ability.Melee")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Range, "P17.Enemy.Ability.Range")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "P17.Enemy.Weapon")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Strafing, "P17.Enemy.Status.Strafing")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_UnderAttack, "P17.Enemy.Status.UnderAttack")

	/// @par Shared tags -------------------------------------------------------
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Hit_React, "P17.Shared.Ability.Hit.React")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "P17.Shared.Ability.Death")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Hit_Melee, "P17.Shared.Event.Hit.Melee")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Hit_React, "P17.Shared.Event.Hit.React")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Spawn_Projectile, "P17.Shared.Event.Spawn.Projectile")
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_Damage_Base, "P17.Shared.SetByCaller.Damage.Base")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead, "P17.Shared.Status.Dead")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Front, "P17.Shared.Status.HitReact.Front")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Back, "P17.Shared.Status.HitReact.Back")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Left, "P17.Shared.Status.HitReact.Left")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Right, "P17.Shared.Status.HitReact.Right")
}
