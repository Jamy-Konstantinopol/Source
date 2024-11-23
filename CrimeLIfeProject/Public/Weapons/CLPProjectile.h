// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "CLPProjectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UCLPWeaponFXComponent;

UCLASS()
class CRIMELIFEPROJECT_API ACLPProjectile : public AActor
{
	GENERATED_BODY()

public:
	ACLPProjectile();

protected:
	virtual void BeginPlay() override;

private:
	AController* GetOwnerController() const;

public:
	void SetShotDirection(const FVector& InDirection);

private:
	FVector ShotDirection;
	void	ApplyDamage();

private:
	UFUNCTION()
	void OnActorDestroyd(AActor* DestroyedActor);

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
						 const FHitResult& InHitResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileLifeSpan = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bDoFullDamage = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX")
	UCLPWeaponFXComponent* WeaponFXComponent;
};
