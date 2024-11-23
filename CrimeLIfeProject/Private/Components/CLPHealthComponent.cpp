// Crime Life Project, All Rights NOT Reserved.

#include "Components/CLPHealthComponent.h"

#include "CLPGameModeBase.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Perception/AISenseConfig_Damage.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPHealthComponent, All, All);

void UCLPHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AddHealth(MaxHealth);

	if (AActor* ActorOwner = GetOwner())
	{
		ActorOwner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamage);
		ActorOwner->OnTakePointDamage.AddDynamic(this, &ThisClass::OnTakePointDamage);
		ActorOwner->OnTakeRadialDamage.AddDynamic(this, &ThisClass::OnTakeRadialDamage);
	}
}

void UCLPHealthComponent::AddHealth(float HealthIncrement)
{
	Health = FMath::Clamp(Health + HealthIncrement, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

void UCLPHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	if (Damage <= 0.f || IsDead() || !GetWorld())
	{
		return;
	}

	AddHealth(-Damage);

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
	else if (bAutoHeal && GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &ThisClass::Heal, HealRate, true, HealFirstDelay);
	}
	ReportDamageEvent(Damage, InstigatedBy);
}

void UCLPHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
	if (InstigatedBy && InstigatedBy->GetPawn() && GetOwner() && GetOwner() != InstigatedBy->GetPawn())
	{
		UAISense_Damage::ReportDamageEvent(this, GetOwner(), InstigatedBy->GetPawn(), Damage, InstigatedBy->GetPawn()->GetActorLocation(),
										   GetOwner()->GetActorLocation());
	}
}

float UCLPHealthComponent::GetDamageModificator(const FName& BoneName)
{
	const ACharacter* CharacterOwner = GetOwner<ACharacter>();
	if (CharacterOwner && CharacterOwner->GetMesh())
	{
		if (const FBodyInstance* BodyInstance = CharacterOwner->GetMesh()->GetBodyInstance(BoneName))
		{
			if (DamageModificatorMap.Contains(BodyInstance->GetSimplePhysicalMaterial()))
			{
				return DamageModificatorMap.FindRef(BodyInstance->GetSimplePhysicalMaterial());
			}
		}
	}
	return 1.f;
}

bool UCLPHealthComponent::TryAddHealth(float HealthIncrement)
{
	if (IsDead() || IsHealthFull())
	{
		return false;
	}
	AddHealth(HealthIncrement);
	return true;
}

float UCLPHealthComponent::GetHealth() const
{
	return Health;
}

float UCLPHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UCLPHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
}

void UCLPHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
											UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType,
											AActor* DamageCauser)
{
	ApplyDamage(Damage * GetDamageModificator(BoneName), InstigatedBy);
}

void UCLPHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo,
											 AController* InstigatedBy, AActor* DamageCauser)
{
	ApplyDamage(Damage, InstigatedBy);
}

void UCLPHealthComponent::Heal()
{
	AddHealth(HealIncrement);

	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UCLPHealthComponent::Killed(AController* KillerController)
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	ACLPGameModeBase* GameMode = Cast<ACLPGameModeBase>(World->GetAuthGameMode());
	if (!GameMode)
	{
		return;
	}

	const APawn* Player			  = Cast<APawn>(GetOwner());
	AController* VictimController = Player ? Player->Controller : nullptr;
	GameMode->Killed(KillerController, VictimController);
}

bool UCLPHealthComponent::IsDead() const
{
	return FMath::IsNearlyZero(Health);
}

bool UCLPHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

float UCLPHealthComponent::GetHealthPercent() const
{
	return GetMaxHealth() == 0 ? 0 : GetHealth() / GetMaxHealth();
}
