// MIT


#include "Components/Mesh/AlsxtPaintableSkeletalMeshComponent.h"
#include "Interfaces/AlsxtMeshPaintingInterface.h"
#include "Settings/AlsxtCharacterSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Utility/AlsLog.h"

UAlsxtPaintableSkeletalMeshComponent::UAlsxtPaintableSkeletalMeshComponent()
{
	// Bind InitializeMaterials to When Materials Change
}

void UAlsxtPaintableSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsMeshPaintingConfigured())
	{
		SceneCaptureComponent = IAlsxtMeshPaintingInterface::Execute_GetSceneCaptureComponent(GetOwner());
		GlobalGeneralMeshPaintingSettings = IAlsxtMeshPaintingInterface::Execute_GetGlobalGeneralMeshPaintingSettings(GetOwner());
		if (IsMeshPaintingEnabled())
		{
			// InitializeMaterials();
			MaterialInstanceDynamic = CreateAndSetMaterialInstanceDynamic(0);

			if (RenderTargetAsset)
			{
				MaterialInstanceDynamic->SetTextureParameterValue("RT_Effects", RenderTargetAsset);
			}
		}
	}
	else
	{
		UE_LOG(LogAls, Error, TEXT("IsMeshPaintingConfigured is false"));
	}
}

void UAlsxtPaintableSkeletalMeshComponent::SetSkeletalMesh(USkeletalMesh* NewMesh, bool bReinitPose)
{
	Super::SetSkeletalMesh(NewMesh, bReinitPose);

	if (GetMaterial(0))
	{
		FAlsxtServerMeshPaintingSettings ServerGeneralMeshPaintingSettings{ IAlsxtMeshPaintingInterface::Execute_GetServerGeneralMeshPaintingSettings(GetOwner()) };
		FAlsxtGeneralMeshPaintingSettings UserGeneralMeshPaintingSettings{ IAlsxtMeshPaintingInterface::Execute_GetUserGeneralMeshPaintingSettings(GetOwner()) };
		if (GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && UserGeneralMeshPaintingSettings.bEnableMeshPainting)
		{
			// InitializeMaterials();
		}
	}
}

bool UAlsxtPaintableSkeletalMeshComponent::IsMeshPaintingConfigured() const
{
	if (!GetSkeletalMeshAsset())
	{
		return false;
	}
	if (!GetMaterial(0))
	{
		return false;
	}
	if (!GetOwner()->GetClass()->ImplementsInterface(UAlsxtMeshPaintingInterface::StaticClass()))
	{
		return false;
	}
	if (!IAlsxtMeshPaintingInterface::Execute_GetSceneCaptureComponent(GetOwner()))
	{
		return false;
	}
	return true;
}

bool UAlsxtPaintableSkeletalMeshComponent::IsMeshPaintingEnabled() const
{
	FAlsxtServerMeshPaintingSettings ServerGeneralMeshPaintingSettings{ IAlsxtMeshPaintingInterface::Execute_GetServerGeneralMeshPaintingSettings(GetOwner()) };
	FAlsxtGeneralMeshPaintingSettings UserGeneralMeshPaintingSettings{ IAlsxtMeshPaintingInterface::Execute_GetUserGeneralMeshPaintingSettings(GetOwner()) };
	return GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && UserGeneralMeshPaintingSettings.bEnableMeshPainting;
}

