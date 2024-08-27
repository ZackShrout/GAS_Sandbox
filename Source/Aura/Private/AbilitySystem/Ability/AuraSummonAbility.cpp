// Copyright BunnySoft.


#include "AbilitySystem/Ability/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward{ GetAvatarActorFromActorInfo()->GetActorForwardVector() };
	const FVector Location{ GetAvatarActorFromActorInfo()->GetActorLocation() };
	const FVector RightOfSpread{ Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector) };
	const FVector LeftOfSpread{ Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector) };
	const float DeltaSpread{ SpawnSpread / NumMinions };
	
	for (int32 i{ 0 }; i < NumMinions; ++i)
	{
		const FVector Direction{ LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector) };

		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location,
										 Location + Direction * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f);

		DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 10.f, 12.f, FColor::Red, false, 3.f);
		DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 10.f, 12.f, FColor::Red, false, 3.f);
	}

	/*DrawDebugSphere(GetWorld(), Location + RightOfSpread * MinSpawnDistance, 15.f, 12.f, FColor::Red, false, 3.f);
	DrawDebugSphere(GetWorld(), Location + RightOfSpread * MaxSpawnDistance, 15.f, 12.f, FColor::Red, false, 3.f);*/

	/*DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MinSpawnDistance, 15.f, 12.f, FColor::Red, false, 3.f);
	DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MaxSpawnDistance, 15.f, 12.f, FColor::Red, false, 3.f);*/

	return TArray<FVector>();
}
