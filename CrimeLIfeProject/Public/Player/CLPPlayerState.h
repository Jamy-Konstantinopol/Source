// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"

#include "CLPPlayerState.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API ACLPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	int32		 GetTeamID() const;
	int32		 GetKillsNum() const;
	int32		 GetDeathsNum() const;
	FLinearColor GetTeamColor() const;


	void SetTeamID(int32 NewTeamID);
	void SetTeamColor(const FLinearColor& NewTeamColor);

	void AddKill();
	void AddDeath();

	void LogInfo() const;

private:
	int32		 TeamID;
	FLinearColor TeamColor;

	int32 KillsNum = 0;
	int32 DeathsNum = 0;
};
