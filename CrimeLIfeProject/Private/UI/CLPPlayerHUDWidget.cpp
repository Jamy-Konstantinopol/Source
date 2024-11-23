// Crime Life Project, All Rights NOT Reserved.

#include "UI/CLPPlayerHUDWidget.h"

#include "Components/CLPHealthComponent.h"
#include "Components/CLPWeaponComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"

void UCLPPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::OnNewPawn);
		OnNewPawn(GetOwningPlayerPawn());
	}
}

bool UCLPPlayerHUDWidget::IsDamageSomeone() const
{
	return bIsDamageSomeone;
}

bool UCLPPlayerHUDWidget::IsKilledSomeone() const
{
	return bIsKilledSomeone;
}

void UCLPPlayerHUDWidget::SetIsDamageSomeone(bool bNewIsDamageSomeone)
{
	bIsDamageSomeone = bNewIsDamageSomeone;
}

void UCLPPlayerHUDWidget::SetIsKilledSomeone(bool bNewIsKilledSomeone)
{
	bIsKilledSomeone = bNewIsKilledSomeone;
}

void UCLPPlayerHUDWidget::OnDamageSomeone()
{
	SetIsDamageSomeone(true);

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UCLPPlayerHUDWidget::SetIsDamageSomeone, false);
	GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, TimerDelegate, EffectDuration, false);
}

void UCLPPlayerHUDWidget::OnKilledSomeone()
{
	SetIsKilledSomeone(true);

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UCLPPlayerHUDWidget::SetIsKilledSomeone, false);
	GetWorld()->GetTimerManager().SetTimer(KillTimerHandle, TimerDelegate, EffectDuration, false);
}

void UCLPPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
	if (NewPawn)
	{
		UCLPHealthComponent* HealthComponent = NewPawn->FindComponentByClass<UCLPHealthComponent>();
		if (HealthComponent && !HealthComponent->OnHealthChanged.IsAlreadyBound(this, &ThisClass::OnHealthChanged))
		{
			HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
			HealthComponent->OnHealthChanged.Broadcast(0.f);
		}

		UCLPWeaponComponent* WeaponComponent = NewPawn->FindComponentByClass<UCLPWeaponComponent>();
		if (WeaponComponent && !WeaponComponent->OnDamageSomeoneSignature.IsBoundToObject(this))
		{
			WeaponComponent->OnDamageSomeoneSignature.AddUObject(this, &ThisClass::OnDamageSomeone);
		}
		if (WeaponComponent && !WeaponComponent->OnKilledSomeoneSignature.IsBoundToObject(this))
		{
			WeaponComponent->OnKilledSomeoneSignature.AddUObject(this, &ThisClass::OnKilledSomeone);
		}
	}
}

bool UCLPPlayerHUDWidget::IsPlayerAlive() const
{
	if (!GetOwningPlayerPawn())
	{
		return false;
	}
	const UCLPHealthComponent* HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<UCLPHealthComponent>();
	return HealthComponent && !HealthComponent->IsDead();
}

bool UCLPPlayerHUDWidget::IsPlayerSpectating() const
{
	const APlayerController* Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool UCLPPlayerHUDWidget::HasEquippedWeapon() const
{
	if (!GetOwningPlayerPawn())
	{
		return false;
	}
	const UCLPWeaponComponent* WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UCLPWeaponComponent>();
	return WeaponComponent && WeaponComponent->GetEquippedWeaponType() != EWeaponType::Unarmed;
}

float UCLPPlayerHUDWidget::GetHealthPercent() const
{
	if (GetOwningPlayerPawn())
	{
		if (const UCLPHealthComponent* HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<UCLPHealthComponent>())
		{
			return HealthComponent->GetHealthPercent();
		}
	}
	return 0.f;
}

int32 UCLPPlayerHUDWidget::GetAmmoCount() const
{
	if (GetOwningPlayerPawn())
	{
		if (const UCLPWeaponComponent* WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UCLPWeaponComponent>())
		{
			return WeaponComponent->GetAmmoCount();
		}
	}
	return int32();
}

FString UCLPPlayerHUDWidget::GetEquippedWeaponTotalAmmoCount() const
{
	if (GetOwningPlayerPawn())
	{
		if (const UCLPWeaponComponent* WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UCLPWeaponComponent>())
		{
			return WeaponComponent->IsEquippedWeaponAmmoInfinity() ? InfinitySign : FString::FromInt(WeaponComponent->GetEquippedWeaponTotalAmmo().AmmoCount);
		}
	}
	return FString::FromInt(int32());
}

UTexture2D* UCLPPlayerHUDWidget::GetAimPointImage() const
{
	if (GetOwningPlayerPawn())
	{
		if (const UCLPWeaponComponent* WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UCLPWeaponComponent>())
		{
			if (WeaponComponent->GetEquippedWeaponType() != EWeaponType::Unarmed)
			{
				return AimCrosshairImage.ArmedAimPointImage;
			}
			return AimCrosshairImage.UnarmedAimPointImage;
		}
	}
	return nullptr;
}

UTexture2D* UCLPPlayerHUDWidget::GetDamageCrosshairImage() const
{
	if (IsKilledSomeone())
	{
		return AimCrosshairImage.KilledCrosshairImage;
	}
	if (IsDamageSomeone())
	{
		return AimCrosshairImage.DamageCrosshairImage;
	}
	return nullptr;
}

void UCLPPlayerHUDWidget::OnHealthChanged(float Health)
{
	if (const UCLPHealthComponent* HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<UCLPHealthComponent>())
	{
		if (HealthComponent->GetHealthPercent() <= DangerHealth && HealthComponent->GetHealthPercent() > 0.f)
		{
			PlayAnimation(DangerHealthAnimation, 0.f, 0, EUMGSequencePlayMode::Forward, 1.f, true);
		}
		else
		{
			StopAnimation(DangerHealthAnimation);
		}
	}
}