void UAlsxtPaintableSkeletalMeshComponent::InitializeMaterials()
{
	FAlsxtServerMeshPaintingSettings ServerGeneralMeshPaintingSettings{ IAlsxtMeshPaintingInterface::Execute_GetServerGeneralMeshPaintingSettings(GetOwner()) };

	// if (GetMaterial(0)->GetPhysicalMaterialMask()->MaskTexture)
	// {
	// 	PhysicalMaterialMapTexture = GetMaterial(0)->GetPhysicalMaterialMask()->MaskTexture;
	// }
	
	PhysicalMaterialMask = GetMaterial(0)->GetPhysicalMaterialMask();

	for (int i = 0; i < 8; i++)
	{
		if (GetMaterial(0)->GetPhysicalMaterialFromMap(i)->IsValidLowLevelFast())
		{
			PhysicalMaterialMap.Add(GetMaterial(0)->GetPhysicalMaterialFromMap(i));
		}		
	}
	
	MIDOriginal = CreateDynamicMaterialInstance(0, GetMaterial(0), "Original");
	UnwrapRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, RTF_R16f, FLinearColor::Black, false, false);
	SetSceneCaptureRenderTarget(UnwrapRenderTarget);
	
	if (CanBePainted(ALSXTMeshPaintTypeTags::BloodDamage))
	{
		MIDBloodDamage = CreateDynamicMaterialInstance(0, GetMaterial(0), "BloodDamage");
		BloodDamageRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, RTF_R16f, FLinearColor::Black, false, false);
		MIDOriginal->SetTextureParameterValue("BloodDamage", BloodDamageRenderTarget);
		MIDBloodDamage->SetTextureParameterValue("Unwrap", UnwrapRenderTarget);

		if (ServerGeneralMeshPaintingSettings.bEnableFadeOutBloodDamage)
		{
			MIDBloodDamageFade = CreateDynamicMaterialInstance(0, GetMaterial(0), "BloodDamageFade");
			BloodDamageFadeRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, RTF_R16f, FLinearColor::Black, false, false);
			MIDBloodDamage->SetTextureParameterValue("Fade", BloodDamageFadeRenderTarget);
		}
	}
	if (CanBePainted(ALSXTMeshPaintTypeTags::SurfaceDamage))
	{
		MIDSurfaceDamage = CreateDynamicMaterialInstance(0, GetMaterial(0), "SurfaceDamage");
		SurfaceDamageRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, RTF_R16f, FLinearColor::Black, false, false);
		MIDOriginal->SetTextureParameterValue("SurfaceDamage", SurfaceDamageRenderTarget);
		MIDSurfaceDamage->SetTextureParameterValue("Unwrap", UnwrapRenderTarget);

		if (ServerGeneralMeshPaintingSettings.bEnableFadeOutSurfaceDamage)
		{
			MIDSurfaceDamageFade = CreateDynamicMaterialInstance(0, GetMaterial(0), "SurfaceDamageFade");
			SurfaceDamageFadeRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, RTF_R16f, FLinearColor::Black, false, false);
			MIDSurfaceDamage->SetTextureParameterValue("Fade", SurfaceDamageFadeRenderTarget);
		}
	}
	if (CanBePainted(ALSXTMeshPaintTypeTags::BackSpatter))
	{
		MIDBackSpatter = CreateDynamicMaterialInstance(0, GetMaterial(0), "BackSpatter");
		BackSpatterRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, RTF_R16f, FLinearColor::Black, false, false);
		MIDOriginal->SetTextureParameterValue("BackSpatter", BackSpatterRenderTarget);
		MIDBackSpatter->SetTextureParameterValue("Unwrap", UnwrapRenderTarget);

		if (ServerGeneralMeshPaintingSettings.bEnableFadeOutBackspatter)
		{
			MIDBackSpatterFade = CreateDynamicMaterialInstance(0, GetMaterial(0), "BackSpatterFade");
			BackSpatterFadeRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, RTF_R16f, FLinearColor::Black, false, false);
			MIDBackSpatter->SetTextureParameterValue("Fade", BackSpatterFadeRenderTarget);
		}
	}
	if (CanBePainted(ALSXTMeshPaintTypeTags::Saturation))
	{
		MIDSaturation = CreateDynamicMaterialInstance(0, GetMaterial(0), "Saturation");
		SaturationRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, RTF_R16f, FLinearColor::Black, false, false);
		MIDOriginal->SetTextureParameterValue("Saturation", SaturationRenderTarget);
		MIDSaturation->SetTextureParameterValue("Unwrap", UnwrapRenderTarget);

		if (ServerGeneralMeshPaintingSettings.bEnableFadeOutSaturation)
		{
			MIDSaturationFade = CreateDynamicMaterialInstance(0, GetMaterial(0), "SaturationFade");
			SaturationFadeRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, RTF_R16f, FLinearColor::Black, false, false);
			MIDSaturation->SetTextureParameterValue("Fade", SaturationFadeRenderTarget);
		}
	}
	if (CanBePainted(ALSXTMeshPaintTypeTags::Burn))
	{
		MIDBurn = CreateDynamicMaterialInstance(0, GetMaterial(0), "Burn");
		BurnRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, RTF_R16f, FLinearColor::Black, false, false);
		MIDOriginal->SetTextureParameterValue("Burn", BurnRenderTarget);
		MIDBurn->SetTextureParameterValue("Unwrap", UnwrapRenderTarget);

		if (ServerGeneralMeshPaintingSettings.bEnableFadeOutBurnDamage)
		{
			MIDBurnFade = CreateDynamicMaterialInstance(0, GetMaterial(0), "BurnFade");
			BurnFadeRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, RTF_R16f, FLinearColor::Black, false, false);
			MIDBurn->SetTextureParameterValue("Fade", BurnFadeRenderTarget);
		}
	}
}

