// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Weapons/CLPAutofirableInterface.h"
#include "Weapons/CLPWeapon.h"

#include "CLPRifle.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API ACLPRifle : public ACLPWeapon, public ICLPAutofirableInterface
{
	GENERATED_BODY()

public:
	ACLPRifle();

protected:
	virtual void MakeShot() override;

public:
	virtual void Fire() override;
	virtual void StopFiring() override;
	virtual void Reload(int32 AmmunitionsToReload) override;

private:
	FTimerHandle ShotTimerHandle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float ShotDelay = 0.15f;
};
