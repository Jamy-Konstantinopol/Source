// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Player/CLPCharacter.h"

#include "CLPPlayerCharacter.generated.h"

class UCLPSpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class CRIMELIFEPROJECT_API ACLPPlayerCharacter : public ACLPCharacter
{
	GENERATED_BODY()

public:
	ACLPPlayerCharacter(const FObjectInitializer& InObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnDeath() override;

private:
	void SetInputMappingContext(UInputMappingContext* NewMappingContext);

protected:
	virtual void Move(const FInputActionValue& InActionValue);
	virtual void Look(const FInputActionValue& InActionValue);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCLPSpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputAction* JogAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputAction* NextWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actions")
	UInputAction* ReloadAction;
};
