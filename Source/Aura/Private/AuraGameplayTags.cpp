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

	// COMBAT
	GameplayTags.Damage = TagManager.AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage"));
}
