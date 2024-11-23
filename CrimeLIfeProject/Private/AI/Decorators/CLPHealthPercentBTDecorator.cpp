// Crime Life Project, All Rights NOT Reserved.

#include "AI/Decorators/CLPHealthPercentBTDecorator.h"

#include "AIController.h"
#include "Components/CLPHealthComponent.h"

UCLPHealthPercentBTDecorator::UCLPHealthPercentBTDecorator()
{
	NodeName = TEXT("Health Percent");
}

bool UCLPHealthPercentBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller || !Controller->GetPawn())
	{
		return false;
	}

	const UCLPHealthComponent* HealthComponent = Controller->GetPawn()->FindComponentByClass<UCLPHealthComponent>();
	if (!HealthComponent || HealthComponent->IsDead())
	{
		return false;
	}

	return HealthComponent->GetHealthPercent() < HealthPercent;
}
