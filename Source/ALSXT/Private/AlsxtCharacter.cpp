#include "AlsxtCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AlsxtAnimationInstance.h"

#include "AlsCharacter.h"
#include "AlsCharacterMovementComponent.h"
#include "AlsCameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Components/SceneComponent.h"
#include "Components/Character/AlsxtProceduralRecoilAnimComponent.h"
#include "Components/Character/AlsxtTargetingComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Settings/AlsxtCharacterSettings.h"
#include "Settings/AlsxtLocomotionActionSettings.h"
#include "Settings/AlsxtVaultingSettings.h"
#include "Settings/AlsxtCombatSettings.h"
#include "Interfaces/AlsxtCollisionInterface.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Interfaces/AlsxtHeldItemInterface.h"
#include "Curves/CurveVector.h"
#include "RootMotionSources/AlsxtRootMotionSource_Vaulting.h"
#include "Utility/AlsConstants.h"
#include "Utility/AlsMacros.h"
#include "Utility/AlsUtility.h"
#include "Utility/AlsVector.h"
#include "Utility/AlsxtGameplayTags.h"
#include "Utility/AlsxtCollisionGameplayTags.h"
#include "Utility/AlsxtStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "State/AlsxtFootstepState.h"
#include "AlsxtCameraAnimationInstance.h"
#include "AlsxtCharacterNpc.h"
#include "AbilitySystem/AttributeSets/AlsxtBreathAttributeSet.h"
#include "AbilitySystem/AttributeSets/AlsxtStaminaAttributeSet.h"
#include "AbilitySystem/Data/AlsxtGASGameplayTags.h"
#include "Settings/AlsxtOverlayLookupTableDataAsset.h"
#include "Utility/AlsxtOverlayGameplayTags.h"

void AAlsxtCharacter::ServerSetDesiredStance_Implementation(FGameplayTag NewDesiredStance)
{
	Super::ServerSetDesiredStance_Implementation(NewDesiredStance);

	Super::ServerSetDesiredStance_Implementation(NewDesiredStance);
	FGameplayTagContainer StanceTags {AlsStanceTags::Standing};
	StanceTags.AddTag(AlsStanceTags::Crouching);
	StanceTags.AddTag(AlsStanceTags::Prone);
	AnimationParametersState.StanceTags.RemoveTags(StanceTags);
	AnimationParametersState.StanceTags.AddTag(NewDesiredStance);
}

void AAlsxtCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (GetPlayerState())
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponent(); 
		if (ASC)
		{
			// Iterate through registered AttributeSets
			for (UAttributeSet* AttributeSet : ASC->GetSpawnedAttributes()) 
			{
				if (AttributeSet)
				{
					// Perform validation checks on the AttributeSet
					// Example: Check if a specific attribute has a non-zero value
					// if (AttributeSet->GetHealth() <= 0.0f) { /* Log error or handle */ }
					UE_LOG(LogTemp, Log, TEXT("AttributeSet %s initialized for PlayerState."), *AttributeSet->GetName());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Null AttributeSet found in ASC for PlayerState."));
				}
			}
		}
	}
}

FAlsxtAnimationParametersState AAlsxtCharacter::GetAnimationParametersState() const
{
	return AnimationParametersState;
}

void AAlsxtCharacter::SetAnimationParametersState_Implementation(FAlsxtAnimationParametersState NewAnimationParametersState)
{
	AnimationParametersState = NewAnimationParametersState;
	
	// Manually mark the property as dirty to force replication with Push Model
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, AnimationParametersState, this)
	OnRep_AnimationParametersState();
}

void AAlsxtCharacter::OnRep_AnimationParametersState()
{
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, AnimationParametersState, this)
}

AAlsxtCharacter::AAlsxtCharacter(const FObjectInitializer& ObjectInitializer) :
                                                                              Super(ObjectInitializer.SetDefaultSubobjectClass<UAlsxtPaintableSkeletalMeshComponent>(AAlsCharacter::MeshComponentName).SetDefaultSubobjectClass<UAlsxtCharacterMovementComponent>(AAlsCharacter::CharacterMovementComponentName))
{
	// Set GAS Defaults for this Sub-Class.
	DesiredGait = AlsGaitTags::Walking;
	ViewMode = AlsViewModeTags::FirstPerson;

	// Setup Components
	GetCapsuleComponent()->SetCapsuleRadius(25);
	GetCapsuleComponent()->bReceivesDecals = false;

	// Containers

	BodyParts = CreateDefaultSubobject<USceneComponent>(TEXT("Body Parts"));
	BodyParts->SetupAttachment(GetMesh());	
	ClothingSlots = CreateDefaultSubobject<USceneComponent>(TEXT("Clothing Slots"));
	ClothingSlots->SetupAttachment(GetMesh());

	// Cameras

	// Default ALS Camera
	Camera = CreateDefaultSubobject<UAlsCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeRotation_Direct({0.0f, 90.0f, 0.0f});

	//Setup Gameplay Camera Component
	GameplayCamera = CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("Gameplay Camera Component"));
	AddOwnedComponent(GameplayCamera);

	// Body Mesh Components

	Head = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(BodyParts);
	Head->SetCollisionProfileName("CharacterMesh");
	Head->bEnableUpdateRateOptimizations = false;
	Head->AlwaysLoadOnClient = true;
	Head->AlwaysLoadOnServer = true;
	Head->bOwnerNoSee = false;
	Head->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Head->bCastDynamicShadow = true;
	Head->bAffectDynamicIndirectLighting = true;
	Head->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Head->SetLeaderPoseComponent(GetMesh());
	Head->bReceivesDecals = false;

	HeadDummyShadow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head Dummy Shadow"));
	HeadDummyShadow->SetupAttachment(BodyParts);
	HeadDummyShadow->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	HeadDummyShadow->SetHiddenInGame(true);
	HeadDummyShadow->bCastHiddenShadow = true;
	HeadDummyShadow->SetLeaderPoseComponent(GetMesh());
	HeadDummyShadow->bReceivesDecals = false;

	Teeth = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Teeth"));
	Teeth->SetupAttachment(BodyParts);
	Teeth->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Teeth->bEnableUpdateRateOptimizations = false;
	Teeth->AlwaysLoadOnClient = true;
	Teeth->AlwaysLoadOnServer = true;
	Teeth->bOwnerNoSee = false;
	Teeth->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Teeth->bCastDynamicShadow = true;
	Teeth->bAffectDynamicIndirectLighting = true;
	Teeth->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Teeth->SetLeaderPoseComponent(GetMesh());
	Teeth->bReceivesDecals = false;

	Tongue = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Tongue"));
	Tongue->SetupAttachment(BodyParts);
	Tongue->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Tongue->bEnableUpdateRateOptimizations = false;
	Tongue->AlwaysLoadOnClient = true;
	Tongue->AlwaysLoadOnServer = true;
	Tongue->bOwnerNoSee = false;
	Tongue->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Tongue->bCastDynamicShadow = true;
	Tongue->bAffectDynamicIndirectLighting = true;
	Tongue->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Tongue->SetLeaderPoseComponent(GetMesh());
	Tongue->bReceivesDecals = false;

	Hair = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(BodyParts);
	Hair->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Hair->bEnableUpdateRateOptimizations = false;
	Hair->AlwaysLoadOnClient = true;
	Hair->AlwaysLoadOnServer = true;
	Hair->bOwnerNoSee = false;
	Hair->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Hair->bCastDynamicShadow = true;
	Hair->bAffectDynamicIndirectLighting = true;
	Hair->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Hair->SetLeaderPoseComponent(GetMesh());
	Hair->bReceivesDecals = false;

	HairDummyShadow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair Dummy Shadow"));
	HairDummyShadow->SetupAttachment(BodyParts);
	HairDummyShadow->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	HairDummyShadow->SetHiddenInGame(true);
	HairDummyShadow->bCastHiddenShadow = true;
	HairDummyShadow->SetLeaderPoseComponent(GetMesh());
	HairDummyShadow->bReceivesDecals = false;

	FacialHair = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Facial Hair"));
	FacialHair->SetupAttachment(BodyParts);
	FacialHair->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	FacialHair->bEnableUpdateRateOptimizations = false;
	FacialHair->AlwaysLoadOnClient = true;
	FacialHair->AlwaysLoadOnServer = true;
	FacialHair->bOwnerNoSee = false;
	FacialHair->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	FacialHair->bCastDynamicShadow = true;
	FacialHair->bAffectDynamicIndirectLighting = true;
	FacialHair->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	FacialHair->SetLeaderPoseComponent(GetMesh());
	FacialHair->bReceivesDecals = false;

	FacialHairDummyShadow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Facial Hair Dummy Shadow"));
	FacialHairDummyShadow->SetupAttachment(BodyParts);
	FacialHairDummyShadow->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	FacialHairDummyShadow->SetHiddenInGame(true);
	FacialHairDummyShadow->bCastHiddenShadow = true;
	FacialHairDummyShadow->SetLeaderPoseComponent(GetMesh());
	FacialHairDummyShadow->bReceivesDecals = false;

	// Equipment Mesh Components

	Headwear = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Headwear"));
	Headwear->SetupAttachment(ClothingSlots);
	Headwear->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Headwear->bEnableUpdateRateOptimizations = false;
	Headwear->AlwaysLoadOnClient = true;
	Headwear->AlwaysLoadOnServer = true;
	Headwear->bOwnerNoSee = false;
	Headwear->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Headwear->bCastDynamicShadow = true;
	Headwear->bAffectDynamicIndirectLighting = true;
	Headwear->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Headwear->SetLeaderPoseComponent(GetMesh());
	Headwear->bReceivesDecals = false;

	HeadwearDummyShadow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Headwear Dummy Shadow"));
	HeadwearDummyShadow->SetupAttachment(ClothingSlots);
	HeadwearDummyShadow->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	HeadwearDummyShadow->SetHiddenInGame(true);
	HeadwearDummyShadow->bCastHiddenShadow = true;
	HeadwearDummyShadow->SetLeaderPoseComponent(GetMesh());
	HeadwearDummyShadow->bReceivesDecals = false;

	Facewear = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Facewear"));
	Facewear->SetupAttachment(ClothingSlots);
	Facewear->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Facewear->bEnableUpdateRateOptimizations = false;
	Facewear->AlwaysLoadOnClient = true;
	Facewear->AlwaysLoadOnServer = true;
	Facewear->bOwnerNoSee = false;
	Facewear->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Facewear->bCastDynamicShadow = true;
	Facewear->bAffectDynamicIndirectLighting = true;
	Facewear->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Facewear->SetLeaderPoseComponent(GetMesh());
	Facewear->bReceivesDecals = false;

	FacewearDummyShadow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Facewear Dummy Shadow"));
	FacewearDummyShadow->SetupAttachment(ClothingSlots);
	FacewearDummyShadow->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	FacewearDummyShadow->SetHiddenInGame(true);
	FacewearDummyShadow->bCastHiddenShadow = true;
	FacewearDummyShadow->SetLeaderPoseComponent(GetMesh());
	FacewearDummyShadow->bReceivesDecals = false;
	
	Eyewear = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Eyewear"));
	Eyewear->SetupAttachment(ClothingSlots);
	Eyewear->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Eyewear->bEnableUpdateRateOptimizations = false;
	Eyewear->AlwaysLoadOnClient = true;
	Eyewear->AlwaysLoadOnServer = true;
	Eyewear->bOwnerNoSee = false;
	Eyewear->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Eyewear->bCastDynamicShadow = true;
	Eyewear->bAffectDynamicIndirectLighting = true;
	Eyewear->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Eyewear->SetLeaderPoseComponent(GetMesh());
	Eyewear->bReceivesDecals = false;

	EyewearDummyShadow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Eyewear Dummy Shadow"));
	EyewearDummyShadow->SetupAttachment(ClothingSlots);
	EyewearDummyShadow->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	EyewearDummyShadow->SetHiddenInGame(true);
	EyewearDummyShadow->bCastHiddenShadow = true;
	EyewearDummyShadow->SetLeaderPoseComponent(GetMesh());
	EyewearDummyShadow->bReceivesDecals = false;
	
	Earwear = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Earwear"));
	Earwear->SetupAttachment(ClothingSlots);
	Earwear->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Earwear->bEnableUpdateRateOptimizations = false;
	Earwear->AlwaysLoadOnClient = true;
	Earwear->AlwaysLoadOnServer = true;
	Earwear->bOwnerNoSee = false;
	Earwear->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Earwear->bCastDynamicShadow = true;
	Earwear->bAffectDynamicIndirectLighting = true;
	Earwear->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Earwear->SetLeaderPoseComponent(GetMesh());
	Earwear->bReceivesDecals = false;
	
	BottomUnderwear = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Bottom Underwear"));
	BottomUnderwear->SetupAttachment(ClothingSlots);
	BottomUnderwear->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	BottomUnderwear->bEnableUpdateRateOptimizations = false;
	BottomUnderwear->AlwaysLoadOnClient = true;
	BottomUnderwear->AlwaysLoadOnServer = true;
	BottomUnderwear->bOwnerNoSee = false;
	BottomUnderwear->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	BottomUnderwear->bCastDynamicShadow = true;
	BottomUnderwear->bAffectDynamicIndirectLighting = true;
	BottomUnderwear->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	BottomUnderwear->SetLeaderPoseComponent(GetMesh());
	BottomUnderwear->bReceivesDecals = false;
	
	TopUnderwear = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Top Underwear"));
	TopUnderwear->SetupAttachment(ClothingSlots);
	TopUnderwear->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	TopUnderwear->bEnableUpdateRateOptimizations = false;
	TopUnderwear->AlwaysLoadOnClient = true;
	TopUnderwear->AlwaysLoadOnServer = true;
	TopUnderwear->bOwnerNoSee = false;
	TopUnderwear->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	TopUnderwear->bCastDynamicShadow = true;
	TopUnderwear->bAffectDynamicIndirectLighting = true;
	TopUnderwear->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	TopUnderwear->SetLeaderPoseComponent(GetMesh());
	TopUnderwear->bReceivesDecals = false;
	
	Bottom = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Bottom"));
	Bottom->SetupAttachment(ClothingSlots);
	Bottom->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Bottom->bEnableUpdateRateOptimizations = false;
	Bottom->AlwaysLoadOnClient = true;
	Bottom->AlwaysLoadOnServer = true;
	Bottom->bOwnerNoSee = false;
	Bottom->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Bottom->bCastDynamicShadow = true;
	Bottom->bAffectDynamicIndirectLighting = true;
	Bottom->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Bottom->SetLeaderPoseComponent(GetMesh());
	Bottom->bReceivesDecals = false;
	
	Top = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Top"));
	Top->SetupAttachment(ClothingSlots);
	Top->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Top->bEnableUpdateRateOptimizations = false;
	Top->AlwaysLoadOnClient = true;
	Top->AlwaysLoadOnServer = true;
	Top->bOwnerNoSee = false;
	Top->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Top->bCastDynamicShadow = true;
	Top->bAffectDynamicIndirectLighting = true;
	Top->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Top->SetLeaderPoseComponent(GetMesh());
	Top->bReceivesDecals = false;
	
	TopJacket = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Top Jacket"));
	TopJacket->SetupAttachment(ClothingSlots);
	TopJacket->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	TopJacket->bEnableUpdateRateOptimizations = false;
	TopJacket->AlwaysLoadOnClient = true;
	TopJacket->AlwaysLoadOnServer = true;
	TopJacket->bOwnerNoSee = false;
	TopJacket->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	TopJacket->bCastDynamicShadow = true;
	TopJacket->bAffectDynamicIndirectLighting = true;
	TopJacket->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	TopJacket->SetLeaderPoseComponent(GetMesh());
	TopJacket->bReceivesDecals = false;
	
	TopVest = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Top Vest"));
	TopVest->SetupAttachment(ClothingSlots);
	TopVest->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	TopVest->bEnableUpdateRateOptimizations = false;
	TopVest->AlwaysLoadOnClient = true;
	TopVest->AlwaysLoadOnServer = true;
	TopVest->bOwnerNoSee = false;
	TopVest->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	TopVest->bCastDynamicShadow = true;
	TopVest->bAffectDynamicIndirectLighting = true;
	TopVest->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	TopVest->SetLeaderPoseComponent(GetMesh());
	TopVest->bReceivesDecals = false;
	
	Gloves = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Gloves"));
	Gloves->SetupAttachment(ClothingSlots);
	Gloves->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Gloves->bEnableUpdateRateOptimizations = false;
	Gloves->AlwaysLoadOnClient = true;
	Gloves->AlwaysLoadOnServer = true;
	Gloves->bOwnerNoSee = false;
	Gloves->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Gloves->bCastDynamicShadow = true;
	Gloves->bAffectDynamicIndirectLighting = true;
	Gloves->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Gloves->SetLeaderPoseComponent(GetMesh());
	Gloves->bReceivesDecals = false;
	
	Footwear = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Footwear"));
	Footwear->SetupAttachment(ClothingSlots);
	Footwear->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Footwear->bEnableUpdateRateOptimizations = false;
	Footwear->AlwaysLoadOnClient = true;
	Footwear->AlwaysLoadOnServer = true;
	Footwear->bOwnerNoSee = false;
	Footwear->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	Footwear->bCastDynamicShadow = true;
	Footwear->bAffectDynamicIndirectLighting = true;
	Footwear->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Footwear->SetLeaderPoseComponent(GetMesh());
	Footwear->bReceivesDecals = false;

	// Overlay Objects

	OverlaySkeletalMesh = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Overlay Skeletal Mesh Primary"));
	OverlaySkeletalMesh->SetupAttachment(GetMesh());
	OverlaySkeletalMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	OverlaySkeletalMesh->bEnableUpdateRateOptimizations = false;
	OverlaySkeletalMesh->AlwaysLoadOnClient = true;
	OverlaySkeletalMesh->AlwaysLoadOnServer = true;
	OverlaySkeletalMesh->bOwnerNoSee = false;
	OverlaySkeletalMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	OverlaySkeletalMesh->bCastDynamicShadow = true;
	OverlaySkeletalMesh->bAffectDynamicIndirectLighting = true;
	OverlaySkeletalMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	OverlaySkeletalMesh->bReceivesDecals = false;

	OverlayStaticMesh = CreateDefaultSubobject<UAlsxtPaintableStaticMeshComponent>(TEXT("Overlay Static Mesh Primary"));
	OverlayStaticMesh->SetupAttachment(GetMesh());
	OverlayStaticMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	OverlayStaticMesh->AlwaysLoadOnClient = true;
	OverlayStaticMesh->AlwaysLoadOnServer = true;
	OverlayStaticMesh->bOwnerNoSee = false;
	OverlayStaticMesh->bCastDynamicShadow = true;
	OverlayStaticMesh->bAffectDynamicIndirectLighting = true;
	OverlayStaticMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	OverlayStaticMesh->bReceivesDecals = false;

	OverlaySkeletalMeshSecondary = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Overlay Skeletal Mesh Secondary"));
	OverlaySkeletalMeshSecondary->SetupAttachment(GetMesh());
	OverlaySkeletalMeshSecondary->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	OverlaySkeletalMeshSecondary->bEnableUpdateRateOptimizations = false;
	OverlaySkeletalMeshSecondary->AlwaysLoadOnClient = true;
	OverlaySkeletalMeshSecondary->AlwaysLoadOnServer = true;
	OverlaySkeletalMeshSecondary->bOwnerNoSee = false;
	OverlaySkeletalMeshSecondary->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered;
	OverlaySkeletalMeshSecondary->bCastDynamicShadow = true;
	OverlaySkeletalMeshSecondary->bAffectDynamicIndirectLighting = true;
	OverlaySkeletalMeshSecondary->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	OverlaySkeletalMeshSecondary->bReceivesDecals = false;

	OverlayStaticMeshSecondary = CreateDefaultSubobject<UAlsxtPaintableStaticMeshComponent>(TEXT("Overlay Static Mesh Secondary"));
	OverlayStaticMeshSecondary->SetupAttachment(GetMesh());
	OverlayStaticMeshSecondary->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	OverlayStaticMeshSecondary->AlwaysLoadOnClient = true;
	OverlayStaticMeshSecondary->AlwaysLoadOnServer = true;
	OverlayStaticMeshSecondary->bOwnerNoSee = false;
	OverlayStaticMeshSecondary->bCastDynamicShadow = true;
	OverlayStaticMeshSecondary->bAffectDynamicIndirectLighting = true;
	OverlayStaticMeshSecondary->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	OverlayStaticMeshSecondary->bReceivesDecals = false;

	PhysicsConstraints = CreateDefaultSubobject<USceneComponent>(TEXT("Physics Constraints"));
	PhysicsConstraints->SetupAttachment(this->RootComponent);

	MeshPaintingSceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Mesh Painting Scene Capture"));
	MeshPaintingSceneCapture->SetupAttachment(GetMesh());
	MeshPaintingSceneCapture->SetRelativeRotation_Direct({ -90.0f, 90.0f, 0.0f });
	MeshPaintingSceneCapture->SetRelativeLocation_Direct({ 0.0f, 0.0f, 200.0f });
	MeshPaintingSceneCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	MeshPaintingSceneCapture->CompositeMode = ESceneCaptureCompositeMode::SCCM_Additive;

	ALSXTCharacterMovement = Cast<UAlsxtCharacterMovementComponent>(GetCharacterMovement());
	ALSXTMesh = Cast<UAlsxtPaintableSkeletalMeshComponent>(GetMesh());
	ALSXTMesh->bReceivesDecals = false;

	// Code Components
	PhysicsControlComponent = CreateDefaultSubobject<UPhysicsControlComponent>(TEXT("Physics Control Component"));
    PhysicsControlComponent->SetupAttachment(GetMesh());
	
	ContextualAnimationComponent = CreateDefaultSubobject<UContextualAnimSceneActorComponent>(TEXT("Contextual Animation Component"));
	AddOwnedComponent(ContextualAnimationComponent);

	ImpactReaction = CreateDefaultSubobject<UAlsxtImpactReactionComponent>(TEXT("Impact Reaction"));
	AddOwnedComponent(ImpactReaction);

	CharacterSound = CreateDefaultSubobject<UAlsxtCharacterSoundComponent>(TEXT("Character Sound"));
	AddOwnedComponent(CharacterSound);

	ProceduralRecoil = CreateDefaultSubobject<UAlsxtProceduralRecoilAnimComponent>(TEXT("Procedural Recoil"));
	AddOwnedComponent(ProceduralRecoil);

	PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("Physical Animation"));
	AddOwnedComponent(PhysicalAnimation);

	OnRagdollingStartedDelegate.BindUFunction(ImpactReaction, "OnRagdollingStarted");
	OnRagdollingEndedDelegate.BindUFunction(ImpactReaction, "OnRagdollingEnded");
	ALSXTCharacterMovement->OnEnterSlideSlopeAngle.AddUniqueDynamic(this, &AAlsxtCharacter::OnEnterSlideSlopeAngle);
	ALSXTCharacterMovement->OnExitSlideSlopeAngle.AddUniqueDynamic(this, &AAlsxtCharacter::OnExitSlideSlopeAngle);
}

