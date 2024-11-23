// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTService.h"

#include "CLPFindEnemyBTService.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UCLPFindEnemyBTService : public UBTService
{
	GENERATED_BODY()

public:
	UCLPFindEnemyBTService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;
};
