// Crime Life Project, All Rights NOT Reserved.

#include "Menu/UI/CLPLevelItemWidget.h"

#include "CLPTypes.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCLPLevelItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (LevelButton)
	{
		LevelButton->OnClicked.AddDynamic(this, &ThisClass::ClickedMapButton);
	}
}

void UCLPLevelItemWidget::Click()
{
	LevelButton->OnClicked.Broadcast();
}

void UCLPLevelItemWidget::SetButtonEnabled()
{
	LevelButton->SetIsEnabled(true);
}

void UCLPLevelItemWidget::SetLevelItemDataWidget(const FLevelItemDataWidget& NewLevelItemDataWidget)
{
	LevelText->SetText(NewLevelItemDataWidget.LevelDisplayName);
	LevelItemDataWidget = NewLevelItemDataWidget;
}

FLevelItemDataWidget UCLPLevelItemWidget::GetLevelItemDataWidget() const
{
	return LevelItemDataWidget;
}

void UCLPLevelItemWidget::ClickedMapButton()
{
	LevelButton->SetIsEnabled(false);
	OnClickedLevelButton.Broadcast(LevelItemDataWidget);
}
