// Crime Life Project, All Rights NOT Reserved.

#include "Pickups/CLPHealthPickup.h"

#include "Components/CLPHealthComponent.h"
#include "Components/CLPWeaponComponent.h"

bool ACLPHealthPickup::GivePickupTo(APawn* Pawn)
{
	if (!Pawn)
	{
		return false;
	}

	UCLPHealthComponent* HealthComponent = Pawn->FindComponentByClass<UCLPHealthComponent>();
	if (!HealthComponent || HealthComponent->IsDead())
	{
		return false;
	}
	return HealthComponent->TryAddHealth(HealAmount);
}
