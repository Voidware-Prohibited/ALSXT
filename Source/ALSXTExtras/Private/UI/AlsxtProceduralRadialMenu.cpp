// MIT


#include "UI/AlsxtProceduralRadialMenu.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"

void UAlsxtRadialMenuOption::NativeConstruct()
{
    Super::NativeConstruct();
    if (Button)
    {
        Button->OnHovered.AddDynamic(this, &UAlsxtRadialMenuOption::OnHover);
        Button->OnUnhovered.AddDynamic(this, &UAlsxtRadialMenuOption::OnUnhover);
        Button->OnClicked.AddDynamic(this, &UAlsxtRadialMenuOption::OnClick);
    }
}

void UAlsxtRadialMenuOption::OnHover()
{
    SetRenderScale(FVector2D(ZoomScale, ZoomScale));
}

void UAlsxtRadialMenuOption::OnUnhover()
{
    SetRenderScale(FVector2D(1.0f, 1.0f));
}

void UAlsxtRadialMenuOption::OnClick()
{
    UAlsxtProceduralRadialMenu* Menu = Cast<UAlsxtProceduralRadialMenu>(GetParent());
    if (Menu) { /* Handle Clicked Logic */ }
}

void UAlsxtProceduralRadialMenu::BuildMenu()
{
    ClearMenu();
    for (int32 i = 0; i < FMath::Min(Layers.Num(), 4); ++i)
    {
        DrawLayer(i, (i > 0 && Layers[i].bStartFromLastClicked) ? LastClickedAngle : Layers[i].StartAngle);
    }
}

void UAlsxtProceduralRadialMenu::ClearMenu()
{
    if (MainCanvas)
    {
	    MainCanvas->ClearChildren();
    }
}

void UAlsxtProceduralRadialMenu::DrawLayer(int32 LayerIdx, float StartAngleOverride)
{
    const FAlsxtRadialMenuLayerSettings& Settings = Layers[LayerIdx];
    int32 NumOptions = Settings.OptionLabels.Num();
    if (NumOptions == 0) return;

    float AngleStep = 360.f / NumOptions;
    float CurrentAngle = StartAngleOverride;

    for (int32 i = 0; i < NumOptions; ++i)
    {
        UAlsxtRadialMenuOption* Option = WidgetTree->ConstructWidget<UAlsxtRadialMenuOption>();
        // ... (Setup Button/Image styles based on LayerIdx) ...
        
        UCanvasPanelSlot* NewSlot = MainCanvas->AddChildToCanvas(Option);
        float MidAngle = FMath::DegreesToRadians(CurrentAngle + (AngleStep / 2.f));
        float Radius = (Settings.InnerRadius + Settings.OuterRadius) / 2.f;

        NewSlot->SetPosition(FVector2D(FMath::Cos(MidAngle) * Radius, FMath::Sin(MidAngle) * Radius));
        NewSlot->SetSize(FVector2D(Settings.OuterRadius - Settings.InnerRadius, 50.f)); // Arbitrary width
        
        Option->ZoomScale = Settings.HoverZoomMultiplier;
        CurrentAngle += AngleStep;
    }
}