// Crime Life Project, All Rights NOT Reserved.

#include "AI/Tasks/CLPNextLocationTaskNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UCLPNextLocationTaskNode::UCLPNextLocationTaskNode()
{
	NodeName = "Next Location";
}

EBTNodeResult::Type UCLPNextLocationTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController*  Controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(Pawn);
	if (!NavigationSystem)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation NavigationLocation;
	FVector		 MoveToLocation = Pawn->GetActorLocation();

	if (!bSelfCenter)
	{
		if (AActor* CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName)))
		{
			MoveToLocation = CenterActor->GetActorLocation();
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}

	if (!NavigationSystem->GetRandomReachablePointInRadius(MoveToLocation, Radius, NavigationLocation))
	{
		return EBTNodeResult::Failed;
	}
	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavigationLocation.Location);

	return EBTNodeResult::Succeeded;
}
