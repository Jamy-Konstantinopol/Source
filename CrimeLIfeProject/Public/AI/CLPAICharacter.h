// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Player/CLPCharacter.h"

#include "CLPAICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class CRIMELIFEPROJECT_API ACLPAICharacter : public ACLPCharacter
{
	GENERATED_BODY()

public:
	ACLPAICharacter(const FObjectInitializer& InObjectInitializer);

protected:
	virtual void OnDeath() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;
};
