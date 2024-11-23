// Crime Life Project, All Rights NOT Reserved.

#include "AI/Services/CLPSwitchWeaponBTService.h"

#include "AIController.h"
#include "Components/CLPWeaponComponent.h"
#include "Player/CLPCharacter.h"

UCLPSwitchWeaponBTService::UCLPSwitchWeaponBTService()
{
	NodeName = TEXT("Switch Weapon");

	Interval = 5.f;
}

void UCLPSwitchWeaponBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (FMath::FRand() < Chance || Chance == 1.f)
	{
		if (UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
		{
			if (const AAIController* Controller = OwnerComp.GetAIOwner())
			{
				if (ACLPCharacter* CLPCharacter = Cast<ACLPCharacter>(Controller->GetPawn()))
				{
					if (UCLPWeaponComponent* WeaponComponent = CLPCharacter->FindComponentByClass<UCLPWeaponComponent>())
					{
						const TArray<FName> WeaponSockets = WeaponComponent->GetSocketsForWeapon();
						int32				WeaponIndex	  = FMath::RandRange(int32(), WeaponSockets.Num() - 1);
						WeaponComponent->EquipWeapon(WeaponSockets[WeaponIndex]);
					}
				}
			}
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
