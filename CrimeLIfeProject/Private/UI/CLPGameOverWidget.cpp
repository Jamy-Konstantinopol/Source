// Crime Life Project, All Rights NOT Reserved.

#include "UI/CLPGameOverWidget.h"

#include "CLPGameModeBase.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CLPPlayerState.h"
#include "UI/CLPPlayerStatRowWidget.h"

void UCLPGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (const UWorld* World = GetWorld())
	{
		if (ACLPGameModeBase* GameMode = Cast<ACLPGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ThisClass::OnMatchStateChanged);
		}
	}

	if (ResetLevelButton)
	{
		ResetLevelButton->OnClicked.AddDynamic(this, &ThisClass::OnResetLevel);
	}
}

void UCLPGameOverWidget::UpdatePlayerStat()
{
	UWorld* World = GetWorld();
	if (!World || !PlayerStatBox)
	{
		return;
	}

	PlayerStatBox->ClearChildren();

	for (auto It = World->GetControllerIterator(); It; ++It)
	{
		const AController* Controller = It->Get();
		if (!Controller)
		{
			continue;
		}

		const ACLPPlayerState* PlayerState = Cast<ACLPPlayerState>(Controller->PlayerState);
		if (!PlayerState)
		{
			continue;
		}

		UCLPPlayerStatRowWidget* PlayerStatWidget = CreateWidget<UCLPPlayerStatRowWidget>(World, PlayerStatRowWidgetClass);
		if (!PlayerStatWidget)
		{
			continue;
		}

		PlayerStatWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
		PlayerStatWidget->SetKillsName(FText::FromString(FString::FromInt(PlayerState->GetKillsNum())));
		PlayerStatWidget->SetDeathsName(FText::FromString(FString::FromInt(PlayerState->GetDeathsNum())));
		PlayerStatWidget->SetTeamName(FText::FromString(FString::FromInt(PlayerState->GetTeamID())));
		PlayerStatWidget->SetPlayerSelfIndicatorImageVisibility(Controller->IsPlayerController());

		PlayerStatBox->AddChild(PlayerStatWidget);
	}
}

void UCLPGameOverWidget::OnMatchStateChanged(EMatchState NewMatchState)
{
	if (NewMatchState == EMatchState::GameOver)
	{
		UpdatePlayerStat();
	}
}

void UCLPGameOverWidget::OnResetLevel()
{
	if (const UWorld* World = GetWorld())
	{
		const FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
		UGameplayStatics::OpenLevel(this, CurrentLevelName);
	}
}
