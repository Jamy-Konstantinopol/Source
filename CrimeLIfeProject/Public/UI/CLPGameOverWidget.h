// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "CLPTypes.h"

#include "CLPGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class CRIMELIFEPROJECT_API UCLPGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	void UpdatePlayerStat();
	void OnMatchStateChanged(EMatchState NewMatchState);

private:
	UFUNCTION()
	void OnResetLevel();

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerStatBox;

	UPROPERTY(meta = (BindWidget))
	UButton* ResetLevelButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;
};
