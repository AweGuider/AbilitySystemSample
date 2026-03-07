#pragma once

#include "AbilityData.h"

#include "Engine/DataAsset.h"

#include "DashAbilityData.generated.h"

UCLASS(BlueprintType)
class ABILITYSYSTEMSAMPLE_API UDashAbilityData : public UAbilityData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category="Dash", meta=(ClampMin="0"))
	float Strength = 1500.f;
};
