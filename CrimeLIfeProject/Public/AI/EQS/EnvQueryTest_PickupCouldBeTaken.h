// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "EnvironmentQuery/EnvQueryTest.h"

#include "EnvQueryTest_PickupCouldBeTaken.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UEnvQueryTest_PickupCouldBeTaken : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer);

public:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