void AAlsxtCharacter::OnRep_IsProne()
{
	if (ALSXTCharacterMovement)
	{
		if (IsCrouched())
		{
			ALSXTCharacterMovement->bWantsToProne = true;
			ALSXTCharacterMovement->Crouch(true);
		}
		else
		{
			ALSXTCharacterMovement->bWantsToProne = false;
			ALSXTCharacterMovement->UnCrouch(true);
		}
		ALSXTCharacterMovement->bNetworkUpdateReceived = true;
	}
}

bool AAlsxtCharacter::IsProne() const
{
	return bIsProne;
}

void AAlsxtCharacter::SetIsProne(const bool bInIsProne)
{
	bIsProne = bInIsProne;
}

bool AAlsxtCharacter::GetOverlaySlot(const FGameplayTag& Slot, FGameplayTag& Overlay, FGameplayTag& Posture)
{
	// if (hasTags)
	Overlay = AlsOverlayModeTags::Axe;
	Posture = AlsOverlayModeTags::Axe;
	return true;
}

bool AAlsxtCharacter::TrySetOverlaySlot(const FGameplayTag& TargetSlot, const FGameplayTag& NewOverlayMode, const FGameplayTag& NewCombatStance, const bool bForce)
{
	bool ReturnValue = false;
	if (bForce)
	{
		if (TargetSlot == ALSXTHandTags::BothHands)
		{
			if (auto OverlayInfo = ALSXTSettings->OverlaySettings.Overlays->Overlays.Find(GetOverlayMode()))
			{
				if (OverlayInfo->AvailableSlots.HasTag(ALSXTHandTags::BothHands))
				{
					const FGameplayTagContainer EmptyContainer;
					AnimationParametersState.OverlaySlots.OverlaySlotLeftHand = EmptyContainer;
					AnimationParametersState.OverlaySlots.OverlaySlotRightHand = EmptyContainer;
					AnimationParametersState.OverlaySlots.OverlaySlotBothHands.AddTagFast(NewOverlayMode);
					AnimationParametersState.OverlaySlots.OverlaySlotBothHands.AppendTags(OverlayInfo->Settings);
					return true;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (TargetSlot == ALSXTHandTags::Left)
			{
				if (auto OverlayInfo = ALSXTSettings->OverlaySettings.Overlays->Overlays.Find(GetOverlayMode()))
				{
					// Find existing entry for slot
					AnimationParametersState.OverlaySlots.OverlaySlotRightHand.HasTag(AlsxtOverlaySlotStanceTags::Active);
					if (ALSXTSettings->OverlaySettings.Overlays->Overlays.Find(GetOverlayMode())->AvailableStances.HasTag(Stance))
					{
						return true;
					}
					
					//OverlayInfo.
					AnimationParametersState.OverlaySlots.OverlaySlotLeftHand.AddTag(NewOverlayMode);
					AnimationParametersState.OverlaySlots.OverlaySlotLeftHand.AddTag(Stance);
					AnimationParametersState.OverlaySlots.OverlaySlotLeftHand.AppendTags(OverlayInfo->Settings);
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (TargetSlot == ALSXTHandTags::Right)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		if (TargetSlot == ALSXTHandTags::BothHands)
		{
			if (auto OverlayInfo = ALSXTSettings->OverlaySettings.Overlays->Overlays.Find(GetOverlayMode()))
			{
				if (OverlayInfo->AvailableSlots.HasTag(TargetSlot))
				{
					const FGameplayTagContainer EmptyContainer;
					AnimationParametersState.OverlaySlots.OverlaySlotLeftHand = EmptyContainer;
					AnimationParametersState.OverlaySlots.OverlaySlotRightHand = EmptyContainer;
					AnimationParametersState.OverlaySlots.OverlaySlotBothHands.AddTagFast(NewOverlayMode);
					AnimationParametersState.OverlaySlots.OverlaySlotBothHands.AppendTags(OverlayInfo->Settings);
					return true;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (TargetSlot == ALSXTHandTags::Left)
			{
				if (ALSXTSettings->OverlaySettings.Overlays->Overlays.Find(GetOverlayMode())->AvailableStances.HasTag(Stance))
				{
					return true;
				}
			}
			else if (TargetSlot == ALSXTHandTags::Right)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return ReturnValue;
}

void AAlsxtCharacter::SetMovingTag(bool bIsMoving)
{
	GetLocalRole() < ROLE_Authority ? Server_SetMovingTag(bIsMoving) : SetMovingTagLocal(bIsMoving);
}

void AAlsxtCharacter::SetMovingTagLocal(bool bIsMoving)
{
	if (!GetAbilitySystemComponent()) return;
	const FGameplayTag MovingTag {AlsxtGASGameplayTags::State::TAG_State_MovementInput};
	// bIsMoving ? GetAbilitySystemComponent()->AddLooseGameplayTag(MovingTag) : GetAbilitySystemComponent()->RemoveLooseGameplayTag(MovingTag);

	if (bIsMoving)
	{
		if (!GetAbilitySystemComponent()->HasMatchingGameplayTag(MovingTag))
		{
			// GetAbilitySystemComponent()->AddLooseGameplayTag(MovingTag);
			GetAbilitySystemComponent()->SetLooseGameplayTagCount(MovingTag, 1); 
		}
	}
	else
	{
		// GetAbilitySystemComponent()->RemoveLooseGameplayTag(MovingTag, 1, EGameplayTagReplicationState::TagAndCountToAll);
		GetAbilitySystemComponent()->SetLooseGameplayTagCount(MovingTag, 0); 
		if (GetAbilitySystemComponent()->HasMatchingGameplayTag(MovingTag))
		{
			// GetAbilitySystemComponent()->RemoveLooseGameplayTag(MovingTag);
			// GetAbilitySystemComponent()->RemoveLooseGameplayTag(MovingTag, 1, EGameplayTagReplicationState::TagAndCountToAll);
			GetAbilitySystemComponent()->SetLooseGameplayTagCount(MovingTag, 0);
		}
	}
}

void AAlsxtCharacter::Server_SetMovingTag_Implementation(bool bIsMoving)
{
	if (!GetAbilitySystemComponent()) return;
	const FGameplayTag MovingTag {AlsxtGASGameplayTags::State::TAG_State_MovementInput};
	// bIsMoving ? GetAbilitySystemComponent()->AddLooseGameplayTag(MovingTag) : GetAbilitySystemComponent()->RemoveLooseGameplayTag(MovingTag);

	if (bIsMoving)
	{
		if (!GetAbilitySystemComponent()->HasMatchingGameplayTag(MovingTag))
		{
			// GetAbilitySystemComponent()->AddLooseGameplayTag(MovingTag);
			GetAbilitySystemComponent()->SetLooseGameplayTagCount(MovingTag, 1); 
		}
	}
	else
	{
		// GetAbilitySystemComponent()->RemoveLooseGameplayTag(MovingTag, 1, EGameplayTagReplicationState::TagAndCountToAll);
		GetAbilitySystemComponent()->SetLooseGameplayTagCount(MovingTag, 0); 
		if (GetAbilitySystemComponent()->HasMatchingGameplayTag(MovingTag))
		{
			// GetAbilitySystemComponent()->RemoveLooseGameplayTag(MovingTag);
			// GetAbilitySystemComponent()->RemoveLooseGameplayTag(MovingTag, 1, EGameplayTagReplicationState::TagAndCountToAll);
			GetAbilitySystemComponent()->SetLooseGameplayTagCount(MovingTag, 0);
		}
	}

	// GetAbilitySystemComponent()->AddLooseGameplayTag(MovingTag);  

// if (!GetAbilitySystemComponent()->HasMatchingGameplayTag(MovingTag))
// {
// 	// The proper way in UE 5.4 to add a replicated loose tag is to call
// 	// the library function with bShouldReplicate = true, which handles
// 	// adding it to both the server's local list and the replicated list
// 	// for clients to pick up.
// 	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(this, FGameplayTagContainer(MovingTag), true);

// 	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(this, FGameplayTagContainer(MovingTag), true);

// 	// Alternatively, for a more direct approach:
// 	// ASC->AddReplicatedLooseGameplayTag(MyUniqueTag); // Replicates to clients, but not added to server's local tags
// 	// ASC->AddLooseGameplayTag(MyUniqueTag);         // Adds locally on the server

// 	// The library function combines these actions effectively.
// 	UE_LOG(LogTemp, Warning, TEXT("Server added unique tag: %s"), *MovingTag.ToString());
// }
// else
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Tag %s already exists on ASC. Not adding duplicate."), *MovingTag.ToString());
// }
}

bool AAlsxtCharacter::Server_SetMovingTag_Validate(bool bIsMoving)
{
	return true;
}

void AAlsxtCharacter::OnReplicated_OverlayModes(const FGameplayTagContainer& PreviousOverlayModes)
{
	OnOverlayModesChanged(PreviousOverlayModes);
}

void AAlsxtCharacter::OnOverlayModesChanged_Implementation(const FGameplayTagContainer& PreviousOverlayModes){}

void AAlsxtCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	RefreshVaulting();

	// if (GetDesiredFreelooking() == ALSXTFreelookingTags::True)
	// {
	// 
	// }

	FVector Difference = GetActorUpVector() - GetCharacterMovement()->CurrentFloor.HitResult.Normal;
	float Angle = FMath::RadiansToDegrees(FMath::Atan2(Difference.X, Difference.Y)) -90;
	if (Angle > 45.00)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%f"), Angle));
	}

	if (IAlsxtHeldItemInterface::Execute_IsHoldingAimableItem(this) && ((GetDesiredCombatStance() == ALSXTCombatStanceTags::Ready) || (GetDesiredCombatStance() == ALSXTCombatStanceTags::Aiming)))
	{
		FAlsxtAimState NewAimState = GetAimState();
		// OverlaySkeletalMesh->GetSock
		// NewAimState.CurrentHeadTargetTransform.Location = IALSXTCharacterInterface::Execute_GetCharacterFirearmSightLocation(this);
	}

	// BreathState.TargetState = CalculateTargetBreathState();

	// if (ShouldTransitionBreathState())
	// {
	// 	TransitionBreathState();
	// }
}

void AAlsxtCharacter::NotifyControllerChanged()
{
	const auto* PreviousPlayer{Cast<APlayerController>(PreviousController)};
	if (IsValid(PreviousPlayer))
	{
		auto* EnhancedInputSubsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PreviousPlayer->GetLocalPlayer())};
		if (IsValid(EnhancedInputSubsystem))
		{
			EnhancedInputSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}

	auto* NewPlayer{Cast<APlayerController>(GetController())};
	if (IsValid(NewPlayer))
	{
		NewPlayer->InputYawScale_DEPRECATED = 1.0f;
		NewPlayer->InputPitchScale_DEPRECATED = 1.0f;
		NewPlayer->InputRollScale_DEPRECATED = 1.0f;
		auto* EnhancedInputSubsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(NewPlayer->GetLocalPlayer())};
		if (IsValid(EnhancedInputSubsystem))
		{
			EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	Super::NotifyControllerChanged();
}

void AAlsxtCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;
	Parameters.Condition = COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bIsProne, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, FootprintsState, Parameters)
	// DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PhysicalAnimationState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredLean, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DefensiveModeState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredFreelooking, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredSex, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredLocomotionVariant, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredInjury, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredReadyStance, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredCombatStance, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredWeaponFirearmStance, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredWeaponReadyPosition, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredDefensiveMode, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredStationaryMode, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredStatus, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredFocus, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredStationaryMode, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredPhysicalAnimationMode, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredGripPosition, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredForegripPosition, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredWeaponCarryPosition, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredVaultType, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredWeaponObstruction, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PreviousLookInput, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, FreelookState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, HeadLookAtState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, AimState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bMovementEnabled, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MovementInput, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, AnimationParametersState, Parameters)
}

void AAlsxtCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AAlsxtCharacter::BeginPlay()
{
	AlsCharacter = Cast<AAlsCharacter>(GetParentActor());
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();

	// GetCapsuleComponent()->OnComponentHit.AddUniqueDynamic(ImpactReaction, &UAlsxtImpactReactionComponent::OnCapsuleHit);
	// CharacterCapsule->OnComponentHit.AddDynamic(this, &UAlsxtImpactReactionComponent::OnCapsuleHit);

	PhysicalAnimation->SetSkeletalMeshComponent(GetMesh());
	ResetPhysicalAnimationMode();
	GetMesh()->SetEnablePhysicsBlending(true);
	// FreelookTimerDelegate.BindUFunction(this, "FreelookTimer");
	BlendOutPhysicalAnimationTimerDelegate.BindUFunction(this, "BlendOutPhysicalAnimation");
	RefreshOverlayObject();
	IAlsxtCharacterInterface::Execute_GetCharacterCameraAnimationInstance(this)->OnFirstPersonOverrideChanged.AddDynamic(this, &AAlsxtCharacter::OnFirstPersonOverrideChanged);

}

void AAlsxtCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	Super::EndPlay(EndPlayReason);
}


UAbilitySystemComponent* AAlsxtCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void AAlsxtCharacter::OnEnterSlideSlopeAngle_Implementation()
{
	SetLocomotionMode(AlsLocomotionModeTags::SlopeSliding);
}

void AAlsxtCharacter::OnExitSlideSlopeAngle_Implementation()
{
	/*
	 * @todo Detect new Locomotion Mode when exiting Slope Sliding
	 **/
	SetLocomotionMode(AlsLocomotionModeTags::Grounded);
}

void AAlsxtCharacter::CalcCamera(const float DeltaTime, FMinimalViewInfo& ViewInfo)
{
	if (Camera->IsActive())
	{
		Camera->GetViewInfo(ViewInfo);
		return;
	}

	Super::CalcCamera(DeltaTime, ViewInfo);
}

void AAlsxtCharacter::SetupPlayerInputComponent(UInputComponent* Input)
{
	// Super::SetupPlayerInputComponent(Input);

	auto* EnhancedInput{Cast<UEnhancedInputComponent>(Input)};
	if (IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ThisClass::InputLookMouse);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::InputLook);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::InputSprint);
		EnhancedInput->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ThisClass::InputWalk);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ThisClass::InputCrouch);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::InputJump);
		EnhancedInput->BindAction(MantleAction, ETriggerEvent::Triggered, this, &ThisClass::InputMantle);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::InputAim);
		EnhancedInput->BindAction(AimToggleAction, ETriggerEvent::Triggered, this, &ThisClass::InputToggleAim);
		EnhancedInput->BindAction(FocusAction, ETriggerEvent::Triggered, this, &ThisClass::InputFocus);
		EnhancedInput->BindAction(RagdollAction, ETriggerEvent::Triggered, this, &ThisClass::InputRagdoll);
		EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &ThisClass::InputRoll);
		EnhancedInput->BindAction(RotationModeAction, ETriggerEvent::Triggered, this, &ThisClass::InputRotationMode);
		EnhancedInput->BindAction(ViewModeAction, ETriggerEvent::Triggered, this, &ThisClass::InputViewMode);
		EnhancedInput->BindAction(SwitchShoulderAction, ETriggerEvent::Triggered, this, &ThisClass::InputSwitchShoulder);
		EnhancedInput->BindAction(FreelookAction, ETriggerEvent::Triggered, this, &ThisClass::InputFreelook);
		EnhancedInput->BindAction(ToggleFreelookAction, ETriggerEvent::Triggered, this, &ThisClass::InputFreelook);
		EnhancedInput->BindAction(ToggleGaitAction, ETriggerEvent::Triggered, this, &ThisClass::InputToggleGait);
		EnhancedInput->BindAction(ToggleCombatReadyAction, ETriggerEvent::Triggered, this, &ThisClass::InputToggleCombatReady);
		EnhancedInput->BindAction(PrimaryInteractionAction, ETriggerEvent::Triggered, this, &ThisClass::InputPrimaryInteraction);
		EnhancedInput->BindAction(SecondaryInteractionAction, ETriggerEvent::Triggered, this, &ThisClass::InputSecondaryInteraction);
		EnhancedInput->BindAction(BlockAction, ETriggerEvent::Triggered, this, &ThisClass::InputBlock);
		EnhancedInput->BindAction(SwitchWeaponReadyPositionAction, ETriggerEvent::Triggered, this, &ThisClass::InputSwitchWeaponReadyPosition);
		EnhancedInput->BindAction(SwitchGripPositionAction, ETriggerEvent::Triggered, this, &ThisClass::InputSwitchGripPosition);
		EnhancedInput->BindAction(SwitchForegripPositionAction, ETriggerEvent::Triggered, this, &ThisClass::InputSwitchForegripPosition);

		InputMantleValue = EnhancedInput->GetBoundActionValue(MantleAction);
		
		OnSetupPlayerInputComponentUpdated.Broadcast();
	}
}

void AAlsxtCharacter::OnStanceChanged_Implementation(FGameplayTag PreviousStance)
{
	Super::OnStanceChanged_Implementation(PreviousStance);

	ImpactReaction->RefreshObstacleNavigationPoses();
}

FAlsxtCameraShakeSetting AAlsxtCharacter::SelectMovementCameraShakeAsset()
{
	FAlsxtCameraShakeSetting FoundCameraShakeSetting;
	if (const TObjectPtr<UAlsxtCameraShakeSettings>* CameraShakeSettings = ALSXTSettings->OverlaySettings.CameraShake.Find(GetOverlayMode()))
	{
		if (GetViewMode() == AlsViewModeTags::FirstPerson)
		{
			if (FAlsxtStanceMovementCameraViewShakeSettings* CameraViewShakeSettings = CameraShakeSettings->Get()->CameraShakeSettings.FirstPerson.Grounded.Find(GetDesiredStance()))
			{
				if (CameraViewShakeSettings->Gaits.Find(GetDesiredGait()))
				{
					FoundCameraShakeSetting = *CameraViewShakeSettings->Gaits.Find(GetDesiredGait());
					return FoundCameraShakeSetting;
				}
			}
		}
		if (GetViewMode() == AlsViewModeTags::ThirdPerson)
		{
			if (FAlsxtStanceMovementCameraViewShakeSettings* CameraViewShakeSettings = CameraShakeSettings->Get()->CameraShakeSettings.ThirdPerson.Grounded.Find(GetDesiredStance()))
			{
				if (CameraViewShakeSettings->Gaits.Find(GetDesiredGait()))
				{
					FoundCameraShakeSetting = *CameraViewShakeSettings->Gaits.Find(GetDesiredGait());
					return FoundCameraShakeSetting;
				}
			}
		}
	}
	return FoundCameraShakeSetting;	
}

// TODO: Implement requiring holding down Jump to Mantle and Vault
bool AAlsxtCharacter::IsMantlingAllowedToStart_Implementation() const
{
	if (IsLocallyControlled() && ALSXTSettings->LocomotionActionSettings.IsValid())
	{
		if (ALSXTSettings->LocomotionActionSettings.Get()->bHoldJumpToMantle)
		{
			// return Super::IsMantlingAllowedToStart() && InputMantleValue.Get<bool>();
			return InputMantleValue.Get<bool>();
		}
		else
		{
			return !LocomotionAction.IsValid();
		}
	}
	else
	{
		return !LocomotionAction.IsValid();
	}

	// return Super::IsMantlingAllowedToStart();
}

bool AAlsxtCharacter::CanProne() const
{
	return false;
}

void AAlsxtCharacter::OnStartProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	
}

void AAlsxtCharacter::OnEndProne(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	
}

TSoftObjectPtr<UGameplayCameraComponent> AAlsxtCharacter::GetCharacterGameplayCameraComponent_Implementation() const
{
	return GameplayCamera;
}

bool AAlsxtCharacter::GetCharacterIsCameraRightShoulder_Implementation() const
{
	return true;
}

bool AAlsxtCharacter::CanCharacterFocus_Implementation() const
{
	return Focus == ALSXTFocusedTags::False;
}

bool AAlsxtCharacter::GetCharacterFocus_Implementation() const
{
	return Focus == ALSXTFocusedTags::True;
}

void AAlsxtCharacter::SetCharacterFocus_Implementation(const bool NewFocus)
{
	NewFocus ? SetDesiredFocus(ALSXTFocusedTags::True) : SetDesiredFocus(ALSXTFocusedTags::False);
}

