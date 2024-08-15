// Copyright BunnySoft.


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AuraGameplayTags.h"

// PROTECTED
void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	if (ICombatInterface* CombatInterface{ Cast<ICombatInterface>(GetAvatarActorFromActorInfo()) })
	{
		const FVector SocketLocation{ CombatInterface->GetCombatSocketLocation() };
		FRotator Rotation{ (TargetLocation - SocketLocation).Rotation() };
		Rotation.Pitch = 0.f;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AAuraProjectile* Projectile{
			GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform,
															GetOwningActorFromActorInfo(),
															Cast<APawn>(GetOwningActorFromActorInfo()),
															ESpawnActorCollisionHandlingMethod::AlwaysSpawn)};

		const UAbilitySystemComponent* SourceASC{ UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()) };
		const FGameplayEffectSpecHandle SpecHandle{ SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext()) };

		const float ScaledDamage{ Damage.GetValueAtLevel(10.f) }; // Hard coded for testing, switch back to GetAbilityLevel()
		const FAuraGameplayTags GameplayTags{ FAuraGameplayTags::Get() };
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);
		
		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
