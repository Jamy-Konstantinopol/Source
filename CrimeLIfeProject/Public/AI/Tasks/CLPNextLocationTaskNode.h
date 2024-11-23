// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTTaskNode.h"

#include "CLPNextLocationTaskNode.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UCLPNextLocationTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCLPNextLocationTaskNode();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Radius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector AimLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bSelfCenter = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "!bSelfCenter"))
	FBlackboardKeySelector CenterActorKey;
};