void AAlsxtCharacter::DisableInputMovement(const bool Disable)
{
	bMovementEnabled = !Disable;
	// Disable ? ALSXTCharacterMovement->DisableMovement() : GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	// Disable ? ALSXTCharacterMovement->SetMovementMode(EMovementMode::MOVE_None) : ALSXTCharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AAlsxtCharacter::DisableLookAt(const bool Disable)
{

}

void AAlsxtCharacter::NotifyLocomotionModeChanged(FGameplayTag PreviousLocomotionMode)
{
	Super::NotifyLocomotionModeChanged(PreviousLocomotionMode);

	AnimationParametersState.LocomotionTags.RemoveTag(PreviousLocomotionMode);
	AnimationParametersState.LocomotionTags.AddTag(LocomotionMode);
}

void AAlsxtCharacter::InputLookMouse(const FInputActionValue& ActionValue)
{
	FVector2D Value{ActionValue.Get<FVector2D>()};

	if (GetViewMode() == AlsViewModeTags::ThirdPerson && ALSXTSettings->IsValidLowLevelFast() && ALSXTSettings.Get()->ViewSettings->IsValidLowLevelFast())
	{
		// Get the input vector (X=Yaw, Y=Pitch)
		FVector2D LookAxisVector = ActionValue.Get<FVector2D>();

		// Apply sensitivity to the input values
		float YawInput = LookAxisVector.X * LookRightMouseSensitivity;
		float PitchInput = LookAxisVector.Y * LookUpMouseSensitivity;

		// Apply the input using the standard controller functions. These functions manage accumulation
		AddControllerYawInput(YawInput * LookRightMouseSensitivity);
		AddControllerPitchInput(PitchInput * LookUpMouseSensitivity);

		// Get the current control rotation
		FRotator CurrentRotation = GetControlRotation();

		// Clamp values using FMath::Clamp
		CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch, ALSXTSettings.Get()->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampY.MinValue, ALSXTSettings.Get()->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampY.MaxValue);
		CurrentRotation.Yaw = FMath::Clamp(CurrentRotation.Yaw, ALSXTSettings.Get()->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampX.MinValue, ALSXTSettings.Get()->ViewSettings.Get()->ThirdPersonControlRotationClamp.ClampX.MaxValue);

		// Set the corrected rotation back to the controller. This is necessary to enforce the clamp *after* the AddControllerInput operations have modified the rotation
		Controller->SetControlRotation(CurrentRotation);
	}
	else
	{
		if (Freelooking == ALSXTFreelookingTags::True)
		{
			Value = FreelookState.LockedLookInput;
		}
		else
		{
			Value = ActionValue.Get<FVector2D>();
		}

		AddControllerPitchInput(Value.Y * LookUpMouseSensitivity);
		PreviousLookInput.Y = Value.Y;
		AddControllerYawInput(Value.X * LookRightMouseSensitivity);
		PreviousLookInput.X = Value.X;
	}

	// if (Freelooking == ALSXTFreelookingTags::True)
	// {
	// 	Value = FreelookState.LockedLookInput;
	// }
	// else
	// {
	// 	Value = ActionValue.Get<FVector2D>();
	// }
// 
	// AddControllerPitchInput(Value.Y * LookUpMouseSensitivity);
	// PreviousLookInput.Y = Value.Y;
	// AddControllerYawInput(Value.X * LookRightMouseSensitivity);
	// PreviousLookInput.X = Value.X;
}

void AAlsxtCharacter::InputLook(const FInputActionValue& ActionValue)
{
	FVector2D Value{ ActionValue.Get<FVector2D>() };

	if (Freelooking == ALSXTFreelookingTags::True)
	{
		Value = FreelookState.LockedLookInput;
	}
	else
	{
		Value = ActionValue.Get<FVector2D>();
	}

	AddControllerPitchInput(Value.Y * LookUpRate * GetWorld()->GetDeltaSeconds());
	PreviousLookInput.Y = Value.Y;
	AddControllerYawInput(Value.X * LookRightRate * GetWorld()->GetDeltaSeconds());
	PreviousLookInput.X = Value.X;
}

void AAlsxtCharacter::InputMove(const FInputActionValue& ActionValue)
{
	if (GetDesiredStatus() == ALSXTStatusTags::Normal && bMovementEnabled)
	{
		const auto Value{ UAlsVector::ClampMagnitude012D(ActionValue.Get<FVector2D>()) };

		FRotator CapsuleRotation = GetActorRotation();
		auto ForwardDirection{ UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(GetViewState().Rotation.Yaw)) };
		auto RightDirection{ UAlsVector::PerpendicularCounterClockwiseXY(ForwardDirection) };
		const auto CharForwardDirection{ UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(CapsuleRotation.Yaw)) };
		const auto CharRightDirection{ UAlsVector::PerpendicularCounterClockwiseXY(CharForwardDirection) };

		if (GetDesiredFreelooking() == ALSXTFreelookingTags::True)
		{
			// AddMovementInput(HeadLookAtState.PreviousControlRotation.Yaw * Value.Y + RightDirection * Value.X);
			
			// const auto FreelookForwardDirection{ UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(GetHeadLookAtState().LockedViewState.Rotation.Yaw)) };
			ForwardDirection = UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(GetFreelookState().LockedViewState.Rotation.Yaw));
			RightDirection = UAlsVector::PerpendicularCounterClockwiseXY(ForwardDirection);
			AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
			MovementInput = ForwardDirection * Value.Y + RightDirection * Value.X;

			// AddMovementInput(CharForwardDirection * Value.Y + CharRightDirection * Value.X);
			// MovementInput = CharForwardDirection * Value.Y + CharRightDirection * Value.X;
			// GetLocomotionState().PreviousVelocity;
			// AddMovementInput(GetLocomotionState().PreviousVelocity);
			// MovementInput = GetLocomotionState().PreviousVelocity;
		}
		else
		{
			ForwardDirection = UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(GetViewState().Rotation.Yaw)) ;
			RightDirection = UAlsVector::PerpendicularCounterClockwiseXY(ForwardDirection);
			AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
			MovementInput = ForwardDirection * Value.Y + RightDirection * Value.X;
		}
	}
}

void AAlsxtCharacter::InputSprint(const FInputActionValue& ActionValue)
{
	// if (CanSprint())
	// {
	// 	SetDesiredGait(ActionValue.Get<bool>() ? AlsGaitTags::Sprinting : AlsGaitTags::Running);
	// }
}

void AAlsxtCharacter::InputWalk()
{
	if (GetDesiredGait() == AlsGaitTags::Walking)
	{
		SetDesiredGait(AlsGaitTags::Running);
	}
	else if (GetDesiredGait() == AlsGaitTags::Running)
	{
		SetDesiredGait(AlsGaitTags::Walking);
	}
}

void AAlsxtCharacter::InputCrouch()
{
	// if (GetDesiredStance() == AlsStanceTags::Standing)
	// {
	// 	if (CanSlide())
	// 	{
	// 		TryStartSliding(1.3f);
	// 	}
	// 	else {
	// 		SetDesiredStance(AlsStanceTags::Crouching);
	// 	}
	// }
	// else if (GetDesiredStance() == AlsStanceTags::Crouching)
	// {
	// 	SetDesiredStance(AlsStanceTags::Standing);
	// 	if (GetDesiredStatus() != ALSXTStatusTags::Normal)
	// 	{
	// 		SetDesiredStatus(ALSXTStatusTags::Normal);
	// 	}
	// }

	if (GetDesiredStance() == AlsStanceTags::Standing)
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		if (ASC)
		{
			FGameplayTag CrouchingTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Crouching"));
        
			if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(CrouchingTag)))
			{
				UCharacterMovementComponent* MoveComp = GetCharacterMovement();
    
				// Get MinForwardVelocity for Sliding
				// float MinSlideVelocity = GetSettings()->Sliding.MinForwardVelocity; // Example
				float MinSlideVelocity = 600.0f; // Replace with your variable from AlsSettings

				// Calculate forward velocity
				float ForwardVelocity = FVector::DotProduct(GetVelocity(), GetActorForwardVector());

				if (CanSlide() && (ForwardVelocity >= MinSlideVelocity))
				{
					TryStartSliding(1.3f);
				}
				else {
					SetDesiredStance(AlsStanceTags::Crouching);
				}
			}
		}
	}
	else
	{
		if (GetDesiredStance() == AlsStanceTags::Crouching)
		{
			SetDesiredStance(AlsStanceTags::Standing);	
		}
	}
}

void AAlsxtCharacter::InputJump(const FInputActionValue& ActionValue)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC)
	{
		FGameplayTag JumpTag = FGameplayTag::RequestGameplayTag(FName("Gameplay.Ability.Jump"));
        
		if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(JumpTag)))
		{
			if (CanJump() && bMovementEnabled)
			{
				if (ActionValue.Get<bool>())
				{
					if (StopRagdolling())
					{
						return;
					}
					if (GetDesiredStatus() != ALSXTStatusTags::Normal)
					{
						SetDesiredStatus(ALSXTStatusTags::Normal);
						return;
					}
					if (TryStartVaultingGrounded())
					{
						return;
					}
					if (StartMantlingGrounded())
					{
						return;
					}
					if (GetStance() == AlsStanceTags::Crouching)
					{
						SetDesiredStance(AlsStanceTags::Standing);
						return;
					}

					Super::Jump();
				}
				else
				{
					StopJumping();
				}
			}
			else
			{
				StopJumping();
			}
		}
	}
	else
	{
		StopJumping();
	}

	// if (CanJump() && bMovementEnabled)
	// {
	// 	if (ActionValue.Get<bool>())
	// 	{
	// 		if (StopRagdolling())
	// 		{
	// 			return;
	// 		}
	// 		if (GetDesiredStatus() != ALSXTStatusTags::Normal)
	// 		{
	// 			SetDesiredStatus(ALSXTStatusTags::Normal);
	// 			return;
	// 		}
	// 		if (TryStartVaultingGrounded())
	// 		{
	// 			return;
	// 		}
	// 		if (StartMantlingGrounded())
	// 		{
	// 			return;
	// 		}
	// 		if (GetStance() == AlsStanceTags::Crouching)
	// 		{
	// 			SetDesiredStance(AlsStanceTags::Standing);
	// 			return;
	// 		}
// 
	// 		Super::Jump();
	// 	}
	// 	else
	// 	{
	// 		StopJumping();
	// 	}
	// }
	// else
	// {
	// 	StopJumping();
	// }
}

void AAlsxtCharacter::InputMantle(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		// RefreshMantle();
	}
	else
	{

	}
}

void AAlsxtCharacter::InputAim(const FInputActionValue& ActionValue)
{
	if (CanAim())
	{
		SetDesiredAiming(ActionValue.Get<bool>());

		if (ActionValue.Get<bool>())
		{
			SetDesiredRotationMode(AlsRotationModeTags::Aiming);
			if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Ready)
			{
				SetDesiredCombatStance(ALSXTCombatStanceTags::Aiming);
			}
			if (IAlsxtHeldItemInterface::Execute_IsHoldingAimableItem(this)) {
				SetDesiredCombatStance(ALSXTCombatStanceTags::Aiming);
				SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Aiming);
				
				// if (GetDesiredCombatStance() != ALSXTCombatStanceTags::Neutral)
				// {
				// 	SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Aiming);
				// }
			}
		}
		else 
		{
			if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Aiming)
			{
				SetDesiredCombatStance(ALSXTCombatStanceTags::Ready);
				SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Ready);
			}
			if (IAlsxtHeldItemInterface::Execute_IsHoldingAimableItem(this)) {
				if (GetDesiredCombatStance() != ALSXTCombatStanceTags::Neutral)
				{
					SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Ready);
				} 
				else
				{
					SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
				}
			}
			else 
			{
				SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
			}
		}
	}
}

void AAlsxtCharacter::InputToggleAim()
{
	if (IAlsxtHeldItemInterface::Execute_IsHoldingAimableItem(this)) 
	{
		if (GetDesiredRotationMode() == AlsRotationModeTags::Aiming)
		{
			SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
			SetDesiredCombatStance(ALSXTCombatStanceTags::Ready);
		}
		if (CanAim())
		{
			SetViewMode(AlsViewModeTags::FirstPerson);
			SetDesiredCombatStance(ALSXTCombatStanceTags::Aiming);
			SetDesiredRotationMode(AlsRotationModeTags::Aiming);
		}
	}
}

void AAlsxtCharacter::InputFocus(const FInputActionValue& ActionValue)
{
	if (CanFocus())
	{
		if (ActionValue.Get<bool>())
		{
			SetDesiredFocus(ALSXTFocusedTags::True);
		}
		else
		{
			SetDesiredFocus(ALSXTFocusedTags::False);
		}
	}
}

void AAlsxtCharacter::InputRagdoll()
{
	if (!StopRagdolling())
	{
		StartRagdolling();
	}
}

void AAlsxtCharacter::InputRoll()
{
	static constexpr auto PlayRate{ 1.3f };
	if(CanRoll() && bMovementEnabled)
	{
		StartRolling(PlayRate);
	}
}

void AAlsxtCharacter::InputRotationMode()
{
	SetDesiredRotationMode(GetDesiredRotationMode() == AlsRotationModeTags::VelocityDirection
		                       ? AlsRotationModeTags::ViewDirection
		                       : AlsRotationModeTags::VelocityDirection);
}

