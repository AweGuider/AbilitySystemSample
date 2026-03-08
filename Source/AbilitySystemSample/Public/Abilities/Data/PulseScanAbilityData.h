#pragma once

#include "AbilityData.h"

#include "Engine/DataAsset.h"

#include "PulseScanAbilityData.generated.h"

UCLASS(BlueprintType)
class ABILITYSYSTEMSAMPLE_API UPulseScanAbilityData : public UAbilityData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category="PulseScan", meta=(ClampMin="0"))
	float Radius = 400.f;

	UPROPERTY(EditDefaultsOnly, Category="PulseScan")
	TEnumAsByte<ECollisionChannel> Channel = ECC_Pawn;

	UPROPERTY(EditDefaultsOnly, Category="PulseScan")
	bool bDrawDebug = true;

	UPROPERTY(EditDefaultsOnly, Category="PulseScan", meta=(ClampMin="0"))
	float DebugDrawSeconds = 3.f;
};
