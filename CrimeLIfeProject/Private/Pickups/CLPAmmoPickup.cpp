// Crime Life Project, All Rights NOT Reserved.

#include "Pickups/CLPAmmoPickup.h"

#include "Components/CLPHealthComponent.h"
#include "Components/CLPWeaponComponent.h"

bool ACLPAmmoPickup::GivePickupTo(APawn* Pawn)
{
	if (!Pawn)
	{
		return false;
	}

	const UCLPHealthComponent* HealthComponent = Pawn->FindComponentByClass<UCLPHealthComponent>();
	UCLPWeaponComponent*	   WeaponComponent = Pawn->FindComponentByClass<UCLPWeaponComponent>();
	if (!HealthComponent || !WeaponComponent || HealthComponent->IsDead())
	{
		return false;
	}
	return WeaponComponent->TryAddAmmo(WeaponType, AmmoAmount);
}
