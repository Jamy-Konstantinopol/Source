// Crime Life Project, All Rights NOT Reserved.

#include "UI/CLPGameDataWidget.h"

#include "CLPGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CLPPlayerState.h"

UCLPGameDataWidget::UCLPGameDataWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer), ZeroTime(TEXT("00") + TimeDelimiter + TEXT("00"))
{
}

FString UCLPGameDataWidget::GetRemainingTime() const
{
	if (const ACLPGameModeBase* CLPGameModeBase = Cast<ACLPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		const int32 Minutes = CLPGameModeBase->GetRoundCountDown() / 60;
		const int32 Seconds = CLPGameModeBase->GetRoundCountDown() % 60;
		return FString::Printf(TEXT("%02i%s%02i"), Minutes, *TimeDelimiter, Seconds);
	}
	return ZeroTime;
}

FString UCLPGameDataWidget::GetRoundCount() const
{
	if (const ACLPGameModeBase* CLPGameModeBase = Cast<ACLPGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		const FString CurrentRound = FString::FromInt(CLPGameModeBase->GetCurrentRound());
		const FString RoundsNum	   = FString::FromInt(CLPGameModeBase->GetGameData().RoundsNum);
		return RoundString + CurrentRound + RoundDelimiter + RoundsNum;
	}
	return KillsString + ZeroTime;
}

FString UCLPGameDataWidget::GetKillsNum() const
{
	const ACLPPlayerState* CLPPlayerState = Cast<ACLPPlayerState>(GetOwningPlayerState());
	return CLPPlayerState ? KillsString + FString::FromInt(CLPPlayerState->GetKillsNum()) : KillsString + FString::FromInt(int32());
}
