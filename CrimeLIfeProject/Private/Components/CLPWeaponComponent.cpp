// Crime Life Project, All Rights NOT Reserved.

#include "Components/CLPWeaponComponent.h"

#include "Animation/AnimMontage.h"
#include "Animations/CLPAnimUtils.h"
#include "Animations/CLPEquipFinishedAnimNotify.h"
#include "Animations/CLPReloadFinishedAnimNotify.h"
#include "Components/CLPCharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/CLPAutofirableInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPWeaponComponent, All, All);

void UCLPWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	constexpr int32 MaxWeaponNum = 4;
	checkf(WeaponsData.Num() > MaxWeaponNum, TEXT("CharacterOwner can hold max %i weapons!"), MaxWeaponNum);

	InitAnimations();
	SpawnWeapons();
	SwitchWeapon(1.f);
}

void UCLPWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyAllWeapons();
}

bool UCLPWeaponComponent::IsActionAllowed()
{
	return !bIsEquipping && !bIsReloading;
}

void UCLPWeaponComponent::AttachWeaponToSocket(ACLPWeapon* Weapon, const FName& InSocketName)
{
	StopFiring();
	Weapon->DisablePhysics();

	if (!GetCharacterOwner())
	{
		UE_LOG(LogCLPWeaponComponent, Error, TEXT("Character error: Character Owner is null"));
	}
	else if (!Weapon)
	{
		UE_LOG(LogCLPWeaponComponent, Warning, TEXT("Weapon warning: Weapon is null"));
	}
	else if (USkeletalMeshComponent* SceneComponent = GetCharacterOwner()->GetMesh())
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(SceneComponent, AttachmentRules, InSocketName);
	}
}

void UCLPWeaponComponent::AddWeaponToAvailableSocket(ACLPWeapon* Weapon, const TArray<FName>& InSockets)
{
	for (const FName& InSocket : InSockets)
	{
		if (!SocketWeaponMap.Contains(InSocket))
		{
			Weapon->DisablePhysics();
			SocketWeaponMap.Add(InSocket, Weapon);
			AttachWeaponToSocket(Weapon, InSocket);
			return;
		}
	}

	UE_LOG(LogCLPWeaponComponent, Display, TEXT("You can't equip more weapons of this type"));
}

void UCLPWeaponComponent::AddWeapon(ACLPWeapon* Weapon)
{
	switch (GetWeaponType(Weapon))
	{
	case EWeaponType::Rifle:
		[[fallthrough]];
	case EWeaponType::Launcher:
		AddWeaponToAvailableSocket(Weapon, RifleSockets);
		break;
	case EWeaponType::Pistol:
		AddWeaponToAvailableSocket(Weapon, PistolSockets);
		break;
	case EWeaponType::Unarmed:
		break;
	default:
		UE_LOG(LogCLPWeaponComponent, Warning, TEXT("Undefined weapon type"));
		break;
	}
}

void UCLPWeaponComponent::SpawnWeapons()
{
	if (!GetCharacterOwner())
	{
		UE_LOG(LogCLPWeaponComponent, Error, TEXT("Spawn error: Character Owner is null"));
		return;
	}

	for (const FWeaponData& WeaponData : WeaponsData)
	{
		if (SocketWeaponMap.Num() == PistolSockets.Num() + RifleSockets.Num())
		{
			break;
		}

		if (ACLPWeapon* Weapon = GetWorld()->SpawnActor<ACLPWeapon>(WeaponData.WeaponClass))
		{
			Weapon->OnClipEmpty.AddUObject(this, &ThisClass::OnClipEmpty);
			Weapon->SetOwner(GetOwner());
			AddWeapon(Weapon);
		}
		else
		{
			UE_LOG(LogCLPWeaponComponent, Error, TEXT("Spawn Weapons error: Weapon is null"));
		}
	}
}

void UCLPWeaponComponent::DropAllWeapons()
{
	TArray<FName> Keys;
	SocketWeaponMap.GetKeys(Keys);

	for (const FName& InKey : Keys)
	{
		DropWeapon(InKey);
	}
}

