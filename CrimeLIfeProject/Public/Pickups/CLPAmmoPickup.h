// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Pickups/CLPPickup.h"
#include "Weapons/CLPWeapon.h"

#include "CLPAmmoPickup.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API ACLPAmmoPickup : public ACLPPickup
{
	GENERATED_BODY()

protected:
	virtual bool GivePickupTo(APawn* Pawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1", UIMin = "1"))
	int32 AmmoAmount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	EWeaponType WeaponType;
};
