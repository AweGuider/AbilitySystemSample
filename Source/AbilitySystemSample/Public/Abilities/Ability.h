#pragma once

#include "CoreMinimal.h"

#include "Ability.generated.h"

class UAbilityComponent;

UENUM()
enum class EAbilityFailReason : uint8
{
	None,
	InvalidOwner,
	Blocked,
};

UCLASS(Abstract)
class ABILITYSYSTEMSAMPLE_API UAbility : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UAbilityComponent* InOwnerComp);

	virtual bool CanActivate(EAbilityFailReason& OutReason) const;
	virtual void Activate();

protected:
	UAbilityComponent* GetOwnerComp() const { return OwnerComp.Get(); }
	AActor* GetAvatarActor() const;
	
	TWeakObjectPtr<UAbilityComponent> OwnerComp;
};
