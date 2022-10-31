// MIT

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "Utility/AlsGameplayTags.h"
#include "OverlayModeMenu.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UOverlayModeMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess))
	FGameplayTag SelectedOverlayModeTag{AlsOverlayModeTags::Default};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess))
	TArray< FGameplayTag > OverlayModeTags;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (DisplayName = "Cycle Overlay Mode"))
	void CycleOverlayMode(bool SelectNext);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (DisplayName = "Approve Overlay Mode"))
	void ApproveOverlayMode();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (DisplayName = "On Selected Overlay Mode Changed"))
	void OnSelectedOverlayModeChanged();
	
};
