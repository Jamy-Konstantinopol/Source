// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CLPPlayerStatRowWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class CRIMELIFEPROJECT_API UCLPPlayerStatRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayerName(const FText& Text);
	void SetKillsName(const FText& Text);
	void SetDeathsName(const FText& Text);
	void SetTeamName(const FText& Text);
	void SetPlayerSelfIndicatorImageVisibility(bool bIsVisible);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillsText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DeathsText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TeamText;

	UPROPERTY(meta = (BindWidget))
	UImage* PlayerSelfIndicatorImage;
};
