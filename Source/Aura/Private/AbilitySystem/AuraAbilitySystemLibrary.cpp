// Copyright BunnySoft.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

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
	const AAuraGameModeBase* AuraGameMode{ Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)) };

	if (!AuraGameMode) return;

	const AActor* AvatarActor{ ASC->GetAvatarActor() };
	const FCharacterClassDefaultInfo ClassDefaultInfo{ AuraGameMode->CharacterClassInfo->GetClassDefaultInfo(CharacterClass) };
	
	FGameplayEffectContextHandle PrimaryContextHandle{ ASC->MakeEffectContext() };
	PrimaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimarySpecHandle{ ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryContextHandle) };
	ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondarySpecHandle = ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->SecondaryAttributes, Level, SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalSpecHandle = ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->VitalAttributes, Level, VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
}
