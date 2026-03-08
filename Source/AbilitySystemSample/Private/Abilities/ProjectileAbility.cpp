#include "Abilities/ProjectileAbility.h"

#include "Abilities/AbilityProjectile.h"
#include "Abilities/Data/ProjectileAbilityData.h"

bool UProjectileAbility::CanActivate(EAbilityFailReason& OutReason) const
{
	if (!Super::CanActivate(OutReason))
	{
		return false;
	}

	const UProjectileAbilityData* ProjectileData = GetProjectileData();
	if (!ProjectileData)
	{
		OutReason = EAbilityFailReason::Blocked;
		return false;
	}

	if (!ProjectileData->ProjectileClass)
	{
		OutReason = EAbilityFailReason::Blocked;
		return false;
	}

	AActor* Avatar = GetAvatarActor();
	if (!Avatar || !Avatar->GetWorld())
	{
		OutReason = EAbilityFailReason::InvalidOwner;
		return false;
	}

	OutReason = EAbilityFailReason::None;
	return true;
}

void UProjectileAbility::Activate()
{
	AActor* Avatar = GetAvatarActor();
	if (!ensure(Avatar))
	{
		return;
	}

	const UProjectileAbilityData* ProjectileData = GetProjectileData();
	if (!ensureMsgf(ProjectileData, TEXT("[ProjectileAbility] Requires UProjectileAbilityData")))
	{
		return;
	}

	if (!ensureMsgf(ProjectileData->ProjectileClass, TEXT("[ProjectileAbility] ProjectileClass is required")))
	{
		return;
	}

	UWorld* World = Avatar->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	APawn* PawnOwner = Cast<APawn>(Avatar);

	// Aim: prefer controller rotation when available, otherwise actor rotation.
	const FRotator AimRot = PawnOwner && PawnOwner->GetController()
		? PawnOwner->GetController()->GetControlRotation()
		: Avatar->GetActorRotation();

	const FVector SpawnLoc = Avatar->GetActorLocation()
		+ AimRot.Vector() * ProjectileData->MuzzleOffset.X
		+ FVector(0, 0, ProjectileData->MuzzleOffset.Z);
	const FVector ShootDir = AimRot.Vector().GetSafeNormal();
	const FVector Velocity = ShootDir * ProjectileData->InitialSpeed;

	FActorSpawnParameters Params;
	Params.Owner = Avatar;
	Params.Instigator = PawnOwner;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAbilityProjectile* Projectile = World->SpawnActor<AAbilityProjectile>(ProjectileData->ProjectileClass, SpawnLoc, AimRot, Params);

	if (!ensureMsgf(Projectile, TEXT("[ProjectileAbility] Failed to spawn projectile for '%s'"), *GetAbilityId().ToString()))
	{
		return;
	}

	Projectile->Init(Velocity, Avatar);
	Projectile->SetLifeSpan(FMath::Max(0.1f, ProjectileData->LifeSeconds));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.8f, FColor::Silver,
			FString::Printf(TEXT("Projectile spawned")));
	}
}

const UProjectileAbilityData* UProjectileAbility::GetProjectileData() const
{
	return Cast<UProjectileAbilityData>(GetData());
}
