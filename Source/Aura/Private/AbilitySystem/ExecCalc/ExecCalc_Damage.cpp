// Copyright BunnySoft.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

// ATTRIBUTE CAPTURES
struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false); // false = no snapshot
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}
// END ATTRIBUTE CAPTURES

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC{ ExecutionParams.GetSourceAbilitySystemComponent() };
	const UAbilitySystemComponent* TargetASC{ ExecutionParams.GetTargetAbilitySystemComponent() };

	AActor* SourceAvatar{ SourceASC ? SourceASC->GetAvatarActor() : nullptr };
	AActor* TargetAvatar{ TargetASC ? TargetASC->GetAvatarActor() : nullptr };

	ICombatInterface* SourceCombatInterface{ Cast<ICombatInterface>(SourceAvatar) };
	ICombatInterface* TargetCombatInterface{ Cast<ICombatInterface>(TargetAvatar) };

	const FGameplayEffectSpec& Spec{ ExecutionParams.GetOwningSpec() };

	const FGameplayTagContainer* SourceTags{ Spec.CapturedSourceTags.GetAggregatedTags() };
	const FGameplayTagContainer* TargetTags{ Spec.CapturedTargetTags.GetAggregatedTags() };
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	const UCharacterClassInfo* CharacterClassInfo{ UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar) };

	// Get damage set by caller magnitude
	float Damage{ Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage) };

	/*** Block Chance ***/
	float TargetBlockChance{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	const bool bBlocked{ FMath::RandRange(1, 100) < TargetBlockChance };
	
	// Successful block halves incoming damage
	if (bBlocked) Damage /= 2.f;

	/*** Armor and Armor Penetration ***/
	float TargetArmor{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetBlockChance = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPenetration{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(TargetArmor, 0.f);

	const FRealCurve* ArmorPenetrationCurve{ CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString()) };
	const float ArmorPenetrationCoefficient{ ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel()) };

	// Armor penetration ignores a percentage of target's armor
	const float EffectiveArmor{ TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f };

	const FRealCurve* EffectiveArmorCurve{ CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString()) };
	const float EffectiveArmorCoefficient{ EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel()) };
	
	// Armor ignores a percentage of damage
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	/*** Critical Hit Chance, Critical Hit Damage, and Critical Hit Resistance ***/
	float SourceCriticalHitChance{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float TargetCriticalHitResistance{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	float SourceCriticalHitDamage{ 0.f };
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	const FRealCurve* CriticalHitResistanceCurve{ CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString()) };
	const float CriticalHitResistanceCoefficient{ CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel()) };
	
	const float EffectiveCriticalChance{ SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient };
	const bool bCriticalHit{ FMath::RandRange(1, 100) < EffectiveCriticalChance };

	if (bCriticalHit) Damage = (Damage * 2.f) + SourceCriticalHitDamage;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
