#include "Abilities/AbilityComponent.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UAbilityComponent::TryActivateAbility(const FName AbilityId)
{
	UAbility* Ability = FindAbility(AbilityId);
	if (!Ability)
	{
		OnAbilityFailed.Broadcast(AbilityId, EAbilityFailReason::NotFound, 0.f);
		UE_LOG(LogAbilitySystem, Warning, TEXT("[AbilityComponent] Ability '%s' not found"),
			*AbilityId.ToString());
		return false;
	}

	const float Remaining = GetCooldownRemaining(AbilityId);
	if (Remaining > KINDA_SMALL_NUMBER)
	{
		OnAbilityFailed.Broadcast(AbilityId, EAbilityFailReason::OnCooldown, Remaining);
		UE_LOG(LogAbilitySystem, Warning, TEXT("[AbilityComponent] Ability '%s' is on cooldown (%.2fs)"),
			*AbilityId.ToString(), Remaining);
		return false;
	}

	EAbilityFailReason Reason = EAbilityFailReason::None;
	if (!Ability-> CanActivate(Reason))
	{
		OnAbilityFailed.Broadcast(AbilityId, Reason, 0.f);
		UE_LOG(LogAbilitySystem, Warning, TEXT("[AbilityComponent] Ability '%s' failed. Reason: %d"),
			*AbilityId.ToString(), (int32)Reason);
		return false;
	}

	Ability->Activate();

	OnAbilityActivated.Broadcast(AbilityId, Ability);
	
	const UAbilityData* Data = Ability->GetData();
	const float Cooldown = Data ? Data->CooldownSeconds : 0.f;
	StartCooldown(AbilityId, Cooldown);
	
	UE_LOG(LogAbilitySystem, Log, TEXT("[AbilityComponent] Activated ability '%s'"),
		*AbilityId.ToString());
	return true;
}

UAbility* UAbilityComponent::FindAbility(const FName AbilityId) const
{
	if (const TObjectPtr<UAbility>* Found = Abilities.Find(AbilityId))
	{
		return Found->Get();
	}
	return nullptr;
}

float UAbilityComponent::GetCooldownRemaining(const FName AbilityId) const
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

bool UAbilityComponent::IsOnCooldown(const FName AbilityId) const
{
	return GetCooldownRemaining(AbilityId) > KINDA_SMALL_NUMBER;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	Abilities.Reset();

	if (AbilityDataAssets.Num() == 0)
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("[AbilitySystem] No AbilityDataAssets assigned on %s"),
			*GetOwner()->GetName());
		return;
	}

	for (const UAbilityData* Data : AbilityDataAssets)
	{
		if (!ensureMsgf(Data, TEXT("[AbilitySystem] Null AbilityDataAssets entry on %s"),
			*GetOwner()->GetName()))
		{
			continue;
		}
		if (!ensureMsgf(!Data->AbilityId.IsNone(), TEXT("[AbilitySystem] AbilityData '%s' has invalid AbilityId"),
			*Data->GetName()))
		{
			continue;
		}
		if (!ensureMsgf(Data->AbilityClass, TEXT("[AbilitySystem] AbilityData '%s' has null AbilityClass"),
			*Data->GetName()))
		{
			continue;
		}
		if (!ensureMsgf(!Abilities.Contains(Data->AbilityId),
			TEXT("[AbilitySystem] Duplicate AbilityId '%s' (asset: %s)"),
			*Data->AbilityId.ToString(), *GetNameSafe(Data)))
		{
			continue;
		}

		UAbility* Ability = NewObject<UAbility>(this, Data->AbilityClass);
		if (!ensureMsgf(Ability, TEXT("[AbilitySystem] Failed to create ability instance from class '%s' for id %s"),
			*GetNameSafe(*Data->AbilityClass), *Data->AbilityId.ToString()))
		{
			continue;
		}

		Ability->Initialize(this, Data);
		Abilities.Add(Data->AbilityId, Ability);
	}
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAbilityComponent::StartCooldown(FName AbilityId, const float CooldownSeconds)
{
	if (CooldownSeconds <= 0.f)
	{
		return;
	}

	const UWorld* World = GetWorld();
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

void UAbilityComponent::HandleCooldownFinished(const FName AbilityId)
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
