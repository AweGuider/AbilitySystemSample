#include "Abilities/DashAbility.h"
#include "Abilities/Data/DashAbilityData.h"

#include "GameFramework/Character.h"

bool UDashAbility::CanActivate(EAbilityFailReason& OutReason) const
{
	if (!Super::CanActivate(OutReason))
	{
		return false;
	}

	const ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (!Character || !Character->GetCharacterMovement())
	{
		OutReason = EAbilityFailReason::InvalidOwner;
		return false;
	}

	return true;
}

void UDashAbility::Activate()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (!ensure(Character))
	{
		return;
	}

	const UDashAbilityData* DashData = Cast<UDashAbilityData>(GetData());
	if (!ensureMsgf(DashData, TEXT("[DashAbility] Requires UDashAbilityData")))
	{
		return;
	}
	const float Strength = DashData->Strength;
	
	const FVector Dir = Character->GetActorForwardVector().GetSafeNormal();
	Character->LaunchCharacter(Dir * Strength, true, true);
}
