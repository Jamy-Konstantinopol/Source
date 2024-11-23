// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "CLPWeaponFXComponent.generated.h"

class UNiagaraSystem;
class UPhysicalMaterial;
class USoundCue;

USTRUCT(BlueprintType)
struct FCLPDecalData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UMaterialInterface* Material;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FVector Size = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	float LifeTime = 15.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	float FadeOutTime = 10.f;
};

USTRUCT(BlueprintType)
struct FCLPImpactData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FCLPDecalData DecalData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* ImpactSound;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRIMELIFEPROJECT_API UCLPWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCLPWeaponFXComponent();

private:
	void SpawnDecal(FCLPImpactData ImpactData, const FHitResult& InHitResult);

public:
	void PlayImpact(const FHitResult& InHitResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FCLPImpactData DefaultImpactData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	TMap<UPhysicalMaterial*, FCLPImpactData> ImpactDataMap;
};
