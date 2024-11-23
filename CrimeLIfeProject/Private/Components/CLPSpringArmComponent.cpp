// Crime Life Project, All Rights NOT Reserved.

#include "Components/CLPSpringArmComponent.h"

#include "Components/CLPCharacterMovementComponent.h"
#include "Engine/World.h"

UCLPSpringArmComponent::UCLPSpringArmComponent()
{
	NotAimTargetArmLength  = TargetArmLength;
	CurrentTargetArmLength = TargetArmLength;
}

void UCLPSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		if (UCLPCharacterMovementComponent* MovementComponent = GetOwner()->FindComponentByClass<UCLPCharacterMovementComponent>())
		{
			MovementComponent->OnAimStateChanged.AddUObject(this, &ThisClass::OnAimStateChanged);
		}
	}
}

void UCLPSpringArmComponent::OnAimStateChanged(bool bIsAiming)
{
	if (GetWorld())
	{
		CurrentTargetArmLength = bIsAiming ? GetAimTargetArmLength() : GetNotAimTargetArmLength();

		GetWorld()->GetTimerManager().ClearTimer(InterpTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(InterpTimerHandle, this, &UCLPSpringArmComponent::UpdateArmLength, InterpStepTime, true);
	}
}

void UCLPSpringArmComponent::UpdateArmLength()
{
	if (!GetWorld())
	{
		return;
	}

	TargetArmLength = FMath::FInterpTo(TargetArmLength, CurrentTargetArmLength, InterpStepTime, InterpTargetArmLength);

	if (FMath::IsNearlyEqual(TargetArmLength, CurrentTargetArmLength, 0.1f))
	{
		GetWorld()->GetTimerManager().ClearTimer(InterpTimerHandle);
	}
}

float UCLPSpringArmComponent::GetAimTargetArmLength()
{
	return AimTargetArmLength;
}

float UCLPSpringArmComponent::GetNotAimTargetArmLength()
{
	return NotAimTargetArmLength;
}
