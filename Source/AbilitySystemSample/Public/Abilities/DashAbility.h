#pragma once

#include "Ability.h"

#include "CoreMinimal.h"

#include "DashAbility.generated.h"

UCLASS()
class ABILITYSYSTEMSAMPLE_API UDashAbility : public UAbility
{
	GENERATED_BODY()

public:
	virtual bool CanActivate(EAbilityFailReason& OutReason) const override;
	virtual void Activate() override;

	virtual float GetCooldownSeconds() const override { return CooldownSeconds;}
	
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float Strength = 1500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Dash", meta=(ClampMin="0"))
	float CooldownSeconds = 1.0f;
};
