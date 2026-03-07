#pragma once

#include "DashAbility.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AbilityComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAbilityActivated, FName /*AbilityId*/, UAbility* /*Ability*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAbilitFailed, FName /*AbilityId*/, EAbilityFailReason /*Reason*/, float /*CooldownRemaining*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCooldownChanged, FName /*AbilityId*/, float /*NewRemaining*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ABILITYSYSTEMSAMPLE_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAbilityComponent();

	bool TryActivateAbility(FName AbilityId);
	UAbility* FindAbility(FName AbilityId) const;

	float GetCooldownRemaining(FName AbilityId) const;
	bool IsOnCooldown(FName AbilityId) const;

	FOnAbilityActivated OnAbilityActivated;
	FOnAbilitFailed OnAbilityFailed;
	FOnCooldownChanged OnCooldownChanged;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void StartCooldown(FName AbilityId, float CooldownSeconds);
	void HandleCooldownFinished(FName AbilityId);
	
	UPROPERTY()
	TMap<FName, TObjectPtr<UAbility>> Abilities;

	UPROPERTY()
	TMap<FName, float> CooldownEndTimes;

	TMap<FName, FTimerHandle> CooldownTimerHandles;
};
