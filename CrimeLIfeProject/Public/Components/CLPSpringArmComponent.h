// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/SpringArmComponent.h"

#include "CLPSpringArmComponent.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UCLPSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	UCLPSpringArmComponent();

protected:
	virtual void BeginPlay() override;

private:
	void OnAimStateChanged(bool bIsAiming);
	void UpdateArmLength();

public:
	float GetAimTargetArmLength();
	float GetNotAimTargetArmLength();

private:
	FTimerHandle InterpTimerHandle;
	float CurrentTargetArmLength;
	float NotAimTargetArmLength;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float AimTargetArmLength = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float InterpTargetArmLength = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float InterpStepTime = 0.02f;
};
