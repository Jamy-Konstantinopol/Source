// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "CLPCharacter.generated.h"

class UCLPHealthComponent;
class UCLPWeaponComponent;
class UCLPCharacterMovementComponent;
class UCharacterTrajectoryComponent;
class USoundCue;
struct FInputActionValue;

UCLASS()
class CRIMELIFEPROJECT_API ACLPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACLPCharacter(const FObjectInitializer& InObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;

private:
	void SetUseControllerRotationYawOnAiming(bool bIsRotationYaw);

protected:
	virtual void OnAimStateChanged(bool bIsAiming);
	virtual void ToggleCrouch();

public:
	bool IsDead() const;

	void SetPlayerColor(const FLinearColor& TeamColor);

private:
	UFUNCTION()
	void OnHealthChanged(float Health);

	UFUNCTION()
	void OnGroundLanded(const FHitResult& InHitResult);

protected:
	UFUNCTION()
	virtual void OnDeath();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCLPHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCLPWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCharacterTrajectoryComponent* CharacterTrajectoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity = FVector2D(1000.0, 1600.0);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamage = FVector2D(5.0, 100.0);

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float InLifespan = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	FName MaterialColorName = "Base Color Tint";

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	int32 TeamColorID = 13;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* DeathSound;
};
