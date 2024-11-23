// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"

#include "CLPAIController.generated.h"

class UCLPAIPerceptionComponent;
class UCLPRespawnComponent;

UCLASS()
class CRIMELIFEPROJECT_API ACLPAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACLPAIController();

protected:
	virtual void Tick(float DeltaTime) override;

private:
	AActor* GetFocusOnActor() const;

protected:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCLPAIPerceptionComponent* CLPAIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCLPRespawnComponent* CLPRespawnComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName FocusOnKeyName = "Enemy";
};
