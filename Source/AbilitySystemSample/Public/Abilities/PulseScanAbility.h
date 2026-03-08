#pragma once

#include "Ability.h"

#include "CoreMinimal.h"

#include "PulseScanAbility.generated.h"

class UPulseScanAbilityData;

UCLASS(BlueprintType)
class ABILITYSYSTEMSAMPLE_API UPulseScanAbility : public UAbility
{
	GENERATED_BODY()

public:
	virtual bool CanActivate(EAbilityFailReason& OutReason) const override;
	virtual void Activate() override;

private:
	const UPulseScanAbilityData* GetPulseData() const;
};
