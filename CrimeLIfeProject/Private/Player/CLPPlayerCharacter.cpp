// Crime Life Project, All Rights NOT Reserved.

#include "Player/CLPPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CLPCharacterMovementComponent.h"
#include "Components/CLPSpringArmComponent.h"
#include "Components/CLPWeaponComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPPlayerCharacter, All, All);

ACLPPlayerCharacter::ACLPPlayerCharacter(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
	CameraBoom = CreateDefaultSubobject<UCLPSpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);
}

void ACLPPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetInputMappingContext(MappingContext);

	check(CameraBoom);
	check(CameraComponent);
}

void ACLPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	check(EnhancedInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &Super::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::ToggleCrouch);
	EnhancedInputComponent->BindAction(JogAction, ETriggerEvent::Started, GetCharacterMovement<UCLPCharacterMovementComponent>(),
									   &UCLPCharacterMovementComponent::Jog);
	EnhancedInputComponent->BindAction(JogAction, ETriggerEvent::Completed, GetCharacterMovement<UCLPCharacterMovementComponent>(),
									   &UCLPCharacterMovementComponent::StopJogging);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, GetCharacterMovement<UCLPCharacterMovementComponent>(),
									   &UCLPCharacterMovementComponent::Aim);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, GetCharacterMovement<UCLPCharacterMovementComponent>(),
									   &UCLPCharacterMovementComponent::StopAiming);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, WeaponComponent, &UCLPWeaponComponent::Fire);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, WeaponComponent, &UCLPWeaponComponent::StopFiring);
	EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Triggered, WeaponComponent, &UCLPWeaponComponent::SwitchWeapon);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, WeaponComponent, &UCLPWeaponComponent::Reload);
}

void ACLPPlayerCharacter::OnDeath()
{
	Super::OnDeath();

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ACLPPlayerCharacter::SetInputMappingContext(UInputMappingContext* NewMappingContext)
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		UE_LOG(LogCLPPlayerCharacter, Error, TEXT("Movement error: Controller is null"));
	}
	else if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(NewMappingContext, 1);
	}
	else
	{
		UE_LOG(LogCLPPlayerCharacter, Error, TEXT("Movement error: Subsystem is null"));
	}
}

void ACLPPlayerCharacter::Move(const FInputActionValue& InActionValue)
{
	if (Controller != nullptr)
	{
		FVector2D MovementVector = InActionValue.Get<FVector2D>();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	else
	{
		UE_LOG(LogCLPPlayerCharacter, Error, TEXT("Movement error: Controller is null"));
	}
}

void ACLPPlayerCharacter::Look(const FInputActionValue& InActionValue)
{
	if (Controller != nullptr)
	{
		FVector2D LookAxisVector = InActionValue.Get<FVector2D>();

		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
	else
	{
		UE_LOG(LogCLPPlayerCharacter, Error, TEXT("Looking error: Controller is null"));
	}
}
