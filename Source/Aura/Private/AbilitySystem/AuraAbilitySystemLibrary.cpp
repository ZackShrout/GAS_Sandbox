// Copyright BunnySoft.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC{ UGameplayStatics::GetPlayerController(WorldContextObject, 0) })
	{
		if (AAuraHUD* AuraHUD{ Cast<AAuraHUD>(PC->GetHUD()) })
		{
			AAuraPlayerState* PS{ PC->GetPlayerState<AAuraPlayerState>() };
			UAbilitySystemComponent* ASC{ PS->GetAbilitySystemComponent() };
			UAttributeSet* AS{ PS->GetAttributeSet() };

			const FWidgetControllerParams Params{ PC, PS, ASC, AS };
			
			return AuraHUD->GetOverlayWidgetController(Params);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC{ UGameplayStatics::GetPlayerController(WorldContextObject, 0) })
	{
		if (AAuraHUD* AuraHUD{ Cast<AAuraHUD>(PC->GetHUD()) })
		{
			AAuraPlayerState* PS{ PC->GetPlayerState<AAuraPlayerState>() };
			UAbilitySystemComponent* ASC{ PS->GetAbilitySystemComponent() };
			UAttributeSet* AS{ PS->GetAttributeSet() };

			const FWidgetControllerParams Params{ PC, PS, ASC, AS };

			return AuraHUD->GetAttributeMenuWidgetController(Params);
		}
	}

	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                            ECharacterClass CharacterClass, float Level,
                                                            UAbilitySystemComponent* ASC)
{
	const AActor* AvatarActor{ ASC->GetAvatarActor() };
	UCharacterClassInfo* CharacterClassInfo{ GetCharacterClassInfo(WorldContextObject) };
	const FCharacterClassDefaultInfo ClassDefaultInfo{ CharacterClassInfo->GetClassDefaultInfo(CharacterClass) };
	
	FGameplayEffectContextHandle PrimaryContextHandle{ ASC->MakeEffectContext() };
	PrimaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimarySpecHandle{ ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryContextHandle) };
	ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondarySpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo{ GetCharacterClassInfo(WorldContextObject) };
	if (!CharacterClassInfo) return;
	
	for (const auto& AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec{ AbilityClass, 1 };
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& DefaultInfo{ CharacterClassInfo->GetClassDefaultInfo(CharacterClass) };
	for (const auto& AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ICombatInterface* CombatInterface{Cast<ICombatInterface>(ASC->GetAvatarActor())})
		{
			FGameplayAbilitySpec AbilitySpec{ AbilityClass, CombatInterface->GetPlayerLevel() };
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode{ Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)) };
	if (!AuraGameMode) return nullptr;

	return AuraGameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext{ static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()) })
		return AuraEffectContext->IsBlockedHit();

	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext{ static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()) })
		return AuraEffectContext->IsCriticalHit();

	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext{ static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()) })
		AuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext{ static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()) })
		AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World{ GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) })
	{
		World->OverlapMultiByObjectType(
			Overlaps, SphereOrigin, FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), SphereParams);

		for (const FOverlapResult& OverlapResult : Overlaps)
		{
			if (OverlapResult.GetActor()->Implements<UCombatInterface>() &&
				!ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
			{
				OutOverlappingActors.AddUnique(OverlapResult.GetActor());
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	if (FirstActor->ActorHasTag("Player") && SecondActor->ActorHasTag("Player")) return false;
	if (FirstActor->ActorHasTag("Enemy") && SecondActor->ActorHasTag("Enemy")) return false;

	return true;
}
