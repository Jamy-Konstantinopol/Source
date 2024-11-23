// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "EnvironmentQuery/EnvQueryContext.h"

#include "CLPEnemyEnvQueryContext.generated.h"

UCLASS()
class CRIMELIFEPROJECT_API UCLPEnemyEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName EnemyActorKeyName = "Enemy";
};
