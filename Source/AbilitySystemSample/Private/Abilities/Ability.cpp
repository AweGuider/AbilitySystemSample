#include "Abilities/Ability.h"

#include "Abilities/AbilityComponent.h"

DEFINE_LOG_CATEGORY(LogAbilitySystem);

void UAbility::Initialize(UAbilityComponent* InOwnerComp, const UAbilityData* InData)
{
	OwnerComp = InOwnerComp;
	Data = InData;
	AbilityId = InData ? InData->AbilityId : NAME_None;
}

bool UAbility::CanActivate(EAbilityFailReason& OutReason) const
{
	if (!OwnerComp.IsValid() || !GetAvatarActor())
	{
		OutReason = EAbilityFailReason::InvalidOwner;
		return false;
	}

	OutReason = EAbilityFailReason::None;
	return true;
}

void UAbility::Activate()
{
	// Base does nothing. Derived abilities implement behavior.
}

AActor* UAbility::GetAvatarActor() const
{
	return OwnerComp.IsValid() ? OwnerComp->GetOwner() : nullptr;
}
