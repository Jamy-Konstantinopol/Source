// Crime Life Project, All Rights NOT Reserved.

#include "Components/CLPCharacterMovementComponent.h"

#include "Player/CLPCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPCharacterMovement, All, All);

UCLPCharacterMovementComponent::UCLPCharacterMovementComponent()
{
	MaxWalkSpeed			  = 300;
	MaxWalkSpeedCrouched	  = 200;
	bOrientRotationToMovement = true;
}

void UCLPCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OnAimStateChanged.AddLambda([this](bool bNewIsAiming) { bIsAiming = bNewIsAiming; });
}

float UCLPCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();

	if (IsCrouching() && IsAiming())
	{
		return MaxCrouchAimingSpeed;
	}
	if (IsAiming())
	{
		return MaxAimingSpeed;
	}
	if (IsJogging())
	{
		return MaxJoggingSpeed;
	}
	return MaxSpeed;
}

void UCLPCharacterMovementComponent::Jog()
{
	bWantsJogging = true;
}

void UCLPCharacterMovementComponent::StopJogging()
{
	bWantsJogging = false;
}

void UCLPCharacterMovementComponent::Aim()
{
	bWantsAiming = true;
}

void UCLPCharacterMovementComponent::StopAiming()
{
	bWantsAiming = false;
}

bool UCLPCharacterMovementComponent::IsAiming() const
{
	bool bNewIsAiming = bWantsAiming && IsWalking();
	if (bIsAiming != bNewIsAiming)
	{
		OnAimStateChanged.Broadcast(bNewIsAiming);
	}

	return bNewIsAiming;
}

bool UCLPCharacterMovementComponent::IsJogging() const
{
	return bWantsJogging && IsWalking() && !IsCrouching() && !IsAiming();
}
