// MIT


#include "Components/Mesh/ALSXTPaintableStaticMeshComponent.h"
#include "Interfaces/ALSXTMeshPaintingInterface.h"
#include "Settings/ALSXTCharacterSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"

UALSXTPaintableStaticMeshComponent::UALSXTPaintableStaticMeshComponent()
{
	// Bind InitializeMaterials to When Materials Change
}

void UALSXTPaintableStaticMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->GetClass()->ImplementsInterface(UALSXTMeshPaintingInterface::StaticClass()))
	{
		if (IALSXTMeshPaintingInterface::Execute_GetSceneCaptureComponent(GetOwner()))
		{
			SceneCaptureComponent = IALSXTMeshPaintingInterface::Execute_GetSceneCaptureComponent(GetOwner());
			if (GetMaterial(0)->IsValidLowLevelFast())
			{
				GlobalGeneralMeshPaintingSettings = IALSXTMeshPaintingInterface::Execute_GetGlobalGeneralMeshPaintingSettings(GetOwner());
				FALSXTServerMeshPaintingSettings ServerGeneralMeshPaintingSettings{ IALSXTMeshPaintingInterface::Execute_GetServerGeneralMeshPaintingSettings(GetOwner()) };
				FALSXTGeneralMeshPaintingSettings UserGeneralMeshPaintingSettings{ IALSXTMeshPaintingInterface::Execute_GetUserGeneralMeshPaintingSettings(GetOwner()) };
				if (GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && UserGeneralMeshPaintingSettings.bEnableMeshPainting)
				{
					// InitializeMaterials();
				}
			}
		}
	}
}

bool UALSXTPaintableStaticMeshComponent::SetStaticMesh(UStaticMesh* NewMesh)
{
	Super::SetStaticMesh(NewMesh);

	if (GetMaterial(0)->IsValidLowLevelFast())
	{
		FALSXTServerMeshPaintingSettings ServerGeneralMeshPaintingSettings{ IALSXTMeshPaintingInterface::Execute_GetServerGeneralMeshPaintingSettings(GetOwner()) };
		FALSXTGeneralMeshPaintingSettings UserGeneralMeshPaintingSettings{ IALSXTMeshPaintingInterface::Execute_GetUserGeneralMeshPaintingSettings(GetOwner()) };
		if (GlobalGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && ServerGeneralMeshPaintingSettings.GeneralSettings.bEnableMeshPainting && UserGeneralMeshPaintingSettings.bEnableMeshPainting)
		{
			// InitializeMaterials();
		}
	}

	return true;
}

