// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "CLPWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);
DECLARE_MULTICAST_DELEGATE(FOnDamageSomeoneSignature);
DECLARE_MULTICAST_DELEGATE(FOnKilledSomeoneSignature);
DECLARE_MULTICAST_DELEGATE(FOnFireSignature);

class USkeletalMeshComponent;
class UCLPWeaponFXComponent;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol	 UMETA(DisplayName = "Pistol"),
	Rifle	 UMETA(DisplayName = "Rifle"),
	Launcher UMETA(DisplayName = "Launcher"),
	Unarmed	 UMETA(DisplayName = "Unarmed"),
};

USTRUCT(BlueprintType)
struct FAmmo
{
	GENERATED_USTRUCT_BODY()

public:
	int32 AmmoCount = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo", meta = (ClampMin = "0", UIMin = "0"))
	int32 MaxAmmoCount = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	bool bInfinity = false;
};

UCLASS()
class CRIMELIFEPROJECT_API ACLPWeapon : public AActor
{
	GENERATED_BODY()

public:
	ACLPWeapon();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnTrace(const FVector& InStart, const FVector& InEnd);

private:
	AController* GetOwnerController() const;

protected:
	bool	   GetPlayerViewPoint(FVector& OutViewLocation, FRotator& OutViewRotation) const;
	bool	   GetCameraTrace(FVector& OutStart, FVector& OutEnd) const;
	FRotator   GetMuzzleSocketWorldRotation() const;
	FVector	   GetMuzzleSocketWorldLocation() const;
	FTransform GetMuzzleSocketWorldTransform() const;

	void		 MakeHit(FHitResult& OutHitResult, const FVector& InTraceStart, const FVector& InTraceEnd);
	void		 MakeDamage(const FHitResult& InHitResult);
	virtual void MakeShot();
	void		 DecreaseAmmo();

public:
	bool		IsAmmoEmpty() const;
	bool		IsClipEmpty() const;
	bool		CanReload() const;
	bool		IsInfinity() const;
	EWeaponType GetWeaponType() const;

	virtual void Fire();
	virtual void Reload(int32 AmmunitionsToReload);

	virtual void EnablePhysics();
	virtual void DisablePhysics();

public:
	FOnClipEmptySignature	  OnClipEmpty;
	FOnDamageSomeoneSignature OnDamageSomeone;
	FOnKilledSomeoneSignature OnKilledSomeone;
	FOnFireSignature		  OnFire;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetAmmoCount() const;

	UFUNCTION(BlueprintCallable)
	int32 GetMaxAmmoCount() const;

	UFUNCTION(BlueprintCallable)
	ACharacter* GetCharacterOwner() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0", UIMin = "0"))
	float Damage = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName = TEXT("MuzzleSocket");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = "0", UIMin = "0"))
	float TraceMaxDistance = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = "0", UIMin = "0"))
	float BulletSpread = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	FAmmo Ammo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimSequence* FireAnimSequence;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VFX")
	UCLPWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	FName TraceTargetName = FName("TraceTarget");
};
