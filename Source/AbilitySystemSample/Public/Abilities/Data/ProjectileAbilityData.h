#pragma once

#include "AbilityData.h"
#include "Abilities/AbilityProjectile.h"

#include "Engine/DataAsset.h"

#include "ProjectileAbilityData.generated.h"

UCLASS(BlueprintType)
class ABILITYSYSTEMSAMPLE_API UProjectileAbilityData : public UAbilityData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AAbilityProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	FVector MuzzleOffset = FVector(100, 0, 50);

	UPROPERTY(EditDefaultsOnly, Category="Projectile", meta=(ClampMin="0"))
	float InitialSpeed = 2500.f;

	UPROPERTY(EditDefaultsOnly, Category="Projectile", meta=(ClampMin="0"))
	float LifeSeconds = 3.f;
};