void UALSXTPaintableStaticMeshComponent::InitializeMaterials()
{
	FALSXTServerMeshPaintingSettings ServerGeneralMeshPaintingSettings{ IALSXTMeshPaintingInterface::Execute_GetServerGeneralMeshPaintingSettings(GetOwner()) };
	PhysicalMaterialMapTexture = GetMaterial(0)->GetPhysicalMaterialMask()->MaskTexture;
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

void UALSXTPaintableStaticMeshComponent::SetSceneCaptureRenderTarget(UTextureRenderTarget2D* NewRenderTarget)
{
	SceneCaptureComponent->TextureTarget = NewRenderTarget;
}

void UALSXTPaintableStaticMeshComponent::SetPhysicalMaterialMask(UPhysicalMaterialMask* NewPhysicalMaterialMask)
{
	PhysicalMaterialMask = NewPhysicalMaterialMask;
}

UALSXTMeshPaintingSettingsMap* UALSXTPaintableStaticMeshComponent::GetMeshPaintingSettingsMap()
{
	return MeshPaintingSettingsMap;
}

void UALSXTPaintableStaticMeshComponent::SetMeshPaintingSettingsMap(UALSXTMeshPaintingSettingsMap* NewMeshPaintingSettingsMap)
{
	// OnChangeMeshPaintingSettingsMap.Broadcast(MeshPaintingSettingsMap, NewMeshPaintingSettingsMap);
	MeshPaintingSettingsMap = NewMeshPaintingSettingsMap;
}

UALSXTMeshPaintingSettings*& UALSXTPaintableStaticMeshComponent::GetMeshPaintingSettings(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	UALSXTMeshPaintingSettingsMap* FoundMeshPaintingSettingsMap = MeshPaintingSettingsMap;
	TMap<TEnumAsByte<EPhysicalSurface>, UALSXTMeshPaintingSettings*> FoundSettings{ MeshPaintingSettingsMap->SettingsMap };
	UALSXTMeshPaintingSettings*& FoundMeshPaintingSettings { *FoundSettings.Find(SurfaceType) };
	return *&FoundMeshPaintingSettings;
}

void UALSXTPaintableStaticMeshComponent::SetMeshPaintingSettings(UALSXTMeshPaintingSettings* NewMeshPaintingSettings)
{
	// OnChangeMeshPaintingSettings.Broadcast(MeshPaintingSettings, NewMeshPaintingSettings);
	MeshPaintingSettings = NewMeshPaintingSettings;
}

FALSXTMeshPaintCriteria UALSXTPaintableStaticMeshComponent::GetMeshPaintCriteriaEntry(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	TArray<FALSXTMeshPaintCriteria> NewMeshPaintCriteriaArray{ GetMeshPaintingSettings(SurfaceType)->MeshPaintCriteria };
	FALSXTMeshPaintCriteria NewMeshPaintCriteria;
	return NewMeshPaintCriteria;
}

FALSXTMeshPaintCriteria UALSXTPaintableStaticMeshComponent::GetItemMeshPaintCriteriaEntry(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	FALSXTMeshPaintCriteria NewMeshPaintCriteria;
	return NewMeshPaintCriteria;
}

void UALSXTPaintableStaticMeshComponent::SetMeshPaintCriteria(TMap<TEnumAsByte<EPhysicalSurface>, FALSXTMeshPaintCriteria> NewMeshPaintCriteria)
{
	FALSXTMeshPaintCriteriaMap PreviousMap;
	PreviousMap.MeshPaintCriteriaMap = MeshPaintCriteria;
	FALSXTMeshPaintCriteriaMap NewMap;
	NewMap.MeshPaintCriteriaMap = NewMeshPaintCriteria;
	// OnChangeItemMeshPaintCriteria.Broadcast(PreviousMap, NewMap);
	MeshPaintCriteria = NewMeshPaintCriteria;
}

void UALSXTPaintableStaticMeshComponent::SetItemMeshPaintCriteria(TMap<TEnumAsByte<EPhysicalSurface>, FALSXTMeshPaintCriteria> NewMeshPaintCriteria)
{
	FALSXTMeshPaintCriteriaMap PreviousMap;
	PreviousMap.MeshPaintCriteriaMap = ItemMeshPaintCriteria;
	FALSXTMeshPaintCriteriaMap NewMap;
	NewMap.MeshPaintCriteriaMap = NewMeshPaintCriteria;
	// OnChangeItemMeshPaintCriteria.Broadcast(PreviousMap, NewMap);
	ItemMeshPaintCriteria = NewMeshPaintCriteria;
}

FGameplayTag UALSXTPaintableStaticMeshComponent::GetElementalCondition()
{
	return ElementalCondition;
}

void UALSXTPaintableStaticMeshComponent::SetElementalCondition(const FGameplayTag NewElementalCondition)
{
	// OnChangeElementalCondition.Broadcast(ElementalCondition, NewElementalCondition);
	ElementalCondition = NewElementalCondition;
}

TEnumAsByte<EPhysicalSurface> UALSXTPaintableStaticMeshComponent::GetSurfaceAtLocation(FVector Location)
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
bool UALSXTPaintableStaticMeshComponent::CanBePainted(const FGameplayTag PaintType)
{
	// Ideally, User Settings should only apply for Single Player/Offline Play. The Server Browser/Match Making should not allow for Users with a setting disabled to connect to a Server with the same setting Enabled
	FALSXTServerMeshPaintingSettings ServerGeneralMeshPaintingSettings{ IALSXTMeshPaintingInterface::Execute_GetServerGeneralMeshPaintingSettings(GetOwner()) };
	FALSXTGeneralMeshPaintingSettings UserGeneralMeshPaintingSettings{ IALSXTMeshPaintingInterface::Execute_GetUserGeneralMeshPaintingSettings(GetOwner()) };

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
bool UALSXTPaintableStaticMeshComponent::ShouldBePainted(TEnumAsByte<EPhysicalSurface> SurfaceType, TEnumAsByte<EPhysicalSurface> ElementSurfaceType, const FGameplayTag PaintType)
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

void UALSXTPaintableStaticMeshComponent::GetMaterialsForPaintType(const FGameplayTag PaintType, UMaterialInstanceDynamic*& MaterialInstance, UMaterialInstanceDynamic*& FadeMaterialInstance, UTextureRenderTarget2D*& RenderTarget, UTextureRenderTarget2D*& FadeRenderTarget, FName& ParamName)
{
	if (PaintType == ALSXTMeshPaintTypeTags::BloodDamage)
	{
		MaterialInstance = MIDBloodDamage;
		FadeMaterialInstance = MIDBloodDamageFade;
		RenderTarget = BloodDamageRenderTarget;
		FadeRenderTarget = BloodDamageFadeRenderTarget;
		ParamName = "BloodDamage";
	}
	if (PaintType == ALSXTMeshPaintTypeTags::SurfaceDamage)
	{
		MaterialInstance = MIDSurfaceDamage;
		FadeMaterialInstance = MIDSurfaceDamageFade;
		RenderTarget = SurfaceDamageRenderTarget;
		FadeRenderTarget = SurfaceDamageFadeRenderTarget;
		ParamName = "SurfaceDamage";
	}
	if (PaintType == ALSXTMeshPaintTypeTags::BackSpatter)
	{
		MaterialInstance = MIDBackSpatter;
		FadeMaterialInstance = MIDBackSpatterFade;
		RenderTarget = BackSpatterRenderTarget;
		FadeRenderTarget = BackSpatterFadeRenderTarget;
		ParamName = "BackSpatter";
	}
	if (PaintType == ALSXTMeshPaintTypeTags::Saturation)
	{
		MaterialInstance = MIDSaturation;
		FadeMaterialInstance = MIDSaturationFade;
		RenderTarget = SaturationRenderTarget;
		FadeRenderTarget = SaturationFadeRenderTarget;
		ParamName = "Saturation";
	}
	if (PaintType == ALSXTMeshPaintTypeTags::Burn)
	{
		MaterialInstance = MIDBurn;
		FadeMaterialInstance = MIDBurnFade;
		RenderTarget = BurnRenderTarget;
		FadeRenderTarget = BurnFadeRenderTarget;
		ParamName = "Burn";
	}
}

void UALSXTPaintableStaticMeshComponent::PaintMesh(TEnumAsByte<EPhysicalSurface> SurfaceType, const FGameplayTag PaintType, FVector Location, float Radius)
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

void UALSXTPaintableStaticMeshComponent::VolumePaintMesh(TEnumAsByte<EPhysicalSurface> SurfaceType, const FGameplayTag PaintType, FVector Origin, FVector Extent)
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

void UALSXTPaintableStaticMeshComponent::ResetChannel(const FGameplayTag PaintType)
{

}

void UALSXTPaintableStaticMeshComponent::ResetAllChannels()
{

}