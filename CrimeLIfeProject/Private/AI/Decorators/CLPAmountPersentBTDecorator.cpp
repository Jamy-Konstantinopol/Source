// Crime Life Project, All Rights NOT Reserved.

#include "AI/Decorators/CLPAmountPersentBTDecorator.h"

#include "AIController.h"
#include "Components/CLPWeaponComponent.h"

UCLPAmountPersentBTDecorator::UCLPAmountPersentBTDecorator()
{
	NodeName = TEXT("Ammo Percent");
}

bool UCLPAmountPersentBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller || !Controller->GetPawn())
	{
		return false;
	}

	const UCLPWeaponComponent* WeaponComponent = Controller->GetPawn()->FindComponentByClass<UCLPWeaponComponent>();
	if (!WeaponComponent)
	{
		return false;
	}
	return WeaponComponent->GetWeaponTypeTotalAmmoCountPercent(WeaponComponent->GetEquippedWeaponType()) < AmmoPercent;
}
