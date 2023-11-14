// Copyright BunnySoft.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Orange, FString("Attribute Menu Widget Controller Constructed"));
}
