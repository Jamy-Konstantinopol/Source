// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CLPAutofirableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCLPAutofirableInterface : public UInterface
{
	GENERATED_BODY()
};

class CRIMELIFEPROJECT_API ICLPAutofirableInterface
{
	GENERATED_BODY()

public:
	virtual void StopFiring() = 0;
};
