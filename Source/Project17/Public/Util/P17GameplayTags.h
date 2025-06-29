// Copyright © 2025, Ivan Kinder

#pragma once

#include "NativeGameplayTags.h"

namespace P17::Tags
{
	/// @par Input tags --------------------------------------------------------
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Move)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Look)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Jump)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Equip_Axe)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Unequip_Axe)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Attack_Light_Axe)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Attack_Heavy_Axe)

	/// @par Player tags -------------------------------------------------------
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Axe)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unequip_Axe)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Axe)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Axe)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Hit_Pause)

	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Hit_Pause)

	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe)

	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_JumpToFinisher)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Light)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Heavy)

	/// @par Enemy tags --------------------------------------------------------
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Melee)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Range)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon)

	/// @par Shared tags -------------------------------------------------------
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_Hit_React)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_Hit_Melee)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_Hit_React)
	PROJECT17_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_Damage_Base)
}
