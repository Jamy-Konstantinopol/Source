// Crime Life Project, All Rights NOT Reserved.

#include "UI/CLPPauseWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void UCLPPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &ThisClass::OnClearPause);
	}
}

void UCLPPauseWidget::OnClearPause()
{
	const UWorld* World = GetWorld();
	if (World && World->GetAuthGameMode())
	{
		World->GetAuthGameMode()->ClearPause();
	}
}
