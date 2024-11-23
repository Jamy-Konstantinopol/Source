// Crime Life Project, All Rights NOT Reserved.

#include "CLPGameModeBase.h"

#include "AIController.h"
#include "Components/CLPRespawnComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Player/CLPCharacter.h"
#include "Player/CLPPlayerController.h"
#include "Player/CLPPlayerState.h"
#include "TimerManager.h"
#include "UI/CLPGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPGameModeBase, All, All);

ACLPGameModeBase::ACLPGameModeBase()
{
	DefaultPawnClass	  = ACLPCharacter::StaticClass();
	HUDClass			  = ACLPGameHUD::StaticClass();
	PlayerControllerClass = ACLPPlayerController::StaticClass();
	PlayerStateClass	  = ACLPPlayerState::StaticClass();
}

void ACLPGameModeBase::StartPlay()
{
	Super::StartPlay();

	OnRestartRound.AddUObject(this, &ThisClass::ResetPlayers);

	CurrentRound = 1;
	SpawnBots();
	CreateTeamsInfo();
	StartRound();
	SetMatchState(EMatchState::InProgress);
}

bool ACLPGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const bool bIsPaused = Super::SetPause(PC, CanUnpauseDelegate);
	if (bIsPaused)
	{
		SetMatchState(EMatchState::Pause);
	}
	return bIsPaused;
}

bool ACLPGameModeBase::ClearPause()
{
	const bool bPauseCleared = Super::ClearPause();
	if (bPauseCleared)
	{
		SetMatchState(EMatchState::InProgress);
	}
	return bPauseCleared;
}

UClass* ACLPGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ACLPGameModeBase::SpawnBots()
{
	if (UWorld* World = GetWorld())
	{
		for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AAIController* AIController = World->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
			RestartPlayer(AIController);
		}
	}
}

void ACLPGameModeBase::StartRound()
{
	RoundCountDown = GameData.RoundsTime;
	GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &ThisClass::GameTimerUndate, 1.f, true);
}

void ACLPGameModeBase::GameTimerUndate()
{
	if (--RoundCountDown <= 0)
	{
		GetWorldTimerManager().ClearTimer(RoundTimerHandle);

		if (++CurrentRound <= GameData.RoundsNum)
		{
			UE_LOG(LogCLPGameModeBase, Display, TEXT("Time: %i / Round: %i, %i"), RoundCountDown, CurrentRound, GameData.RoundsNum)
			OnRestartRound.Broadcast();
			StartRound();
		}
		else
		{
			GameOver();
		}
	}
}

void ACLPGameModeBase::StartRespawn(AController* RespawnController)
{
	if (!RespawnController || RoundCountDown < GameData.RespawnTime)
	{
		return;
	}

	UCLPRespawnComponent* RespawnComponent = RespawnController->FindComponentByClass<UCLPRespawnComponent>();
	if (!RespawnComponent)
	{
		return;
	}
	RespawnComponent->Respawn(GameData.RespawnTime);
}

void ACLPGameModeBase::GameOver()
{
	UE_LOG(LogCLPGameModeBase, Display, TEXT("========== GAME OVER =========="))
	LogPlayerInfo();

	for (APawn* Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}

	SetMatchState(EMatchState::GameOver);
}

void ACLPGameModeBase::ResetPlayers()
{
	if (const UWorld* World = GetWorld())
	{
		for (auto It = World->GetControllerIterator(); It; ++It)
		{
			ResetOnePlayer(It->Get());
		}
	}
}

void ACLPGameModeBase::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}
	RestartPlayer(Controller);
	SetPlayerColor(Controller);
}

void ACLPGameModeBase::CreateTeamsInfo()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	int32 TeamID = 1;
	for (auto It = World->GetControllerIterator(); It; ++It)
	{
		AController* Controller = It->Get();
		if (!Controller)
		{
			continue;
		}

		ACLPPlayerState* PlayerState = Cast<ACLPPlayerState>(Controller->PlayerState);
		if (!PlayerState)
		{
			continue;
		}

		PlayerState->SetTeamID(TeamID);
		PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
		PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
		SetPlayerColor(Controller);

		TeamID = TeamID == 1 ? 2 : 1;
	}
}

FLinearColor ACLPGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
	if (TeamID - 1 < GameData.TeamColors.Num())
	{
		return GameData.TeamColors[TeamID - 1];
	}
	return GameData.DefaultTeamColor;
}

void ACLPGameModeBase::SetPlayerColor(AController* Controller)
{
	if (!Controller)
	{
		return;
	}

	ACLPCharacter*		   Character   = Cast<ACLPCharacter>(Controller->GetPawn());
	const ACLPPlayerState* PlayerState = Cast<ACLPPlayerState>(Controller->PlayerState);
	if (!Character || !PlayerState)
	{
		return;
	}

	Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ACLPGameModeBase::SetMatchState(EMatchState NewMatchState)
{
	if (NewMatchState != MatchState)
	{
		MatchState = NewMatchState;
		OnMatchStateChanged.Broadcast(NewMatchState);
	}
}

void ACLPGameModeBase::LogPlayerInfo() const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (auto It = World->GetControllerIterator(); It; ++It)
	{
		AController* Controller = It->Get();
		if (!Controller)
		{
			continue;
		}

		if (ACLPPlayerState* PlayerState = Cast<ACLPPlayerState>(Controller->PlayerState))
		{
			PlayerState->LogInfo();
		}
	}
}

void ACLPGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
	ACLPPlayerState* KillerPlayerState = KillerController ? Cast<ACLPPlayerState>(KillerController->PlayerState) : nullptr;
	ACLPPlayerState* VictimPlayerState = VictimController ? Cast<ACLPPlayerState>(VictimController->PlayerState) : nullptr;

	if (KillerPlayerState && VictimPlayerState)
	{
		KillerPlayerState->AddKill();
		VictimPlayerState->AddDeath();
		StartRespawn(VictimController);
	}
}

void ACLPGameModeBase::RespawnRequest(AController* RespawnController)
{
	ResetOnePlayer(RespawnController);
}

int32 ACLPGameModeBase::GetRoundCountDown() const
{
	return RoundCountDown;
}

int32 ACLPGameModeBase::GetCurrentRound() const
{
	return CurrentRound;
}

FGameData ACLPGameModeBase::GetGameData() const
{
	return GameData;
}
