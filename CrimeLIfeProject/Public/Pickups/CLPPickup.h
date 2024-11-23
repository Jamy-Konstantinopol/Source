// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "CLPPickup.generated.h"

class USphereComponent;
class USoundCue;

UCLASS()
class CRIMELIFEPROJECT_API ACLPPickup : public AActor
{
	GENERATED_BODY()

public:
	ACLPPickup();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	void PickupWasTaken();
	void Respawn();

protected:
	virtual bool GivePickupTo(APawn* Pawn);

public:
	bool CanBeTaken() const;

private:
	FTimerHandle RespawnTimerHandle;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	float RespawnTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* TakeSound;
};
