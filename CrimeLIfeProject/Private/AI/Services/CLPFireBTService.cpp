// Crime Life Project, All Rights NOT Reserved.

#include "AI/Services/CLPFireBTService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CLPCharacterMovementComponent.h"
#include "Components/CLPWeaponComponent.h"
#include "Player/CLPCharacter.h"

UCLPFireBTService::UCLPFireBTService()
{
	NodeName = TEXT("Fire");
}

void UCLPFireBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
	{
		if (const AAIController* Controller = OwnerComp.GetAIOwner())
		{
			if (ACLPCharacter* CLPCharacter = Cast<ACLPCharacter>(Controller->GetPawn()))
			{
				if (UCLPWeaponComponent* WeaponComponent = CLPCharacter->FindComponentByClass<UCLPWeaponComponent>())
				{
					if (UCLPCharacterMovementComponent* CLPCharacterMovementComponent = CLPCharacter->FindComponentByClass<UCLPCharacterMovementComponent>())
					{
						const bool bHasAim = !!Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);
						bHasAim ? CLPCharacterMovementComponent->Aim() : CLPCharacterMovementComponent->StopAiming();
						bHasAim ? WeaponComponent->Fire() : WeaponComponent->StopFiring();
					}
				}
			}
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
