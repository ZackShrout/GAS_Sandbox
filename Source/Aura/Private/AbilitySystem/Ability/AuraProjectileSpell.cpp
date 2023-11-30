// Copyright BunnySoft.


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

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

		// TODO: Give projectile a gameplay effect spec to deal damage

		Projectile->FinishSpawning(SpawnTransform);
	}
}
