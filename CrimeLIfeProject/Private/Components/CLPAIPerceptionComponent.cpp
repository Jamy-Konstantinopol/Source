// Crime Life Project, All Rights NOT Reserved.

#include "Components/CLPAIPerceptionComponent.h"

#include "AIController.h"
#include "CLPUtils.h"
#include "Components/CLPHealthComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Sight.h"

AActor* UCLPAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PerciveActors;
	GetCurrentlyPerceivedActors(UAISenseConfig_Sight::StaticClass(), PerciveActors);

	if (PerciveActors.Num() == 0)
	{
		GetCurrentlyPerceivedActors(UAISenseConfig_Damage::StaticClass(), PerciveActors);
		if (PerciveActors.Num() == 0)
		{
			return nullptr;
		}
	}

	AAIController* Controller = Cast<AAIController>(GetOwner());
	if (!Controller)
	{
		return nullptr;
	}

	const APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		return nullptr;
	}

	float	BestDistance = MAX_FLT;
	AActor* BestPawn	 = nullptr;

	for (AActor*& PerciveActor : PerciveActors)
	{
		const UCLPHealthComponent* HealthComponent = PerciveActor->FindComponentByClass<UCLPHealthComponent>();
		const APawn*			   PercievePawn	   = Cast<APawn>(PerciveActor);
		const bool				   bAreEnemies	   = PercievePawn && CLPUtils::AreEnemies(Controller, PercievePawn->Controller);

		if (HealthComponent && !HealthComponent->IsDead() && bAreEnemies)
		{
			const float CurrentDistance = (PerciveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn	 = PerciveActor;
			}
		}
	}

	return BestPawn;
}
