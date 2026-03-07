#include "Abilities/AbilityComponent.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UAbilityComponent::TryActivateAbility(FName AbilityId)
{
	if (AbilityId != "Dash")
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityComponent] Unknown ability ID '%s'"), *AbilityId.ToString());
		return false;
	}

	if (!ensure(DashAbility))
	{
		return false;
	}

	EAbilityFailReason Reason = EAbilityFailReason::None;
	if (!DashAbility-> CanActivate(Reason))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityComponent] Failed to activate ability '%s'. Reason: %d"), *AbilityId.ToString(), (int32)Reason);
		return false;
	}

	DashAbility->Activate();
	return true;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	DashAbility = NewObject<UDashAbility>(this);
	if (ensure(DashAbility))
	{
		DashAbility->Initialize(this);
	}
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
