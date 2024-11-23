// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Weapons/CLPWeapon.h"

#include "CLPWeaponComponent.generated.h"

class UAnimMontage;
class ACLPCharacter;
struct FInputActionValue;

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ACLPWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRIMELIFEPROJECT_API UCLPWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	bool IsActionAllowed();

	void AttachWeaponToSocket(ACLPWeapon* Weapon, const FName& InSocketName);
	void AddWeaponToAvailableSocket(ACLPWeapon* Weapon, const TArray<FName>& InSockets);
	void AddWeapon(ACLPWeapon* Weapon);
	void SpawnWeapons();
	void DropAllWeapons();
	void DestroyAllWeapons();

	void PlayAnimMontage(UAnimMontage* AnimMontage);
	void InitAnimations();

	void RemoveWeaponDelegates(ACLPWeapon* OldWeapon);
	void SetupWeaponDelegates(ACLPWeapon* NewWeapon);

	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);
	void OnClipEmpty();
	void OnDamageSomeone();
	void OnKilledSomeone();

	int32 GetAmmunitionsToReload() const;

protected:
	bool CanFire();
	bool CanEquip();
	bool CanReload();

public:
	bool IsEquippedWeaponAmmoInfinity() const;
	bool IsWeaponTotalAmmoInfinity(EWeaponType WeaponType) const;
	bool IsEquippedWeaponTotalAmmoInfinity() const;
	bool IsWeaponAmmoEmpty(ACLPWeapon* Weapon) const;
	bool IsEquippedWeaponAmmoEmpty() const;

	ACharacter*	  GetCharacterOwner() const;
	int32		  GetWeaponsCount() const;
	ACLPWeapon*	  GetWeapon(FName SocketName) const;
	FAmmo		  GetWeaponTypeTotalAmmo(EWeaponType WeaponType) const;
	ACLPWeapon*	  GetEquippedWeapon() const;
	FAmmo		  GetEquippedWeaponTotalAmmo() const;
	TArray<FName> GetSocketsForWeapon() const;

	void StopFiring();
	void Reload();
	void EquipWeapon(FName SocketName);
	void DropWeapon(FName SocketName);

	virtual void Fire();
	virtual void SwitchWeapon(const FInputActionValue& InInputValue);

	bool TryAddAmmo(EWeaponType WeaponType, int32 AmmoAmount);

protected:
	int32 WeaponIterator = 0;
	bool  bIsEquipping	 = false;
	bool  bIsReloading	 = false;

public:
	FOnDamageSomeoneSignature OnDamageSomeoneSignature;
	FOnKilledSomeoneSignature OnKilledSomeoneSignature;

public:
	UFUNCTION(BlueprintCallable)
	EWeaponType GetWeaponType(ACLPWeapon* Weapon) const;

	UFUNCTION(BlueprintCallable)
	EWeaponType GetEquippedWeaponType() const;

	UFUNCTION(BlueprintCallable)
	int32 GetAmmoCount() const;

	UFUNCTION(BlueprintCallable)
	float GetWeaponTypeTotalAmmoCountPercent(EWeaponType WeaponType) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<FWeaponData> WeaponsData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TMap<FName, ACLPWeapon*> SocketWeaponMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TMap<EWeaponType, FAmmo> WeaponTypeAmmoMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 WeaponTotalAmmoMaxAmount = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Socket")
	TArray<FName> RifleSockets = { TEXT("RightRifleSocket"), TEXT("LeftRifleSocket") };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Socket")
	TArray<FName> PistolSockets = { TEXT("RightPistolSocket"), TEXT("LeftPistolSocket") };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Socket")
	FName WeaponSocketName = TEXT("WeaponSocket");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Socket")
	FName EquippedWeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* EquipAnimMontage;
};
