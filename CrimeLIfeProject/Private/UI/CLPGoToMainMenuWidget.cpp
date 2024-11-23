// Crime Life Project, All Rights NOT Reserved.

#include "UI/CLPGoToMainMenuWidget.h"

#include "CLPGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPMainMenuWidget, All, All);

void UCLPGoToMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (MainMenuLevelButton)
	{
		MainMenuLevelButton->OnClicked.AddDynamic(this, &ThisClass::OnGoToMainMenuLevel);
	}
}

void UCLPGoToMainMenuWidget::OnGoToMainMenuLevel()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const UCLPGameInstance* GameInstance = World->GetGameInstance<UCLPGameInstance>();
	if (!GameInstance)
	{
		return;
	}
	if (!GameInstance->GetMainMenuLevelName().IsNone())
	{
		UGameplayStatics::OpenLevel(this, GameInstance->GetMainMenuLevelName());
	}
	else
	{
		UE_LOG(LogCLPMainMenuWidget, Warning, TEXT("Level Name is \"None\""))
	}
}
