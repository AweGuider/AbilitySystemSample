#pragma once

#include "DashAbility.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AbilityComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ABILITYSYSTEMSAMPLE_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAbilityComponent();

	bool TryActivateAbility(FName AbilityId) const;
	UAbility* FindAbility(FName AbilityId) const;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TMap<FName, TObjectPtr<UAbility>> Abilities;
};
