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
		const FGameplayAbilitySpecHandle SpecHandle{ GetAbilitySpecHandle() };
		const FPredictionKey PredictionKey{ GetActivationPredictionKey() };

		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey)
		                      .AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		if (!AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey))
		{
			SetWaitingOnRemotePlayerData();
		}
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

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
		ValidData.Broadcast(DataHandle);
}
