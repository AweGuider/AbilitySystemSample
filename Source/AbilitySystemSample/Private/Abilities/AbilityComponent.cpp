#include "Abilities/AbilityComponent.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UAbilityComponent::TryActivateAbility(const FName AbilityId) const
{
	UAbility* Ability = FindAbility(AbilityId);
	if (!Ability)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityComponent] Ability '%s' not found"), *AbilityId.ToString());
		return false;
	}

	EAbilityFailReason Reason = EAbilityFailReason::None;
	if (!Ability-> CanActivate(Reason))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityComponent] Ability '%s' failed. Reason: %d"), *AbilityId.ToString(), (int32)Reason);
		return false;
	}

	Ability->Activate();
	UE_LOG(LogTemp, Log, TEXT("[AbilityComponent] Activated ability '%s'"), *AbilityId.ToString());
	return true;
}

UAbility* UAbilityComponent::FindAbility(FName AbilityId) const
{
	if (const TObjectPtr<UAbility>* Found = Abilities.Find(AbilityId))
	{
		return Found->Get();
	}
	return nullptr;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	UDashAbility* Dash = NewObject<UDashAbility>(this);
	if (ensure(Dash))
	{
		Dash->Initialize(this, "Dash");
		ensureMsgf(!Abilities.Contains(Dash->GetAbilityId()),
			TEXT("Duplicate ability ID '%s'"), *Dash->GetAbilityId().ToString());

		Abilities.Add(Dash->GetAbilityId(), Dash);
	}
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
