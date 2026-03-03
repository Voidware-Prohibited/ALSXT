// MIT

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AlsxtProceduralRadialMenu.generated.h"

USTRUCT(BlueprintType)
struct FAlsxtRadialMenuLayerSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InnerRadius = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OuterRadius = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartAngle = -90.f; // Degrees, clockwise
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStartFromLastClicked = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoverZoomMultiplier = 1.2f; // 100% +
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> OptionLabels;
};

UCLASS()
class UAlsxtRadialMenuOption : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly) UImage* BackgroundImage;
	UPROPERTY(BlueprintReadOnly) UTextBlock* Label;
	UPROPERTY(BlueprintReadOnly) UButton* Button;
	int32 LayerIndex;
	int32 OptionIndex;
	float OriginalScale = 1.f;
	float ZoomScale = 1.2f;

	virtual void NativeConstruct() override;
	UFUNCTION() void OnHover();
	UFUNCTION() void OnUnhover();
	UFUNCTION() void OnClick();
};

/**
 * 
 */
UCLASS()
class ALSXTEXTRAS_API UAlsxtProceduralRadialMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UCanvasPanel* MainCanvas;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UImage* CenterDisplay;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAlsxtRadialMenuLayerSettings> Layers;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CursorStartAngle = -90.f;

	UFUNCTION(BlueprintCallable)
	void BuildMenu();

private:
	void ClearMenu();
	void DrawLayer(int32 LayerIdx, float StartAngleOverride = -1.f);
	float LastClickedAngle = -90.f;
};
