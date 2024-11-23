// Crime Life Project, All Rights NOT Reserved.

#include "Pickups/CLPPickup.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPPickup, All, All);

ACLPPickup::ACLPPickup()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->InitSphereRadius(100.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);
}

void ACLPPickup::BeginPlay()
{
	Super::BeginPlay();

	check(CollisionComponent);
}

void ACLPPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APawn* OtherPawn = Cast<APawn>(OtherActor);
	if (GivePickupTo(OtherPawn))
	{
		PickupWasTaken();
	}
}

void ACLPPickup::PickupWasTaken()
{
	if (GetRootComponent())
	{
		CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		GetRootComponent()->SetVisibility(false, true);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ThisClass::Respawn, RespawnTime);
		UGameplayStatics::PlaySoundAtLocation(this, TakeSound, GetActorLocation());
	}
}

void ACLPPickup::Respawn()
{
	if (GetRootComponent())
	{
		CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		GetRootComponent()->SetVisibility(true, true);
	}
}

bool ACLPPickup::GivePickupTo(APawn* Pawn)
{
	return false;
}

bool ACLPPickup::CanBeTaken() const
{
	return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}
