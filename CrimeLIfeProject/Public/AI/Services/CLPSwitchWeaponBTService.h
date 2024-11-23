// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTService.h"

#include "CLPSwitchWeaponBTService.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UCLPSwitchWeaponBTService : public UBTService
{
	GENERATED_BODY()

public:
	UCLPSwitchWeaponBTService();

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
	float Chance = 0.2f;
};
