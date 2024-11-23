// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "CLPHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, Health);

class UPhysicalMaterial;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRIMELIFEPROJECT_API UCLPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	void AddHealth(float HealthIncrement);
	void ApplyDamage(float Damage, AController* InstigatedBy);
	void ReportDamageEvent(float Damage, AController* InstigatedBy);
	float GetDamageModificator(const FName& BoneName);

public:
	bool TryAddHealth(float HealthIncrement);

	float GetHealth() const;
	float GetMaxHealth() const;

private:
	float		 Health = 0.f;
	FTimerHandle HealTimerHandle;

private:
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent,
						   FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
	void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, const FHitResult& HitInfo,
							class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION() void Heal();

	void Killed(AController* KillerController);

public:
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintCallable)
	bool IsHealthFull() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0", UIMin = "0"))
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	bool bAutoHeal = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "bAutoHeal"))
	float HealFirstDelay = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "bAutoHeal"))
	float HealRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "bAutoHeal"))
	float HealIncrement = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TMap<UPhysicalMaterial*, float> DamageModificatorMap;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeathSignature OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;
};
