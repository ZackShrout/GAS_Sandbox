// Copyright BunnySoft.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

// PUBLIC
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS{ CastChecked<UAuraAttributeSet>(AttributeSet) };
	check(AttributeInfo);

	for (const auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS{ CastChecked<UAuraAttributeSet>(AttributeSet) };
	check(AttributeInfo);

	for (const auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			});
	}
}

// PRIVATE
void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info{ AttributeInfo->FindAttributeInfoForTag(AttributeTag) };
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