void AAlsxtCharacter::InputViewMode()
{
	const FGameplayTag PreviousViewMode{ GetViewMode() };
	auto DesiredViewMode{ FGameplayTag::EmptyTag };
	DesiredViewMode == (GetViewMode() == AlsViewModeTags::ThirdPerson ? AlsViewModeTags::FirstPerson : AlsViewModeTags::ThirdPerson);
	if (CanSetToViewMode(DesiredViewMode)) 
	{
		SetViewMode(GetViewMode() == AlsViewModeTags::ThirdPerson ? AlsViewModeTags::FirstPerson : AlsViewModeTags::ThirdPerson);
		OnViewModeChanged(PreviousViewMode);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AAlsxtCharacter::InputSwitchShoulder()
{
	Camera->SetRightShoulder(!Camera->IsRightShoulder());
}

void AAlsxtCharacter::InputFreelook(const FInputActionValue& ActionValue)
{
	if (CanFreelook())
	{
		if (ActionValue.Get<bool>())
		{
			ActivateFreelooking();
		}
		else
		{
			DeactivateFreelooking();
		}
	}
}

void AAlsxtCharacter::InputSwitchGripPosition()
{
	if (CanSwitchGripPosition())
	{
		FGameplayTagContainer AvailableGripPositions = GetAvailableGripPositions();
		TArray<FGameplayTag> AvailableGripPositionsArray;
		AvailableGripPositions.GetGameplayTagArray(AvailableGripPositionsArray);
		if (AvailableGripPositions.IsEmpty() || AvailableGripPositions.Num() == 1 && AvailableGripPositionsArray[0] == GetDesiredGripPosition())
		{
			return;
		}
		int LastIndex = AvailableGripPositionsArray.Num() - 1;
		int CurrentIndex = AvailableGripPositionsArray.IndexOfByKey(GetDesiredGripPosition());
		int NextIndex;
		if ((CurrentIndex + 1) > LastIndex)
		{
			NextIndex = 0;
		}
		else
		{
			NextIndex = CurrentIndex + 1;
		}

		SetDesiredGripPosition(AvailableGripPositionsArray[NextIndex]);
	}
}

// void AALSXTCharacter::InputSwitchGripPosition()
// {
// 	if (CanSwitchGripPosition())
// 	{
// 		IdleAnimation->ResetIdleCounterTimer();
// 	}
// }

void AAlsxtCharacter::InputSwitchForegripPosition()
{
	if (CanSwitchForegripPosition())
	{
		FGameplayTagContainer AvailableForegripPositions = GetAvailableForegripPositions();
		TArray<FGameplayTag> AvailableForegripPositionsArray;
		AvailableForegripPositions.GetGameplayTagArray(AvailableForegripPositionsArray);
		if (AvailableForegripPositions.IsEmpty() || AvailableForegripPositions.Num() == 1 && AvailableForegripPositionsArray[0] == GetDesiredGripPosition())
		{
			return;
		}
		int LastIndex = AvailableForegripPositionsArray.Num() - 1;
		int CurrentIndex = AvailableForegripPositionsArray.IndexOfByKey(GetDesiredForegripPosition());
		int NextIndex;
		if ((CurrentIndex + 1) > LastIndex)
		{
			NextIndex = 0;
		}
		else
		{
			NextIndex = CurrentIndex + 1;
		}
		
		SetDesiredForegripPosition(AvailableForegripPositionsArray[NextIndex]);
		FAlsxtHeldItemState NewHeldItemState = GetHeldItemState();
		NewHeldItemState.GripState.Foregrip.Grip = IAlsxtHeldItemInterface::Execute_GetHeldItemForegrip(this);
		SetHeldItemState(NewHeldItemState);
	}
}

void AAlsxtCharacter::ApplyDesiredStance()
{
	if (!GetLocomotionAction().IsValid())
	{
		if (GetLocomotionMode() == AlsLocomotionModeTags::Grounded)
		{
			if (GetDesiredStance() == AlsStanceTags::Standing)
			{
				UnCrouch();
			}
			else if (GetDesiredStance() == AlsStanceTags::Crouching)
			{
				Crouch();
			}
		}
		else if (GetLocomotionMode() == AlsLocomotionModeTags::InAir)
		{
			UnCrouch();
		}
	}
	else
	{
		if ((GetLocomotionAction() == AlsLocomotionActionTags::Rolling && Settings->Rolling.bCrouchOnStart) || (GetLocomotionAction() == AlsLocomotionActionTags::Sliding && ALSXTSettings->Sliding.bCrouchOnStart))
		{
			Crouch();
		}
	}
}

// Input Actions

bool AAlsxtCharacter::CanFreelook_Implementation() const {return true;}

void AAlsxtCharacter::ALSXTRefreshRotationInstant(const float TargetYawAngle, const ETeleportType Teleport)
{
	SetRotationInstant(TargetYawAngle, Teleport);
}

void AAlsxtCharacter::SetMovementModeLocked(bool bNewMovementModeLocked)
{
	bMovementEnabled = !bNewMovementModeLocked;
	ForceNetUpdate();
}

const float AAlsxtCharacter::GetCameraZoom() const
{
	return CameraZoom;
}

void AAlsxtCharacter::SetCameraZoom(const float NewCameraZoom)
{
	CameraZoom = NewCameraZoom;
}

void AAlsxtCharacter::Crouch(const bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);

	// Change stance instantly without waiting for ACharacter::OnStartCrouch().

	if (!GetCharacterMovement()->bWantsToCrouch)
	{
		return;
	}

	if ((GetLocomotionAction() == AlsLocomotionActionTags::Rolling) || (GetLocomotionAction() == AlsLocomotionActionTags::Sliding))
	{
		SetDesiredStance(GetDesiredStance()); // Keep desired stance when rolling.
		return;
	}

	SetDesiredStance(AlsStanceTags::Crouching);
}

void AAlsxtCharacter::StartBlendOutPhysicalAnimation()
{
	GetWorld()->GetTimerManager().SetTimer(BlendOutPhysicalAnimationTimerHandle, BlendOutPhysicalAnimationTimerDelegate, 0.01f, true);
}

void AAlsxtCharacter::BlendOutPhysicalAnimation()
{
	FAlsxtPhysicalAnimationState NewPhysicalAnimationState = GetPhysicalAnimationState();
	NewPhysicalAnimationState.Alpha = NewPhysicalAnimationState.Alpha - 0.01;
	SetPhysicalAnimationState(NewPhysicalAnimationState);

	for (FName AffectedBoneBelow : GetPhysicalAnimationState().AffectedBonesBelow)
	{
		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(AffectedBoneBelow, GetPhysicalAnimationState().Alpha, false, true);
	}	

	if (GetPhysicalAnimationState().Alpha <= 0.0f)
	{
		EndBlendOutPhysicalAnimation();
	}
}

void AAlsxtCharacter::EndBlendOutPhysicalAnimation()
{
	GetWorld()->GetTimerManager().ClearTimer(BlendOutPhysicalAnimationTimerHandle);
	FAlsxtPhysicalAnimationState NewPhysicalAnimationState;
	NewPhysicalAnimationState.Mode = FGameplayTag::EmptyTag;
	NewPhysicalAnimationState.ProfileName = "CharacterMesh";
	SetPhysicalAnimationState(NewPhysicalAnimationState);
	FAlsxtDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
	NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
	NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
	NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;

	if (GetDefensiveModeState().AnticipationMode != ALSXTDefensiveModeTags::Anticipation)
	{
		NewDefensiveModeState.Mode = AlsxtPhysicalAnimationModeTags::None;
		NewDefensiveModeState.AnticipationMode = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationSide = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationHeight = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationForm = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationTransform = {{0, 0, 0}, {0, 0, 0}, { 0, 0, 0 }};
	}

	NewDefensiveModeState.ObstacleTransform = {{0, 0, 0}, {0, 0, 0}, { 0, 0, 0 }};
	SetDefensiveModeState(NewDefensiveModeState);
	
	// FALSXTPhysicalAnimationState NewPhysicalAnimationState;
	// SetPhysicalAnimationState(NewPhysicalAnimationState);
	TArray<FName> EmptyNames;
	SetPhysicalAnimationMode(AlsxtPhysicalAnimationModeTags::None, EmptyNames);
}

void AAlsxtCharacter::SetAnimationBlendState(FAlsxtAnimationBlendState NewAnimationBlendState)
{
	OnAnimationBlendStateChanged(GetAnimationBlendState());
}

void AAlsxtCharacter::OnOverlayModeChanged_Implementation(FGameplayTag PreviousOverlayMode)
{
	Super::OnOverlayModeChanged_Implementation(PreviousOverlayMode);

	ImpactReaction->RefreshBlockingPoses();
	ImpactReaction->RefreshCrowdNavigationPoses();
	RefreshOverlayLinkedAnimationLayer();
	RefreshOverlayObject();
}

void AAlsxtCharacter::OnAnimationBlendStateChanged_Implementation(FAlsxtAnimationBlendState PreviousBlendState)
{
	
}

void AAlsxtCharacter::OnJumped_Implementation()
{
	FALSXTCharacterVoiceParameters CharacterVoiceParams = IAlsxtCharacterCustomizationComponentInterface::Execute_GetVoiceParameters(this);
	CharacterSound->PlayActionSound(true, true, true, ALSXTCharacterMovementSoundTags::Jumping, CharacterVoiceParams.Sex, CharacterVoiceParams.Variant, IAlsxtCharacterInterface::Execute_GetCharacterOverlayMode(this), ALSXTActionStrengthTags::Medium, IAlsxtCharacterInterface::Execute_GetStamina(this));

}

void AAlsxtCharacter::OnMantlingStarted_Implementation(const FAlsMantlingParameters& Parameters)
{
	FGameplayTag TypeTag{ FGameplayTag::EmptyTag };
	FALSXTCharacterVoiceParameters CharacterVoiceParams = IAlsxtCharacterCustomizationComponentInterface::Execute_GetVoiceParameters(this);

	if (Parameters.MantlingType == EAlsMantlingType::Low)
	{
		TypeTag = ALSXTCharacterMovementSoundTags::MantlingLow;
		ClearOverlayObject();
	}
	else
	{
		TypeTag = ALSXTCharacterMovementSoundTags::MantlingHigh;
	}

	CharacterSound->PlayActionSound(true, true, true, TypeTag, CharacterVoiceParams.Sex, CharacterVoiceParams.Variant, IAlsxtCharacterInterface::Execute_GetCharacterOverlayMode(this), ALSXTActionStrengthTags::Medium, IAlsxtCharacterInterface::Execute_GetStamina(this));
}

void AAlsxtCharacter::OnMantlingEnded_Implementation()
{
	ClearOverlayObject();
}

void AAlsxtCharacter::OnRagdollingStarted_Implementation()
{
	FALSXTCharacterVoiceParameters CharacterVoiceParams = IAlsxtCharacterCustomizationComponentInterface::Execute_GetVoiceParameters(this);
	RefreshOverlayObject();
	CharacterSound->PlayDamageSound(true, true, true, CharacterVoiceParams.Sex, CharacterVoiceParams.Variant, IAlsxtCharacterInterface::Execute_GetCharacterOverlayMode(this), ALSXTAttackMethodTags::Regular, ALSXTActionStrengthTags::Medium, ALSXTImpactFormTags::Blunt, IAlsxtCharacterInterface::Execute_GetStamina(this));
}

void AAlsxtCharacter::OnRagdollingEnded_Implementation()
{
	RefreshOverlayObject();
}

void AAlsxtCharacter::OnSlidingStarted_Implementation()
{
	FALSXTCharacterVoiceParameters CharacterVoiceParams = IAlsxtCharacterCustomizationComponentInterface::Execute_GetVoiceParameters(this);
	CharacterSound->PlayActionSound(true, true, true, ALSXTCharacterMovementSoundTags::Sliding, CharacterVoiceParams.Sex, CharacterVoiceParams.Variant, IAlsxtCharacterInterface::Execute_GetCharacterOverlayMode(this), ALSXTActionStrengthTags::Medium, IAlsxtCharacterInterface::Execute_GetStamina(this));
}

bool AAlsxtCharacter::IsHoldingItem_Implementation() const
{
	return (IsValid(OverlaySkeletalMesh) || IsValid(OverlayStaticMesh));
}

bool AAlsxtCharacter::IsHoldingAimableItem_Implementation() const
{
	
	return IAlsxtHeldItemInterface::Execute_IsHoldingItem(this) && ALSXTSettings->OverlaySettings.AimableOverlayModes.HasTagExact(OverlayMode);
	// return (IsDesiredAiming() && CanAimDownSights() && (GetViewMode() == AlsViewModeTags::FirstPerson) && (GetDesiredCombatStance() != ALSXTCombatStanceTags::Neutral));
}

bool AAlsxtCharacter::IsAimingDownSights_Implementation() const
{
	return (IsDesiredAiming() && CanAimDownSights() && (GetViewMode() == AlsViewModeTags::FirstPerson) && (GetDesiredCombatStance() != ALSXTCombatStanceTags::Neutral));
}

FAlsxtGlobalGeneralMeshPaintingSettings AAlsxtCharacter::GetGlobalGeneralMeshPaintingSettings_Implementation() const
{
	return ALSXTSettings->MeshPainting;
}

USceneCaptureComponent2D* AAlsxtCharacter::GetSceneCaptureComponent_Implementation() const
{
	return MeshPaintingSceneCapture;
}

void AAlsxtCharacter::GetElementalCondition_Implementation(UPrimitiveComponent* Component, FGameplayTag& ElementalCondition) const
{
	if (Cast<UAlsxtPaintableSkeletalMeshComponent>(Component))
	{
		ElementalCondition = Cast<UAlsxtPaintableSkeletalMeshComponent>(Component)->GetElementalCondition();
	}
	if (Cast<UAlsxtPaintableStaticMeshComponent>(Component))
	{
		ElementalCondition = Cast<UAlsxtPaintableStaticMeshComponent>(Component)->GetElementalCondition();
	}
}

void AAlsxtCharacter::PaintMesh_Implementation(UPrimitiveComponent* Component, EPhysicalSurface SurfaceType, const FGameplayTag PaintType, FVector Location, float Radius)
{
	if (Cast<UAlsxtPaintableSkeletalMeshComponent>(Component))
	{
		Cast<UAlsxtPaintableSkeletalMeshComponent>(Component)->PaintMesh(SurfaceType, PaintType, Location, Radius);
	}
	if (Cast<UAlsxtPaintableStaticMeshComponent>(Component))
	{
		Cast<UAlsxtPaintableStaticMeshComponent>(Component)->PaintMesh(SurfaceType, PaintType, Location, Radius);
	}
}

void AAlsxtCharacter::VolumePaintMesh_Implementation(UPrimitiveComponent* Component, EPhysicalSurface SurfaceType, const FGameplayTag PaintType, FVector Origin, FVector Extent)
{
	if (Cast<UAlsxtPaintableSkeletalMeshComponent>(Component))
	{
		Cast<UAlsxtPaintableSkeletalMeshComponent>(Component)->VolumePaintMesh(SurfaceType, PaintType, Origin, Extent);
	}
	if (Cast<UAlsxtPaintableStaticMeshComponent>(Component))
	{
		Cast<UAlsxtPaintableStaticMeshComponent>(Component)->VolumePaintMesh(SurfaceType, PaintType, Origin, Extent);
	}
}

void AAlsxtCharacter::ResetPaintTypeOnComponent_Implementation(UPrimitiveComponent* Component, const FGameplayTag PaintType)
{
	if (Cast<UAlsxtPaintableSkeletalMeshComponent>(Component))
	{
		Cast<UAlsxtPaintableSkeletalMeshComponent>(Component)->ResetAllChannels();
	}
	if (Cast<UAlsxtPaintableStaticMeshComponent>(Component))
	{
		Cast<UAlsxtPaintableStaticMeshComponent>(Component)->ResetAllChannels();
	}
}

void AAlsxtCharacter::ResetPaintOnComponent_Implementation(UPrimitiveComponent* Component)
{
	if (Cast<UAlsxtPaintableSkeletalMeshComponent>(Component))
	{
		Cast<UAlsxtPaintableSkeletalMeshComponent>(Component)->ResetAllChannels();
	}
	if (Cast<UAlsxtPaintableStaticMeshComponent>(Component))
	{
		Cast<UAlsxtPaintableStaticMeshComponent>(Component)->ResetAllChannels();
	}
}

void AAlsxtCharacter::ResetPaintTypeOnAllComponents_Implementation(const FGameplayTag PaintType)
{

}

void AAlsxtCharacter::ResetPaintOnAllComponents_Implementation()
{
	
}

// PhysicalAnimation State
void AAlsxtCharacter::SetPhysicalAnimationState(const FAlsxtPhysicalAnimationState& NewPhysicalAnimationState)
{
	// if (HasAuthority())
	// {
	// 	ServerSetFootprintsState(Foot, NewFootprintsState);
	// }
	// else
	// {
	// 	MulticastSetFootprintsState(Foot, NewFootprintsState);
	// }
	
	const auto PreviousPhysicalAnimationState{ PhysicalAnimationState };
	PhysicalAnimationState = NewPhysicalAnimationState;
	// OnPhysicalAnimationStateChanged(PreviousPhysicalAnimationState);
	// 
	// if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	// {
	// 	ServerSetPhysicalAnimationState(NewPhysicalAnimationState);
	// }
}

void AAlsxtCharacter::OnPhysicalAnimationStateChanged(const FAlsxtPhysicalAnimationState& PreviousPhysicalAnimationState)
{

}

void AAlsxtCharacter::ServerSetPhysicalAnimationState_Implementation(const FAlsxtPhysicalAnimationState& NewPhysicalAnimationState)
{
	MulticastSetPhysicalAnimationState(NewPhysicalAnimationState);
}

void AAlsxtCharacter::MulticastSetPhysicalAnimationState_Implementation(const FAlsxtPhysicalAnimationState& NewPhysicalAnimationState)
{
	SetPhysicalAnimationState(NewPhysicalAnimationState);
}

void AAlsxtCharacter::ServerProcessNewPhysicalAnimationState_Implementation(const FAlsxtPhysicalAnimationState& NewPhysicalAnimationState)
{
	ProcessNewPhysicalAnimationState(NewPhysicalAnimationState);
}

// void AALSXTCharacter::OnReplicate_PhysicalAnimationState(const FALSXTPhysicalAnimationState& PreviousPhysicalAnimationState)
// {
// 	OnPhysicalAnimationStateChanged(PreviousPhysicalAnimationState);
// }

// Held Item State
void AAlsxtCharacter::SetHeldItemState(const FAlsxtHeldItemState& NewHeldItemState)
{
	const auto PreviousHeldItemState{ HeldItemState };
	HeldItemState = NewHeldItemState;
	OnHeldItemStateChanged(PreviousHeldItemState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetHeldItemState(NewHeldItemState);
	}
}

void AAlsxtCharacter::OnHeldItemStateChanged(const FAlsxtHeldItemState& PreviousHeldItemState)
{

}

void AAlsxtCharacter::ServerSetHeldItemState_Implementation(const FAlsxtHeldItemState& NewHeldItemState)
{
	MulticastSetHeldItemState(NewHeldItemState);
}

void AAlsxtCharacter::MulticastSetHeldItemState_Implementation(const FAlsxtHeldItemState& NewHeldItemState)
{
	SetHeldItemState(NewHeldItemState);
}

void AAlsxtCharacter::ServerProcessNewHeldItemState_Implementation(const FAlsxtHeldItemState& NewHeldItemState)
{
	ProcessNewHeldItemState(NewHeldItemState);
}

void AAlsxtCharacter::OnReplicate_HeldItemState(const FAlsxtHeldItemState& PreviousHeldItemState)
{
	OnHeldItemStateChanged(PreviousHeldItemState);
}


// Breath State
void AAlsxtCharacter::SetBreathState(const FAlsxtBreathState& NewBreathState)
{
	const auto PreviousBreathState{ BreathState };
	BreathState = NewBreathState;
	OnBreathStateChanged(PreviousBreathState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetBreathState(NewBreathState);
	}

	// if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	// {
	// 	ServerSetBreathState(NewBreathState);
	// }

	// if (GetLocalRole() >= ROLE_Authority)
	// {
	// 	BreathState = NewBreathState;
	// }
	// else
	// {
	// 	ServerSetBreathState(NewBreathState);
	// }

	// MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, BreathState, this)

}

void AAlsxtCharacter::ServerSetBreathState_Implementation(const FAlsxtBreathState& NewBreathState)
{
	SetBreathState(NewBreathState);
}

void AAlsxtCharacter::ClientSetBreathState_Implementation(const FAlsxtBreathState& NewBreathState)
{
	SetBreathState(NewBreathState);
}

void AAlsxtCharacter::ServerProcessNewBreathState_Implementation(const FAlsxtBreathState& NewBreathState)
{
	ProcessNewBreathState(NewBreathState);
}

void AAlsxtCharacter::OnReplicate_BreathState(const FAlsxtBreathState& PreviousBreathState)
{
	OnBreathStateChanged(PreviousBreathState);
}

void AAlsxtCharacter::OnBreathStateChanged_Implementation(const FAlsxtBreathState& PreviousBreathState) {}

// ALSXT Pose State
void AAlsxtCharacter::SetALSXTPoseState(const FAlsxtPoseState& NewALSXTPoseState)
{
	const auto PreviousALSXTPoseState{ ALSXTPoseState };

	ALSXTPoseState = NewALSXTPoseState;

	OnALSXTPoseStateChanged(PreviousALSXTPoseState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetALSXTPoseState(NewALSXTPoseState);
	}
}

void AAlsxtCharacter::ServerSetALSXTPoseState_Implementation(const FAlsxtPoseState& NewALSXTPoseState)
{
	SetALSXTPoseState(NewALSXTPoseState);
}

void AAlsxtCharacter::ServerProcessNewALSXTPoseState_Implementation(const FAlsxtPoseState& NewALSXTPoseState)
{
	ProcessNewALSXTPoseState(NewALSXTPoseState);
}

void AAlsxtCharacter::OnReplicate_ALSXTPoseState(const FAlsxtPoseState& PreviousALSXTPoseState)
{
	OnALSXTPoseStateChanged(PreviousALSXTPoseState);
}

void AAlsxtCharacter::OnALSXTPoseStateChanged_Implementation(const FAlsxtPoseState& PreviousALSXTPoseState) {}


// Vaulting State
void AAlsxtCharacter::SetVaultingState(const FAlsxtVaultingState& NewVaultingState)
{
	const auto PreviousVaultingState{ VaultingState };

	VaultingState = NewVaultingState;

	OnVaultingStateChanged(PreviousVaultingState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetVaultingState(NewVaultingState);
	}
}

void AAlsxtCharacter::ServerSetVaultingState_Implementation(const FAlsxtVaultingState& NewVaultingState)
{
	SetVaultingState(NewVaultingState);
}


void AAlsxtCharacter::ServerProcessNewVaultingState_Implementation(const FAlsxtVaultingState& NewVaultingState)
{
	ProcessNewVaultingState(NewVaultingState);
}

void AAlsxtCharacter::OnReplicate_VaultingState(const FAlsxtVaultingState& PreviousVaultingState)
{
	OnVaultingStateChanged(PreviousVaultingState);
}

void AAlsxtCharacter::OnVaultingStateChanged_Implementation(const FAlsxtVaultingState& PreviousVaultingState) {}

// Stationary Mode State
void AAlsxtCharacter::SetStationaryModeState(const FAlsxtStationaryModeState& NewStationaryModeState)
{
	const auto PreviousStationaryModeState{ StationaryModeState };

	StationaryModeState = NewStationaryModeState;

	OnStationaryModeStateChanged(PreviousStationaryModeState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetStationaryModeState(NewStationaryModeState);
	}
}

void AAlsxtCharacter::ServerSetStationaryModeState_Implementation(const FAlsxtStationaryModeState& NewStationaryModeState)
{
	SetStationaryModeState(NewStationaryModeState);
}


void AAlsxtCharacter::ServerProcessNewStationaryModeState_Implementation(const FAlsxtStationaryModeState& NewStationaryModeState)
{
	ProcessNewStationaryModeState(NewStationaryModeState);
}

void AAlsxtCharacter::OnReplicate_StationaryModeState(const FAlsxtStationaryModeState& PreviousStationaryModeState)
{
	OnStationaryModeStateChanged(PreviousStationaryModeState);
}

void AAlsxtCharacter::OnStationaryModeStateChanged_Implementation(const FAlsxtStationaryModeState& PreviousStationaryModeState) {}

// Footprints State
void AAlsxtCharacter::ClientSetFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState)
{
	if (HasAuthority())
	{
		ServerSetFootprintsState(Foot, NewFootprintsState);
	}
	else
	{
		MulticastSetFootprintsState(Foot, NewFootprintsState);
	}
}

void AAlsxtCharacter::ResetFootSaturationTimeline_Implementation(const EAlsFootBone& Foot)
{
	GetFootTimeline(Foot)->SetPlaybackPosition(0.0, false, false);
}

void AAlsxtCharacter::MulticastSetFootprintsState_Implementation(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState)
{
	SetFootprintsState(Foot, NewFootprintsState);
}

void AAlsxtCharacter::SetFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState)
{
	const auto PreviousFootprintsState{ FootprintsState };

	FootprintsState = NewFootprintsState;

	OnFootprintsStateChanged(PreviousFootprintsState);
}

void AAlsxtCharacter::ServerSetFootprintsState_Implementation(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState)
{
	MulticastSetFootprintsState(Foot, NewFootprintsState);
}


void AAlsxtCharacter::ServerProcessNewFootprintsState_Implementation(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState)
{
	ProcessNewFootprintsState(Foot, NewFootprintsState);
}

void AAlsxtCharacter::OnReplicate_FootprintsState(const FAlsxtFootprintsState& PreviousFootprintsState)
{
	OnFootprintsStateChanged(PreviousFootprintsState);
}

void AAlsxtCharacter::OnFootprintsStateChanged_Implementation(const FAlsxtFootprintsState& PreviousFootprintsState) {}

void AAlsxtCharacter::InputToggleGait()
{
	if (CanToggleGait())
	{
		if ((GetDesiredGait() == AlsGaitTags::Walking))
		{
			SetDesiredGait(AlsGaitTags::Running);
		}
		else
		{
			SetDesiredGait(AlsGaitTags::Walking);
		}
	}
}

void AAlsxtCharacter::InputToggleCombatReady()
{
	if (CanToggleCombatReady())
	{
		if ((GetDesiredCombatStance() == FGameplayTag::EmptyTag) || (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral))
		{
			if (CanBecomeCombatReady())
			{
				SetDesiredCombatStance(ALSXTCombatStanceTags::Ready);
				if (IAlsxtHeldItemInterface::Execute_IsHoldingAimableItem(this))
				{
					if (GetRotationMode() != AlsRotationModeTags::Aiming)
					{
						SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::LowReady);
					}
					else
					{
						SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Ready);
					}
				}
				else
				{
					SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Ready);
				}
			}
		}
		else
		{
			SetDesiredCombatStance(ALSXTCombatStanceTags::Neutral);
			SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::PatrolReady);
		}
	}
}

void AAlsxtCharacter::InputBlock(const FInputActionValue& ActionValue)
{
	FAlsxtDefensiveModeState PreviousDefensiveModeState = GetDefensiveModeState();

	if (CanEnterBlockingDefensiveMode())
	{
		if (ActionValue.Get<bool>() == true)
		{
			FAlsxtDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
			NewDefensiveModeState.Mode = ALSXTDefensiveModeTags::Anticipation;
			NewDefensiveModeState.AnticipationMode = ALSXTDefensiveModeTags::Anticipation;
			NewDefensiveModeState.AnticipationSide = PreviousDefensiveModeState.AnticipationSide == FGameplayTag::EmptyTag ? ALSXTImpactSideTags::Front : PreviousDefensiveModeState.AnticipationSide;
			NewDefensiveModeState.AnticipationHeight = PreviousDefensiveModeState.AnticipationHeight == FGameplayTag::EmptyTag ? ALSXTImpactHeightTags::Middle : PreviousDefensiveModeState.AnticipationHeight;
			NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;
			SetDefensiveModeState(NewDefensiveModeState);
			// SetDesiredDefensiveMode(ALSXTDefensiveModeTags::Blocking);

		}
		else 
		{
			// ResetDefensiveModeState();
			FAlsxtDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
			NewDefensiveModeState.Mode = PreviousDefensiveModeState.Mode == ALSXTDefensiveModeTags::Anticipation ? FGameplayTag::EmptyTag : PreviousDefensiveModeState.Mode;
			NewDefensiveModeState.AnticipationMode = FGameplayTag::EmptyTag;
			NewDefensiveModeState.AnticipationSide = FGameplayTag::EmptyTag;
			NewDefensiveModeState.AnticipationHeight = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;
			SetDefensiveModeState(NewDefensiveModeState);
			// SetDesiredDefensiveMode(FGameplayTag::EmptyTag);
		}
	}
	else if ((DesiredDefensiveMode == ALSXTDefensiveModeTags::Blocking) && (ActionValue.Get<bool>()  == false))
	{
		FAlsxtDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
		NewDefensiveModeState.Mode = PreviousDefensiveModeState.Mode == ALSXTDefensiveModeTags::Anticipation ? FGameplayTag::EmptyTag : PreviousDefensiveModeState.Mode;
		NewDefensiveModeState.AnticipationMode = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationSide = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationHeight = FGameplayTag::EmptyTag;
		NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
		NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
		NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;
		SetDefensiveModeState(NewDefensiveModeState);
		SetDesiredDefensiveMode(FGameplayTag::EmptyTag);
	}
}

bool AAlsxtCharacter::CanPerformPrimaryInteraction_Implementation() const
{
	return true;
}

bool AAlsxtCharacter::CanPerformSecondaryInteraction_Implementation() const
{
	return true;
}

void AAlsxtCharacter::InputSwitchWeaponFirearmStance()
{
	if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral)
	{
		return;
	}
	else
	{
		FGameplayTagContainer AvailableStances = GetAvailableFirearmStances();
		TArray<FGameplayTag> AvailableStancesArray;
		AvailableStances.GetGameplayTagArray(AvailableStancesArray);
		if (AvailableStances.Num() <= 0 || AvailableStances.Num() == 1 && AvailableStancesArray[0] == GetDesiredWeaponFirearmStance())
		{
			return;
		}
		int CurrentIndex = AvailableStancesArray.IndexOfByKey(GetDesiredWeaponFirearmStance());
		int NextIndex;
		if ((CurrentIndex + 1) > (AvailableStancesArray.Num() - 1))
		{
			NextIndex = 0;
		}
		else
		{
			NextIndex = CurrentIndex + 1;
		}
		SetDesiredWeaponFirearmStance(AvailableStancesArray[NextIndex]);
	}
}

