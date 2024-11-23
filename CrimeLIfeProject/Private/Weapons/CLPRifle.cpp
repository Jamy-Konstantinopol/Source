// Crime Life Project, All Rights NOT Reserved.

#include "Weapons/CLPRifle.h"

#include "DrawDebugHelpers.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPRifle, All, All);

ACLPRifle::ACLPRifle()
{
	WeaponType = EWeaponType::Rifle;
}

void ACLPRifle::MakeShot()
{
	FVector CameraTraceStart;
	FVector CameraTraceEnd;
	if (!GetCameraTrace(CameraTraceStart, CameraTraceEnd))
	{
		StopFiring();
	}
	else
	{
		Super::MakeShot();
	}
}

void ACLPRifle::Fire()
{
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ThisClass::MakeShot, ShotDelay, true);
	Super::Fire();
}

void ACLPRifle::StopFiring()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ACLPRifle::Reload(int32 AmmunitionsToReload)
{
	StopFiring();
	Super::Reload(AmmunitionsToReload);
}
