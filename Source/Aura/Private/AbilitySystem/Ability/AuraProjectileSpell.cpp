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

	if (!HasAuthority(&ActivationInfo)) return;

	if (ICombatInterface* CombatInterface{ Cast<ICombatInterface>(GetAvatarActorFromActorInfo()) })
	{
		const FVector SocketLocation{ CombatInterface->GetCombatSocketLocation() };
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		// TODO: Set projectile rotation

		AAuraProjectile* Projectile{
			GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform,
			                                                GetOwningActorFromActorInfo(),
			                                                Cast<APawn>(GetOwningActorFromActorInfo()),
			                                                ESpawnActorCollisionHandlingMethod::AlwaysSpawn)};

		// TODO: Give projectile a gameplay effect spec to deal damage

		Projectile->FinishSpawning(SpawnTransform);
	}
}
