// Crime Life Project, All Rights NOT Reserved.

#include "UI/CLPGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "CLPGameModeBase.h"
#include "Engine/Canvas.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPGameHUD, All, All);

void ACLPGameHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	GameWidgetMap.Add(EMatchState::InProgress, CreateWidget<UUserWidget>(World, PlayerHUDWidgetClass));
	GameWidgetMap.Add(EMatchState::Pause, CreateWidget<UUserWidget>(World, PauseWidgetClass));
	GameWidgetMap.Add(EMatchState::GameOver, CreateWidget<UUserWidget>(World, GameOverWidgetClass));

	for (TPair<EMatchState, UUserWidget*> GameWidgetPair : GameWidgetMap)
	{
		if (GameWidgetPair.Value)
		{
			GameWidgetPair.Value->AddToViewport();
			GameWidgetPair.Value->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (World)
	{
		if (ACLPGameModeBase* GameMode = Cast<ACLPGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ThisClass::OnMatchStateChanged);
		}
	}
}

void ACLPGameHUD::OnMatchStateChanged(EMatchState MatchState)
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrentWidget = GameWidgetMap.FindRef(MatchState);
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->PlaySound(OpenUISound);
	}
}
