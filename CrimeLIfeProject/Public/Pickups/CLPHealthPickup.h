// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Pickups/CLPPickup.h"

#include "CLPHealthPickup.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API ACLPHealthPickup : public ACLPPickup
{
	GENERATED_BODY()

protected:
	virtual bool GivePickupTo(APawn* Pawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1", UIMin = "1"))
	int32 HealAmount = 40;
};
