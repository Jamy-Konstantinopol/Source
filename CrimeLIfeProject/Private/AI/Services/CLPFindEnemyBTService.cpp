// Crime Life Project, All Rights NOT Reserved.

#include "AI/Services/CLPFindEnemyBTService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CLPAIPerceptionComponent.h"

UCLPFindEnemyBTService::UCLPFindEnemyBTService()
{
	NodeName = TEXT("Find Enemy");
}

void UCLPFindEnemyBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		if (const AAIController* Controller = OwnerComp.GetAIOwner())
		{
			if (const UCLPAIPerceptionComponent* PerceptionComponent = Controller->FindComponentByClass<UCLPAIPerceptionComponent>())
			{
				Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
			}
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
