#include "Abilities/AbilityComponent.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UAbilityComponent::TryActivateAbility(const FName AbilityId)
{
	UAbility* Ability = FindAbility(AbilityId);
	if (!Ability)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityComponent] Ability '%s' not found"), *AbilityId.ToString());
		return false;
	}

	const float Remaining = GetCooldownRemaining(AbilityId);
	if (Remaining > KINDA_SMALL_NUMBER)
	{
		OnAbilityFailed.Broadcast(AbilityId, EAbilityFailReason::OnCooldown, Remaining);
		UE_LOG(LogTemp, Warning, TEXT("[AbilityComponent] Ability '%s' is on cooldown (%.2fs)"),
			*AbilityId.ToString(), Remaining);
		return false;
	}

	EAbilityFailReason Reason = EAbilityFailReason::None;
	if (!Ability-> CanActivate(Reason))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AbilityComponent] Ability '%s' failed. Reason: %d"), *AbilityId.ToString(), (int32)Reason);
		return false;
	}

	Ability->Activate();

	OnAbilityActivated.Broadcast(AbilityId, Ability);
	StartCooldown(AbilityId, Ability->GetCooldownSeconds());
	
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

float UAbilityComponent::GetCooldownRemaining(FName AbilityId) const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return 0.f;
	}

	const float Now = World->GetTimeSeconds();
	const float* EndTime = CooldownEndTimes.Find(AbilityId);
	if (!EndTime)
	{
		return 0.f;
	}
	return FMath::Max(0.f, *EndTime - Now);
}

bool UAbilityComponent::IsOnCooldown(FName AbilityId) const
{
	return GetCooldownRemaining(AbilityId) > KINDA_SMALL_NUMBER;
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

void UAbilityComponent::StartCooldown(FName AbilityId, float CooldownSeconds)
{
	if (CooldownSeconds <= 0.f)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!ensure(World))
	{
		return;
	}

	const float Now = World->GetTimeSeconds();
	CooldownEndTimes.Add(AbilityId, Now + CooldownSeconds);

	OnCooldownChanged.Broadcast(AbilityId, CooldownSeconds);

	FTimerHandle& Handle = CooldownTimerHandles.FindOrAdd(AbilityId);
	World->GetTimerManager().ClearTimer(Handle);

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UAbilityComponent::HandleCooldownFinished, AbilityId);

	World->GetTimerManager().SetTimer(Handle, Delegate, CooldownSeconds, false);
}

void UAbilityComponent::HandleCooldownFinished(FName AbilityId)
{
	CooldownEndTimes.Remove(AbilityId);

	if (FTimerHandle* Handle = CooldownTimerHandles.Find(AbilityId))
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(*Handle);
		}
	}

	OnCooldownChanged.Broadcast(AbilityId, 0.f);
}
