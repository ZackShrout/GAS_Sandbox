// Copyright BunnySoft.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Singleton containing native gameplay tags
 */
struct FAuraGameplayTags
{
 static const FAuraGameplayTags& Get() { return GameplayTags; }
 static void InitializeNativeGameplayTags();
 
protected:

private:
 static FAuraGameplayTags GameplayTags;
};
