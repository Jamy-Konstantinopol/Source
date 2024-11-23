// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "CLPCharacterMovementComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAimStateChangedSignature, bool);

class ACLPCharacter;

UCLASS()
class CRIMELIFEPROJECT_API UCLPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UCLPCharacterMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual float GetMaxSpeed() const override;

	virtual void Jog();
	virtual void StopJogging();
	virtual void Aim();
	virtual void StopAiming();

private:
	bool bIsAiming	   = IsAiming();
	bool bWantsJogging = false;
	bool bWantsAiming  = false;

public:
	FOnAimStateChangedSignature OnAimStateChanged;

public:
	UFUNCTION(BlueprintCallable)
	bool IsAiming() const;

	UFUNCTION(BlueprintCallable)
	bool IsJogging() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking", meta = (ClampMin = "0", UIMin = "0", ForceUnits = "cm/s"))
	float MaxJoggingSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking", meta = (ClampMin = "0", UIMin = "0", ForceUnits = "cm/s"))
	float MaxAimingSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking", meta = (ClampMin = "0", UIMin = "0", ForceUnits = "cm/s"))
	float MaxCrouchAimingSpeed = 150.f;
};
