// Crime Life Project, All Rights NOT Reserved.

#include "UI/CLPPlayerStatRowWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCLPPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if (PlayerText)
	{
		PlayerText->SetText(Text);
	}
}

void UCLPPlayerStatRowWidget::SetKillsName(const FText& Text)
{
	if (KillsText)
	{
		KillsText->SetText(Text);
	}
}

void UCLPPlayerStatRowWidget::SetDeathsName(const FText& Text)
{
	if (DeathsText)
	{
		DeathsText->SetText(Text);
	}
}

void UCLPPlayerStatRowWidget::SetTeamName(const FText& Text)
{
	if (TeamText)
	{
		TeamText->SetText(Text);
	}
}

void UCLPPlayerStatRowWidget::SetPlayerSelfIndicatorImageVisibility(bool bIsVisible)
{
	if (PlayerSelfIndicatorImage)
	{
		PlayerSelfIndicatorImage->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}