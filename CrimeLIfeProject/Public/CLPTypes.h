#pragma once

#include "CoreMinimal.h"

#include "CLPTypes.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	WaitingToStart UMETA(DisplayName = "WaitingToStart"),
	InProgress	   UMETA(DisplayName = "InProgress"),
	Pause		   UMETA(DisplayName = "Pause"),
	GameOver	   UMETA(DisplayName = "GameOver"),
};

USTRUCT(BlueprintType)
struct FLevelItemDataWidget
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText LevelDisplayName = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FName LevelName = FName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* LevelPicture;
};