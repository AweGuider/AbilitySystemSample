#pragma once

#include "Ability.h"

#include "CoreMinimal.h"

#include "DashAbility.generated.h"

UCLASS(BlueprintType)
class ABILITYSYSTEMSAMPLE_API UDashAbility : public UAbility
{
	GENERATED_BODY()

public:
	virtual bool CanActivate(EAbilityFailReason& OutReason) const override;
	virtual void Activate() override;
};
