#pragma once

#include "NativeGameplayTags.h"
#include "Runtime/PhysicsCore/Public/Chaos/ChaosEngineInterface.h"
#include "ALSXTMeshPaintingInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UALSXTMeshPaintingInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTMeshPaintingInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	USceneCaptureComponent2D* GetSceneCaptureComponent() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void GetElementalCondition(USceneComponent* Component, UPARAM(meta = (Categories = "Als.Elemental Condition"))FGameplayTag& ElementalCondition) const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void PaintMesh(USceneComponent* Component, EPhysicalSurface SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Location, float Radius) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void VolumePaintMesh(USceneComponent* Component, EPhysicalSurface SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Origin, FVector Extent) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void ResetPaintTypeOnComponent(USceneComponent* Component, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void ResetPaintOnComponent(USceneComponent* Component) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void ResetPaintTypeOnAllComponents(UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Mesh Painting Interface")
	void ResetPaintOnAllComponents() const;
};