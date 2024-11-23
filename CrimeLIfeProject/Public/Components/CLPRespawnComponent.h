// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "CLPRespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRIMELIFEPROJECT_API UCLPRespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void Respawn(int32 RespawnTime);

	int32 GetRespawnCountDown() const;
	bool  IsRespawnInProgress() const;

private:
	void RespawnTimerUpdate();

private:
	FTimerHandle RespawnTimerHandle;
	int32		 RespawnCountDown = 0;
};