void UAlsxtPaintableSkeletalMeshComponent::SetSceneCaptureRenderTarget(UTextureRenderTarget2D* NewRenderTarget)
{
	SceneCaptureComponent->TextureTarget = NewRenderTarget;
}

void UAlsxtPaintableSkeletalMeshComponent::SetPhysicalMaterialMask(UPhysicalMaterialMask* NewPhysicalMaterialMask)
{
	PhysicalMaterialMask = NewPhysicalMaterialMask;
}

UAlsxtMeshPaintingSettingsMap* UAlsxtPaintableSkeletalMeshComponent::GetMeshPaintingSettingsMap() const
{
	return MeshPaintingSettingsMap;
}

void UAlsxtPaintableSkeletalMeshComponent::SetMeshPaintingSettingsMap(UAlsxtMeshPaintingSettingsMap* NewMeshPaintingSettingsMap)
{
	OnChangeMeshPaintingSettingsMap.Broadcast(MeshPaintingSettingsMap, NewMeshPaintingSettingsMap);
	MeshPaintingSettingsMap = NewMeshPaintingSettingsMap;
}

UAlsxtMeshPaintingSettings*& UAlsxtPaintableSkeletalMeshComponent::GetMeshPaintingSettings(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	UAlsxtMeshPaintingSettingsMap* FoundMeshPaintingSettingsMap = MeshPaintingSettingsMap;
	TMap<TEnumAsByte<EPhysicalSurface>, UAlsxtMeshPaintingSettings*> FoundSettings{ MeshPaintingSettingsMap->SettingsMap };
	UAlsxtMeshPaintingSettings*& FoundMeshPaintingSettings { *FoundSettings.Find(SurfaceType) };
	return *&FoundMeshPaintingSettings;
}

void UAlsxtPaintableSkeletalMeshComponent::SetMeshPaintingSettings(UAlsxtMeshPaintingSettings* NewMeshPaintingSettings)
{
	OnChangeMeshPaintingSettings.Broadcast(MeshPaintingSettings, NewMeshPaintingSettings);
	MeshPaintingSettings = NewMeshPaintingSettings;
}

FALSXTMeshPaintCriteria UAlsxtPaintableSkeletalMeshComponent::GetMeshPaintCriteriaEntry(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	TArray<FALSXTMeshPaintCriteria> NewMeshPaintCriteriaArray{ GetMeshPaintingSettings(SurfaceType)->MeshPaintCriteria };
	FALSXTMeshPaintCriteria NewMeshPaintCriteria;
	return NewMeshPaintCriteria;
}

FALSXTMeshPaintCriteria UAlsxtPaintableSkeletalMeshComponent::GetItemMeshPaintCriteriaEntry(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	FALSXTMeshPaintCriteria NewMeshPaintCriteria;
	return NewMeshPaintCriteria;
}

