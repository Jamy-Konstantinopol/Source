// Crime Life Project, All Rights NOT Reserved.

#include "AI/CLPAIController.h"

#include "AI/CLPAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CLPAIPerceptionComponent.h"
#include "Components/CLPRespawnComponent.h"

ACLPAIController::ACLPAIController()
{
	CLPAIPerceptionComponent = CreateDefaultSubobject<UCLPAIPerceptionComponent>("CLPAIPerceptionComponent");
	SetPerceptionComponent(*CLPAIPerceptionComponent);

	CLPRespawnComponent = CreateDefaultSubobject<UCLPRespawnComponent>("CLPRespawnComponent");

	bWantsPlayerState = true;
}

void ACLPAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

AActor* ACLPAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent())
	{
		return nullptr;
	}
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}

void ACLPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const ACLPAICharacter* CLPCharacter = Cast<ACLPAICharacter>(InPawn))
	{
		RunBehaviorTree(CLPCharacter->BehaviorTreeAsset);
	}
}
