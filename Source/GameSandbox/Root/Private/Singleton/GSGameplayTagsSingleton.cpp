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

	// Abilities tags.
	Singleton.P16Tags.Ability.Tag         = Manager.AddNativeGameplayTag("P16.Ability", "Ability tag");
	Singleton.P16Tags.Ability.NoneTag     = Manager.AddNativeGameplayTag("P16.Ability.None", "None ability tag");
	Singleton.P16Tags.Ability.AttackTag   = Manager.AddNativeGameplayTag("P16.Ability.Attack", "Attack ability tag");
	Singleton.P16Tags.Ability.SummonTag   = Manager.AddNativeGameplayTag("P16.Ability.Summon", "Summon ability tag");
	Singleton.P16Tags.Ability.HitReactTag = Manager.AddNativeGameplayTag("P16.Ability.HitReact", "HitReact ability tag");

	Singleton.P16Tags.Ability.Fire.FireballTag         = Manager.AddNativeGameplayTag("P16.Ability.Fire.Fireball", "Fireball ability tag");
	Singleton.P16Tags.Ability.Lightning.ElectrocuteTag = Manager.AddNativeGameplayTag("P16.Ability.Lightning.Electrocute", "Electrocute ability tag");
	Singleton.P16Tags.Ability.Status.Tag               = Manager.AddNativeGameplayTag("P16.Ability.Status", "Ability status tag");
	Singleton.P16Tags.Ability.Status.LockedTag         = Manager.AddNativeGameplayTag("P16.Ability.Status.Locked", "Locked ability tag");
	Singleton.P16Tags.Ability.Status.EligibleTag       = Manager.AddNativeGameplayTag("P16.Ability.Status.Eligible", "Eligible ability tag");
	Singleton.P16Tags.Ability.Status.UnlockedTag       = Manager.AddNativeGameplayTag("P16.Ability.Status.Unlocked", "Unlocked ability tag");
	Singleton.P16Tags.Ability.Status.EquippedTag       = Manager.AddNativeGameplayTag("P16.Ability.Status.Equipped", "Equipped ability tag");
	Singleton.P16Tags.Ability.Type.OffensiveTag        = Manager.AddNativeGameplayTag("P16.Ability.Type.Offensive", "Tag for offensive abilities");
	Singleton.P16Tags.Ability.Type.PassiveTag          = Manager.AddNativeGameplayTag("P16.Ability.Type.Passive", "Tag for passive abilities");
	Singleton.P16Tags.Ability.Type.NoneTag             = Manager.AddNativeGameplayTag("P16.Ability.Type.None", "Tag for untyped abilities");

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
	Singleton.P16Tags.Attribute.Meta.IncomingXPTag                 = Manager.AddNativeGameplayTag("P16.Attribute.Meta.IncomingXP", "Amount of incoming experience");

	// Resistance tags.
	Singleton.P16Tags.Attribute.Resistance.ArcaneTag    = Manager.AddNativeGameplayTag("P16.Attribute.Resistance.Arcane", "Resistance to Arcane damage");
	Singleton.P16Tags.Attribute.Resistance.FireTag      = Manager.AddNativeGameplayTag("P16.Attribute.Resistance.Fire", "Resistance to Fire damage");
	Singleton.P16Tags.Attribute.Resistance.LightningTag = Manager.AddNativeGameplayTag("P16.Attribute.Resistance.Lightning", "Resistance to Lightning damage");
	Singleton.P16Tags.Attribute.Resistance.PhysicalTag  = Manager.AddNativeGameplayTag("P16.Attribute.Resistance.Physical", "Resistance to Physical damage");

	// Cooldown tags.
	Singleton.P16Tags.Cooldown.Fire.FireballTag         = Manager.AddNativeGameplayTag("P16.Cooldown.Fire.Fireball", "Fireball cooldown tag");
	Singleton.P16Tags.Cooldown.Lightning.ElectrocuteTag = Manager.AddNativeGameplayTag("P16.Cooldown.Lightning.Electrocute", "Electrocute cooldown tag");

	// Damage tags.
	Singleton.P16Tags.Damage.Tag          = Manager.AddNativeGameplayTag("P16.Damage", "Damage");
	Singleton.P16Tags.Damage.FireTag      = Manager.AddNativeGameplayTag("P16.Damage.Fire", "Fire damage type");
	Singleton.P16Tags.Damage.LightningTag = Manager.AddNativeGameplayTag("P16.Damage.Lightning", "Lightning damage type");
	Singleton.P16Tags.Damage.ArcaneTag    = Manager.AddNativeGameplayTag("P16.Damage.Arcane", "Arcane damage type");
	Singleton.P16Tags.Damage.PhysicalTag  = Manager.AddNativeGameplayTag("P16.Damage.Physical", "Physical damage type");

	// Debuff tags.
	Singleton.P16Tags.Debuff.Type.Tag          = Manager.AddNativeGameplayTag("P16.Debuff.Type", "Parent tag for debuff types.");
	Singleton.P16Tags.Debuff.Type.BurnTag      = Manager.AddNativeGameplayTag("P16.Debuff.Type.Burn", "Burn debuff for fire damage type");
	Singleton.P16Tags.Debuff.Type.StunTag      = Manager.AddNativeGameplayTag("P16.Debuff.Type.Stun", "Stun debuff for lightning damage type");
	Singleton.P16Tags.Debuff.Type.HexTag       = Manager.AddNativeGameplayTag("P16.Debuff.Type.Hex", "Hex debuff for arcane damage type");
	Singleton.P16Tags.Debuff.Type.BleedTag     = Manager.AddNativeGameplayTag("P16.Debuff.Type.Bleed", "Bleed debuff for physical damage type");
	Singleton.P16Tags.Debuff.Info.ChanceTag    = Manager.AddNativeGameplayTag("P16.Debuff.Info.Chance", "Debuff chance");
	Singleton.P16Tags.Debuff.Info.DamageTag    = Manager.AddNativeGameplayTag("P16.Debuff.Info.Damage", "Debuff damage");
	Singleton.P16Tags.Debuff.Info.FrequencyTag = Manager.AddNativeGameplayTag("P16.Debuff.Info.Frequency", "Debuff frequency");
	Singleton.P16Tags.Debuff.Info.DurationTag  = Manager.AddNativeGameplayTag("P16.Debuff.Info.Duration", "Debuff duration");

	// Effect tags.
	Singleton.P16Tags.Effect.HitReactTag     = Manager.AddNativeGameplayTag("P16.Effect.HitReact", "Effect tag granted when Hit Reacting");
	Singleton.P16Tags.Effect.BeingShockedTag = Manager.AddNativeGameplayTag("P16.Effect.BeingShocked", "Effect tag granted when Being Shocked");

	// Event tags.
	Singleton.P16Tags.Event.Montage.FireballTag    = Manager.AddNativeGameplayTag("P16.Event.Montage.Fireball", "Event tag for the fireball montage");
	Singleton.P16Tags.Event.Montage.ElectrocuteTag = Manager.AddNativeGameplayTag("P16.Event.Montage.Electrocute", "Event tag for the electrocute montage");

	// Combat socket tags.
	Singleton.P16Tags.CombatSocket.WeaponTag    = Manager.AddNativeGameplayTag("P16.CombatSocket.Weapon", "Event tag for the weapon attack montage");
	Singleton.P16Tags.CombatSocket.LeftHandTag  = Manager.AddNativeGameplayTag("P16.CombatSocket.LeftHand", "Event tag for the leftHand attack montage");
	Singleton.P16Tags.CombatSocket.RightHandTag = Manager.AddNativeGameplayTag("P16.CombatSocket.RightHand", "Event tag for the rightHand attack montage");
	Singleton.P16Tags.CombatSocket.TailTag      = Manager.AddNativeGameplayTag("P16.CombatSocket.Tail", "Event tag for the tail attack montage");

	// Montage tags.
	Singleton.P16Tags.Montage.Attack.FirstTag  = Manager.AddNativeGameplayTag("P16.Montage.Attack.First", "Event tag for the first attack montage");
	Singleton.P16Tags.Montage.Attack.SecondTag = Manager.AddNativeGameplayTag("P16.Montage.Attack.Second", "Event tag for the second attack montage");
	Singleton.P16Tags.Montage.Attack.ThirdTag  = Manager.AddNativeGameplayTag("P16.Montage.Attack.Third", "Event tag for the third attack montage");
	Singleton.P16Tags.Montage.Attack.FourthTag = Manager.AddNativeGameplayTag("P16.Montage.Attack.Fourth", "Event tag for the fourth attack montage");

	// Input tags.
	Singleton.P16Tags.Input.Tag                 = Manager.AddNativeGameplayTag("P16.Input", "Input tag");
	Singleton.P16Tags.Input.MouseButtonLeftTag  = Manager.AddNativeGameplayTag("P16.Input.MouseButtonLeft", "Input tag for the left mouse button");
	Singleton.P16Tags.Input.MouseButtonRightTag = Manager.AddNativeGameplayTag("P16.Input.MouseButtonRight", "Input tag for the right mouse button");
	Singleton.P16Tags.Input.Number1Tag          = Manager.AddNativeGameplayTag("P16.Input.Number1", "Input tag for the number 1 button");
	Singleton.P16Tags.Input.Number2Tag          = Manager.AddNativeGameplayTag("P16.Input.Number2", "Input tag for the number 2 button");
	Singleton.P16Tags.Input.Number3Tag          = Manager.AddNativeGameplayTag("P16.Input.Number3", "Input tag for the number 3 button");
	Singleton.P16Tags.Input.Number4Tag          = Manager.AddNativeGameplayTag("P16.Input.Number4", "Input tag for the number 4 button");
	Singleton.P16Tags.Input.Passive1Tag         = Manager.AddNativeGameplayTag("P16.Input.Passive1", "Input tag for the passive 1 button");
	Singleton.P16Tags.Input.Passive2Tag         = Manager.AddNativeGameplayTag("P16.Input.Passive2", "Input tag for the passive 2 button");

	// Player block tags.
	Singleton.P16Tags.Player.Blocked.InputPressedTag  = Manager.AddNativeGameplayTag("P16.Player.Blocked.InputPressed", "Block input pressed callback");
	Singleton.P16Tags.Player.Blocked.InputHeldTag     = Manager.AddNativeGameplayTag("P16.Player.Blocked.InputHeld", "Block input held callback");
	Singleton.P16Tags.Player.Blocked.InputReleasedTag = Manager.AddNativeGameplayTag("P16.Player.Blocked.InputReleased", "Block input released callback");
	Singleton.P16Tags.Player.Blocked.CursorTraceTag   = Manager.AddNativeGameplayTag("P16.Player.Blocked.CursorTrace", "Block tracing under the cursor");

	// Damage types to resistances map.
	const FP16TagAttributeDamage&     Damage                   = Singleton.P16Tags.Damage;
	const FP16TagAttributeResistance& Resistance               = Singleton.P16Tags.Attribute.Resistance;
	TMap<FGameplayTag, FGameplayTag>& DamageTypesToResistances = Singleton.P16Tags.Maps.DamageTypesToResistances;
	// clang-format off
	Singleton.P16Tags.Maps.DamageTypesToResistances.Add(Damage.FireTag,      Resistance.FireTag);
	Singleton.P16Tags.Maps.DamageTypesToResistances.Add(Damage.LightningTag, Resistance.LightningTag);
	Singleton.P16Tags.Maps.DamageTypesToResistances.Add(Damage.ArcaneTag,    Resistance.ArcaneTag);
	Singleton.P16Tags.Maps.DamageTypesToResistances.Add(Damage.PhysicalTag,  Resistance.PhysicalTag);
	// clang-format on

	// Damage types to debuffs map.
	const FP16TagDebuff&              Debuff               = Singleton.P16Tags.Debuff;
	TMap<FGameplayTag, FGameplayTag>& DamageTypesToDebuffs = Singleton.P16Tags.Maps.DamageTypesToDebuffs;
	// clang-format off
	DamageTypesToDebuffs.Add(Damage.FireTag,      Debuff.Type.BurnTag);
	DamageTypesToDebuffs.Add(Damage.LightningTag, Debuff.Type.StunTag);
	DamageTypesToDebuffs.Add(Damage.ArcaneTag,    Debuff.Type.HexTag);
	DamageTypesToDebuffs.Add(Damage.PhysicalTag,  Debuff.Type.BleedTag);
	// clang-format on
}
