// Crime Life Project, All Rights NOT Reserved.

#include "Player/CLPPlayerController.h"

#include "CLPGameModeBase.h"
#include "Components/CLPRespawnComponent.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACLPPlayerController::ACLPPlayerController()
{
	CLPRespawnComponent = CreateDefaultSubobject<UCLPRespawnComponent>("CLPRespawnComponent");
}

void ACLPPlayerController::BeginPlay()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}

	if (UWorld* World = GetWorld())
	{
		if (ACLPGameModeBase* GameMode = Cast<ACLPGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ThisClass::OnMatchStateChanged);
		}
	}
}

void ACLPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	check(EnhancedInputComponent);

	EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Started, this, &ThisClass::PauseGame);
}

void ACLPPlayerController::PauseGame()
{
	const UWorld* World = GetWorld();
	if (!World || !World->GetAuthGameMode())
	{
		return;
	}

	World->GetAuthGameMode()->SetPause(this);
}

void ACLPPlayerController::OnMatchStateChanged(EMatchState NewMatchState)
{
	if (NewMatchState == EMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}
