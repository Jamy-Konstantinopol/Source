// Crime Life Project, All Rights NOT Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "CLPTypes.h"

#include "CLPLevelItemWidget.generated.h"

class UButton;
class UTextBlock;
class UTexture2D;

DECLARE_MULTICAST_DELEGATE_OneParam(OnClickedLevelButtonSignature, const FLevelItemDataWidget&);

UCLASS()
class CRIMELIFEPROJECT_API UCLPLevelItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	void Click();
	void SetButtonEnabled();

	void				 SetLevelItemDataWidget(const FLevelItemDataWidget& NewLevelItemDataWidget);
	FLevelItemDataWidget GetLevelItemDataWidget() const;

public:
	OnClickedLevelButtonSignature OnClickedLevelButton;

private:
	UFUNCTION()
	void ClickedMapButton();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* LevelButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FLevelItemDataWidget LevelItemDataWidget;
};
