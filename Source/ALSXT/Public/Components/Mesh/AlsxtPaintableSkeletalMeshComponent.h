// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/Mesh/AlsxtViewModelSkeletalMeshComponent.h"
#include "NativeGameplayTags.h"
#include "Utility/AlsxtGameplayTags.h"
#include "Utility/AlsxtStructs.h"
#include "Settings/AlsxtCharacterSettings.h"
#include "Settings/AlsxtMeshPaintingSettings.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PhysicalMaterials/PhysicalMaterialMask.h"
#include "UObject/Script.h"
#include "Engine/TextureRenderTarget2D.h"
#include "AlsxtPaintableSkeletalMeshComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeSkeletalMeshMaterialSignature, UMaterialInterface*, PreviousMaterial, UMaterialInterface*, NewMaterial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeSkeletalMeshPaintingSettingsMapSignature, UAlsxtMeshPaintingSettingsMap*, PreviousSettingsMap, UAlsxtMeshPaintingSettingsMap*, NewSettingsMap);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeSkeletalMeshPaintingSettingsSignature, UAlsxtMeshPaintingSettings*, PreviousSettings, UAlsxtMeshPaintingSettings*, NewSettings);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeSkeletalMeshPaintCriteriaSignature, FALSXTMeshPaintCriteriaMap, PreviousItemCriteria, FALSXTMeshPaintCriteriaMap, NewItemCriteria);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeItemSkeletalMeshPaintCriteriaSignature, FALSXTMeshPaintCriteriaMap, PreviousItemCriteria, FALSXTMeshPaintCriteriaMap, NewItemCriteria);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeSkeletalMeshElementalConditionSignature, const FGameplayTag, PreviousCondition, const FGameplayTag, NewCondition);

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Rendering, Common), hidecategories=Object, config=Engine, editinlinenew, meta =(BlueprintSpawnableComponent))
class ALSXT_API UAlsxtPaintableSkeletalMeshComponent : public UAlsxtViewModelSkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UAlsxtPaintableSkeletalMeshComponent();

private:
	UPROPERTY(EditAnywhere, Setter = SetMeshPaintingSettingsMap, BlueprintSetter = SetMeshPaintingSettingsMap, Getter = GetMeshPaintingSettingsMap, BlueprintGetter = GetMeshPaintingSettingsMap, Category = "Settings")
	UAlsxtMeshPaintingSettingsMap* MeshPaintingSettingsMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess), Category = "Viewport Mask")
	UMaterialInterface* ViewportMask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess), Category = "Viewport Mask")
	FSlateColor ViewportMaskColor;

