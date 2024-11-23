// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTDecorator.h"
#include "Weapons/CLPWeapon.h"

#include "CLPAmountPersentBTDecorator.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UCLPAmountPersentBTDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	UCLPAmountPersentBTDecorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AmmoPercent = 0.7f;
};
