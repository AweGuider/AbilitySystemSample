#pragma once

#include "Data/AbilityData.h"

#include "CoreMinimal.h"

#include "Ability.generated.h"

class UAbilityComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogAbilitySystem, Log, All);

UENUM()
enum class EAbilityFailReason : uint8
{
	None,
	NotFound,
	OnCooldown,
	InvalidOwner,
	Blocked,
};

inline const TCHAR* LexToString(EAbilityFailReason Reason)
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

UCLASS(Abstract, BlueprintType)
class ABILITYSYSTEMSAMPLE_API UAbility : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UAbilityComponent* InOwnerComp, const UAbilityData* InData);

	FName GetAbilityId() const { return AbilityId; }
	const UAbilityData* GetData() const { return Data; }

	virtual bool CanActivate(EAbilityFailReason& OutReason) const;
	virtual void Activate();

	virtual float GetCooldownSeconds() const { return 0.f; }

protected:
	UAbilityComponent* GetOwnerComp() const { return OwnerComp.Get(); }
	AActor* GetAvatarActor() const;
	
	UPROPERTY()
	FName AbilityId;
	UPROPERTY()
	TObjectPtr<const UAbilityData> Data = nullptr;
	
	TWeakObjectPtr<UAbilityComponent> OwnerComp;
};
