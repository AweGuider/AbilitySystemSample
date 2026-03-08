#pragma once

#include "Ability.h"

#include "CoreMinimal.h"

#include "ProjectileAbility.generated.h"

class UProjectileAbilityData;

UCLASS(BlueprintType)
class ABILITYSYSTEMSAMPLE_API UProjectileAbility : public UAbility
{
	GENERATED_BODY()

public:
	virtual bool CanActivate(EAbilityFailReason& OutReason) const override;
	virtual void Activate() override;

private:
	const UProjectileAbilityData* GetProjectileData() const;
};
