// Copyright BunnySoft.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& TagManager{ UGameplayTagsManager::Get() };

	// PRIMARY ATTRIBUTES
	GameplayTags.Attributes_Primary_Strength = TagManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage"));
	
	GameplayTags.Attributes_Primary_Intelligence = TagManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damage"));
	
	GameplayTags.Attributes_Primary_Resilience = TagManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases armor and armor penetration"));
	
	GameplayTags.Attributes_Primary_Vigor = TagManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health"));

	// SECONDARY ATTRIBUTES
	GameplayTags.Attributes_Secondary_Armor = TagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, increases Block Chance"));
	
	GameplayTags.Attributes_Secondary_ArmorPenetration = TagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignores percentage of enemy Armor, increases Critical Hit Chance"));
	
	GameplayTags.Attributes_Secondary_BlockChance = TagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half"));
	
	GameplayTags.Attributes_Secondary_CriticalHitChance = TagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage, plus Critical Hit Damage (bonus damage)"));
	
	GameplayTags.Attributes_Secondary_CriticalHitDamage = TagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a Critical Hit is scored"));
	
	GameplayTags.Attributes_Secondary_CriticalHitResistance = TagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies"));
	
	GameplayTags.Attributes_Secondary_HealthRegeneration = TagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every second"));
	
	GameplayTags.Attributes_Secondary_ManaRegeneration = TagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every second"));
	
	GameplayTags.Attributes_Secondary_MaxHealth = TagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("The total amount of Health a character can have"));
	
	GameplayTags.Attributes_Secondary_MaxMana = TagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("The total amount of Mana a character can have"));

	// RESISTANCES
	GameplayTags.Attributes_Resistance_Fire = TagManager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("Resistance to Fire Damage"));

	GameplayTags.Attributes_Resistance_Lightning = TagManager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Resistance to Lightning Damage"));

	GameplayTags.Attributes_Resistance_Arcane = TagManager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"),
		FString("Resistance to Arcane Damage"));

	GameplayTags.Attributes_Resistance_Physical = TagManager.AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Resistance to Physical Damage"));

	// Input Tags
	GameplayTags.InputTag_LMB = TagManager.AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input tag for Left Mouse Button"));

	GameplayTags.InputTag_RMB = TagManager.AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input tag for Right Mouse Button"));

	GameplayTags.InputTag_1 = TagManager.AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input tag for the 1 key"));

	GameplayTags.InputTag_2 = TagManager.AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input tag for the 2 key"));

	GameplayTags.InputTag_3 = TagManager.AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input tag for the 3 key"));

	GameplayTags.InputTag_4 = TagManager.AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input tag for the 4 key"));

	// DAMAGE
	GameplayTags.Damage = TagManager.AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage"));

	GameplayTags.Damage_Fire = TagManager.AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type"));

	GameplayTags.Damage_Lightning = TagManager.AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Type"));

	GameplayTags.Damage_Arcane = TagManager.AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage Type"));

	GameplayTags.Damage_Physical = TagManager.AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type"));

	// ABILITIES
	GameplayTags.Abilities_Attack = TagManager.AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability Tag"));

	// EFFECTS
	GameplayTags.Effects_HitReact = TagManager.AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when Hit Reacting"));

	// MONTAGE
	GameplayTags.Montage_Attack_Weapon = TagManager.AddNativeGameplayTag(
		FName("Montage.Attack.Weapon"),
		FString("Weapon"));

	GameplayTags.Montage_Attack_RightHand = TagManager.AddNativeGameplayTag(
		FName("Montage.Attack.RightHand"),
		FString("Right Hand"));

	GameplayTags.Montage_Attack_LeftHand = TagManager.AddNativeGameplayTag(
		FName("Montage.Attack.LeftHand"),
		FString("Left Hand"));
	
	// Map of Damage Types to Resistances
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
}