void UAlsxtPaintableSkeletalMeshComponent::SetMeshPaintCriteria(TMap<TEnumAsByte<EPhysicalSurface>, FALSXTMeshPaintCriteria> NewMeshPaintCriteria)
{
	FALSXTMeshPaintCriteriaMap PreviousMap;
	PreviousMap.MeshPaintCriteriaMap = MeshPaintCriteria;
	FALSXTMeshPaintCriteriaMap NewMap;
	NewMap.MeshPaintCriteriaMap = NewMeshPaintCriteria;
	OnChangeItemMeshPaintCriteria.Broadcast(PreviousMap, NewMap);
	MeshPaintCriteria = NewMeshPaintCriteria;
}

void UAlsxtPaintableSkeletalMeshComponent::SetItemMeshPaintCriteria(TMap<TEnumAsByte<EPhysicalSurface>, FALSXTMeshPaintCriteria> NewMeshPaintCriteria)
{
	FALSXTMeshPaintCriteriaMap PreviousMap;
	PreviousMap.MeshPaintCriteriaMap = ItemMeshPaintCriteria;
	FALSXTMeshPaintCriteriaMap NewMap;
	NewMap.MeshPaintCriteriaMap = NewMeshPaintCriteria;
	OnChangeItemMeshPaintCriteria.Broadcast(PreviousMap, NewMap);
	ItemMeshPaintCriteria = NewMeshPaintCriteria;
}

FGameplayTag UAlsxtPaintableSkeletalMeshComponent::GetElementalCondition()
{
	return ElementalCondition;
}

void UAlsxtPaintableSkeletalMeshComponent::SetElementalCondition(const FGameplayTag NewElementalCondition)
{
	OnChangeElementalCondition.Broadcast(ElementalCondition, NewElementalCondition);
	ElementalCondition = NewElementalCondition;
}

TEnumAsByte<EPhysicalSurface> UAlsxtPaintableSkeletalMeshComponent::GetSurfaceAtLocation(FVector Location)
{
	TEnumAsByte<EPhysicalSurface> NewSurface;

	TArray<FHitResult> OutHits;
	TArray<AActor*> IgnoredActors;
	// IgnoredActors.Add(Character);	// Add Self to Initial Trace Ignored Actors
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	// TraceObjectTypes = ;
	bool isHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Location, Location, 2, TraceObjectTypes, false, IgnoredActors, EDrawDebugTrace::None, OutHits, true, FLinearColor::Green, FLinearColor::Red, 0.0f);

	if (isHit)
	{
		for (auto& Hit : OutHits)
		{
			if (Hit.GetComponent() == this)
			{				
				NewSurface = Hit.PhysMaterial->SurfaceType;
				return NewSurface;
			}
		}
	}

	return NewSurface;
}

// Check if a Paint Type is Enabled Globally (ALSXT Character Settings), on Server (Delegate Implementable), and in User Preferences (Delegate Implementable). Global, Server and User must be True to return True. Default: All True
bool UAlsxtPaintableSkeletalMeshComponent::CanBePainted(const FGameplayTag PaintType)
{
	// Ideally, User Settings should only apply for Single Player/Offline Play. The Server Browser/Match Making should not allow for Users with a setting disabled to connect to a Server with the same setting Enabled
	FAlsxtServerMeshPaintingSettings ServerGeneralMeshPaintingSettings{ IAlsxtMeshPaintingInterface::Execute_GetServerGeneralMeshPaintingSettings(GetOwner()) };
	FAlsxtGeneralMeshPaintingSettings UserGeneralMeshPaintingSettings{ IAlsxtMeshPaintingInterface::Execute_GetUserGeneralMeshPaintingSettings(GetOwner()) };

	if (PaintType == ALSXTMeshPaintTypeTags::BloodDamage)
	{
		return GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && UserGeneralMeshPaintingSettings.bEnableMeshPainting && GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableBloodDamage && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableBloodDamage && UserGeneralMeshPaintingSettings.bEnableBloodDamage;
	}
	if (PaintType == ALSXTMeshPaintTypeTags::BackSpatter)
	{
		return GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && UserGeneralMeshPaintingSettings.bEnableMeshPainting && GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableBackspatter && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableBackspatter && UserGeneralMeshPaintingSettings.bEnableBackspatter;
	}
	if (PaintType == ALSXTMeshPaintTypeTags::Burn)
	{
		return GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && UserGeneralMeshPaintingSettings.bEnableMeshPainting && GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableBurnDamage && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableBurnDamage && UserGeneralMeshPaintingSettings.bEnableBurnDamage;
	}
	if (PaintType == ALSXTMeshPaintTypeTags::SurfaceDamage)
	{
		return GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && UserGeneralMeshPaintingSettings.bEnableMeshPainting && GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableSurfaceDamage && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableSurfaceDamage && UserGeneralMeshPaintingSettings.bEnableSurfaceDamage;
	}
	if (PaintType == ALSXTMeshPaintTypeTags::Saturation)
	{
		return GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && UserGeneralMeshPaintingSettings.bEnableMeshPainting && GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableSaturation && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableSaturation && UserGeneralMeshPaintingSettings.bEnableSaturation;
	}
	else
	{
		return false;
	}
}

