// Crime Life Project, All Rights NOT Reserved.

#include "Weapons/CLPLauncher.h"

#include "Engine/World.h"
#include "Weapons/CLPProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPLauncher, All, All);

ACLPLauncher::ACLPLauncher()
{
	WeaponType = EWeaponType::Launcher;
}

void ACLPLauncher::MakeShot()
{
	if (IsClipEmpty())
	{
		OnClipEmpty.Broadcast();
		return;
	}

	if (!GetWorld())
	{
		UE_LOG(LogCLPLauncher, Error, TEXT("Shot error: World is nullptr"));
		return;
	}

	FVector CameraTraceStart;
	FVector CameraTraceEnd;

	if (!GetCameraTrace(CameraTraceStart, CameraTraceEnd))
	{
		UE_LOG(LogCLPLauncher, Error, TEXT("Shot error: could not init Camera Trace"));
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, CameraTraceStart, CameraTraceEnd);

	const FVector EndPoint	= HitResult.bBlockingHit ? HitResult.ImpactPoint : CameraTraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleSocketWorldLocation()).GetSafeNormal();

	if (ACLPProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACLPProjectile>(ProjectileClass, GetMuzzleSocketWorldTransform()))
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(GetMuzzleSocketWorldTransform());
	}

	OnFire.Broadcast();
}
