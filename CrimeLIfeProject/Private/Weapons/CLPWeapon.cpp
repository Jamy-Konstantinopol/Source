// Crime Life Project, All Rights NOT Reserved.

#include "Weapons/CLPWeapon.h"

#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Player/CLPCharacter.h"
#include "TimerManager.h"
#include "Weapons/Components/CLPWeaponFXComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPWeapon, All, All);

ACLPWeapon::ACLPWeapon()
{
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	EnablePhysics();
	SetRootComponent(WeaponMeshComponent);
	if (WeaponMeshComponent)
	{
		WeaponMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}

	WeaponFXComponent = CreateDefaultSubobject<UCLPWeaponFXComponent>(TEXT("WeaponFXComponent"));
}

void ACLPWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMeshComponent);
	check(WeaponFXComponent);

	Ammo.AmmoCount = Ammo.MaxAmmoCount;

	// clang-format off
	OnFire.AddLambda([this]()
	{
		if (WeaponMeshComponent)
		{
			WeaponMeshComponent->PlayAnimation(FireAnimSequence, false);
		}
	});
	// clang-format on

	OnFire.AddUObject(this, &ThisClass::DecreaseAmmo);
}

void ACLPWeapon::SpawnTrace(const FVector& InStart, const FVector& InEnd)
{
	UNiagaraComponent* TraceFCXomponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, InStart);
	if (TraceFCXomponent)
	{
		TraceFCXomponent->SetVariableVec3(TraceTargetName, InEnd);
	}
}

AController* ACLPWeapon::GetOwnerController() const
{
	const APawn* Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

bool ACLPWeapon::GetPlayerViewPoint(FVector& OutViewLocation, FRotator& OutViewRotation) const
{
	if (const APawn* Character = Cast<APawn>(GetOwner()))
	{
		if (Character->IsPlayerControlled())
		{
			if (const APlayerController* Controller = Character->GetController<APlayerController>())
			{
				Controller->GetPlayerViewPoint(OutViewLocation, OutViewRotation);
				return true;
			}
		}
		else
		{
			OutViewLocation = GetMuzzleSocketWorldLocation();
			OutViewRotation = GetMuzzleSocketWorldRotation();
			return true;
		}
	}
	return false;
}

bool ACLPWeapon::GetCameraTrace(FVector& OutStart, FVector& OutEnd) const
{
	FVector	 CameraLocation;
	FRotator CameraRotation;

	if (GetPlayerViewPoint(CameraLocation, CameraRotation))
	{
		OutStart = CameraLocation;
		OutEnd	 = CameraLocation + CameraRotation.Vector() * TraceMaxDistance;
		return true;
	}

	return false;
}

FRotator ACLPWeapon::GetMuzzleSocketWorldRotation() const
{
	return WeaponMeshComponent->GetSocketRotation(MuzzleSocketName);
}

FVector ACLPWeapon::GetMuzzleSocketWorldLocation() const
{
	return WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);
}

FTransform ACLPWeapon::GetMuzzleSocketWorldTransform() const
{
	return WeaponMeshComponent->GetSocketTransform(MuzzleSocketName);
}

void ACLPWeapon::MakeHit(FHitResult& OutHitResult, const FVector& InTraceStart, const FVector& InTraceEnd)
{
	if (GetWorld())
	{
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());
		CollisionParams.bReturnPhysicalMaterial = true;
		GetWorld()->LineTraceSingleByChannel(OutHitResult, InTraceStart, InTraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
	}
	else
	{
		UE_LOG(LogCLPWeapon, Error, TEXT("Hit error: World is nullptr"));
	}
}

void ACLPWeapon::MakeDamage(const FHitResult& InHitResult)
{
	APawn* DamagedPawn = Cast<APawn>(InHitResult.GetActor());
	if (!DamagedPawn)
	{
		return;
	}

	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.HitInfo = InHitResult;
	DamagedPawn->TakeDamage(Damage, PointDamageEvent, GetOwnerController(), nullptr);

	const ACLPCharacter* DamagedCLPCharacter = Cast<ACLPCharacter>(DamagedPawn);
	if (!DamagedCLPCharacter)
	{
		return;
	}

	if (DamagedCLPCharacter->IsDead())
	{
		OnKilledSomeone.Broadcast();
	}
	else
	{
		OnDamageSomeone.Broadcast();
	}
}

void ACLPWeapon::MakeShot()
{
	if (!GetWorld())
	{
		UE_LOG(LogCLPWeapon, Error, TEXT("Shot error: World is nullptr"));
		return;
	}
	if (IsClipEmpty())
	{
		OnClipEmpty.Broadcast();
		return;
	}

	FVector CameraTraceStart;
	FVector CameraTraceEnd;

	if (!GetCameraTrace(CameraTraceStart, CameraTraceEnd))
	{
		UE_LOG(LogCLPWeapon, Error, TEXT("Shot error: could not init Camera Trace"));
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, CameraTraceStart, CameraTraceEnd);

	FVector TraceShotStart = GetMuzzleSocketWorldLocation();
	FVector TraceShotEnd;
	if (HitResult.bBlockingHit)
	{
		TraceShotEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpact(HitResult);
	}
	else
	{
		FVector ShotDirection	= (CameraTraceEnd - CameraTraceStart).GetSafeNormal();
		FVector RandomDirection = FMath::VRandCone(ShotDirection, FMath::DegreesToRadians(BulletSpread));
		TraceShotEnd			= CameraTraceStart + RandomDirection * (CameraTraceEnd - CameraTraceStart).Size();
	}

	SpawnTrace(TraceShotStart, TraceShotEnd);
	OnFire.Broadcast();
}

void ACLPWeapon::DecreaseAmmo()
{
	if (IsClipEmpty())
	{
		OnClipEmpty.Broadcast();
		return;
	}
	--Ammo.AmmoCount;

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		OnClipEmpty.Broadcast();
	}
}

bool ACLPWeapon::IsAmmoEmpty() const
{
	return !IsInfinity() && IsClipEmpty();
}

bool ACLPWeapon::IsClipEmpty() const
{
	return Ammo.AmmoCount <= 0;
}

bool ACLPWeapon::CanReload() const
{
	return Ammo.AmmoCount < Ammo.MaxAmmoCount;
}

bool ACLPWeapon::IsInfinity() const
{
	return Ammo.bInfinity;
}

EWeaponType ACLPWeapon::GetWeaponType() const
{
	return WeaponType;
}

void ACLPWeapon::Fire()
{
	MakeShot();
}

void ACLPWeapon::Reload(int32 AmmunitionsToReload)
{
	Ammo.AmmoCount += AmmunitionsToReload;
}

void ACLPWeapon::EnablePhysics()
{
	WeaponMeshComponent->SetSimulatePhysics(true);
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACLPWeapon::DisablePhysics()
{
	WeaponMeshComponent->SetSimulatePhysics(false);
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

int32 ACLPWeapon::GetAmmoCount() const
{
	return Ammo.AmmoCount;
}

int32 ACLPWeapon::GetMaxAmmoCount() const
{
	return Ammo.MaxAmmoCount;
}

ACharacter* ACLPWeapon::GetCharacterOwner() const
{
	return Cast<ACharacter>(GetOwner());
}