void UCLPWeaponComponent::DestroyAllWeapons()
{
	TArray<FName> Keys;
	SocketWeaponMap.GetKeys(Keys);

	for (const FName& InKey : Keys)
	{
		if (SocketWeaponMap.FindRef(InKey))
		{
			SocketWeaponMap.FindRef(InKey)->Destroy();
		}
	}
}

void UCLPWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	if (GetCharacterOwner())
	{
		GetCharacterOwner()->PlayAnimMontage(AnimMontage);
	}
	else
	{
		UE_LOG(LogCLPWeaponComponent, Error, TEXT("Play Anim Montage error: Character Owner is null"));
	}
}

void UCLPWeaponComponent::InitAnimations()
{
	if (UCLPEquipFinishedAnimNotify* FinishedAnimNotify = CCLPAnimUtils::FindNotifyEvent<UCLPEquipFinishedAnimNotify>(EquipAnimMontage))
	{
		FinishedAnimNotify->OnNotified.AddUObject(this, &ThisClass::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogCLPWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set!"));
		checkNoEntry();
	}

	for (const FWeaponData WeaponData : WeaponsData)
	{
		if (UCLPReloadFinishedAnimNotify* FinishedAnimNotify = CCLPAnimUtils::FindNotifyEvent<UCLPReloadFinishedAnimNotify>(WeaponData.ReloadAnimMontage))
		{
			FinishedAnimNotify->OnNotified.AddUObject(this, &ThisClass::OnReloadFinished);
		}
		else
		{
			UE_LOG(LogCLPWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set!"));
			checkNoEntry();
		}
	}
}

void UCLPWeaponComponent::RemoveWeaponDelegates(ACLPWeapon* OldWeapon)
{
	if (OldWeapon)
	{
		OldWeapon->OnDamageSomeone.RemoveAll(this);
		OldWeapon->OnKilledSomeone.RemoveAll(this);
	}
}

void UCLPWeaponComponent::SetupWeaponDelegates(ACLPWeapon* NewWeapon)
{
	if (NewWeapon)
	{
		NewWeapon->OnDamageSomeone.AddUObject(this, &ThisClass::OnDamageSomeone);
		NewWeapon->OnKilledSomeone.AddUObject(this, &ThisClass::OnKilledSomeone);
	}
}

void UCLPWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	if (!GetCharacterOwner())
	{
		UE_LOG(LogCLPWeaponComponent, Error, TEXT("On Equip Finished error: Character Owner is null"));
		return;
	}
	else if (MeshComp != GetCharacterOwner()->GetMesh())
	{
		return;
	}
	bIsEquipping = false;
}

void UCLPWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
	if (!GetCharacterOwner())
	{
		UE_LOG(LogCLPWeaponComponent, Error, TEXT("On Reload Finished error: Character Owner is null"));
	}
	else if (MeshComp == GetCharacterOwner()->GetMesh())
	{
		bIsReloading = false;
	}
}

void UCLPWeaponComponent::OnClipEmpty()
{
	Reload();
}

void UCLPWeaponComponent::OnDamageSomeone()
{
	OnDamageSomeoneSignature.Broadcast();
}

void UCLPWeaponComponent::OnKilledSomeone()
{
	OnKilledSomeoneSignature.Broadcast();
}

int32 UCLPWeaponComponent::GetAmmunitionsToReload() const
{
	int32 SpentAmmunition = GetEquippedWeapon()->GetMaxAmmoCount() - GetEquippedWeapon()->GetAmmoCount();
	return FMath::Min(SpentAmmunition, GetEquippedWeaponTotalAmmo().AmmoCount);
}

bool UCLPWeaponComponent::CanFire()
{
	if (const AActor* Actor = GetOwner())
	{
		if (const UCLPCharacterMovementComponent* CLPCharacterMovementComponent = Actor->FindComponentByClass<UCLPCharacterMovementComponent>())
		{
			return CLPCharacterMovementComponent->IsAiming() && GetEquippedWeapon() && IsActionAllowed();
		}
	}
	return false;
}

bool UCLPWeaponComponent::CanEquip()
{
	return IsActionAllowed();
}

