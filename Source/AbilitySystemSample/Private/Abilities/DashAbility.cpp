#include "Abilities/DashAbility.h"

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
	
	const FVector Dir = Character->GetActorForwardVector().GetSafeNormal();
	Character->LaunchCharacter(Dir * Strength, true, true);
}