void AAlsxtCharacter::InputSwitchWeaponReadyPosition()
{
	TArray<FGameplayTag> AvailablePositions;
	// TArray<FGameplayTag> AvailablePositions = (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral) ? {ALSXTWeaponReadyPositionTags::PatrolReady, ALSXTWeaponReadyPositionTags::HighReady, ALSXTWeaponReadyPositionTags::LowReady, ALSXTWeaponReadyPositionTags::Hidden } : { ALSXTWeaponReadyPositionTags::Ready, ALSXTWeaponReadyPositionTags::Retention, ALSXTWeaponReadyPositionTags::Hip, ALSXTWeaponReadyPositionTags::PatrolReady, ALSXTWeaponReadyPositionTags::HighReady, ALSXTWeaponReadyPositionTags::LowReady, ALSXTWeaponReadyPositionTags::Hidden };
	if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral)
	{
		AvailablePositions = {ALSXTWeaponReadyPositionTags::PatrolReady, ALSXTWeaponReadyPositionTags::HighReady, ALSXTWeaponReadyPositionTags::LowReady, ALSXTWeaponReadyPositionTags::Hidden };
	}
	else
	{
		AvailablePositions = { ALSXTWeaponReadyPositionTags::Ready, ALSXTWeaponReadyPositionTags::Retention, ALSXTWeaponReadyPositionTags::Hip, ALSXTWeaponReadyPositionTags::PatrolReady, ALSXTWeaponReadyPositionTags::HighReady, ALSXTWeaponReadyPositionTags::LowReady, ALSXTWeaponReadyPositionTags::Hidden };
	}
	int CurrentIndex = AvailablePositions.IndexOfByKey(GetDesiredWeaponReadyPosition());
	int NextIndex = (CurrentIndex + 1) > (AvailablePositions.Num() - 1) ? 0 : CurrentIndex + 1;

	// if ((CurrentIndex + 1) > (AvailablePositions.Num() - 1))
	// {
	// 	NextIndex = 0;
	// }
	// else
	// {
	// 	NextIndex = CurrentIndex + 1;
	// }
	SetDesiredWeaponReadyPosition(AvailablePositions[NextIndex]);
}

void AAlsxtCharacter::OnFirstPersonOverrideChanged_Implementation(float FirstPersonOverride)
{
	//
}

void AAlsxtCharacter::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused, float& VerticalLocation)
{
	if (Camera->IsActive())
	{
		Camera->DisplayDebug(Canvas, DebugDisplay, VerticalLocation);
	}

	Super::DisplayDebug(Canvas, DebugDisplay, Unused, VerticalLocation);
}

// First Person Eye Focus

