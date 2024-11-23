// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "CLPTypes.h"
#include "Engine/GameInstance.h"

#include "CLPGameInstance.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UCLPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	const TArray<FLevelItemDataWidget>& GetLevelItemDataWidgets() const;
	FName								GetMainMenuLevelName() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FLevelItemDataWidget> LevelItemDataWidgets;

	UPROPERTY(EditAnywhere, Category = "Game")
	FName MainMenuLevelName = FName();
};