// Check if a Surface Type can be painted by Paint Type of Element Surface Type. Performs a search for the provided Surface in the current Criteria Map, and Item Mesh Criteria (if it is set)
bool UAlsxtPaintableSkeletalMeshComponent::ShouldBePainted(TEnumAsByte<EPhysicalSurface> SurfaceType, TEnumAsByte<EPhysicalSurface> ElementSurfaceType, const FGameplayTag PaintType)
{
	if (!ItemMeshPaintCriteria.IsEmpty())
	{
		FALSXTMeshPaintCriteria NewGeneralMeshPaintCriteria{ GetMeshPaintCriteriaEntry(SurfaceType) };
		FALSXTMeshPaintCriteria NewItemMeshPaintCriteria{ GetItemMeshPaintCriteriaEntry(SurfaceType) };
		if (!NewItemMeshPaintCriteria.Surfaces.IsEmpty())
		{
			bool GeneralCriteria = (NewGeneralMeshPaintCriteria.Surfaces.Contains(ElementSurfaceType)) &&  (NewGeneralMeshPaintCriteria.Conditions.HasTag(ElementalCondition)) && (NewGeneralMeshPaintCriteria.PaintType == PaintType);
			bool ItemCriteria = (NewGeneralMeshPaintCriteria.Surfaces.Contains(ElementSurfaceType)) && (NewItemMeshPaintCriteria.Conditions.HasTag(ElementalCondition)) && (NewItemMeshPaintCriteria.PaintType == PaintType);
			if (GeneralCriteria)
			{
				return ItemCriteria;
			}
			else
			{
				return (NewGeneralMeshPaintCriteria.Conditions.HasTag(ElementalCondition)) && (NewGeneralMeshPaintCriteria.PaintType == PaintType);
			}
		}
		else
		{
			return (NewGeneralMeshPaintCriteria.Conditions.HasTag(ElementalCondition)) && (NewGeneralMeshPaintCriteria.PaintType == PaintType);
		}
	}
	else
	{
		FALSXTMeshPaintCriteria NewGeneralMeshPaintCriteria{ GetMeshPaintCriteriaEntry(SurfaceType) };
		return (NewGeneralMeshPaintCriteria.Conditions.HasTag(ElementalCondition)) && (NewGeneralMeshPaintCriteria.PaintType == PaintType);
	}
}

