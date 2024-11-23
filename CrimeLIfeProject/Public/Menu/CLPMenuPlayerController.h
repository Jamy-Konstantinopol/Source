// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "CLPMenuPlayerController.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API ACLPMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
