// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Menu/UI/CLPLevelItemWidget.h"

#include "CLPMenuWidget.generated.h"

class UButton;
class UImage;
class UHorizontalBox;
class UCLPLevelItemWidget;
class USoundCue;

UCLASS()
class CRIMELIFEPROJECT_API UCLPMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	void OnLevelItemClicked(const FLevelItemDataWidget& LevelItemDataWidget);

private:
	FName						 CurrentLevelName = FName();
	TArray<UCLPLevelItemWidget*> CLPLevelItemWidgets;

private:
	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnQuitGame();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	UImage* CurrentLevelPicture;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CurrentLevelsHorizontalBox;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HideAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCLPLevelItemWidget> CLPLevelItemWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* StartGameSound;
};
