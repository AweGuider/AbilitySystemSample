#pragma once

#include "CoreMinimal.h"

#include "Ability.generated.h"

class UAbilityComponent;

UENUM()
enum class EAbilityFailReason : uint8
{
	None,
	NotFound,
	InvalidOwner,
	Blocked,
};

UCLASS(Abstract)
class ABILITYSYSTEMSAMPLE_API UAbility : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UAbilityComponent* InOwnerComp, const FName InAbilityId);

	FName GetAbilityId() const { return AbilityId; }

	virtual bool CanActivate(EAbilityFailReason& OutReason) const;
	virtual void Activate();

protected:
	UAbilityComponent* GetOwnerComp() const { return OwnerComp.Get(); }
	AActor* GetAvatarActor() const;
	
	TWeakObjectPtr<UAbilityComponent> OwnerComp;
	FName AbilityId;
};
