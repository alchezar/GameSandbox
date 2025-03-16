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
	Singleton.P16Tags.Attribute.Primary.StrengthTag                = Manager.AddNativeGameplayTag("P16.Attribute.Primary.Strength", "Increases physical damage");
	Singleton.P16Tags.Attribute.Primary.IntelligenceTag            = Manager.AddNativeGameplayTag("P16.Attribute.Primary.Intelligence", "Increases magical damage");
	Singleton.P16Tags.Attribute.Primary.ResilienceTag              = Manager.AddNativeGameplayTag("P16.Attribute.Primary.Resilience", "Increases armor and armor penetration");
	Singleton.P16Tags.Attribute.Primary.VigorTag                   = Manager.AddNativeGameplayTag("P16.Attribute.Primary.Vigor", "Increases health");
	Singleton.P16Tags.Attribute.Secondary.ArmorTag                 = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.Armor", "Reduce damage taken, improves Block Chance");
	Singleton.P16Tags.Attribute.Secondary.ArmorPenetrationTag      = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.ArmorPenetration", "Ignored percentage of enemy armor, increases the critical hit chance");
	Singleton.P16Tags.Attribute.Secondary.BlockChanceTag           = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.BlockChance", "Chance to cut incoming damage in half");
	Singleton.P16Tags.Attribute.Secondary.CriticalHitChanceTag     = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.CriticalHitChance", "Chance to double damage plus critical hit bonus");
	Singleton.P16Tags.Attribute.Secondary.CriticalHitDamageTag     = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.CriticalHitDamage", "Bonus damage added when a critical hit is scored");
	Singleton.P16Tags.Attribute.Secondary.CriticalHitResistanceTag = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.CriticalHitResistance", "Reduces the critical hit chance of attacking enemies");
	Singleton.P16Tags.Attribute.Secondary.HealthRegenerationTag    = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.HealthRegeneration", "Amount of health regenerated every 1 second");
	Singleton.P16Tags.Attribute.Secondary.ManaRegenerationTag      = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.ManaRegeneration", "Amount of mana regenerated every 1 second");
	Singleton.P16Tags.Attribute.Secondary.MaxHealthTag             = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.MaxHealth", "Maximum amount of health obtainable");
	Singleton.P16Tags.Attribute.Secondary.MaxManaTag               = Manager.AddNativeGameplayTag("P16.Attribute.Secondary.MaxMana", "Maximum amount of mana obtainable");
	Singleton.P16Tags.Attribute.Vital.HealthTag                    = Manager.AddNativeGameplayTag("P16.Attribute.Vital.Health", "Current amount of health");
	Singleton.P16Tags.Attribute.Vital.ManaTag                      = Manager.AddNativeGameplayTag("P16.Attribute.Vital.Mana", "Current amount of mana");

	// Resistance tags.
	Singleton.P16Tags.Attribute.Resistance.ArcaneTag    = Manager.AddNativeGameplayTag("P16.Attribute.Resistance.Arcane", "Resistance to Arcane damage");
	Singleton.P16Tags.Attribute.Resistance.FireTag      = Manager.AddNativeGameplayTag("P16.Attribute.Resistance.Fire", "Resistance to Fire damage");
	Singleton.P16Tags.Attribute.Resistance.LightningTag = Manager.AddNativeGameplayTag("P16.Attribute.Resistance.Lightning", "Resistance to Lightning damage");
	Singleton.P16Tags.Attribute.Resistance.PhysicalTag  = Manager.AddNativeGameplayTag("P16.Attribute.Resistance.Physical", "Resistance to Physical damage");

	// Damage tags.
	Singleton.P16Tags.Damage.Tag                = Manager.AddNativeGameplayTag("P16.Damage", "Damage");
	Singleton.P16Tags.Damage.ArcaneTag          = Manager.AddNativeGameplayTag("P16.Damage.Arcane", "Arcane damage type");
	Singleton.P16Tags.Damage.FireTag            = Manager.AddNativeGameplayTag("P16.Damage.Fire", "Fire damage type");
	Singleton.P16Tags.Damage.LightningTag       = Manager.AddNativeGameplayTag("P16.Damage.Lightning", "Lightning damage type");
	Singleton.P16Tags.Damage.PhysicalTag        = Manager.AddNativeGameplayTag("P16.Damage.Physical", "Physical damage type");
	Singleton.P16Tags.Effect.HitReactTag        = Manager.AddNativeGameplayTag("P16.Effect.HitReact", "Effect tag granted when Hit Reacting");
	Singleton.P16Tags.Event.Montage.FireballTag = Manager.AddNativeGameplayTag("P16.Event.Montage.Fireball", "Event tag for the fireball montage");

	// Input tags.
	Singleton.P16Tags.Input.MouseButtonLeftTag  = Manager.AddNativeGameplayTag("P16.Input.MouseButtonLeft", "Input tag for the left mouse button");
	Singleton.P16Tags.Input.MouseButtonRightTag = Manager.AddNativeGameplayTag("P16.Input.MouseButtonRight", "Input tag for the right mouse button");
	Singleton.P16Tags.Input.Number1Tag          = Manager.AddNativeGameplayTag("P16.Input.Number1", "Input tag for the number 1 button");
	Singleton.P16Tags.Input.Number2Tag          = Manager.AddNativeGameplayTag("P16.Input.Number2", "Input tag for the number 2 button");
	Singleton.P16Tags.Input.Number3Tag          = Manager.AddNativeGameplayTag("P16.Input.Number3", "Input tag for the number 3 button");
	Singleton.P16Tags.Input.Number4Tag          = Manager.AddNativeGameplayTag("P16.Input.Number4", "Input tag for the number 4 button");

	// Damage types to resistances map.
	// clang-format off
	const FP16TagAttributeDamage&     Damage     = Singleton.P16Tags.Damage;
	const FP16TagAttributeResistance& Resistance = Singleton.P16Tags.Attribute.Resistance;
	Singleton.P16Tags.Maps.DamageTypesToResistances.Add(Damage.ArcaneTag,    Resistance.ArcaneTag);
	Singleton.P16Tags.Maps.DamageTypesToResistances.Add(Damage.FireTag,      Resistance.FireTag);
	Singleton.P16Tags.Maps.DamageTypesToResistances.Add(Damage.LightningTag, Resistance.LightningTag);
	Singleton.P16Tags.Maps.DamageTypesToResistances.Add(Damage.PhysicalTag,  Resistance.PhysicalTag);
	// clang-format on
}
