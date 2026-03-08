#pragma once

#include "CoreMinimal.h"

#include "AbilityProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ABILITYSYSTEMSAMPLE_API AAbilityProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAbilityProjectile();

	void Init(const FVector& InVelocity, AActor* InInstigatorActor);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void DestroyProjectile(const FHitResult& ImpactResult);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> Movement;

	TWeakObjectPtr<AActor> InstigatorActor;
};
