// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Perception/AIPerceptionComponent.h"

#include "CLPAIPerceptionComponent.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UCLPAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	AActor* GetClosestEnemy() const;
};
