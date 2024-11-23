// Crime Life Project, All Rights NOT Reserved.

#include "Menu/UI/CLPMenuHUD.h"

#include "Blueprint/UserWidget.h"

void ACLPMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UUserWidget* MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass))
	{
		MenuWidget->AddToViewport();
	}
}