bool UCLPWeaponComponent::CanReload()
{
	FAmmo		Ammo		   = WeaponTypeAmmoMap.FindRef(GetEquippedWeaponType());
	ACLPWeapon* EquippedWeaopn = GetEquippedWeapon();
	return EquippedWeaopn && IsActionAllowed() && EquippedWeaopn->CanReload() && Ammo.AmmoCount > 0;
}

bool UCLPWeaponComponent::IsEquippedWeaponAmmoInfinity() const
{
	return GetEquippedWeapon() ? GetEquippedWeapon()->IsInfinity() : false;
}

bool UCLPWeaponComponent::IsWeaponTotalAmmoInfinity(EWeaponType WeaponType) const
{
	return GetWeaponTypeTotalAmmo(WeaponType).bInfinity;
}

bool UCLPWeaponComponent::IsEquippedWeaponTotalAmmoInfinity() const
{
	return IsWeaponTotalAmmoInfinity(GetEquippedWeaponType());
}

bool UCLPWeaponComponent::IsWeaponAmmoEmpty(ACLPWeapon* Weapon) const
{
	if (!Weapon)
	{
		return false;
	}

	const EWeaponType WeaponType = GetWeaponType(Weapon);
	return !IsWeaponTotalAmmoInfinity(WeaponType) && GetWeaponTypeTotalAmmo(WeaponType).AmmoCount == 0 && Weapon->IsAmmoEmpty();
}

bool UCLPWeaponComponent::IsEquippedWeaponAmmoEmpty() const
{
	return IsWeaponAmmoEmpty(GetEquippedWeapon());
}

ACharacter* UCLPWeaponComponent::GetCharacterOwner() const
{
	return Cast<ACharacter>(GetOwner());
}

int32 UCLPWeaponComponent::GetWeaponsCount() const
{
	return SocketWeaponMap.Num();
}

ACLPWeapon* UCLPWeaponComponent::GetWeapon(FName SocketName) const
{
	return SocketWeaponMap.FindRef(SocketName);
}

FAmmo UCLPWeaponComponent::GetWeaponTypeTotalAmmo(EWeaponType WeaponType) const
{
	return WeaponTypeAmmoMap.FindRef(WeaponType);
}

ACLPWeapon* UCLPWeaponComponent::GetEquippedWeapon() const
{
	return GetWeapon(EquippedWeaponSocketName);
}

FAmmo UCLPWeaponComponent::GetEquippedWeaponTotalAmmo() const
{
	return GetWeaponTypeTotalAmmo(GetEquippedWeaponType());
}

TArray<FName> UCLPWeaponComponent::GetSocketsForWeapon() const
{
	TArray<FName> WeaponSockets;

	WeaponSockets.Append(PistolSockets);
	WeaponSockets.Append(RifleSockets);

	return WeaponSockets;
}

void UCLPWeaponComponent::StopFiring()
{
	if (ICLPAutofirableInterface* Autofirable = Cast<ICLPAutofirableInterface>(GetEquippedWeapon()))
	{
		Autofirable->StopFiring();
	}
}

void UCLPWeaponComponent::Reload()
{
	if (!CanReload())
	{
		return;
	}

	// clang-format off
	const FWeaponData* CurrentWeaponClass =
		WeaponsData.FindByPredicate([this](const FWeaponData& InData)
		{
			return InData.WeaponClass == GetEquippedWeapon()->GetClass();
		});
	// clang-format on

	if (CurrentWeaponClass)
	{
		bIsReloading = true;
		StopFiring();
		int32 AmmunitionsToReload = GetAmmunitionsToReload();
		GetEquippedWeapon()->Reload(AmmunitionsToReload);
		FAmmo* AmmoInfo = WeaponTypeAmmoMap.Find(GetEquippedWeaponType());
		if (AmmoInfo)
		{
			AmmoInfo->AmmoCount -= AmmunitionsToReload;
		}

		PlayAnimMontage(CurrentWeaponClass->ReloadAnimMontage);
	}
}

