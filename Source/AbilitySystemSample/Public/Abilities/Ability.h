#pragma once

#include "CoreMinimal.h"

#include "Ability.generated.h"

class UAbilityComponent;

UENUM()
enum class EAbilityFailReason : uint8
{
	None,
	NotFound,
	OnCooldown,
	InvalidOwner,
	Blocked,
};

static const TCHAR* LexToString(EAbilityFailReason Reason)
{
	switch (Reason)
	{
	case EAbilityFailReason::None:        return TEXT("None");
	case EAbilityFailReason::NotFound:    return TEXT("NotFound");
	case EAbilityFailReason::OnCooldown:  return TEXT("OnCooldown");
	case EAbilityFailReason::InvalidOwner:return TEXT("InvalidOwner");
	case EAbilityFailReason::Blocked:     return TEXT("Blocked");
	default:                              return TEXT("Unknown");
	}
}

UCLASS(Abstract)
class ABILITYSYSTEMSAMPLE_API UAbility : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UAbilityComponent* InOwnerComp, const FName InAbilityId);

	FName GetAbilityId() const { return AbilityId; }

	virtual bool CanActivate(EAbilityFailReason& OutReason) const;
	virtual void Activate();

	virtual float GetCooldownSeconds() const { return 0.f; }

protected:
	UAbilityComponent* GetOwnerComp() const { return OwnerComp.Get(); }
	AActor* GetAvatarActor() const;
	
	TWeakObjectPtr<UAbilityComponent> OwnerComp;
	FName AbilityId;
};
