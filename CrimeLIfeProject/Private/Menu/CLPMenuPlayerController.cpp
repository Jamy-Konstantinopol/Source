// Crime Life Project, All Rights NOT Reserved.

#include "Menu/CLPMenuPlayerController.h"

void ACLPMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}