public:
	UPROPERTY(BlueprintAssignable)
	FOnChangeSkeletalMeshMaterialSignature OnChangeMeshMaterial;

	UPROPERTY(BlueprintAssignable)
	FOnChangeSkeletalMeshPaintingSettingsMapSignature OnChangeMeshPaintingSettingsMap;
	
	UPROPERTY(BlueprintAssignable)
	FOnChangeSkeletalMeshPaintingSettingsSignature OnChangeMeshPaintingSettings;

	UPROPERTY(BlueprintAssignable)
	FOnChangeSkeletalMeshPaintCriteriaSignature OnChangeMeshPaintCriteria;

	UPROPERTY(BlueprintAssignable)
	FOnChangeItemSkeletalMeshPaintCriteriaSignature OnChangeItemMeshPaintCriteria;

	UPROPERTY(BlueprintAssignable)
	FOnChangeSkeletalMeshElementalConditionSignature OnChangeElementalCondition;

	TSoftObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* RenderTargetAsset;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FAlsxtGlobalGeneralMeshPaintingSettings GlobalGeneralMeshPaintingSettings;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	FAlsxtGeneralMeshPaintingSettings ComponentGeneralMeshPaintingSettings;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UAlsxtMeshPaintingSettings* MeshPaintingSettings;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTMeshPaintCriteria> MeshPaintCriteria;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTMeshPaintCriteria> ItemMeshPaintCriteria;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UTexture> PhysicalMaterialMapTexture;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UPhysicalMaterialMask* PhysicalMaterialMask;

	UPROPERTY(BlueprintReadOnly, Category = Mesh, Meta = (AllowPrivateAccess))
	TArray<UPhysicalMaterial*> PhysicalMaterialMap;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (Categories = "Als.Elemental Condition", AllowPrivateAccess))
	FGameplayTag ElementalCondition{ ALSXTElementalConditionTags::Dry };

	// Dynamic Material Instances

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MaterialInstanceDynamic;
	
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MIDOriginal;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MIDBloodDamage;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MIDBloodDamageFade;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MIDSurfaceDamage;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MIDSurfaceDamageFade;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MIDBackSpatter;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MIDBackSpatterFade;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MIDSaturation;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MIDSaturationFade;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MIDBurn;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UMaterialInstanceDynamic* MIDBurnFade;

	// Render Targets
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* UnwrapRenderTarget;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* BloodDamageRenderTarget;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* BloodDamageFadeRenderTarget;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* SurfaceDamageRenderTarget;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* SurfaceDamageFadeRenderTarget;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* BackSpatterRenderTarget;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* BackSpatterFadeRenderTarget;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* SaturationRenderTarget;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* SaturationFadeRenderTarget;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* BurnRenderTarget;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* BurnFadeRenderTarget;

	// Init
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void InitializeMaterials();

	void SetSkeletalMesh(USkeletalMesh* NewMesh, bool bReinitPose) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings")
	bool IsMeshPaintingConfigured() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings")
	bool IsMeshPaintingEnabled() const;

	// Scene Capture
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetSceneCaptureRenderTarget(UTextureRenderTarget2D* NewRenderTarget);

	// Settings Map
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings")
	UAlsxtMeshPaintingSettingsMap* GetMeshPaintingSettingsMap() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetMeshPaintingSettingsMap(UAlsxtMeshPaintingSettingsMap* NewMeshPaintingSettingsMap);

	// Settings
	UFUNCTION(BlueprintCallable, Category = "Settings")
	UAlsxtMeshPaintingSettings*& GetMeshPaintingSettings(TEnumAsByte<EPhysicalSurface> SurfaceType);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetMeshPaintingSettings(UAlsxtMeshPaintingSettings* NewMeshPaintingSettings);

	// Criteria
	UFUNCTION(BlueprintCallable, Category = "Settings")
	FALSXTMeshPaintCriteria GetMeshPaintCriteriaEntry(TEnumAsByte<EPhysicalSurface> SurfaceType);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	FALSXTMeshPaintCriteria GetItemMeshPaintCriteriaEntry(TEnumAsByte<EPhysicalSurface> SurfaceType);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetMeshPaintCriteria(TMap<TEnumAsByte<EPhysicalSurface>, FALSXTMeshPaintCriteria> NewMeshPaintCriteria);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetItemMeshPaintCriteria(TMap<TEnumAsByte<EPhysicalSurface>, FALSXTMeshPaintCriteria> NewMeshPaintCriteria);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetPhysicalMaterialMask(UPhysicalMaterialMask* NewPhysicalMaterialMask);

	// Elemental Condition
	UFUNCTION(BlueprintCallable, Category = "Settings")
	FGameplayTag GetElementalCondition();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetElementalCondition(UPARAM(meta = (Categories = "Als.Elemental Condition"))const FGameplayTag NewElementalCondition);

	// Return Physical Surface at Location
	UFUNCTION(BlueprintCallable, Category = "Settings")
	TEnumAsByte<EPhysicalSurface> GetSurfaceAtLocation(FVector Location);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	bool CanBePainted(UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	bool ShouldBePainted(TEnumAsByte<EPhysicalSurface> SurfaceType, TEnumAsByte<EPhysicalSurface> ElementSurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void GetMaterialsForPaintType(UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, UMaterialInstanceDynamic*& MaterialInstance, UMaterialInstanceDynamic*& FadeMaterialInstance, UTextureRenderTarget2D*& CurrentRenderTarget, UTextureRenderTarget2D*& FadeRenderTarget, FName& ParamName);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void PaintMesh(TEnumAsByte<EPhysicalSurface> SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Location, float Radius);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void VolumePaintMesh(TEnumAsByte<EPhysicalSurface> SurfaceType, UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType, FVector Origin, FVector Extent);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ResetChannel(UPARAM(meta = (Categories = "Als.Mesh Paint Type"))const FGameplayTag PaintType);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ResetAllChannels();

protected:
	virtual void BeginPlay() override;
	
};