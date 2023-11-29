// Copyright BunnySoft.


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	return NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
}

void UTargetDataUnderMouse::Activate()
{
	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
		SendMouseCursorData();
	else
	{
		// TODO: We are on the server, so listen for target data
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction{ AbilitySystemComponent.Get() };
	
	FGameplayAbilityTargetData_SingleTargetHit* Data{ new FGameplayAbilityTargetData_SingleTargetHit() };
	Ability->GetCurrentActorInfo()->PlayerController.Get()->GetHitResultUnderCursor(
		ECC_Visibility, false, Data->HitResult);
	
	FGameplayAbilityTargetDataHandle Handle;
	Handle.Add(Data);
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), Handle,
	                                                      FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);
	if (ShouldBroadcastAbilityTaskDelegates())
		ValidData.Broadcast(Handle);
}
