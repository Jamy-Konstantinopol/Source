// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CLPPauseWidget.generated.h"

class UButton;

UCLASS()
class CRIMELIFEPROJECT_API UCLPPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClearPause();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;
};
