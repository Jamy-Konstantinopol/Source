// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "CLPTypes.h"
#include "GameFramework/GameModeBase.h"

#include "CLPGameModeBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnRestartRoundSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, EMatchState);

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", UIMin = "1", ClampMax = "100", UIMax = "100"))
	int32 PlayersNum = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", UIMin = "1", ClampMax = "20", UIMax = "20"))
	int32 RoundsNum = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", UIMin = "3", ClampMax = "600", UIMax = "600", Units = "s"))
	int32 RoundsTime = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FLinearColor DefaultTeamColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TArray<FLinearColor> TeamColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", UIMin = "1", ClampMax = "15", UIMax = "15", Units = "s"))
	int32 RespawnTime = 3;
};

class AAIController;

UCLASS()
class CRIMELIFEPROJECT_API ACLPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACLPGameModeBase();

public:
	virtual void StartPlay() override;
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

private:
	void SpawnBots();
	void StartRound();
	void GameTimerUndate();
	void StartRespawn(AController* RespawnController);
	void GameOver();

	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);

	void		 CreateTeamsInfo();
	FLinearColor DetermineColorByTeamID(int32 TeamID) const;
	void		 SetPlayerColor(AController* Controller);
	void		 SetMatchState(EMatchState NewMatchState);

	void LogPlayerInfo() const;

public:
	void Killed(AController* KillerController, AController* VictimController);
	void RespawnRequest(AController* RespawnController);

	int32	  GetRoundCountDown() const;
	int32	  GetCurrentRound() const;
	FGameData GetGameData() const;

private:
	int32		 CurrentRound	= 1;
	int32		 RoundCountDown = 0.f;
	EMatchState	 MatchState		= EMatchState::WaitingToStart;
	FTimerHandle RoundTimerHandle;

public:
	FOnRestartRoundSignature	  OnRestartRound;
	FOnMatchStateChangedSignature OnMatchStateChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	FGameData GameData;
};
