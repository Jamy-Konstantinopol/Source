// Crime Life Project, All Rights NOT Reserved.

#include "Player/CLPCharacter.h"

#include "Components/CLPCharacterMovementComponent.h"
#include "Components/CLPHealthComponent.h"
#include "Components/CLPWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MotionTrajectory/Public/CharacterTrajectoryComponent.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPCharacter, All, All);

ACLPCharacter::ACLPCharacter(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer.SetDefaultSubobjectClass<UCLPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	bUseControllerRotationPitch = false;
	SetUseControllerRotationYawOnAiming(GetCharacterMovement<UCLPCharacterMovementComponent>()->IsAiming());
	bUseControllerRotationRoll = false;

	HealthComponent				 = CreateDefaultSubobject<UCLPHealthComponent>(TEXT("HealthComponent"));
	WeaponComponent				 = CreateDefaultSubobject<UCLPWeaponComponent>(TEXT("WeaponComponent"));
	CharacterTrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("CharacterTrajectoryComponent"));
}

void ACLPCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(WeaponComponent);
	check(GetCharacterMovement<UCLPCharacterMovementComponent>());
	check(GetMesh());

	HealthComponent->OnDeath.AddDynamic(this, &ThisClass::OnDeath);
	HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
	GetCharacterMovement<UCLPCharacterMovementComponent>()->OnAimStateChanged.AddUObject(this, &ThisClass::OnAimStateChanged);
	LandedDelegate.AddDynamic(this, &ThisClass::OnGroundLanded);
}

void ACLPCharacter::SetUseControllerRotationYawOnAiming(bool bIsRotationYaw)
{
	bUseControllerRotationYaw = bIsRotationYaw;
}

void ACLPCharacter::Jump()
{
	if (!GetCharacterMovement()->IsCrouching())
	{
		Super::Jump();
	}
}

void ACLPCharacter::OnAimStateChanged(bool bIsAiming)
{
	SetUseControllerRotationYawOnAiming(bIsAiming);

	if (!bIsAiming)
	{
		WeaponComponent->StopFiring();
	}
}

void ACLPCharacter::ToggleCrouch()
{
	if (GetMovementComponent()->IsFalling() || GetMovementComponent()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

bool ACLPCharacter::IsDead() const
{
	return FMath::IsNearlyZero(HealthComponent->GetHealth());
}

void ACLPCharacter::SetPlayerColor(const FLinearColor& TeamColor)
{
	UMaterialInstanceDynamic* MaterialInstanceDynamic = GetMesh()->CreateAndSetMaterialInstanceDynamic(TeamColorID);
	if (!MaterialInstanceDynamic)
	{
		return;
	}
	MaterialInstanceDynamic->SetVectorParameterValue(MaterialColorName, TeamColor);
}

void ACLPCharacter::OnHealthChanged(float Health)
{
	UE_LOG(LogCLPCharacter, Display, TEXT("Health: %.0f"), Health);
}

void ACLPCharacter::OnGroundLanded(const FHitResult& InHitResult)
{
	const float FallVelocity = GetVelocity().Length();

	UE_LOG(LogCLPCharacter, Display, TEXT("OnLanded: %.0f"), FallVelocity);

	if (FallVelocity < LandedDamageVelocity.X)
	{
		return;
	}

	const float FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocity);
	TakeDamage(FinalDamage, FPointDamageEvent(), nullptr, nullptr);
}

void ACLPCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(InLifespan);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFiring();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
}
