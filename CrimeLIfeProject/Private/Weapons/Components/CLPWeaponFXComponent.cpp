// Crime Life Project, All Rights NOT Reserved.

#include "Weapons/Components/CLPWeaponFXComponent.h"

#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"

UCLPWeaponFXComponent::UCLPWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCLPWeaponFXComponent::SpawnDecal(FCLPImpactData ImpactData, const FHitResult& InHitResult)
{
	UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactData.DecalData.Material, ImpactData.DecalData.Size,
																			 InHitResult.ImpactPoint, InHitResult.ImpactNormal.Rotation());

	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
	}
}

void UCLPWeaponFXComponent::PlayImpact(const FHitResult& InHitResult)
{
	FCLPImpactData ImpactData = DefaultImpactData;

	if (InHitResult.PhysMaterial.IsValid())
	{
		const UPhysicalMaterial* Key = InHitResult.PhysMaterial.Get();
		if (ImpactDataMap.Contains(Key))
		{
			ImpactData = ImpactDataMap.FindRef(Key);
		}
	}

	UGameplayStatics::PlaySoundAtLocation(this, ImpactData.ImpactSound, InHitResult.ImpactPoint);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactData.NiagaraEffect, InHitResult.ImpactPoint, InHitResult.ImpactNormal.Rotation());
	SpawnDecal(ImpactData, InHitResult);
}
