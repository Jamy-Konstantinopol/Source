// Crime Life Project, All Rights NOT Reserved.

#include "Menu/UI/CLPMenuWidget.h"

#include "CLPGameInstance.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPMenuWidget, All, All);

void UCLPMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGame);
	}

	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitGame);
	}

	if (CurrentLevelsHorizontalBox)
	{
		CurrentLevelsHorizontalBox->ClearChildren();

		UCLPGameInstance* CLPGameInstance = GetGameInstance<UCLPGameInstance>();
		if (!CLPGameInstance)
		{
			return;
		}

		for (const FLevelItemDataWidget& LevelItemDataWidget : CLPGameInstance->GetLevelItemDataWidgets())
		{
			if (UCLPLevelItemWidget* CLPLevelItemWidget = CreateWidget<UCLPLevelItemWidget>(this, CLPLevelItemWidgetClass))
			{
				CLPLevelItemWidget->SetLevelItemDataWidget(LevelItemDataWidget);
				CLPLevelItemWidget->OnClickedLevelButton.AddUObject(this, &ThisClass::OnLevelItemClicked);
				CurrentLevelsHorizontalBox->AddChild(CLPLevelItemWidget);
				CLPLevelItemWidgets.Add(CLPLevelItemWidget);
				CLPLevelItemWidget->Click();
			}
		}
	}
}

void UCLPMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation != HideAnimation)
	{
		return;
	}

	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UCLPGameInstance* GameInstance = World->GetGameInstance<UCLPGameInstance>();
	if (!GameInstance)
	{
		return;
	}
	if (CurrentLevelName.IsNone())
	{
		UE_LOG(LogCLPMenuWidget, Warning, TEXT("Level Name is \"None\""))
	}
	else
	{
		UGameplayStatics::OpenLevel(this, CurrentLevelName);
	}
}

void UCLPMenuWidget::OnLevelItemClicked(const FLevelItemDataWidget& LevelItemDataWidget)
{
	for (UCLPLevelItemWidget* CLPLevelItemWidget : CLPLevelItemWidgets)
	{
		const bool bIsCurrentDataWidget = CLPLevelItemWidget->GetLevelItemDataWidget().LevelName == LevelItemDataWidget.LevelName;
		if (bIsCurrentDataWidget)
		{
			CurrentLevelPicture->SetBrushFromTexture(LevelItemDataWidget.LevelPicture);
			CurrentLevelName = LevelItemDataWidget.LevelName;
		}
		else
		{
			CLPLevelItemWidget->SetButtonEnabled();
		}
	}
}

void UCLPMenuWidget::OnStartGame()
{
	PlayAnimation(HideAnimation);
	PlaySound(StartGameSound);
}

void UCLPMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
