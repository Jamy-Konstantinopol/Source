// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTDecorator.h"

#include "CLPHealthPercentBTDecorator.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UCLPHealthPercentBTDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	UCLPHealthPercentBTDecorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float HealthPercent = 0.7f;
};
