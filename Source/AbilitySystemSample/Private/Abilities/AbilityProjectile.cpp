#include "Abilities/AbilityProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAbilityProjectile::AAbilityProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Collision->InitSphereRadius(12.f);
	Collision->SetCollisionProfileName(TEXT("Projectile"));
	Collision->SetGenerateOverlapEvents(false);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->UpdatedComponent = Collision;
	Movement->bRotationFollowsVelocity = true;
	Movement->bShouldBounce = false;
	Movement->ProjectileGravityScale = 0.f;

	Movement->OnProjectileStop.AddDynamic(this, &AAbilityProjectile::DestroyProjectile);
}

void AAbilityProjectile::Init(const FVector& InVelocity, AActor* InInstigatorActor)
{
	InstigatorActor = InInstigatorActor;

	if (InstigatorActor.IsValid())
	{
		Collision->IgnoreActorWhenMoving(InstigatorActor.Get(),true);
	}

	if (ensure(Movement))
	{
		Movement->Velocity = InVelocity;
		Movement->Activate();
	}
}

void AAbilityProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilityProjectile::DestroyProjectile(const FHitResult& ImpactResult)
{
	AActor* HitActor = ImpactResult.GetActor();

	if (HitActor != InstigatorActor.Get())
	{
		Destroy();
	}
}
