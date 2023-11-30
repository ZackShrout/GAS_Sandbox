// Copyright BunnySoft.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

// PUBLIC
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
}

// PROTECTED
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
		Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

// PRIVATE
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnhighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC())
		GetASC()->AbilityInputTagReleased(InputTag);

	// Bypass movement if this is not LMB or we are releasing after using an ability
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) || bTargeting || bShiftKeyDown) return;

	// We are releasing LMB after moving, not after using an ability
	const APawn* ControlledPawn{ GetPawn() };

	// Check to see if this was a short press, i.e., if we are pathing to a point clicked
	if (FollowTime <= ShortPressThreshold && ControlledPawn)
	{
		if (UNavigationPath* NavPath{UNavigationSystemV1::FindPathToLocationSynchronously(
			this,ControlledPawn->GetActorLocation(),CachedDestination)})
		{
			Spline->ClearSplinePoints();
			for (const auto& PointLoc : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
			}

			CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
			bAutoRunning = true;
		}
	}

	FollowTime = 0.f;
	bTargeting = false;
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// Check to see if we are using an ability
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) ||
		bTargeting || bShiftKeyDown)
	{
		if (GetASC())
			GetASC()->AbilityInputTagHeld(InputTag);

		return;
	}

	// We are moving, not using an ability
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	if (CursorHit.bBlockingHit)
		CachedDestination = CursorHit.ImpactPoint;

	if (APawn* ControlledPawn{ GetPawn() })
	{
		const FVector WorldDirection{ (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal() };
		ControlledPawn->AddMovementInput(WorldDirection);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	if (APawn* ControlledPawn{ GetPawn() })
	{
		const FVector LocationOnSpline{
			Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World) };
		const FVector Direction{
			Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World) };
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination{ static_cast<float>((LocationOnSpline - CachedDestination).Length()) };
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::ShiftPressed()
{
	bShiftKeyDown = true;
}

void AAuraPlayerController::ShiftReleased()
{
	bShiftKeyDown = false;
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AuraAbilitySystemComponent;
}
