// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ALSXTViewModelStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Rendering, Common), hidecategories=(Object,Activation,"Components|Activation"), ShowCategories=(Mobility), editinlinenew, meta=(BlueprintSpawnableComponent))
class ALSXT_API UALSXTViewModelStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	UALSXTViewModelStaticMeshComponent();

protected:
	uint8 MaxOwnerAttempts = 10;
	UPROPERTY(EditDefaultsOnly, Category = "View Model")
	float DesiredFOV = 90.0f;
	
	TWeakObjectPtr<APlayerController> PlayerController;

	virtual void BeginPlay() override;
	virtual FMatrix GetRenderMatrix() const override;
};