bool AAlsxtCharacter::IsFirstPersonEyeFocusActive() const
{
	if (GetViewMode() == AlsViewModeTags::FirstPerson) 
	{
		if (GetDesiredFocus() == ALSXTFocusedTags::True) 
		{
			if (IAlsxtHeldItemInterface::Execute_IsHoldingAimableItem(this))
			{
				if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

// Lean

void AAlsxtCharacter::SetDesiredLean(const FGameplayTag& NewLeanTag)
{
	if (DesiredLean != NewLeanTag)
	{
		DesiredLean = NewLeanTag;
		const auto PreviousLean{ Lean };

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredLean, this)

		if (GetLocalRole() == ROLE_AutonomousProxy)
		{
			ServerSetDesiredLean(NewLeanTag);
		}
		else if (GetLocalRole() == ROLE_Authority)
		{
			OnLeanChanged(PreviousLean);
		}
	}
}

void AAlsxtCharacter::ServerSetDesiredLean_Implementation(const FGameplayTag& NewLeanTag)
{
	SetDesiredLean(NewLeanTag);
}

void AAlsxtCharacter::SetLean(const FGameplayTag& NewLeanTag)
{

	if (Lean != NewLeanTag)
	{
		const auto PreviousLean{ Lean };

		Lean = NewLeanTag;

		OnLeanChanged(PreviousLean);
	}
}

void AAlsxtCharacter::OnLeanChanged_Implementation(const FGameplayTag& PreviousLeanTag) {}

// Freelooking

void AAlsxtCharacter::IsFreelooking(bool& bIsFreelooking, bool& bIsFreelookingInFirstPerson) const
{

	bIsFreelooking = (GetDesiredFreelooking() == ALSXTFreelookingTags::True) ? true : false;
	bIsFreelookingInFirstPerson = (bIsFreelooking && ((GetViewMode() == AlsViewModeTags::FirstPerson) ? true : false));
}

void AAlsxtCharacter::ActivateFreelooking()
{
	FAlsxtFreelookState NewFreelookState = GetFreelookState();

	NewFreelookState.bFreelooking = true;
	NewFreelookState.Freelooking = ALSXTFreelookingTags::True;
	
	// Lock the current movement reference to the current actor yaw
	NewFreelookState.LockedControlRotation = FRotator(0, GetActorRotation().Yaw, 0);

	//PreviousYaw = GetViewState().Rotation.Yaw;
	// PreviousYaw = FMath::GetMappedRangeValueClamped(FVector2D(0, 359.998993), FVector2D(0.0, 1.0), GetControlRotation().Yaw);
	//FMath::GetMappedRangeValueClamped(FVector2D(-90,90), FVector2D(0,1), GetViewState().Rotation.Pitch)
	// PreviousPitch = FMath::GetMappedRangeValueClamped(FVector2D(89.900002, -89.899994), FVector2D(0.0, 1.0), GetViewState().Rotation.Pitch);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f"), GetControlRotation().Yaw));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%f"), PreviousYaw));
	// LockRotation(GetActorRotation().Yaw);
	SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
	SetDesiredFreelooking(ALSXTFreelookingTags::True);
	// NewFreelookState.Freelooking = ALSXTFreelookingTags::True;
	// NewFreelookState.LockedControlRotation = GetControlRotation();
	// NewFreelookState.LockedViewState = GetViewState();
	SetFreelookState(NewFreelookState);
}

void AAlsxtCharacter::DeactivateFreelooking()
{
	FAlsxtFreelookState NewFreelookState = GetFreelookState();

	NewFreelookState.bFreelooking = false;
	NewFreelookState.Freelooking = ALSXTFreelookingTags::False;
	// UnLockRotation();
	SetDesiredFreelooking(ALSXTFreelookingTags::False);
	SetFreelookState(NewFreelookState);
	// HeadLookAtState.LockedViewState.PreviousYawAngle = 0.0f;
	// HeadLookAtState.LockedViewState.Rotation = FRotator(0.0f, 0.0f, 0.0f);
	// HeadLookAtState.LockedViewState.YawSpeed = 0.0f;
}

void AAlsxtCharacter::SetDesiredFreelooking(const FGameplayTag& NewFreelookingTag)
{
	if (DesiredFreelooking != NewFreelookingTag)
	{
		DesiredFreelooking = NewFreelookingTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredFreelooking, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredFreelooking(NewFreelookingTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredFreelooking_Implementation(const FGameplayTag& NewFreelookingTag)
{
	SetDesiredFreelooking(NewFreelookingTag);
}

void AAlsxtCharacter::SetFreelooking(const FGameplayTag& NewFreelookingTag)
{

	if (Freelooking != NewFreelookingTag)
	{
		const auto PreviousFreelooking{ Freelooking };

		Freelooking = NewFreelookingTag;
		FAlsxtFreelookState NewState = GetFreelookState();

		if (NewFreelookingTag == ALSXTFreelookingTags::True)
		{
			// FreelookTimerHandle.IsValid()
			
			NewState.Freelooking = ALSXTFreelookingTags::True;
			SetFreelookState(NewState);
			BeginFreelookTimer();
		}
		if (NewFreelookingTag == ALSXTFreelookingTags::False)
		{
			NewState.Freelooking = ALSXTFreelookingTags::False;
			SetFreelookState(NewState);
			EndFreelookTimer();
		}

		OnFreelookingChanged(PreviousFreelooking);
	}
}

void AAlsxtCharacter::OnFreelookingChanged_Implementation(const FGameplayTag& PreviousFreelookingTag) 
{
	
}

// Sex

void AAlsxtCharacter::SetDesiredSex(const FGameplayTag& NewSexTag)
{
	if (DesiredSex != NewSexTag)
	{
		DesiredSex = NewSexTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredSex, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredSex(NewSexTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredSex_Implementation(const FGameplayTag& NewSexTag)
{
	SetDesiredSex(NewSexTag);
}

void AAlsxtCharacter::SetSex(const FGameplayTag& NewSexTag)
{

	if (Sex != NewSexTag)
	{
		const auto PreviousSex{ Sex };

		Sex = NewSexTag;

		OnSexChanged(PreviousSex);
	}
}

void AAlsxtCharacter::OnSexChanged_Implementation(const FGameplayTag& PreviousSexTag) {}

// LocomotionVariant

void AAlsxtCharacter::SetDesiredLocomotionVariant(const FGameplayTag& NewLocomotionVariantTag)
{
	if (DesiredLocomotionVariant != NewLocomotionVariantTag)
	{
		DesiredLocomotionVariant = NewLocomotionVariantTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredLocomotionVariant, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredLocomotionVariant(NewLocomotionVariantTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredLocomotionVariant_Implementation(const FGameplayTag& NewLocomotionVariantTag)
{
	SetDesiredLocomotionVariant(NewLocomotionVariantTag);
}

void AAlsxtCharacter::SetLocomotionVariant(const FGameplayTag& NewLocomotionVariantTag)
{

	if (LocomotionVariant != NewLocomotionVariantTag)
	{
		const auto PreviousLocomotionVariant{ LocomotionVariant };

		LocomotionVariant = NewLocomotionVariantTag;

		OnLocomotionVariantChanged(PreviousLocomotionVariant);
	}
}

void AAlsxtCharacter::OnLocomotionVariantChanged_Implementation(const FGameplayTag& PreviousLocomotionVariantTag) {}

// Injury

void AAlsxtCharacter::SetDesiredInjury(const FGameplayTag& NewInjuryTag)
{
	if (DesiredInjury != NewInjuryTag)
	{
		DesiredInjury = NewInjuryTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredInjury, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredInjury(NewInjuryTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredInjury_Implementation(const FGameplayTag& NewInjuryTag)
{
	SetDesiredInjury(NewInjuryTag);
}

void AAlsxtCharacter::SetInjury(const FGameplayTag& NewInjuryTag)
{

	if (Injury != NewInjuryTag)
	{
		const auto PreviousInjury{ Injury };

		Injury = NewInjuryTag;

		OnInjuryChanged(PreviousInjury);
	}
}

void AAlsxtCharacter::OnInjuryChanged_Implementation(const FGameplayTag& PreviousInjuryTag) {}

// CombatStance

void AAlsxtCharacter::SetDesiredReadyStance(const FGameplayTag& NewReadyStanceTag)
{
	if (DesiredReadyStance != NewReadyStanceTag)
	{
		DesiredReadyStance = NewReadyStanceTag;
		const auto PreviousReadyStance{ ReadyStance };

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredReadyStance, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredReadyStance(NewReadyStanceTag);
				if (NewReadyStanceTag != ALSXTCombatStanceTags::Neutral)
				{
					if (IAlsxtHeldItemInterface::Execute_IsHoldingAimableItem(this))
					{
						SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::LowReady);
					}
					else
					{
						SetDesiredWeaponReadyPosition(ALSXTWeaponReadyPositionTags::Ready);
					}
					SetDesiredRotationMode(AlsRotationModeTags::Aiming);
				}
				else
				{
					SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
				}
			}
			else if (GetLocalRole() == ROLE_Authority)
			{
				OnReadyStanceChanged(PreviousReadyStance);
			}
	}
}

void AAlsxtCharacter::SetDesiredCombatStance(const FGameplayTag& NewCombatStanceTag)
{
	if (DesiredCombatStance != NewCombatStanceTag)
	{
		DesiredCombatStance = NewCombatStanceTag;
		const auto PreviousCombatStance{ CombatStance };

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredCombatStance, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredCombatStance(NewCombatStanceTag);
			}
			else if (GetLocalRole() == ROLE_Authority)
			{
				OnCombatStanceChanged(PreviousCombatStance);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredReadyStance_Implementation(const FGameplayTag& NewReadyStanceTag)
{
	SetDesiredReadyStance(NewReadyStanceTag);
}

void AAlsxtCharacter::ServerSetDesiredCombatStance_Implementation(const FGameplayTag& NewCombatStanceTag)
{
	SetDesiredCombatStance(NewCombatStanceTag);
}

void AAlsxtCharacter::SetReadyStance(const FGameplayTag& NewReadyStanceTag)
{

	if (CombatStance != NewReadyStanceTag)
	{
		const auto PreviousReadyStance{ ReadyStance };

		CombatStance = NewReadyStanceTag;

		OnReadyStanceChanged(PreviousReadyStance);
	}
}

bool AAlsxtCharacter::CanCharacterSwitchCombatStance_Implementation() const
{
	return true;
}

void AAlsxtCharacter::SetCharacterCameraRightShoulder_Implementation(const bool NewCameraRightShoulder)
{
	Camera->SetRightShoulder(!Camera->IsRightShoulder());
}

void AAlsxtCharacter::SetCombatStance(const FGameplayTag& NewCombatStanceTag)
{

	if (CombatStance != NewCombatStanceTag)
	{
		const auto PreviousCombatStance{ CombatStance };

		CombatStance = NewCombatStanceTag;

		OnCombatStanceChanged(PreviousCombatStance);
	}
}

void AAlsxtCharacter::OnReadyStanceChanged_Implementation(const FGameplayTag& PreviousCombatReadyTag)
{
	// auto NewAnimationParametersState = AAlsxtCharacter::Execute_GetCharacterAnimationParametersState(this);
	FGameplayTag NewReadinessReadyTag = AAlsxtCharacter::Execute_GetCharacterReadiness(this);
	// NewAnimationParametersState.StanceTags.AddTag(NewReadinessReadyTag);
	// SetAnimationParametersState(NewAnimationParametersState);
	
	FGameplayTagContainer TagsToRemove = UGameplayTagsManager::Get().RequestGameplayTagChildren(FGameplayTag::RequestGameplayTag(FName("Alsxt.Readiness")));
	AnimationParametersState.StanceTags.RemoveTags(TagsToRemove);
	AnimationParametersState.StanceTags.AddTag(NewReadinessReadyTag);
}

void AAlsxtCharacter::OnCombatStanceChanged_Implementation(const FGameplayTag& PreviousCombatStanceTag)
{
	// auto NewAnimationParametersState = AAlsxtCharacter::Execute_GetCharacterAnimationParametersState(this);
	FGameplayTag NewCombatStanceTag = AAlsxtCharacter::Execute_GetCharacterCombatStance(this);
	// NewAnimationParametersState.StanceTags.AddTag(NewCombatStanceTag);
	// SetAnimationParametersState(NewAnimationParametersState);

	FGameplayTagContainer TagsToRemove = UGameplayTagsManager::Get().RequestGameplayTagChildren(FGameplayTag::RequestGameplayTag(FName("Als.Combat Stance")));
	AnimationParametersState.StanceTags.RemoveTags(TagsToRemove);
	AnimationParametersState.StanceTags.AddTag(NewCombatStanceTag);
}

// WeaponFirearmStance

void AAlsxtCharacter::SetDesiredWeaponFirearmStance(const FGameplayTag& NewWeaponFirearmStanceTag)
{
	if (DesiredWeaponFirearmStance != NewWeaponFirearmStanceTag)
	{
		DesiredWeaponFirearmStance = NewWeaponFirearmStanceTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredWeaponFirearmStance, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredWeaponFirearmStance(NewWeaponFirearmStanceTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredWeaponFirearmStance_Implementation(const FGameplayTag& NewWeaponFirearmStanceTag)
{
	SetDesiredWeaponFirearmStance(NewWeaponFirearmStanceTag);
}

void AAlsxtCharacter::SetWeaponFirearmStance(const FGameplayTag& NewWeaponFirearmStanceTag)
{

	if (WeaponFirearmStance != NewWeaponFirearmStanceTag)
	{
		const auto PreviousWeaponFirearmStance{ WeaponFirearmStance };

		WeaponFirearmStance = NewWeaponFirearmStanceTag;

		OnWeaponFirearmStanceChanged(PreviousWeaponFirearmStance);
	}
}

void AAlsxtCharacter::OnWeaponFirearmStanceChanged_Implementation(const FGameplayTag& PreviousWeaponFirearmStanceTag) {}

// WeaponReadyPosition

void AAlsxtCharacter::SetDesiredWeaponReadyPosition(const FGameplayTag& NewWeaponReadyPositionTag)
{
	if (DesiredWeaponReadyPosition != NewWeaponReadyPositionTag)
	{
		DesiredWeaponReadyPosition = NewWeaponReadyPositionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredWeaponReadyPosition, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredWeaponReadyPosition(NewWeaponReadyPositionTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredWeaponReadyPosition_Implementation(const FGameplayTag& NewWeaponReadyPositionTag)
{
	SetDesiredWeaponReadyPosition(NewWeaponReadyPositionTag);
}

void AAlsxtCharacter::SetWeaponReadyPosition(const FGameplayTag& NewWeaponReadyPositionTag)
{

	if (WeaponReadyPosition != NewWeaponReadyPositionTag)
	{
		const auto PreviousWeaponReadyPosition{ WeaponReadyPosition };

		WeaponReadyPosition = NewWeaponReadyPositionTag;

		OnWeaponReadyPositionChanged(PreviousWeaponReadyPosition);
	}
}

// DefensiveMode

bool AAlsxtCharacter::IsInDefensiveMode() const
{
	if (GetDefensiveMode() != FGameplayTag::EmptyTag)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool AAlsxtCharacter::IsInAnticipationMode() const
{
	if (GetDefensiveMode() == ALSXTDefensiveModeTags::Anticipation)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AAlsxtCharacter::SetDesiredDefensiveMode(const FGameplayTag& NewDefensiveModeTag)
{
	if (DesiredDefensiveMode != NewDefensiveModeTag)
	{
		DesiredDefensiveMode = NewDefensiveModeTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredDefensiveMode, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredDefensiveMode(NewDefensiveModeTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredDefensiveMode_Implementation(const FGameplayTag& NewDefensiveModeTag)
{
	SetDesiredDefensiveMode(NewDefensiveModeTag);
}

void AAlsxtCharacter::SetDefensiveMode(const FGameplayTag& NewDefensiveModeTag)
{
	if (DefensiveMode != NewDefensiveModeTag)
	{
		const auto PreviousDefensiveMode{ DefensiveMode };
		DefensiveMode = NewDefensiveModeTag;
		OnDefensiveModeChanged(PreviousDefensiveMode);

		if (DefensiveMode == ALSXTDefensiveModeTags::Anticipation)
		{

		}
		if (DefensiveMode == ALSXTDefensiveModeTags::Blocking)
		{

		}
	}
}

void AAlsxtCharacter::OnDefensiveModeChanged_Implementation(const FGameplayTag& PreviousDefensiveModeTag) {}

void AAlsxtCharacter::SetAimState(const FAlsxtAimState& NewAimState)
{
	const auto PreviousAimState{ AimState };

	AimState = NewAimState;

	OnAimStateChanged(PreviousAimState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetAimState(NewAimState);
	}
}

void AAlsxtCharacter::ServerSetAimState_Implementation(const FAlsxtAimState& NewAimState)
{
	SetAimState(NewAimState);
}


void AAlsxtCharacter::ServerProcessNewAimState_Implementation(const FAlsxtAimState& NewAimState)
{
	ProcessNewAimState(NewAimState);
}

void AAlsxtCharacter::OnReplicate_AimState(const FAlsxtAimState& PreviousAimState)
{
	OnAimStateChanged(PreviousAimState);
}

void AAlsxtCharacter::OnAimStateChanged_Implementation(const FAlsxtAimState& PreviousAimState) {}


//Freelooking


void AAlsxtCharacter::CharacterActivateFreelooking_Implementation()
{
	IAlsxtCharacterInterface::CharacterActivateFreelooking_Implementation();

	FAlsxtFreelookState NewFreelookState = GetFreelookState();
	NewFreelookState.bFreelooking = true;
	NewFreelookState.Freelooking = ALSXTFreelookingTags::True;
	
	// Lock the input direction to the character's current facing or velocity direction
	NewFreelookState.LockedControlRotation = GetActorRotation(); 
	SetFreelookState(NewFreelookState);
}

void AAlsxtCharacter::CharacterDeactivateFreelooking_Implementation()
{
	IAlsxtCharacterInterface::CharacterDeactivateFreelooking_Implementation();

	FAlsxtFreelookState NewFreelookState = GetFreelookState();
	NewFreelookState.bFreelooking = false;
	NewFreelookState.Freelooking = ALSXTFreelookingTags::False;
	SetFreelookState(NewFreelookState);
}

void AAlsxtCharacter::SetFreelookState(const FAlsxtFreelookState& NewFreelookState)
{
	const auto PreviousFreelookState{ FreelookState };
	FreelookState = NewFreelookState;
	OnFreelookStateChanged(PreviousFreelookState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetFreelookState(NewFreelookState);
	}
}

bool AAlsxtCharacter::DoesOverlayObjectUseLeftHandIK_Implementation() const
{
	return false;
}

void AAlsxtCharacter::ServerSetFreelookState_Implementation(const FAlsxtFreelookState& NewFreelookState)
{
	SetFreelookState(NewFreelookState);
}


void AAlsxtCharacter::ServerProcessNewFreelookState_Implementation(const FAlsxtFreelookState& NewFreelookState)
{
	ProcessNewFreelookState(NewFreelookState);
}

void AAlsxtCharacter::OnReplicate_FreelookState(const FAlsxtFreelookState& PreviousFreelookState)
{
	OnFreelookStateChanged(PreviousFreelookState);
}

void AAlsxtCharacter::OnFreelookStateChanged_Implementation(const FAlsxtFreelookState& PreviousFreelookState) {}

// Head Look At State

bool AAlsxtCharacter::CanHeadLookAt_Implementation() const { return (GetDesiredFreelooking() == ALSXTFreelookingTags::False); };

void AAlsxtCharacter::SetHeadLookAtState(const FAlsxtHeadLookAtState& NewHeadLookAtState)
{
	const auto PreviousHeadLookAtState{ HeadLookAtState };
	HeadLookAtState = NewHeadLookAtState;
	OnHeadLookAtStateChanged(PreviousHeadLookAtState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetHeadLookAtState(NewHeadLookAtState);
	}
}

void AAlsxtCharacter::ServerSetHeadLookAtState_Implementation(const FAlsxtHeadLookAtState& NewHeadLookAtState)
{
	SetHeadLookAtState(NewHeadLookAtState);
}

void AAlsxtCharacter::ServerProcessNewHeadLookAtState_Implementation(const FAlsxtHeadLookAtState& NewHeadLookAtState)
{
	ProcessNewHeadLookAtState(NewHeadLookAtState);
}

void AAlsxtCharacter::AddHeadLookAtEntry_Implementation(FAlsxtHeadLookAtEntry HeadLookAtEntry)
{
	HeadLookAtEntries.Add(HeadLookAtEntry);
}

void AAlsxtCharacter::RemoveHeadLookAtEntry_Implementation(FAlsxtHeadLookAtEntry HeadLookAtEntry)
{
	HeadLookAtEntries.Remove(HeadLookAtEntry);
}

void AAlsxtCharacter::BeginHeadLookAt_Implementation(FAlsxtHeadLookAtEntry HeadLookAtEntry)
{
	IAlsxtHeadLookAtInterface::Execute_GetBestHeadLookAtEntry(this);
}

void AAlsxtCharacter::EndHeadLookAt_Implementation()
{

}

void AAlsxtCharacter::OnReplicate_HeadLookAtState(const FAlsxtHeadLookAtState& PreviousHeadLookAtState)
{
	OnHeadLookAtStateChanged(PreviousHeadLookAtState);
}

void AAlsxtCharacter::OnHeadLookAtStateChanged_Implementation(const FAlsxtHeadLookAtState& PreviousHeadLookAtState) {}

//

void AAlsxtCharacter::SetSlidingState(const FAlsxtSlidingState& NewSlidingState)
{
	const auto PreviousSlidingState{ SlidingState };

	SlidingState = NewSlidingState;

	OnSlidingStateChanged(PreviousSlidingState);

	// if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	// {
	// 	ServerSetSlidingState(NewSlidingState);
	// }
}

void AAlsxtCharacter::ServerSetSlidingState_Implementation(const FAlsxtSlidingState& NewSlidingState)
{
	// SetSlidingState(NewSlidingState);
	MulticastSetSlidingState(NewSlidingState);
}


void AAlsxtCharacter::ServerProcessNewSlidingState_Implementation(const FAlsxtSlidingState& NewSlidingState)
{
	ProcessNewSlidingState(NewSlidingState);
}

void AAlsxtCharacter::OnReplicate_SlidingState(const FAlsxtSlidingState& PreviousSlidingState)
{
	OnSlidingStateChanged(PreviousSlidingState);
}

// Sliding State
void AAlsxtCharacter::ClientSetSlidingState(const FAlsxtSlidingState& NewSlidingState)
{
	if (HasAuthority())
	{
		ServerSetSlidingState(NewSlidingState);
	}
	else
	{
		MulticastSetSlidingState(NewSlidingState);
	}
}

void AAlsxtCharacter::MulticastSetSlidingState_Implementation(const FAlsxtSlidingState& NewSlidingState)
{
	SetSlidingState(NewSlidingState);
}

void AAlsxtCharacter::OnSlidingStateChanged_Implementation(const FAlsxtSlidingState& PreviousSlidingState) {}

//

void AAlsxtCharacter::SetDefensiveModeState(const FAlsxtDefensiveModeState& NewDefensiveModeState)
{
	// const auto PreviousDefensiveModeState{ DefensiveModeState };
	// if (GetLocalRole() == ROLE_AutonomousProxy)
	// {
	// 	// MulticastSetDefensiveModeState(NewDefensiveModeState);
	// 	ServerSetDefensiveModeState(NewDefensiveModeState);
	// 
	// }
	// else if (GetLocalRole() == ROLE_Authority)
	// {
	// 	SetDefensiveModeStateImplementation(NewDefensiveModeState);
	// 	// OnDefensiveModeStateChanged(PreviousDefensiveModeState);
	// }
	DefensiveModeState = NewDefensiveModeState;
	const auto PreviousDefensiveModeState{ DefensiveModeState };	
	OnDefensiveModeStateChanged(PreviousDefensiveModeState);
	// MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DefensiveModeState, this);

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		// MulticastSetDefensiveModeState(NewDefensiveModeState);
		ServerSetDefensiveModeState(NewDefensiveModeState);

	}
	// else if (GetLocalRole() == ROLE_Authority)
	// {
	// 	OnDefensiveModeStateChanged(PreviousDefensiveModeState);
	// }

	// ForceNetUpdate();
}

void AAlsxtCharacter::SetDefensiveModeAnimations(const FAlsxtDefensiveModeAnimations& NewDefensiveModeAnimations)
{
	// const auto PreviousDefensiveModeState{ DefensiveModeState };
	// if (GetLocalRole() == ROLE_AutonomousProxy)
	// {
	// 	// MulticastSetDefensiveModeState(NewDefensiveModeState);
	// 	ServerSetDefensiveModeState(NewDefensiveModeState);
	// 
	// }
	// else if (GetLocalRole() == ROLE_Authority)
	// {
	// 	SetDefensiveModeStateImplementation(NewDefensiveModeState);
	// 	// OnDefensiveModeStateChanged(PreviousDefensiveModeState);
	// }
	DefensiveModeAnimations = NewDefensiveModeAnimations;
	const auto PreviousDefensiveModeState{ DefensiveModeAnimations };	
	// OnDefensiveModeStateChanged(PreviousDefensiveModeState);
	// MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DefensiveModeState, this);

	// if (GetLocalRole() == ROLE_AutonomousProxy)
	// {
	// 	// MulticastSetDefensiveModeState(NewDefensiveModeState);
	// 	ServerSetDefensiveModeState(NewDefensiveModeState);
	// 
	// }



	// else if (GetLocalRole() == ROLE_Authority)
	// {
	// 	OnDefensiveModeStateChanged(PreviousDefensiveModeState);
	// }

	// ForceNetUpdate();
}

void AAlsxtCharacter::SetDefensiveModeStateImplementation(const FAlsxtDefensiveModeState& NewDefensiveModeState)
{
	// const auto PreviousDefensiveModeState{ DefensiveModeState };
	// DefensiveModeState = NewDefensiveModeState;
	// OnDefensiveModeStateChanged(PreviousDefensiveModeState);
	// MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DefensiveModeState, this)

	DefensiveModeState = NewDefensiveModeState;
	const auto PreviousDefensiveModeState{ DefensiveModeState };
	// MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DefensiveModeState, this)
	OnDefensiveModeStateChanged(PreviousDefensiveModeState);

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		// MulticastSetDefensiveModeState(NewDefensiveModeState);
		ServerSetDefensiveModeState(NewDefensiveModeState);

	}
	else if (GetLocalRole() == ROLE_Authority)
	{
		OnDefensiveModeStateChanged(PreviousDefensiveModeState);
	}

	ForceNetUpdate();

	// if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	// {
	// 	ServerSetDefensiveModeState(NewDefensiveModeState);
	// }
}

void AAlsxtCharacter::ResetDefensiveModeState()
{
	// Reset but keep Nontage
	FAlsxtDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
	FAlsxtDefensiveModeState PreviousDefensiveModeState = GetDefensiveModeState();
	// NewDefensiveModeState.Mode = FGameplayTag::EmptyTag;
	// NewDefensiveModeState.Form = FGameplayTag::EmptyTag;
	// NewDefensiveModeState.Velocity = FGameplayTag::EmptyTag;
	// DefensiveModeState = NewDefensiveModeState;
	// OnDefensiveModeStateChanged(PreviousDefensiveModeState);
	// 
	// if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	// {
	// 	ServerSetDefensiveModeState(NewDefensiveModeState);
	// }
}

void AAlsxtCharacter::ServerSetDefensiveModeState_Implementation(const FAlsxtDefensiveModeState& NewDefensiveModeState)
{
	// MulticastSetDefensiveModeState(NewDefensiveModeState);
	SetDefensiveModeState(NewDefensiveModeState);
}


void AAlsxtCharacter::ServerProcessNewDefensiveModeState_Implementation(const FAlsxtDefensiveModeState& NewDefensiveModeState)
{
	ProcessNewDefensiveModeState(NewDefensiveModeState);
}

void AAlsxtCharacter::OnReplicate_DefensiveModeState(const FAlsxtDefensiveModeState& PreviousDefensiveModeState)
{
	OnDefensiveModeStateChanged(PreviousDefensiveModeState);
}

// Defensive ModeState
void AAlsxtCharacter::ClientSetDefensiveModeState(const FAlsxtDefensiveModeState& NewDefensiveModeState)
{
	if (HasAuthority())
	{
		ServerSetDefensiveModeState(NewDefensiveModeState);
	}
	else
	{
		MulticastSetDefensiveModeState(NewDefensiveModeState);
	}
}

void AAlsxtCharacter::MulticastSetDefensiveModeState_Implementation(const FAlsxtDefensiveModeState& NewDefensiveModeState)
{
	SetDefensiveModeState(NewDefensiveModeState);
}

void AAlsxtCharacter::OnDefensiveModeStateChanged_Implementation(const FAlsxtDefensiveModeState& PreviousDefensiveModeState) {}

// Blocking

void AAlsxtCharacter::SetDesiredStationaryMode(const FGameplayTag& NewStationaryModeTag)
{
	if (DesiredStationaryMode != NewStationaryModeTag)
	{
		DesiredStationaryMode = NewStationaryModeTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredStationaryMode, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredStationaryMode(NewStationaryModeTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredStationaryMode_Implementation(const FGameplayTag& NewStationaryModeTag)
{
	SetDesiredStationaryMode(NewStationaryModeTag);
}

void AAlsxtCharacter::SetStationaryMode(const FGameplayTag& NewStationaryModeTag)
{

	if (StationaryMode != NewStationaryModeTag)
	{
		const auto PreviousStationaryMode{ StationaryMode };

		StationaryMode = NewStationaryModeTag;

		OnStationaryModeChanged(PreviousStationaryMode);
	}
}

void AAlsxtCharacter::OnStationaryModeChanged_Implementation(const FGameplayTag& PreviousStationaryModeTag) {}

// Status

void AAlsxtCharacter::SetDesiredStatus(const FGameplayTag& NewStatusTag)
{
	if (DesiredStatus != NewStatusTag)
	{

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredStatus, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredStatus(NewStatusTag);
			}
			else if (GetLocalRole() == ROLE_SimulatedProxy && GetRemoteRole() == ROLE_Authority)
			{
				// MulticastSetDesiredStatus(NewStatusTag);
				// ServerSetDesiredStatus(NewStatusTag);
				SetStatus(NewStatusTag);
				DesiredStatus = NewStatusTag;
				Status = NewStatusTag;
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredStatus_Implementation(const FGameplayTag& NewStatusTag)
{
	// SetDesiredStatus(NewStatusTag);
	MulticastSetDesiredStatus(NewStatusTag);
}

void AAlsxtCharacter::MulticastSetDesiredStatus_Implementation(const FGameplayTag& NewStatusTag)
{
	SetStatus(NewStatusTag);
}

void AAlsxtCharacter::SetStatus(const FGameplayTag& NewStatusTag)
{

	if (Status != NewStatusTag)
	{
		const auto PreviousStatus{ Status };

		Status = NewStatusTag;

		if (NewStatusTag != ALSXTStatusTags::Normal)
		{
			SetDesiredCombatStance(ALSXTCombatStanceTags::Neutral);
			SetDesiredStance(AlsStanceTags::Crouching);
			Crouch();
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		}
		else
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
			SetDesiredStance(AlsStanceTags::Standing);
			UnCrouch();
			// IALSXTCharacterInterface::Execute_TryGetUp(this);
		}

		OnStatusChanged(PreviousStatus);
	}
}

void AAlsxtCharacter::OnStatusChanged_Implementation(const FGameplayTag& PreviousStatusTag) {}

// Focus

void AAlsxtCharacter::SetDesiredFocus(const FGameplayTag& NewFocusTag)
{
	if (DesiredFocus != NewFocusTag)
	{
		DesiredFocus = NewFocusTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredFocus, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredFocus(NewFocusTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredFocus_Implementation(const FGameplayTag& NewFocusTag)
{
	SetDesiredFocus(NewFocusTag);
}

void AAlsxtCharacter::SetFocus(const FGameplayTag& NewFocusTag)
{

	if (Focus != NewFocusTag)
	{
		const auto PreviousFocus{ Focus };

		Focus = NewFocusTag;

		OnFocusChanged(PreviousFocus);
	}
}

void AAlsxtCharacter::OnFocusChanged_Implementation(const FGameplayTag& PreviousFocusTag) {}

void AAlsxtCharacter::AddCollisionImpulse_Implementation(FVector NewImpulse)
{
	LaunchCharacter(NewImpulse, false, false);
}

// Attack Hit

void AAlsxtCharacter::OnStaticMeshAttackCollision_Implementation(FAttackDoubleHitResult Hit)
{
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	float Scale;
	GetCameraShakeInfoFromHit(Hit, CameraShakeClass, Scale);
	if (CameraShakeClass != nullptr)
	{
		UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(CameraShakeClass, Scale);
	}
	// FALSXTDamageResult DamageResult = GetActualDamage(Hit);
	ImpactReaction->OnStaticMeshAttackCollision(Hit);
}

void AAlsxtCharacter::OnActorAttackCollision_Implementation(FAttackDoubleHitResult Hit)
{
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	float Scale;
	GetCameraShakeInfoFromHit(Hit, CameraShakeClass, Scale);
	if (CameraShakeClass != nullptr)
	{
		UGameplayStatics::GetPlayerController(this, 0)->ClientStartCameraShake(CameraShakeClass, Scale);
	}
	// FALSXTDamageResult DamageResult = GetActualDamage(Hit);
	ImpactReaction->AttackReaction(Hit);
	CharacterSound->PlayDamageSound(true, true, true, GetDesiredSex(), IAlsxtCharacterCustomizationComponentInterface::Execute_GetVoiceParameters(this).Variant, GetOverlayMode(), ALSXTAttackMethodTags::Regular, Hit.Strength, Hit.DoubleHitResult.ImpactForm, Hit.BaseDamage);
}

FAlsxtPhysicalAnimationState AAlsxtCharacter::GetCharacterPhysicalAnimationState_Implementation() const
{
	return GetPhysicalAnimationState();
}

FALSXTJigglePhysicsSettings AAlsxtCharacter::GetCharacterJigglePhysicsSettings_Implementation() const
{
	FALSXTJigglePhysicsSettings DefualtJigglePhysicsSettings;
	DefualtJigglePhysicsSettings.EnableBellyJigglePhysics = false;
	DefualtJigglePhysicsSettings.EnableBreastsJigglePhysics = true;
	DefualtJigglePhysicsSettings.EnableButtJigglePhysics = true;	
	return DefualtJigglePhysicsSettings;
}

void AAlsxtCharacter::SetCharacterPhysicalAnimationState_Implementation(FAlsxtPhysicalAnimationState NewPhysicalAnimationState)
{
	SetPhysicalAnimationState(NewPhysicalAnimationState);
}

FGameplayTag AAlsxtCharacter::GetCharacterPhysicalAnimationMode_Implementation() const
{
	return GetPhysicalAnimationMode();
}

void AAlsxtCharacter::SetCharacterPhysicalAnimationMode_Implementation(const FGameplayTag& NewPhysicalAnimationMode, const TArray<FName>& BelowBoneNames)
{
		SetDesiredPhysicalAnimationMode(NewPhysicalAnimationMode, BelowBoneNames);
}

void AAlsxtCharacter::ResetCharacterPhysicalAnimationMode_Implementation()
{
	ResetPhysicalAnimationMode();
}

void AAlsxtCharacter::BeginAnticipationTimer()
{
	GetWorld()->GetTimerManager().SetTimer(AnticipationTimerHandle, AnticipationTimerDelegate, 0.1f, true);
}

void AAlsxtCharacter::AnticipationTimer()
{
	// ImpactReaction->AnticipationTrace();
	//
	// FALSXTDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
	// FAnticipationPose NewDefensiveMontage;
	// NewDefensiveModeState.Mode = PreviousDefensiveModeState.Mode == FGameplayTag::EmptyTag ? ALSXTDefensiveModeTags::Blocking : PreviousDefensiveModeState.Mode;
	// NewDefensiveModeState.Side = PreviousDefensiveModeState.Side == FGameplayTag::EmptyTag ? ALSXTImpactSideTags::Front : PreviousDefensiveModeState.Side;
	// NewDefensiveModeState.Form = PreviousDefensiveModeState.Form == FGameplayTag::EmptyTag ? ALSXTImpactFormTags::Blunt : PreviousDefensiveModeState.Form;
	// NewDefensiveModeState.Velocity = PreviousDefensiveModeState.Velocity == FGameplayTag::EmptyTag ? ALSXTImpactVelocityTags::Slow : PreviousDefensiveModeState.Velocity;
	// NewDefensiveMontage = SelectAttackAnticipationMontage(NewDefensiveModeState.Velocity, GetDesiredStance(), NewDefensiveModeState.Side, NewDefensiveModeState.Form);
	// NewDefensiveModeState.AnticipationPose = NewDefensiveMontage.Pose;
	// SetDefensiveModeState(NewDefensiveModeState);
}

void AAlsxtCharacter::EndAnticipationTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AnticipationTimerHandle);
}

void AAlsxtCharacter::BeginFreelookTimer()
{
	if (ALSXTSettings->Freelook.bEnableFreelook)
	{
		GetWorld()->GetTimerManager().SetTimer(FreelookTimerHandle, FreelookTimerDelegate, 0.1f, true);
	}
}

void AAlsxtCharacter::FreelookTimer()
{
	FGameplayTag CurrentViewMode = GetViewMode();
	FAlsxtFreelookState NewState;

	if (CurrentViewMode == AlsViewModeTags::FirstPerson && ALSXTSettings->Freelook.bEnableFirstPersonFreelook)
	{
		FVector2D FirstPersonYawRange = ALSXTSettings->Freelook.FirstPersonYawRange;
		FVector2D FirstPersonPitchRange = ALSXTSettings->Freelook.FirstPersonPitchRange;
		FMinimalViewInfo ViewInfo;
		Camera->GetViewInfo(ViewInfo);
		FVector CurrentViewTarget = ViewInfo.Location + (Camera->GetForwardVector() * 1000.0);
		FRotator LookAtRotation = UKismetMathLibrary::FindRelativeLookAtRotation(GetMesh()->GetSocketTransform("head", ERelativeTransformSpace::RTS_Component), CurrentViewTarget);
		LookAtRotation.Yaw = FMath::Clamp(LookAtRotation.Yaw, FirstPersonYawRange.X, FirstPersonYawRange.Y);
		LookAtRotation.Pitch = FMath::Clamp(LookAtRotation.Pitch, FirstPersonPitchRange.X, FirstPersonPitchRange.Y);
		NewState.LockedPitchAngle = LookAtRotation.Pitch;
		NewState.LockedYawAngle = LookAtRotation.Yaw;
		NewState.LockedControlRotation = LookAtRotation;
		SetFreelookState(NewState);
	}
	if (CurrentViewMode == AlsViewModeTags::ThirdPerson && ALSXTSettings->Freelook.bEnableThirdPersonFreelook)
	{
		FVector2D ThirdPersonYawRange = ALSXTSettings->Freelook.ThirdPersonYawRange;
		FVector2D ThirdPersonPitchRange = ALSXTSettings->Freelook.ThirdPersonPitchRange;
		FMinimalViewInfo ViewInfo;
		Camera->GetViewInfo(ViewInfo);
		FVector CurrentViewTarget = ViewInfo.Location + (Camera->GetForwardVector() * 1000.0);
		FRotator LookAtRotation = UKismetMathLibrary::FindRelativeLookAtRotation(GetMesh()->GetSocketTransform("head", ERelativeTransformSpace::RTS_Component), CurrentViewTarget);
		LookAtRotation.Yaw = FMath::Clamp(LookAtRotation.Yaw, ThirdPersonYawRange.X, ThirdPersonYawRange.Y);
		LookAtRotation.Pitch = FMath::Clamp(LookAtRotation.Pitch, ThirdPersonPitchRange.X, ThirdPersonPitchRange.Y);
		NewState.LockedPitchAngle = LookAtRotation.Pitch;
		NewState.LockedYawAngle = LookAtRotation.Yaw;
		NewState.LockedControlRotation = LookAtRotation;
		SetFreelookState(NewState);
	}
}

void AAlsxtCharacter::EndFreelookTimer()
{
	// Clear Freelook Trace Timer
	FAlsxtFreelookState EmptyState;
	SetFreelookState(EmptyState);
	GetWorld()->GetTimerManager().ClearTimer(FreelookTimerHandle);
}

// PhysicalAnimationMode

void AAlsxtCharacter::SetDesiredPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const TArray<FName>& BoneNames)
{
	FString Tag = NewPhysicalAnimationModeTag.ToString();
	FString ClientRole;
	ClientRole = UEnum::GetValueAsString(GetLocalRole());
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, Tag);
	if (DesiredPhysicalAnimationMode != NewPhysicalAnimationModeTag)
	{
		DesiredPhysicalAnimationMode = NewPhysicalAnimationModeTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredPhysicalAnimationMode, this)

			if (GetLocalRole() == ROLE_Authority)
			{
				ServerSetDesiredPhysicalAnimationMode(NewPhysicalAnimationModeTag, BoneNames);
			}
			else if (GetLocalRole() >= ROLE_SimulatedProxy)
			{
				SetPhysicalAnimationMode(NewPhysicalAnimationModeTag, BoneNames);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredPhysicalAnimationMode_Implementation(const FGameplayTag& NewPhysicalAnimationModeTag, const TArray<FName>& BoneNames)
{
	SetDesiredPhysicalAnimationMode(NewPhysicalAnimationModeTag, BoneNames);
}

void AAlsxtCharacter::SetPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const TArray<FName>& BoneNames)
{
	if (GetPhysicalAnimationState().Mode != NewPhysicalAnimationModeTag && GetPhysicalAnimationState().AffectedBonesBelow != BoneNames)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("SetPhysicalAnimationMode"));
		const auto PreviousPhysicalAnimationMode{ PhysicalAnimationMode };

		if (NewPhysicalAnimationModeTag == AlsxtPhysicalAnimationModeTags::None || NewPhysicalAnimationModeTag == FGameplayTag::EmptyTag)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ALSXTPhysicalAnimationModeTags::None"));
			PhysicalAnimationMode = AlsxtPhysicalAnimationModeTags::None;
			GetMesh()->SetCollisionProfileName("CharacterMesh");
			PhysicalAnimation->ApplyPhysicalAnimationProfileBelow("pelvis", "Default", true, false);
			GetCapsuleComponent()->SetCapsuleRadius(25);		
			FAlsxtPhysicalAnimationState NewPhysicalAnimationState;
			NewPhysicalAnimationState.Mode = FGameplayTag::EmptyTag;
			NewPhysicalAnimationState.ProfileName = "CharacterMesh";
			SetPhysicalAnimationState(NewPhysicalAnimationState);
			FAlsxtDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
			NewDefensiveModeState.Mode = AlsxtPhysicalAnimationModeTags::None;
			NewDefensiveModeState.AnticipationMode = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
			NewDefensiveModeState.AnticipationSide = FGameplayTag::EmptyTag;
			NewDefensiveModeState.AnticipationHeight = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
			NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;
			NewDefensiveModeState.AnticipationTransform = {{0, 0, 0}, {0, 0, 0}, { 0, 0, 0 }};
			NewDefensiveModeState.ObstacleTransform = {{0, 0, 0}, {0, 0, 0}, { 0, 0, 0 }};
			NewDefensiveModeState.AnticipationForm = FGameplayTag::EmptyTag;
			SetDefensiveModeState(NewDefensiveModeState);
			OnPhysicalAnimationModeChanged(PreviousPhysicalAnimationMode);
			StartBlendOutPhysicalAnimation();
			return;
			// GetMesh()->SetAllBodiesSimulatePhysics(false);
		}
		if (NewPhysicalAnimationModeTag == AlsxtPhysicalAnimationModeTags::Navigation)
		{
			GetMesh()->SetCollisionProfileName("PhysicalAnimation");		
			GetCapsuleComponent()->SetCapsuleRadius(14);

			// GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true, false);
			// PhysicalAnimation->ApplyPhysicalAnimationProfileBelow("pelvis", "Hit", false, false);
			FAlsxtPhysicalAnimationState NewPhysicalAnimationState;
			NewPhysicalAnimationState.AffectedBonesBelow = BoneNames;
			NewPhysicalAnimationState.Alpha = 0.5;
			NewPhysicalAnimationState.Mode = AlsxtPhysicalAnimationModeTags::Navigation;
			NewPhysicalAnimationState.ProfileName = "Bump";

			for (FName BoneName : BoneNames)
			{
				PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(BoneName, "Bump", false, false);
				GetMesh()->SetAllBodiesBelowSimulatePhysics(BoneName, true, true);
				GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(BoneName, 0.5f, false, true);	
			}
			
			SetPhysicalAnimationState(NewPhysicalAnimationState);
			//StartBlendOutPhysicalAnimation();
			PhysicalAnimationMode = NewPhysicalAnimationModeTag;
			OnPhysicalAnimationModeChanged(PreviousPhysicalAnimationMode);
			return;
		}
		if (NewPhysicalAnimationModeTag == AlsxtPhysicalAnimationModeTags::Bump)
		{
			GetMesh()->SetCollisionProfileName("PhysicalAnimation");
			
			// GetMesh()->SetAllBodiesBelowSimulatePhysics(BoneName, true, false);
			GetCapsuleComponent()->SetCapsuleRadius(14);
			FAlsxtPhysicalAnimationState NewPhysicalAnimationState;
			NewPhysicalAnimationState.AffectedBonesBelow = BoneNames;
			NewPhysicalAnimationState.Alpha = 0.5;
			NewPhysicalAnimationState.Mode = AlsxtPhysicalAnimationModeTags::Bump;
			NewPhysicalAnimationState.ProfileName = "Bump";

			for (FName BoneName : BoneNames)
			{
				PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(BoneName, "Bump", false, false);
				GetMesh()->SetAllBodiesBelowSimulatePhysics(BoneName, true, true);
				GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(BoneName, 0.5f, false, true);
			}
			SetPhysicalAnimationState(NewPhysicalAnimationState);
			StartBlendOutPhysicalAnimation();
			PhysicalAnimationMode = NewPhysicalAnimationModeTag;
			OnPhysicalAnimationModeChanged(PreviousPhysicalAnimationMode);
			return;
		}
		if (NewPhysicalAnimationModeTag == AlsxtPhysicalAnimationModeTags::Hit)
		{
			GetMesh()->SetCollisionProfileName("PhysicalAnimation");
			GetCapsuleComponent()->SetCapsuleRadius(8);
			
			GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true, false);
			PhysicalAnimation->ApplyPhysicalAnimationProfileBelow("pelvis", "Hit", false, false);
			FAlsxtPhysicalAnimationState NewPhysicalAnimationState;
			NewPhysicalAnimationState.AffectedBonesBelow = BoneNames;
			NewPhysicalAnimationState.Alpha = 1.0f;
			NewPhysicalAnimationState.Mode = AlsxtPhysicalAnimationModeTags::Hit;
			NewPhysicalAnimationState.ProfileName = "Hit";

			for (FName BoneName : BoneNames)
			{							
				GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(BoneName, 1.0f, false, true);
			}
			
			SetPhysicalAnimationState(NewPhysicalAnimationState);
			StartBlendOutPhysicalAnimation();
			PhysicalAnimationMode = NewPhysicalAnimationModeTag;
			OnPhysicalAnimationModeChanged(PreviousPhysicalAnimationMode);
			return;
		}

		// PhysicalAnimationMode = NewPhysicalAnimationModeTag;
		// OnPhysicalAnimationModeChanged(PreviousPhysicalAnimationMode);
		// GetMesh()->Weight
		// GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(BoneName, 0.5f, false, true);
	}
}

void AAlsxtCharacter::ResetPhysicalAnimationMode()
{
	// if (PhysicalAnimationMode == FGameplayTag::EmptyTag || PhysicalAnimationMode == ALSXTPhysicalAnimationModeTags::None)
	// {
	// 	return;
	// }

	const auto PreviousPhysicalAnimationMode{ PhysicalAnimationMode };
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	PhysicalAnimation->ApplyPhysicalAnimationProfileBelow("", "None", true, true);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight("", 0.0f, false, true);
	GetCapsuleComponent()->SetCapsuleRadius(25);
	TArray<FName> AffectedBonesBelowNames;
	FAlsxtPhysicalAnimationState NewPhysicalAnimationState;
	NewPhysicalAnimationState.Mode = AlsxtPhysicalAnimationModeTags::None;
	NewPhysicalAnimationState.ProfileName = "CharacterMesh";
	NewPhysicalAnimationState.AffectedBonesBelow = AffectedBonesBelowNames;
	// NewPhysicalAnimationState.Alpha = 0.0f;
	StartBlendOutPhysicalAnimation();
	SetPhysicalAnimationState(NewPhysicalAnimationState);
	OnPhysicalAnimationModeChanged(PreviousPhysicalAnimationMode);
	FAlsxtDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
	
	NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
	NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
	NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;	
	NewDefensiveModeState.ObstacleTransform = {{0, 0, 0}, {0, 0, 0}, { 0, 0, 0 }};

	if (GetDefensiveModeState().AnticipationMode != ALSXTDefensiveModeTags::Anticipation)
	{
		NewDefensiveModeState.Mode = AlsxtPhysicalAnimationModeTags::None;
		NewDefensiveModeState.AnticipationMode = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationSide = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationHeight = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationForm = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationTransform = {{0, 0, 0}, {0, 0, 0}, { 0, 0, 0 }};
	}
	
	SetDefensiveModeState(NewDefensiveModeState);
	PhysicalAnimationMode = AlsxtPhysicalAnimationModeTags::None;
}

void AAlsxtCharacter::PhysicalAnimationBlendOut() 
{
	FAlsxtPhysicalAnimationState NewPhysicalAnimationState = GetPhysicalAnimationState();
	NewPhysicalAnimationState.Alpha = GetPhysicalAnimationState().Alpha - 0.1;
	SetPhysicalAnimationState(NewPhysicalAnimationState);
	if (GetPhysicalAnimationState().Alpha <= 0.0)
	{
		ResetPhysicalAnimationMode();
	}
}

void AAlsxtCharacter::OnPhysicalAnimationModeChanged_Implementation(const FGameplayTag& PreviousPhysicalAnimationModeTag) {}

// GripPosition

void AAlsxtCharacter::SetDesiredGripPosition(const FGameplayTag& NewGripPositionTag)
{
	if (DesiredGripPosition != NewGripPositionTag)
	{
		DesiredGripPosition = NewGripPositionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredGripPosition, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredGripPosition(NewGripPositionTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredGripPosition_Implementation(const FGameplayTag& NewGripPositionTag)
{
	SetDesiredGripPosition(NewGripPositionTag);
}

void AAlsxtCharacter::SetGripPosition(const FGameplayTag& NewGripPositionTag)
{

	if (GripPosition != NewGripPositionTag)
	{
		const auto PreviousGripPosition{ GripPosition };

		GripPosition = NewGripPositionTag;

		OnGripPositionChanged(PreviousGripPosition);
	}
}

void AAlsxtCharacter::OnGripPositionChanged_Implementation(const FGameplayTag& PreviousGripPositionTag) {}

// ForegripPosition

const FName AAlsxtCharacter::GetSocketNameForForegripPosition(const FGameplayTag& ForegripPositionTag) const
{
	FForegripPositions ForegripPositions = ALSXTSettings->ForegripPosition.ForegripPositions;

	if (ForegripPositionTag == ALSXTForegripPositionTags::Default)
	{
		return ForegripPositions.Default.SocketName;
	}
	if (ForegripPositionTag == ALSXTForegripPositionTags::MagazineWell)
	{
		return ForegripPositions.MagazineWell.SocketName;
	}
	if (ForegripPositionTag == ALSXTForegripPositionTags::VerticalGrip)
	{
		return ForegripPositions.Vertical.SocketName;
	}
	if (ForegripPositionTag == ALSXTForegripPositionTags::AngledForeGrip)
	{
		return ForegripPositions.Angled.SocketName;
	}
	if (ForegripPositionTag == ALSXTForegripPositionTags::TopHorizontalGrip)
	{
		return ForegripPositions.Top.SocketName;
	}
	else
	{
		return ForegripPositions.Default.SocketName;
	}
}

const FName AAlsxtCharacter::GetSocketNameForGripPosition(const FGameplayTag& ForegripPositionTag) const
{
	FForegripPositions ForegripPositions = ALSXTSettings->ForegripPosition.ForegripPositions;

	if (ForegripPositionTag == ALSXTGripPositionTags::Default)
	{
		return ForegripPositions.Default.SocketName;
	}
	if (ForegripPositionTag == ALSXTForegripPositionTags::MagazineWell)
	{
		return ForegripPositions.MagazineWell.SocketName;
	}
	if (ForegripPositionTag == ALSXTForegripPositionTags::VerticalGrip)
	{
		return ForegripPositions.Vertical.SocketName;
	}
	if (ForegripPositionTag == ALSXTForegripPositionTags::AngledForeGrip)
	{
		return ForegripPositions.Angled.SocketName;
	}
	if (ForegripPositionTag == ALSXTForegripPositionTags::TopHorizontalGrip)
	{
		return ForegripPositions.Top.SocketName;
	}
	else
	{
		return ForegripPositions.Default.SocketName;
	}
}

void AAlsxtCharacter::SetDesiredForegripPosition(const FGameplayTag& NewForegripPositionTag)
{
	if (DesiredForegripPosition != NewForegripPositionTag)
	{
		DesiredForegripPosition = NewForegripPositionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredForegripPosition, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredForegripPosition(NewForegripPositionTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredForegripPosition_Implementation(const FGameplayTag& NewForegripPositionTag)
{
	SetDesiredForegripPosition(NewForegripPositionTag);
}

void AAlsxtCharacter::SetForegripPosition(const FGameplayTag& NewForegripPositionTag)
{

	if (ForegripPosition != NewForegripPositionTag)
	{
		const auto PreviousForegripPosition{ ForegripPosition };

		ForegripPosition = NewForegripPositionTag;

		OnForegripPositionChanged(PreviousForegripPosition);
	}
}

void AAlsxtCharacter::OnForegripPositionChanged_Implementation(const FGameplayTag& PreviousForegripPositionTag) {}


// WeaponCarryPosition

void AAlsxtCharacter::SetDesiredWeaponCarryPosition(const FGameplayTag& NewWeaponCarryPositionTag)
{
	if (DesiredWeaponCarryPosition != NewWeaponCarryPositionTag)
	{
		DesiredWeaponCarryPosition = NewWeaponCarryPositionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredWeaponCarryPosition, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredWeaponCarryPosition(NewWeaponCarryPositionTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredWeaponCarryPosition_Implementation(const FGameplayTag& NewWeaponCarryPositionTag)
{
	SetDesiredWeaponCarryPosition(NewWeaponCarryPositionTag);
}

void AAlsxtCharacter::SetWeaponCarryPosition(const FGameplayTag& NewWeaponCarryPositionTag)
{

	if (WeaponCarryPosition != NewWeaponCarryPositionTag)
	{
		const auto PreviousWeaponCarryPosition{ WeaponCarryPosition };

		WeaponCarryPosition = NewWeaponCarryPositionTag;

		OnWeaponCarryPositionChanged(PreviousWeaponCarryPosition);
	}
}

void AAlsxtCharacter::OnWeaponCarryPositionChanged_Implementation(const FGameplayTag& PreviousWeaponCarryPositionTag) {}

// VaultType

void AAlsxtCharacter::SetDesiredVaultType(const FGameplayTag& NewVaultTypeTag)
{
	if (DesiredVaultType != NewVaultTypeTag)
	{
		DesiredVaultType = NewVaultTypeTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredVaultType, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredVaultType(NewVaultTypeTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredVaultType_Implementation(const FGameplayTag& NewVaultTypeTag)
{
	SetDesiredVaultType(NewVaultTypeTag);
}

void AAlsxtCharacter::SetVaultType(const FGameplayTag& NewVaultTypeTag)
{

	if (VaultType != NewVaultTypeTag)
	{
		const auto PreviousVaultType{ VaultType };

		VaultType = NewVaultTypeTag;

		OnVaultTypeChanged(PreviousVaultType);
	}
}

void AAlsxtCharacter::OnVaultTypeChanged_Implementation(const FGameplayTag& PreviousVaultTypeTag) {}

// WeaponObstruction

void AAlsxtCharacter::SetDesiredWeaponObstruction(const FGameplayTag& NewWeaponObstructionTag)
{
	if (DesiredWeaponObstruction != NewWeaponObstructionTag)
	{
		DesiredWeaponObstruction = NewWeaponObstructionTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredWeaponObstruction, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredWeaponObstruction(NewWeaponObstructionTag);
			}
	}
}

void AAlsxtCharacter::ServerSetDesiredWeaponObstruction_Implementation(const FGameplayTag& NewWeaponObstructionTag)
{
	SetDesiredWeaponObstruction(NewWeaponObstructionTag);
}

void AAlsxtCharacter::SetWeaponObstruction(const FGameplayTag& NewWeaponObstructionTag)
{

	if (WeaponObstruction != NewWeaponObstructionTag)
	{
		const auto PreviousWeaponObstruction{ WeaponObstruction };

		WeaponObstruction = NewWeaponObstructionTag;

		OnWeaponObstructionChanged(PreviousWeaponObstruction);
	}
}

void AAlsxtCharacter::OnWeaponObstructionChanged_Implementation(const FGameplayTag& PreviousWeaponObstructionTag) {}

void AAlsxtCharacter::UpdateBreathState()
{
	FAlsxtBreathState NewBreathState;
	const float Stamina = GetCharacterStatusState().CurrentStamina;
	if (this->Implements<UAlsxtCharacterInterface>())
	{
		FGameplayTag BreathType = IAlsxtCharacterInterface::Execute_GetBreathType(this);
		if (ShouldTransitionBreathState() && BreathState.HoldingBreath != ALSXTHoldingBreathTags::True)
		{
			FAlsxtTargetBreathState NewTargetState = CalculateTargetBreathState();
			BreathState.TargetState = NewTargetState;
			NewBreathState = GetBreathState();
			NewBreathState.TargetState = NewTargetState;
			SetBreathState(NewBreathState);
		}
	}
}

bool AAlsxtCharacter::ShouldUpdateBreathState() const
{
	FAlsxtStatusState NewStatusState = IAlsxtCharacterInterface::Execute_GetCharacterStatusState(this);
	float CurrentStamina = IAlsxtCharacterInterface::Execute_GetStamina(this);
	return NewStatusState.CurrentStamina != CurrentStamina;
}

bool AAlsxtCharacter::ShouldTransitionBreathState()
{
	return (GetBreathState().CurrentBreathAlpha != GetBreathState().TargetState.Alpha || GetBreathState().CurrentBreathRate != GetBreathState().TargetState.Rate);
}


FAlsxtTargetBreathState AAlsxtCharacter::CalculateTargetBreathState()
{
	FAlsxtTargetBreathState NewTargetBreathState;

	const FGameplayAttribute BreathingRateAttribute = UAlsxtBreathAttributeSet::GetCurrentBreathRateAttribute();
	float GasBreathRate {0.0f};
	if (BreathingRateAttribute.IsValid())
	{
		// You can use the GetNumericAttribute function to get the current value.
		// IAlsxtAbilitySystemInterface::Execute_GetAlsxtAbilitySystemComponent(this);
		// GasBreathRate = GetAlsxtAbilitySystemComponent()->GetNumericAttribute(BreathingRateAttribute);
	}

	if (BreathState.HoldingBreath == ALSXTHoldingBreathTags::True)
	{
		NewTargetBreathState.Alpha = 0.0;
		NewTargetBreathState.Rate = 0.0;
		return NewTargetBreathState;
	}
	if (BreathState.HoldingBreath == ALSXTHoldingBreathTags::Released)
	{
		NewTargetBreathState.Alpha = GetBreathState().PreviousBreathAlpha;
		NewTargetBreathState.Rate = GetBreathState().PreviousBreathRate;
		// NewTargetBreathState.Rate = GasBreathRate;
		return NewTargetBreathState;
	}
	if (BreathState.HoldingBreath == ALSXTHoldingBreathTags::Exhausted)
	{
		NewTargetBreathState.Alpha = 1.0;
		NewTargetBreathState.Rate = 1.0;
		return NewTargetBreathState;
	}
	else
	{
		FVector2D ConversionRange{ 0, 1 };
		FVector2D UtilizedStaminaRange{ 0, ALSXTSettings->StatusSettings.StaminaThresholdSettings.StaminaOptimalThreshold };
		float CurrentStaminaConverted = FMath::GetMappedRangeValueClamped(UtilizedStaminaRange, ConversionRange, IAlsxtCharacterInterface::Execute_GetCharacterStatusState(this).CurrentStamina);
		float PlayRateConverted = FMath::GetMappedRangeValueClamped(ConversionRange, IAlsxtCharacterSoundComponentInterface::Execute_GetBreathEffectsSettings(this).BreathAnimationPlayRateRange, CurrentStaminaConverted);
		float BlendConverted = FMath::GetMappedRangeValueClamped(ConversionRange, IAlsxtCharacterSoundComponentInterface::Execute_GetBreathEffectsSettings(this).BreathAnimationBlendRange, CurrentStaminaConverted);
		NewTargetBreathState.Alpha = BlendConverted;
		NewTargetBreathState.Rate = PlayRateConverted;
		// NewTargetBreathState.Rate = GasBreathRate;
		NewTargetBreathState.TransitionRate = 1.0;
		return NewTargetBreathState;
	}
}

void AAlsxtCharacter::SetTargetBreathState(const FAlsxtTargetBreathState& NewTargetBreathState)
{
	BreathState.TargetState = NewTargetBreathState;
}

void AAlsxtCharacter::TransitionBreathState()
{
	if (BreathState.HoldingBreath == ALSXTHoldingBreathTags::False)
	{
		BreathState.PreviousBreathAlpha = BreathState.CurrentBreathAlpha;
		BreathState.PreviousBreathRate = BreathState.CurrentBreathRate;
		BreathState.CurrentBreathAlpha = BreathState.TargetState.Alpha;
		BreathState.CurrentBreathRate = BreathState.TargetState.Rate;
	}
}

void AAlsxtCharacter::UpdateAnimationBlendState() const
{
	
}

bool AAlsxtCharacter::ShouldUpdateAnimationBlendState() const
{
	return true;
}

FAlsxtAnimationBlendState AAlsxtCharacter::GetAnimationBlendState_Implementation() const
{
	return IAlsxtCharacterInterface::GetAnimationBlendState_Implementation();
}

FAlsxtTargetBreathState AAlsxtCharacter::CalculateAnimationBlendState_Implementation()
{
	return FAlsxtTargetBreathState();
}

void AAlsxtCharacter::OnAIJumpObstacle_Implementation()
{
	// if (TryVault())
	// {
	// 	StopJumping();
	// 	StartVault();
	// }
	// else
	// {
	// 	Jump();
	// }
	Jump();
}

bool AAlsxtCharacter::CanSprint_Implementation() const { return true; }
bool AAlsxtCharacter::CanFocus_Implementation() const { return true; }
bool AAlsxtCharacter::CanToggleGait_Implementation() const { return true; }
bool AAlsxtCharacter::CanLean_Implementation() const { return true; }
bool AAlsxtCharacter::CanAimDownSights_Implementation() const { return true; }
bool AAlsxtCharacter::CanAim_Implementation() const { return true; }
bool AAlsxtCharacter::CanRoll_Implementation() const { return true; }
bool AAlsxtCharacter::CanVault_Implementation() const { return true; }
bool AAlsxtCharacter::CanSlide_Implementation() const { return true; }
bool AAlsxtCharacter::CanWallRun_Implementation() const { return true; }
bool AAlsxtCharacter::CanWallJump_Implementation() const { return true; }

void AAlsxtCharacter::AIObstacleTrace_Implementation() {}
void AAlsxtCharacter::StartVault_Implementation() {}
void AAlsxtCharacter::StartWallrun_Implementation() {}
void AAlsxtCharacter::OnWeaponReadyPositionChanged_Implementation(const FGameplayTag& PreviousWeaponReadyPositionTag) {}

AAlsxtCharacter* AAlsxtCharacter::GetCharacter_Implementation()
{
	return this;
}

UAlsxtAnimationInstance* AAlsxtCharacter::GetCharacterAnimInstance_Implementation() const
{
	return Cast<UAlsxtAnimationInstance>(GetMesh()->GetAnimInstance());
}

UAlsxtCharacterSettings* AAlsxtCharacter::GetCharacterSettings_Implementation() const
{
	return ALSXTSettings;
}

UInputComponent* AAlsxtCharacter::GetCharacterInputComponent_Implementation() const
{
	return InputComponent;
}

USkeletalMeshComponent* AAlsxtCharacter::GetCharacterMesh_Implementation() const
{
	return GetMesh();
}

FGameplayTag AAlsxtCharacter::GetCharacterSex_Implementation() const
{
	return GetDesiredSex();
}

FGameplayTag AAlsxtCharacter::GetCharacterStance_Implementation() const
{
	return GetDesiredStance();
}

FGameplayTag AAlsxtCharacter::GetCharacterOverlayMode_Implementation() const
{
	return GetOverlayMode();
}

FGameplayTag AAlsxtCharacter::GetCharacterDominantOverlayLayer_Implementation(FGameplayTag& OverlaySlot) const
{
	return IAlsxtCharacterInterface::GetCharacterDominantOverlayLayer_Implementation(OverlaySlot);
}

TSoftObjectPtr<UAlsxtOverlayLookupTableDataAsset> AAlsxtCharacter::GetCharacterOverlayLookupTable_Implementation() const
{
	return nullptr;
}

FAlsxtOverlayInfo AAlsxtCharacter::GetCharacterOverlayInfo_Implementation(const FGameplayTag& OverlayModeTag) const
{
	if (!OverlayLookupTable.IsNull())
	{
		if (TSoftObjectPtr<UAlsxtOverlayDataAsset>* OverlayDataAsset = ALSXTSettings->OverlaySettings.OverlayLookupTable.Get()->OverlayDataMap.Find(OverlayModeTag))
		{
			return OverlayDataAsset->Get()->OverlaySettings;
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("AAlsxtCharacter::GetCharacterOverlayInfo: OverlayMode not found in OverlayLookupTable.OverlayDataMap")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("AAlsxtCharacter::GetCharacterOverlayInfo: OverlayLookupTable Not Valid")));
	}
	FAlsxtOverlayInfo EmptyOverlayInfo {};
	return EmptyOverlayInfo;
}

FAlsxtOverlayInfo AAlsxtCharacter::GetCharacterOverlayObjectInfo_Implementation(const FGameplayTag& OverlayModeTag) const
{
	return IAlsxtCharacterInterface::GetCharacterOverlayObjectInfo_Implementation(OverlayModeTag);
}

FAlsxtOverlayInfo AAlsxtCharacter::GetCharacterAimableOverlayObjectInfo_Implementation(const FGameplayTag& OverlayModeTag) const
{
	return IAlsxtCharacterInterface::GetCharacterAimableOverlayObjectInfo_Implementation(OverlayModeTag);
}

FAlsxtOverlayLayers AAlsxtCharacter::GetCharacterOverlayLayers_Implementation() const
{
	FAlsxtOverlayLayers EmptyOverlayLayers {};
	return EmptyOverlayLayers;
}

FGameplayTag AAlsxtCharacter::GetCharacterCombatStance_Implementation() const
{
	return GetDesiredCombatStance();
}

void AAlsxtCharacter::SetCharacterCombatStance_Implementation(const FGameplayTag& NewCombatStance)
{
	return SetDesiredCombatStance(NewCombatStance);
}

FGameplayTag AAlsxtCharacter::GetCharacterWeaponReadyPosition_Implementation() const
{
	return GetDesiredWeaponReadyPosition();
}

FGameplayTagContainer AAlsxtCharacter::GetCharacterAvailableWeaponReadyPositions_Implementation() const
{
	return IAlsxtCharacterInterface::Execute_GetCharacterAvailableWeaponReadyPositions(this);
}

void AAlsxtCharacter::SetCharacterWeaponReadyPosition_Implementation(FGameplayTag NewWeaponReadyPosition)
{
	SetDesiredWeaponReadyPosition(NewWeaponReadyPosition);
}

FGameplayTag AAlsxtCharacter::GetCharacterWeaponFirearmStance_Implementation() const
{
	return GetDesiredWeaponFirearmStance();
}

FGameplayTag AAlsxtCharacter::GetCharacterInjury_Implementation() const
{
	return GetDesiredInjury();
}

UAlsCameraComponent* AAlsxtCharacter::GetCharacterCamera_Implementation() const
{
	return Camera;
}

UAlsxtCameraAnimationInstance* AAlsxtCharacter::GetCharacterCameraAnimationInstance_Implementation() const
{
	return Cast<UAlsxtCameraAnimationInstance>(Camera->GetAnimInstance());
}

FRotator AAlsxtCharacter::GetCharacterControlRotation_Implementation() const
{
	return GetControlRotation();
}

FVector AAlsxtCharacter::GetCharacterFirstPersonCameraLocation_Implementation() const
{
	return Camera->GetFirstPersonCameraLocation();
}

FAlsxtBreathState AAlsxtCharacter::GetCharacterBreathState_Implementation() const
{
	return GetBreathState();
}

FAlsxtPoseState AAlsxtCharacter::GetCharacterPoseState_Implementation() const
{
	return GetALSXTPoseState();
}

FGameplayTag AAlsxtCharacter::GetCharacterLocomotionMode_Implementation() const
{
	return GetLocomotionMode();
}

FGameplayTag AAlsxtCharacter::GetCharacterGait_Implementation() const
{
	return GetDesiredGait();
}

void AAlsxtCharacter::SetCharacterGait_Implementation(const FGameplayTag& NewGait)
{
	SetDesiredGait(NewGait);
}

FGameplayTag AAlsxtCharacter::GetCharacterReadiness_Implementation() const
{
	return GetDesiredReadyStance();
}

void AAlsxtCharacter::SetCharacterReadiness_Implementation(const FGameplayTag& NewReadiness)
{
	SetDesiredReadyStance(NewReadiness);
}

FGameplayTag AAlsxtCharacter::GetCharacterRotationMode_Implementation() const
{
	return GetDesiredRotationMode();
}

void AAlsxtCharacter::SetCharacterRotationMode_Implementation(const FGameplayTag& NewRotationMode)
{
	SetDesiredRotationMode(NewRotationMode);
}

FGameplayTag AAlsxtCharacter::GetCharacterViewMode_Implementation() const
{
	return GetViewMode();
}

FGameplayTag AAlsxtCharacter::GetCharacterLean_Implementation() const
{
	return GetDesiredLean();
}

FAlsxtStationaryModeState AAlsxtCharacter::GetCharacterStationaryModeState_Implementation() const
{
	return GetStationaryModeState();
}

FAlsxtSlidingState AAlsxtCharacter::GetCharacterSlidingState_Implementation() const
{
	return GetSlidingState();
}

FAlsxtDefensiveModeState AAlsxtCharacter::GetCharacterDefensiveModeState_Implementation() const
{
	return GetDefensiveModeState();
}

FAlsxtDefensiveModeAnimations AAlsxtCharacter::GetCharacterDefensiveModeAnimations_Implementation() const
{
	return GetDefensiveModeAnimations();
}

FAlsxtBumpPoseState AAlsxtCharacter::GetCrowdNavigationPoseState_Implementation() const
{
	return ImpactReaction->GetCrowdNavigationPoseState();
}

UCapsuleComponent* AAlsxtCharacter::GetCharacterCapsuleComponent_Implementation() const
{
	return GetCapsuleComponent();
}

UAlsCharacterMovementComponent* AAlsxtCharacter::GetCharacterMovementComponent_Implementation() const
{
	return ALSXTCharacterMovement;
}

bool AAlsxtCharacter::IsBlocking_Implementation() const
{
	return IsBlocking();
}

bool AAlsxtCharacter::IsCharacterPlayerControlled_Implementation() const
{
	return IsPlayerControlled();
}

bool AAlsxtCharacter::GetCharacterCanRoll_Implementation() const
{
	return CanRoll();
}

void AAlsxtCharacter::StartCharacterRoll_Implementation(float PlayRate)
{
	StartRolling(PlayRate);
}

bool AAlsxtCharacter::CanCharacterSprint_Implementation() const
{
	return CanSprint();
}

bool AAlsxtCharacter::GetCanCharacterAim_Implementation() const
{
	return CanAim();
}

bool AAlsxtCharacter::GetCharacterAim_Implementation() const
{
	return IsDesiredAiming();
}

void AAlsxtCharacter::SetCharacterAim_Implementation(const bool NewStance)
{
	SetDesiredAiming(NewStance);
}

bool AAlsxtCharacter::GetCharacterFirstPersonFocus_Implementation() const
{
	return IsFirstPersonEyeFocusActive();
}

bool AAlsxtCharacter::GetCharacterAimingDownSights_Implementation() const
{
	return IsAimingDownSights();
}

bool AAlsxtCharacter::CanEmote_Implementation() const
{
	return true;
}

bool AAlsxtCharacter::CanGesture_Implementation() const
{
	return true;
}

FAlsxtAnimationParametersState AAlsxtCharacter::GetCharacterAnimationParametersState_Implementation() const
{
	return GetAnimationParametersState();
}

FGameplayTag AAlsxtCharacter::GetCharacterLocomotionAction_Implementation() const
{
	return GetLocomotionAction();
}

void AAlsxtCharacter::SetCharacterLocomotionAction_Implementation(const FGameplayTag& NewLocomotionAction)
{
	SetLocomotionAction(NewLocomotionAction);
}

void AAlsxtCharacter::ResetCharacterDefensiveModeState_Implementation()
{
	SetDefensiveMode(FGameplayTag::EmptyTag);
	FAlsxtDefensiveModeState EmptyDefensiveModeState;
	SetDefensiveModeState(EmptyDefensiveModeState);
}

FGameplayTag AAlsxtCharacter::GetCharacterDefensiveMode_Implementation() const
{
	return DesiredDefensiveMode;
}

void AAlsxtCharacter::SetCharacterDefensiveMode_Implementation(const FGameplayTag& NewDefensiveMode)
{
	SetDefensiveMode(NewDefensiveMode);
}

void AAlsxtCharacter::SetCharacterStatus_Implementation(const FGameplayTag& NewStatus)
{
	SetDesiredStatus(NewStatus);
}

void AAlsxtCharacter::SetCharacterLocomotionVariant_Implementation(const FGameplayTag& NewLocomotionVariant)
{
	SetDesiredLocomotionVariant(NewLocomotionVariant);
}

void AAlsxtCharacter::SetCharacterInjury_Implementation(const FGameplayTag& NewInjury)
{
	SetDesiredInjury(NewInjury);
}

void AAlsxtCharacter::SetViewportMask_Implementation(const FGameplayTag& EquipmentSlot, const UMaterialInterface* NewViewportMask)
{
	// SetDesiredStatus(NewStatus);
}

void AAlsxtCharacter::SetCharacterDefensiveModeState_Implementation(FAlsxtDefensiveModeState NewDefensiveModeState)
{
	SetDefensiveModeState(NewDefensiveModeState);
}

void AAlsxtCharacter::SetCharacterDefensiveModeAnimations_Implementation(FAlsxtDefensiveModeAnimations NewDefensiveModeAnimations)
{
	SetDefensiveModeAnimations(NewDefensiveModeAnimations);
}

void AAlsxtCharacter::SetCharacterMovementModeLocked_Implementation(bool NewLocked)
{
	SetMovementModeLocked(NewLocked);
	// NewLocked ? ALSXTCharacterMovement->SetMovementMode(EMovementMode::MOVE_None) : ALSXTCharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AAlsxtCharacter::SetCharacterStance_Implementation(const FGameplayTag& NewStance)
{
	SetDesiredStance(NewStance);
}

FGameplayTag AAlsxtCharacter::GetCharacterFreelooking_Implementation() const
{
	return GetDesiredFreelooking();
}

void AAlsxtCharacter::SetCharacterRagdoll_Implementation(const bool NewRagdoll)
{
	if (NewRagdoll)
	{
		StartRagdolling();
	}
	else
	{
		StopRagdolling();
	}
}



FGameplayTag AAlsxtCharacter::GetCharacterForegripPosition_Implementation() const
{
	return GetDesiredForegripPosition();
}

FGameplayTag AAlsxtCharacter::GetCharacterWeaponCarryPosition_Implementation() const
{
	return GetDesiredWeaponCarryPosition();
}

FTransform AAlsxtCharacter::GetCharacterCurrentForegripTransform_Implementation() const
{
	return GetCurrentForegripTransform();
}

FGameplayTag AAlsxtCharacter::GetCharacterVaultType_Implementation() const
{
	return GetDesiredVaultType();
}

FAlsxtAimState AAlsxtCharacter::GetCharacterAimState_Implementation() const
{
	return GetAimState();
}

FAlsxtHeldItemState AAlsxtCharacter::GetCharacterHeldItemState_Implementation() const
{
	return GetHeldItemState();
}

FAlsxtFreelookState AAlsxtCharacter::GetCharacterFreelookState_Implementation() const
{
	return GetFreelookState();
}

FAlsxtHeadLookAtState AAlsxtCharacter::GetCharacterHeadLookAtState_Implementation() const
{
	return GetHeadLookAtState();
}

FAlsxtHeadLookAtEntry AAlsxtCharacter::GetBestHeadLookAtEntry_Implementation() const
{
	TArray<FAlsxtHeadLookAtEntry> BestHeadLookAtEntries;
	FAlsxtHeadLookAtEntry BestHeadLookAtEntry;
	FAlsxtHeadLookAtEntry CurrentBestHeadLookAtEntry;

	if (HeadLookAtEntries.Num() > 1)
	{
		return BestHeadLookAtEntry;
	}

	for (auto HeadLookAtEntry : HeadLookAtEntries)
	{
		if (HeadLookAtEntry.HeadLookAtActor.Score > CurrentBestHeadLookAtEntry.HeadLookAtActor.Score)
		{
			CurrentBestHeadLookAtEntry = HeadLookAtEntry;
		}
		if (HeadLookAtEntry.HeadLookAtActor.Score == CurrentBestHeadLookAtEntry.HeadLookAtActor.Score)
		{
			if (HeadLookAtEntry.HeadLookAtActor.Distance < CurrentBestHeadLookAtEntry.HeadLookAtActor.Distance)
			{
				if (HeadLookAtEntry.HeadLookAtActor.Score == CurrentBestHeadLookAtEntry.HeadLookAtActor.Score)
				{
					CurrentBestHeadLookAtEntry = HeadLookAtEntry;
				}
			}
		}
	}

	return CurrentBestHeadLookAtEntry;
}

FGameplayTag AAlsxtCharacter::GetCharacterLocomotionVariant_Implementation() const
{
	return GetDesiredLocomotionVariant();
}

FAlsxtFootprintsState AAlsxtCharacter::GetCharacterFootprintsState_Implementation() const
{
	return GetFootprintsState();
}

FAlsxtFootwearDetails AAlsxtCharacter::GetCharacterFootwearDetails_Implementation() const
{
	return GetFootwearDetails();
}

// Character Sound Component Interface Functions
void AAlsxtCharacter::PlayBreathEffects_Implementation(const FGameplayTag& StaminaOverride)
{
	CharacterSound->PlayCharacterBreathEffects(StaminaOverride);
	// CharacterSound->MulticastPlayCharacterBreathEffects(StaminaOverride);
}

void AAlsxtCharacter::PlayCharacterMovementSound_Implementation(bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Object Weight"))const FGameplayTag& Weight)
{
	CharacterSound->PlayCharacterMovementSound(AccentSound, WeaponSound, Type, Weight);
}

void AAlsxtCharacter::PlayActionSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, const FGameplayTag& Type, const FGameplayTag& SoundSex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina)
{
	CharacterSound->PlayActionSound(MovementSound, AccentSound, WeaponSound, Type, SoundSex, Variant, Overlay, Strength, Stamina);
}

void AAlsxtCharacter::PlayAttackSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, const FGameplayTag& SoundSex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina)
{
	CharacterSound->PlayAttackSound(MovementSound, AccentSound, WeaponSound, SoundSex, Variant, Overlay, Strength, AttackMode, Stamina);
}

void AAlsxtCharacter::PlayDamageSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, const FGameplayTag& SoundSex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage)
{
	CharacterSound->PlayDamageSound(MovementSound, AccentSound, WeaponSound, SoundSex, Variant, Overlay, AttackMethod, Strength, AttackForm, Damage);
}

bool AAlsxtCharacter::CanPlayCharacterMovementSound_Implementation() const
{
	return true;
}

bool AAlsxtCharacter::CanPlayWeaponMovementSound_Implementation() const
{
	return true;
}

bool AAlsxtCharacter::ShouldPlayWeaponMovementSound_Implementation(const FGameplayTag& Type, const FGameplayTag& Strength) const
{
	return true;
}

bool AAlsxtCharacter::ShouldPlayMovementAccentSound_Implementation(const FGameplayTag& Type, const FGameplayTag& Weight) const
{
	return true;
}

float AAlsxtCharacter::GetCurrentStamina_Implementation() const
{
	return CharacterSound->CurrentStamina;
}

FGameplayTag AAlsxtCharacter::GetCurrentStaminaTag_Implementation() const
{
	return CharacterSound->CurrentStaminaTag;
}

FGameplayTag AAlsxtCharacter::GetCurrentBreathType_Implementation() const
{
	return CharacterSound->CurrentBreathType;
}


FString AAlsxtCharacter::GetCurrentBreathSounds_Implementation() const
{
	FString BreathSoundsString;
	for (auto BreathSound : CharacterSound->CurrentBreathSounds)
	{
		for (auto Sound : BreathSound.Sounds)
		{
			BreathSoundsString.Append(Sound.Sound->GetName());
			BreathSoundsString.Append(" ");
		}
	}	
	return BreathSoundsString;
}

FString AAlsxtCharacter::GetCurrentBreathSound_Implementation() const
{
	if (IsValid(CharacterSound->CurrentBreathSound.Sound))
	{
		return CharacterSound->CurrentBreathSound.Sound->GetName();
	}
	else
	{
		return "None";
	}
}

// FString AALSXTCharacter::GetCurrentVocalizations_Implementation() const
// {
// 	return CharacterSound->CurrentBreathType;
// }

FString AAlsxtCharacter::GetCurrentVocalization_Implementation() const
{
	if (IsValid(CharacterSound->CurrentVocalizationSound.Sound))
	{
		return CharacterSound->CurrentVocalizationSound.Sound->GetName();
	}
	else
	{
		return "None";
	}
	
}

bool AAlsxtCharacter::ShouldIdle_Implementation() const
{
	return true;
}

void AAlsxtCharacter::OnEnterStationaryModeBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	FAlsxtStationaryModeState NewStationaryModeState = GetStationaryModeState();
	NewStationaryModeState.Mode = NewStationaryModeState.TargetMode;
	NewStationaryModeState.TargetMode = FGameplayTag::EmptyTag;
	SetStationaryModeState(NewStationaryModeState);
}

void AAlsxtCharacter::OnSlidingStartedBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->Montage_JumpToSection("Edit", Montage);
}
// void AALSXTCharacter::OnExitStationaryModeBlendOut(UAnimMontage* Montage, bool bInterrupted) {}