void UCLPWeaponComponent::EquipWeapon(FName SocketName)
{
	if (!CanEquip())
	{
		return;
	}
	if (!EquippedWeaponSocketName.IsNone())
	{
		AttachWeaponToSocket(GetEquippedWeapon(), EquippedWeaponSocketName);
	}

	if (ACLPWeapon* NewWeapon = GetWeapon(SocketName))
	{
		RemoveWeaponDelegates(GetEquippedWeapon());
		AttachWeaponToSocket(NewWeapon, WeaponSocketName);
		EquippedWeaponSocketName = SocketName;
		bIsEquipping			 = true;
		PlayAnimMontage(EquipAnimMontage);
		SetupWeaponDelegates(NewWeapon);
	}
}

void UCLPWeaponComponent::DropWeapon(FName SocketName)
{
	if (GetWeapon(SocketName))
	{
		GetWeapon(SocketName)->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		GetWeapon(SocketName)->EnablePhysics();
		SocketWeaponMap.Remove(SocketName);
	}
	else
	{
		UE_LOG(LogCLPWeaponComponent, Warning, TEXT("Weapon warning: Weapon is null"));
	}
}

void UCLPWeaponComponent::Fire()
{
	if (CanFire())
	{
		GetEquippedWeapon()->Fire();
	}
}

void UCLPWeaponComponent::SwitchWeapon(const FInputActionValue& InInputValue)
{
	if (!CanEquip())
	{
		return;
	}

	float AxisValue			 = InInputValue.Get<float>();
	int32 WeaponSocketsCount = GetWeaponsCount();
	if (WeaponSocketsCount == 0)
	{
		UE_LOG(LogCLPWeaponComponent, Warning, TEXT("No weapons to equip"));
		return;
	}

	TArray<FName> WeaponSocketNames;
	SocketWeaponMap.GetKeys(WeaponSocketNames);

	WeaponIterator = (WeaponIterator + FMath::RoundToInt(AxisValue)) % WeaponSocketsCount;
	if (WeaponIterator < 0)
	{
		WeaponIterator += WeaponSocketsCount;
	}

	EquipWeapon(WeaponSocketNames[WeaponIterator]);
}

bool UCLPWeaponComponent::TryAddAmmo(EWeaponType WeaponType, int32 AmmoAmount)
{
	auto AddAmmo = [](int32 AmmoAmount, FAmmo Ammo) { return FMath::Clamp(AmmoAmount, 0, Ammo.MaxAmmoCount); };

	if (!WeaponTypeAmmoMap.Contains(WeaponType))
	{
		FAmmo Ammo		  = FAmmo();
		Ammo.MaxAmmoCount = WeaponTotalAmmoMaxAmount;
		Ammo.AmmoCount	  = AddAmmo(AmmoAmount, Ammo);

		WeaponTypeAmmoMap.Add(WeaponType, Ammo);
		return true;
	}
	if (FAmmo* AmmoInfo = WeaponTypeAmmoMap.Find(WeaponType))
	{
		const int32 OldAmmoCount = AmmoInfo->AmmoCount;
		AmmoInfo->AmmoCount		 = AddAmmo(AmmoInfo->AmmoCount + AmmoAmount, *AmmoInfo);
		return OldAmmoCount < AmmoInfo->AmmoCount;
	}
	return false;
}

EWeaponType UCLPWeaponComponent::GetWeaponType(ACLPWeapon* Weapon) const
{
	return Weapon ? Weapon->GetWeaponType() : EWeaponType::Unarmed;
}

EWeaponType UCLPWeaponComponent::GetEquippedWeaponType() const
{
	return GetWeaponType(GetEquippedWeapon());
}

int32 UCLPWeaponComponent::GetAmmoCount() const
{
	return GetEquippedWeapon() ? GetEquippedWeapon()->GetAmmoCount() : int32();
}

float UCLPWeaponComponent::GetWeaponTypeTotalAmmoCountPercent(EWeaponType WeaponType) const
{
	if (WeaponTypeAmmoMap.Contains(WeaponType))
	{
		const FAmmo InAmmo = WeaponTypeAmmoMap.FindRef(WeaponType);
		return static_cast<float>(InAmmo.AmmoCount) / InAmmo.MaxAmmoCount;
	}
	return 0.f;
}
