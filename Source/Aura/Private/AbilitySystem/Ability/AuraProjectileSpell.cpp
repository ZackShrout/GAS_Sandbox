// Copyright BunnySoft.


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Kismet/KismetSystemLibrary.h"

// PROTECTED
void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UKismetSystemLibrary::PrintString(this, FString("Have at you!"), true, true, FColor::Yellow, 3.f);
}
