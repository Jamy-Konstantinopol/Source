// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CLPPlayerHUDWidget.generated.h"

USTRUCT(BlueprintType)
struct FAimCrosshairImage
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* UnarmedAimPointImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* ArmedAimPointImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* KilledCrosshairImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* DamageCrosshairImage;
};

UCLASS()
class CRIMELIFEPROJECT_API UCLPPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	bool IsDamageSomeone() const;
	bool IsKilledSomeone() const;

	void SetIsDamageSomeone(bool bNewIsDamageSomeone);
	void SetIsKilledSomeone(bool bNewIsKilledSomeone);

	void OnDamageSomeone();
	void OnKilledSomeone();
	void OnNewPawn(APawn* NewPawn);

private:
	FTimerHandle DamageTimerHandle;
	FTimerHandle KillTimerHandle;
	bool		 bIsDamageSomeone = false;
	bool		 bIsKilledSomeone = false;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool HasEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetAmmoCount() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	FString GetEquippedWeaponTotalAmmoCount() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	UTexture2D* GetAimPointImage() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	UTexture2D* GetDamageCrosshairImage() const;

	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(float Health);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FAimCrosshairImage AimCrosshairImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	float EffectDuration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FString InfinitySign = TEXT("Inf");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float DangerHealth = 0.4f;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* DangerHealthAnimation;
};
