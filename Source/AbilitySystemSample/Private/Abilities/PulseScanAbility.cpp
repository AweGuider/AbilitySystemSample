#include "Abilities/PulseScanAbility.h"

#include "Abilities/Data/PulseScanAbilityData.h"

bool UPulseScanAbility::CanActivate(EAbilityFailReason& OutReason) const
{
	if (!Super::CanActivate(OutReason))
	{
		return false;
	}

	const UPulseScanAbilityData* PulseScanData = GetPulseData();
	if (!PulseScanData)
	{
		OutReason = EAbilityFailReason::Blocked;
		return false;
	}

	if (PulseScanData->Radius <= 0.f)
	{
		OutReason = EAbilityFailReason::Blocked;
		return false;
	}

	OutReason = EAbilityFailReason::None;
	return true;
}

void UPulseScanAbility::Activate()
{
	const AActor* Avatar = GetAvatarActor();
	if (!ensure(Avatar))
	{
		return;
	}

	const UPulseScanAbilityData* PulseScanData  = GetPulseData();
	if (!ensureMsgf(PulseScanData , TEXT("[PulseScanAbility] Requires UProjectileAbilityData")))
	{
		return;
	}

	const UWorld* World = Avatar->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	const FVector Center = Avatar->GetActorLocation();
	const float Radius = PulseScanData->Radius;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(PulseScan), /*bTraceComplex*/ false);
	Params.AddIgnoredActor(Avatar);

	TArray<FOverlapResult> Overlaps;
	const bool bAny = World->OverlapMultiByChannel(
		Overlaps,
		Center,
		FQuat::Identity,
		PulseScanData->Channel,
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	UE_LOG(LogAbilitySystem, Log, TEXT("[PulseScanAbility] PulseScan '%s': %s (%d results, radius=%.0f)"),
		*GetAbilityId().ToString(),
		bAny ? TEXT("HIT") : TEXT("EMPTY"),
		Overlaps.Num(),
		Radius
	);

	for (const FOverlapResult& Result : Overlaps)
	{
		if (AActor* HitActor = Result.GetActor())
		{
			UE_LOG(LogAbilitySystem, Log, TEXT("  - Hit: %s"), *HitActor->GetActorLabel());
		}
	}

	if (PulseScanData->bDrawDebug)
	{
		DrawDebugSphere(World, Center, Radius, 24, bAny ? FColor::Green : FColor::Red,
			false, PulseScanData->DebugDrawSeconds, 0, 2.f);
	}
}

const UPulseScanAbilityData* UPulseScanAbility::GetPulseData() const
{
	return Cast<UPulseScanAbilityData>(GetData());
}
