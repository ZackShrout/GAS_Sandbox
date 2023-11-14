// Copyright BunnySoft.


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag,
                                                           bool bLogNotFound/* = false*/) const
{
	for (const auto& Info : AttributeInformation)
	{
		if (Info.AttributeTag == AttributeTag)
			return Info;
	}

	if (bLogNotFound)
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));

	return FAuraAttributeInfo();
}
