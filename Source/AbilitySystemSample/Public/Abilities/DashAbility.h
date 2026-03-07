#pragma once

#include "Ability.h"

#include "CoreMinimal.h"

#include "DashAbility.generated.h"

UCLASS()
class ABILITYSYSTEMSAMPLE_API UDashAbility : public UAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float Strength = 1500.f;

	virtual bool CanActivate(EAbilityFailReason& OutReason) const override;
	virtual void Activate() override;
};
