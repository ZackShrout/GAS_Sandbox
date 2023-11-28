// Copyright BunnySoft.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
class USplineComponent;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AutoRun();
	UAuraAbilitySystemComponent* GetASC();
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	// Enemy Highlighting **
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	bool bTargeting{ false };
	// ** End Enemy Highlighting

	FHitResult CursorHit;

	// Click to Move **
	FVector CachedDestination{ FVector::ZeroVector };
	float FollowTime{ 0.f };
	float ShortPressThreshold{ 0.5f };
	bool bAutoRunning{ false };

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius{ 50.f };

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	// ** End Click to Move
};
