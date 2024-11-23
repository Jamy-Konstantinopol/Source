// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/CLPWeaponComponent.h"

#include "CLPAIWeaponComponent.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UCLPAIWeaponComponent : public UCLPWeaponComponent
{
	GENERATED_BODY()

public:
	virtual void Fire() override;
	virtual void SwitchWeapon(const FInputActionValue& InInputValue) override;
};