void UAlsxtPaintableSkeletalMeshComponent::GetMaterialsForPaintType(const FGameplayTag PaintType, UMaterialInstanceDynamic*& MaterialInstance, UMaterialInstanceDynamic*& FadeMaterialInstance, UTextureRenderTarget2D*& CurrentRenderTarget, UTextureRenderTarget2D*& FadeRenderTarget, FName& ParamName)
{
	if (PaintType == ALSXTMeshPaintTypeTags::BloodDamage)
	{
		MaterialInstance = MIDBloodDamage;
		FadeMaterialInstance = MIDBloodDamageFade;
		RenderTargetAsset = BloodDamageRenderTarget;
		FadeRenderTarget = BloodDamageFadeRenderTarget;
		ParamName = "BloodDamage";
	}
	if (PaintType == ALSXTMeshPaintTypeTags::SurfaceDamage)
	{
		MaterialInstance = MIDSurfaceDamage;
		FadeMaterialInstance = MIDSurfaceDamageFade;
		RenderTargetAsset = SurfaceDamageRenderTarget;
		FadeRenderTarget = SurfaceDamageFadeRenderTarget;
		ParamName = "SurfaceDamage";
	}
	if (PaintType == ALSXTMeshPaintTypeTags::BackSpatter)
	{
		MaterialInstance = MIDBackSpatter;
		FadeMaterialInstance = MIDBackSpatterFade;
		RenderTargetAsset = BackSpatterRenderTarget;
		FadeRenderTarget = BackSpatterFadeRenderTarget;
		ParamName = "BackSpatter";
	}
	if (PaintType == ALSXTMeshPaintTypeTags::Saturation)
	{
		MaterialInstance = MIDSaturation;
		FadeMaterialInstance = MIDSaturationFade;
		RenderTargetAsset = SaturationRenderTarget;
		FadeRenderTarget = SaturationFadeRenderTarget;
		ParamName = "Saturation";
	}
	if (PaintType == ALSXTMeshPaintTypeTags::Burn)
	{
		MaterialInstance = MIDBurn;
		FadeMaterialInstance = MIDBurnFade;
		RenderTargetAsset = BurnRenderTarget;
		FadeRenderTarget = BurnFadeRenderTarget;
		ParamName = "Burn";
	}
}

void UAlsxtPaintableSkeletalMeshComponent::PaintMesh(TEnumAsByte<EPhysicalSurface> SurfaceType, const FGameplayTag PaintType, FVector Location, float Radius)
{
	if (CanBePainted(PaintType))
	{
		TEnumAsByte<EPhysicalSurface> FoundSurfaceType = GetSurfaceAtLocation(Location);

		if (ShouldBePainted(FoundSurfaceType, SurfaceType, PaintType))
		{
			FALSXTMeshPaintCriteria NewMeshPaintCriteria{ GetMeshPaintCriteriaEntry(SurfaceType) };
			UMaterialInstanceDynamic* MI;
			UMaterialInstanceDynamic* FMI;
			UTextureRenderTarget2D* RT;
			UTextureRenderTarget2D* FRT;
			FName ParamName;
			GetMaterialsForPaintType(PaintType, MI, FMI, RT, FRT, ParamName);
			MI->SetScalarParameterValue("Radius", Radius);
			MI->SetVectorParameterValue(ParamName, Location);
			SceneCaptureComponent->ShowOnlyComponent(this);
			SceneCaptureComponent->CaptureScene();
			UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), RT, MI);
		}
	}
}

void UAlsxtPaintableSkeletalMeshComponent::VolumePaintMesh(TEnumAsByte<EPhysicalSurface> SurfaceType, const FGameplayTag PaintType, FVector Origin, FVector Extent)
{
	if (CanBePainted(PaintType))
	{
		TEnumAsByte<EPhysicalSurface> FoundSurfaceType = GetSurfaceAtLocation(Origin);

		if (ShouldBePainted(FoundSurfaceType, SurfaceType, PaintType))
		{
			FALSXTMeshPaintCriteria NewMeshPaintCriteria{ GetMeshPaintCriteriaEntry(SurfaceType) };
			// UALSXTMeshPaintingSettings*& MeshPaintingSettings{ GetMeshPaintingSettings(SurfaceType) };
		}
	}
}

void UAlsxtPaintableSkeletalMeshComponent::ResetChannel(const FGameplayTag PaintType)
{

}

void UAlsxtPaintableSkeletalMeshComponent::ResetAllChannels()
{

}