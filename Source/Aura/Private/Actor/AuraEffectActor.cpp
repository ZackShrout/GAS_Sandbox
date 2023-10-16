// Copyright BunnySoft.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC{ UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor) };
	if (!TargetASC) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle{ TargetASC->MakeEffectContext() };
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle{ TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle) };
	const FActiveGameplayEffectHandle ActiveEffectHandle{ TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()) };

	if (EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite &&
		InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC{ UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor) };
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (const auto HandlePair : ActiveEffectHandles)
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}

		for (const auto& Handle : HandlesToRemove)
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
	}
}
