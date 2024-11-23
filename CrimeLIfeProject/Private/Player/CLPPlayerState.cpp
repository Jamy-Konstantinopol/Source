// Crime Life Project, All Rights NOT Reserved.

#include "Player/CLPPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPPlayerState, All, All);

int32 ACLPPlayerState::GetTeamID() const
{
	return TeamID;
}

int32 ACLPPlayerState::GetKillsNum() const
{
	return KillsNum;
}

int32 ACLPPlayerState::GetDeathsNum() const
{
	return DeathsNum;
}

FLinearColor ACLPPlayerState::GetTeamColor() const
{
	return TeamColor;
}

void ACLPPlayerState::SetTeamID(int32 NewTeamID)
{
	TeamID = NewTeamID;
}

void ACLPPlayerState::SetTeamColor(const FLinearColor& NewTeamColor)
{
	TeamColor = NewTeamColor;
}

void ACLPPlayerState::AddKill()
{
	KillsNum++;
}

void ACLPPlayerState::AddDeath()
{
	DeathsNum++;
}

void ACLPPlayerState::LogInfo() const
{
	UE_LOG(LogCLPPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}
