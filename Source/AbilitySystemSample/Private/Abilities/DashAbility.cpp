#include "Abilities/DashAbility.h"

#include "GameFramework/Character.h"

bool UDashAbility::CanActivate(EAbilityFailReason& OutReason) const
{
	if (!Super::CanActivate(OutReason))
	{
		return false;
	}

	const ACharacter* Char = Cast<ACharacter>(GetAvatarActor());
	if (!Char || !Char->GetCharacterMovement())
	{
		OutReason = EAbilityFailReason::InvalidOwner;
		return false;
	}

	return true;
}

void UDashAbility::Activate()
{
	ACharacter* Char = Cast<ACharacter>(GetAvatarActor());
	if (!ensure(Char))
	{
		return;
	}
	
	const FVector Dir = Char->GetActorForwardVector().GetSafeNormal();
	Char->LaunchCharacter(Dir * Strength, true, true);

	UE_LOG(LogTemp, Log, TEXT("[Ability] Dash activated!"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.2, FColor::Green, TEXT("Dash activated"));
	}
}
