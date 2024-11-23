// Crime Life Project, All Rights NOT Reserved.

#include "Weapons/CLPProjectile.h"

#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/Components/CLPWeaponFXComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPProjectile, All, All);

ACLPProjectile::ACLPProjectile()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	StaticMeshComponent->bReturnMaterialOnMove = true;
	SetRootComponent(StaticMeshComponent);

	MovementComponent				= CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	MovementComponent->InitialSpeed = 1800.f;

	WeaponFXComponent = CreateDefaultSubobject<UCLPWeaponFXComponent>(TEXT("WeaponFXComponent"));
}

void ACLPProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(MovementComponent);
	check(StaticMeshComponent);
	check(WeaponFXComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	SetLifeSpan(ProjectileLifeSpan);
	if (GetWorld())
	{
		OnDestroyed.AddDynamic(this, &ThisClass::OnActorDestroyd);
	}

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);
}

AController* ACLPProjectile::GetOwnerController() const
{
	const APawn* Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

void ACLPProjectile::SetShotDirection(const FVector& InDirection)
{
	ShotDirection = InDirection;
}

void ACLPProjectile::ApplyDamage()
{
	UGameplayStatics::ApplyRadialDamage //
		(GetWorld(), Damage, GetActorLocation(), DamageRadius, UDamageType::StaticClass(), {}, this, GetOwnerController(), bDoFullDamage);
}

void ACLPProjectile::OnActorDestroyd(AActor* DestroyedActor)
{
	if (DestroyedActor)
	{
		ApplyDamage();
	}
}

void ACLPProjectile::OnProjectileHit //
	(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& InHitResult)
{
	if (!GetWorld())
	{
		UE_LOG(LogCLPProjectile, Error, TEXT("Hit error: World is nullptr"));
		return;
	}
	WeaponFXComponent->PlayImpact(InHitResult);
	Destroy();
}
