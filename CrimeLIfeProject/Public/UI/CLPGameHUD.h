// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "CLPTypes.h"
#include "GameFramework/HUD.h"

#include "CLPGameHUD.generated.h"

class UUserWidget;
class USoundCue;

UCLASS()
class CRIMELIFEPROJECT_API ACLPGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void OnMatchStateChanged(EMatchState MatchState);

private:
	UPROPERTY()
	TMap<EMatchState, UUserWidget*> GameWidgetMap;

	UPROPERTY()
	UUserWidget* CurrentWidget;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* OpenUISound;
};
