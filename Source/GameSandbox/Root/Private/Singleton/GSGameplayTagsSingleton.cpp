// Copyright © 2025, Ivan Kinder

#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"

#include "GameplayTagsManager.h"

FGSGameplayTagsSingleton FGSGameplayTagsSingleton::Singleton;

void FGSGameplayTagsSingleton::InitializeNativeGameplayTags()
{
	InitializeProject16GameplayTags();
}

void FGSGameplayTagsSingleton::InitializeProject16GameplayTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	// Attribute tags.
	Singleton.P16Tags.Attribute_Primary_Strength                = Manager.AddNativeGameplayTag("P16.Attribute.Primary.Strength", "Increases physical damage");
	Singleton.P16Tags.Attribute_Primary_Intelligence            = Manager.AddNativeGameplayTag("P16.Attribute.Primary.Intelligence", "Increases magical damage");
	Singleton.P16Tags.Attribute_Primary_Resilience              = Manager.AddNativeGameplayTag("P16.Attribute.Primary.Resilience", "Increases armor and armor penetration");
	Singleton.P16Tags.Attribute_Primary_Vigor                   = Manager.AddNativeGameplayTag("P16.Attribute.Primary.Vigor", "Increases health");
	Singleton.P16Tags.Attribute_Secondary_Armor                 = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.Armor", "Reduce damage taken, improves Block Chance");
	Singleton.P16Tags.Attribute_Secondary_ArmorPenetration      = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.ArmorPenetration", "Ignored percentage of enemy armor, increases the critical hit chance");
	Singleton.P16Tags.Attribute_Secondary_BlockChance           = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.BlockChance", "Chance to cut incoming damage in half");
	Singleton.P16Tags.Attribute_Secondary_CriticalHitChance     = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.CriticalHitChance", "Chance to double damage plus critical hit bonus");
	Singleton.P16Tags.Attribute_Secondary_CriticalHitDamage     = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.CriticalHitDamage", "Bonus damage added when a critical hit is scored");
	Singleton.P16Tags.Attribute_Secondary_CriticalHitResistance = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.CriticalHitResistance", "Reduces the critical hit chance of attacking enemies");
	Singleton.P16Tags.Attribute_Secondary_HealthRegeneration    = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.HealthRegeneration", "Amount of health regenerated every 1 second");
	Singleton.P16Tags.Attribute_Secondary_ManaRegeneration      = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.ManaRegeneration", "Amount of mana regenerated every 1 second");
	Singleton.P16Tags.Attribute_Secondary_MaxHealth             = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.MaxHealth", "Maximum amount of health obtainable");
	Singleton.P16Tags.Attribute_Secondary_MaxMana               = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.MaxMana", "Maximum amount of mana obtainable");
	Singleton.P16Tags.Attribute_Vital_Health                    = Manager.AddNativeGameplayTag("P16.Attribute.Vital.Health", "Current amount of health");
	Singleton.P16Tags.Attribute_Vital_Mana                      = Manager.AddNativeGameplayTag("P16.Attribute.Vital.Mana", "Current amount of mana");

	// Input tags.
	Singleton.P16Tags.Input_MouseButtonLeft  = Manager.AddNativeGameplayTag("P16.Input.MouseButtonLeft", "Input tag for the left mouse button");
	Singleton.P16Tags.Input_MouseButtonRight = Manager.AddNativeGameplayTag("P16.Input.MouseButtonRight", "Input tag for the right mouse button");
	Singleton.P16Tags.Input_Number1          = Manager.AddNativeGameplayTag("P16.Input.Number1", "Input tag for the number 1 button");
	Singleton.P16Tags.Input_Number2          = Manager.AddNativeGameplayTag("P16.Input.Number2", "Input tag for the number 2 button");
	Singleton.P16Tags.Input_Number3          = Manager.AddNativeGameplayTag("P16.Input.Number3", "Input tag for the number 3 button");
	Singleton.P16Tags.Input_Number4          = Manager.AddNativeGameplayTag("P16.Input.Number4", "Input tag for the number 4 button");

	// Event tags.
	Singleton.P16Tags.Event_Montage_Fireball = Manager.AddNativeGameplayTag("P16.Event.Montage.Fireball", "Event tag for the fireball montage");
}
