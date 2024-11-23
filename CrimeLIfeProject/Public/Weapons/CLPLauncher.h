// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Weapons/CLPWeapon.h"

#include "CLPLauncher.generated.h"

class ACLPProjectile;

UCLASS()
class CRIMELIFEPROJECT_API ACLPLauncher : public ACLPWeapon
{
	GENERATED_BODY()

public:
	ACLPLauncher();

protected:
	virtual void MakeShot() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<ACLPProjectile> ProjectileClass;
};
