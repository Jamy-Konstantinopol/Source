// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CLPGoToMainMenuWidget.generated.h"

class UButton;

UCLASS()
class CRIMELIFEPROJECT_API UCLPGoToMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnGoToMainMenuLevel();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuLevelButton;
};
