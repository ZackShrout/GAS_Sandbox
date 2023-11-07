// Copyright BunnySoft.


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
	
}

void AAuraCharacterBase::InitializePrimaryAttributes() const
{
	check(DefaultPrimaryAttributes);
	UAbilitySystemComponent* ASC{ GetAbilitySystemComponent() };
	check(IsValid(ASC));

	const FGameplayEffectContextHandle ContextHandle{ ASC->MakeEffectContext() };
	const FGameplayEffectSpecHandle SpecHandle{ ASC->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle) };
	ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
}
