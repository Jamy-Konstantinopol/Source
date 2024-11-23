// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CLPGameDataWidget.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UCLPGameDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCLPGameDataWidget(const FObjectInitializer& ObjectInitializer);

private:
	const FString ZeroTime;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetRemainingTime() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetRoundCount() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetKillsNum() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FString TimeDelimiter = TEXT(":");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FString RoundDelimiter = TEXT("/");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FString RoundString = TEXT("Round: ");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FString KillsString = TEXT("Kills: ");
};
