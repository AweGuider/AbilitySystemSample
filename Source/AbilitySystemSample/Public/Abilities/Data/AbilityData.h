#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "AbilityData.generated.h"

class UAbility;

UCLASS(Abstract, BlueprintType)
class ABILITYSYSTEMSAMPLE_API UAbilityData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	FName AbilityId;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<UAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly, Category="Timing", meta=(ClampMin="0"))
	float CooldownSeconds = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Timing", meta=(ClampMin="0"))
	float DurationSeconds = 0.f;
};
