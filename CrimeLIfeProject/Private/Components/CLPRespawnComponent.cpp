// Crime Life Project, All Rights NOT Reserved.

#include "Components/CLPRespawnComponent.h"

#include "CLPGameModeBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UCLPRespawnComponent::Respawn(int32 RespawnTime)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	RespawnCountDown = RespawnTime;
	World->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ThisClass::RespawnTimerUpdate, 1.f, true);
}

int32 UCLPRespawnComponent::GetRespawnCountDown() const
{
	return RespawnCountDown;
}

bool UCLPRespawnComponent::IsRespawnInProgress() const
{
	const UWorld* World = GetWorld();
	return World ? World->GetTimerManager().IsTimerActive(RespawnTimerHandle) : false;
}

void UCLPRespawnComponent::RespawnTimerUpdate()
{
	if (--RespawnCountDown <= 0)
	{
		const UWorld* World = GetWorld();
		if (!World)
		{
			return;
		}
		World->GetTimerManager().ClearTimer(RespawnTimerHandle);

		ACLPGameModeBase* GameMode = Cast<ACLPGameModeBase>(World->GetAuthGameMode());
		if (!GameMode)
		{
			return;
		}

		GameMode->RespawnRequest(Cast<AController>(GetOwner()));
	}
}
