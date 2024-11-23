// Crime Life Project, All Rights NOT Reserved.

#include "AI/CLPAICharacter.h"

#include "AI/CLPAIController.h"
#include "Components/CLPAIWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"

ACLPAICharacter::ACLPAICharacter(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer.SetDefaultSubobjectClass<UCLPAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI	  = EAutoPossessAI::Disabled;
	AIControllerClass = ACLPAIController ::StaticClass();

	bUseControllerRotationYaw = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->RotationRate				  = FRotator(0.f, 200.f, 0.f);
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void ACLPAICharacter::OnDeath()
{
	Super::OnDeath();

	const AAIController* CLPController = Cast<AAIController>(Controller);
	if (CLPController && CLPController->BrainComponent)
	{
		CLPController->BrainComponent->Cleanup();
	}
}
