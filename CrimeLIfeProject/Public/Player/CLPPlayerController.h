// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "CLPTypes.h"
#include "GameFramework/PlayerController.h"

#include "CLPPlayerController.generated.h"

class UCLPRespawnComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class CRIMELIFEPROJECT_API ACLPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACLPPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void PauseGame();
	void OnMatchStateChanged(EMatchState NewMatchState);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCLPRespawnComponent* CLPRespawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputAction* PauseGameAction;
};
