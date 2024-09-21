#pragma once

#include "GameplayTagContainer.h"
#include "Runtime/PhysicsCore/Public/Chaos/ChaosEngineInterface.h"
#include "Settings/ALSXTMeshPaintingSettings.h"
#include "ALSXTMeshPaintingInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UALSXTMeshPaintingInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTMeshPaintingInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	FALSXTGlobalGeneralMeshPaintingSettings GetGlobalGeneralMeshPaintingSettings() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mesh Painting Interface")
	FALSXTServerMeshPaintingSettings GetServerGeneralMeshPaintingSettings() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mesh Painting Interface")
	FALSXTGeneralMeshPaintingSettings GetUserGeneralMeshPaintingSettings() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	USceneCaptureComponent2D* GetSceneCaptureComponent() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void GetElementalCondition(UPrimitiveComponent* Component, UPARAM(meta = (Categories = "Als.Elemental Condition"))FGameplayTag& ElementalCondition) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface", Meta = (ForceAsFunction))
	void PaintMesh(UPrimitiveComponent* Component, EPhysicalSurface SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Location, float Size);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface", Meta = (ForceAsFunction))
	void VolumePaintMesh(UPrimitiveComponent* Component, EPhysicalSurface SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Origin, FVector Extent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void ResetPaintTypeOnComponent(UPrimitiveComponent* Component, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void ResetPaintOnComponent(UPrimitiveComponent* Component);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void ResetPaintTypeOnAllComponents(UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void ResetPaintOnAllComponents() ;
};