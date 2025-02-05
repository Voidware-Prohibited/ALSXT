#include "ALSXTCharacter.h"
#include "ALSXTAnimationInstance.h"

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
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "Components/Character/ALSXTIdleAnimationComponent.h"
#include "Components/Character/ALSXTEmoteComponent.h"
#include "Components/Character/ALSXTGestureComponent.h"
#include "Settings/ALSXTCharacterSettings.h"
#include "Settings/ALSXTLocomotionActionSettings.h"
#include "Settings/ALSXTVaultingSettings.h"
#include "Settings/ALSXTCombatSettings.h"
#include "Interfaces/ALSXTCollisionInterface.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Interfaces/ALSXTHeldItemInterface.h"
#include "Curves/CurveVector.h"
#include "RootMotionSources/ALSXTRootMotionSource_Vaulting.h"
#include "Utility/AlsConstants.h"
#include "Utility/AlsMacros.h"
#include "Utility/AlsUtility.h"
#include "Utility/AlsVector.h"
#include "Utility/ALSXTGameplayTags.h"
#include "Utility/ALSXTCollisionGameplayTags.h"
#include "Utility/ALSXTStructs.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "State/ALSXTFootstepState.h"
#include "ALSXTCameraAnimationInstance.h"

AALSXTCharacter::AALSXTCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UALSXTPaintableSkeletalMeshComponent>(AAlsCharacter::MeshComponentName).SetDefaultSubobjectClass<UALSXTCharacterMovementComponent>(AAlsCharacter::CharacterMovementComponentName))
{
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

	// TODO Move to Advanced or TV
	KillerCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Killer Camera Spring Arm"));
	KillerCameraSpringArm->SetupAttachment(GetMesh());
	KillerCameraSpringArm->SetRelativeRotation_Direct({ 10.0f, 300.0f, 0.0f });
	KillerCameraSpringArm->SetRelativeLocation_Direct({ 0.0f, 0.0f, 122.0f });
	KillerCameraSpringArm->TargetArmLength = 250.0;
	KillerCamera = CreateDefaultSubobject<UCineCameraComponent>(TEXT("Killer Camera"));
	KillerCamera->SetupAttachment(KillerCameraSpringArm);
	KillerCamera->SetRelativeRotation_Direct({ 0.0f, 0.0f, 0.0f });
	KillerCamera->SetRelativeLocation_Direct({ -250.0f, 0.0f, 0.0f });

	// Body Mesh Components

	Head = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Head"));
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

	Teeth = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Teeth"));
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

	Tongue = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Tongue"));
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

	Hair = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Hair"));
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

	FacialHair = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Facial Hair"));
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

	Headwear = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Headwear"));
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

	Facewear = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Facewear"));
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
	
	Eyewear = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Eyewear"));
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
	
	Earwear = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Earwear"));
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
	
	BottomUnderwear = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Bottom Underwear"));
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
	
	TopUnderwear = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Top Underwear"));
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
	
	Bottom = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Bottom"));
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
	
	Top = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Top"));
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
	
	TopJacket = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Top Jacket"));
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
	
	TopVest = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Top Vest"));
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
	
	Gloves = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Gloves"));
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
	
	Footwear = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Footwear"));
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

	OverlaySkeletalMesh = CreateDefaultSubobject<UALSXTPaintableSkeletalMeshComponent>(TEXT("Overlay Skeletal Mesh"));
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

	OverlayStaticMesh = CreateDefaultSubobject<UALSXTPaintableStaticMeshComponent>(TEXT("Overlay Static Mesh"));
	OverlayStaticMesh->SetupAttachment(GetMesh());
	OverlayStaticMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	OverlayStaticMesh->AlwaysLoadOnClient = true;
	OverlayStaticMesh->AlwaysLoadOnServer = true;
	OverlayStaticMesh->bOwnerNoSee = false;
	OverlayStaticMesh->bCastDynamicShadow = true;
	OverlayStaticMesh->bAffectDynamicIndirectLighting = true;
	OverlayStaticMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	OverlayStaticMesh->bReceivesDecals = false;

	PhysicsConstraints = CreateDefaultSubobject<USceneComponent>(TEXT("Physics Constraints"));
	PhysicsConstraints->SetupAttachment(this->RootComponent);

	MeshPaintingSceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Mesh Painting Scene Capture"));
	MeshPaintingSceneCapture->SetupAttachment(GetMesh());
	MeshPaintingSceneCapture->SetRelativeRotation_Direct({ -90.0f, 90.0f, 0.0f });
	MeshPaintingSceneCapture->SetRelativeLocation_Direct({ 0.0f, 0.0f, 200.0f });
	MeshPaintingSceneCapture->ProjectionType = ECameraProjectionMode::Orthographic;
	MeshPaintingSceneCapture->CompositeMode = ESceneCaptureCompositeMode::SCCM_Additive;

	ALSXTCharacterMovement = Cast<UALSXTCharacterMovementComponent>(GetCharacterMovement());
	ALSXTMesh = Cast<UALSXTPaintableSkeletalMeshComponent>(GetMesh());
	ALSXTMesh->bReceivesDecals = false;

	// Code Components
	
	CharacterCustomization = CreateDefaultSubobject<UALSXTCharacterCustomizationComponent>(TEXT("Character Customization"));
	AddOwnedComponent(CharacterCustomization);

	ImpactReaction = CreateDefaultSubobject<UALSXTImpactReactionComponent>(TEXT("Impact Reaction"));
	AddOwnedComponent(ImpactReaction);

	CharacterSound = CreateDefaultSubobject<UALSXTCharacterSoundComponent>(TEXT("Character Sound"));
	AddOwnedComponent(CharacterSound);

	PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("Physical Animation"));
	AddOwnedComponent(PhysicalAnimation);

	IdleAnimation = CreateDefaultSubobject<UALSXTIdleAnimationComponent>(TEXT("Idle Animation"));
	AddOwnedComponent(IdleAnimation);

	Emotes = CreateDefaultSubobject<UALSXTEmoteComponent>(TEXT("Emotes"));
	AddOwnedComponent(Emotes);

	Gestures = CreateDefaultSubobject<UALSXTGestureComponent>(TEXT("Gestures"));
	AddOwnedComponent(Gestures);

	OnRagdollingStartedDelegate.BindUFunction(ImpactReaction, "OnRagdollingStarted");
	OnRagdollingEndedDelegate.BindUFunction(ImpactReaction, "OnRagdollingEnded");
}

void AALSXTCharacter::Tick(const float DeltaTime)
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

	if (IALSXTHeldItemInterface::Execute_IsHoldingAimableItem(this) && ((GetDesiredCombatStance() == ALSXTCombatStanceTags::Ready) || (GetDesiredCombatStance() == ALSXTCombatStanceTags::Aiming)))
	{
		FALSXTAimState NewAimState = GetAimState();
		// OverlaySkeletalMesh->GetSock
		// NewAimState.CurrentHeadTargetTransform.Location = IALSXTCharacterInterface::Execute_GetCharacterFirearmSightLocation(this);
	}

	BreathState.TargetState = CalculateTargetBreathState();

	if (ShouldTransitionBreathState())
	{
		TransitionBreathState();
	}
}

void AALSXTCharacter::NotifyControllerChanged()
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

void AALSXTCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;
	Parameters.Condition = COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, FootprintsState, Parameters)
	// DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PhysicalAnimationState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredLean, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DefensiveModeState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredFreelooking, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredSex, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredLocomotionVariant, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredInjury, Parameters)
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
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredEmote, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, AimState, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bMovementEnabled, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MovementInput, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredGesture, Parameters)
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, DesiredGestureHand, Parameters)
}

void AALSXTCharacter::BeginPlay()
{
	AlsCharacter = Cast<AAlsCharacter>(GetParentActor());
	Super::BeginPlay();

	// GetCapsuleComponent()->OnComponentHit.AddUniqueDynamic(ImpactReaction, &UALSXTImpactReactionComponent::OnCapsuleHit);
	// CharacterCapsule->OnComponentHit.AddDynamic(this, &UALSXTImpactReactionComponent::OnCapsuleHit);

	PhysicalAnimation->SetSkeletalMeshComponent(GetMesh());
	ResetPhysicalAnimationMode();
	GetMesh()->SetEnablePhysicsBlending(true);
	// FreelookTimerDelegate.BindUFunction(this, "FreelookTimer");
	BlendOutPhysicalAnimationTimerDelegate.BindUFunction(this, "BlendOutPhysicalAnimation");
	RefreshOverlayObject();
	IALSXTCharacterInterface::Execute_GetCharacterCameraAnimationInstance(this)->OnFirstPersonOverrideChanged.AddDynamic(this, &AALSXTCharacter::OnFirstPersonOverrideChanged);

}

void AALSXTCharacter::CalcCamera(const float DeltaTime, FMinimalViewInfo& ViewInfo)
{
	if (Camera->IsActive())
	{
		Camera->GetViewInfo(ViewInfo);
		return;
	}

	Super::CalcCamera(DeltaTime, ViewInfo);
}

void AALSXTCharacter::SetupPlayerInputComponent(UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);

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
		EnhancedInput->BindAction(LeanLeftAction, ETriggerEvent::Triggered, this, &ThisClass::InputLeanLeft);
		EnhancedInput->BindAction(ToggleLeanLeftAction, ETriggerEvent::Triggered, this, &ThisClass::InputLeanLeft);
		EnhancedInput->BindAction(LeanRightAction, ETriggerEvent::Triggered, this, &ThisClass::InputLeanRight);
		EnhancedInput->BindAction(ToggleLeanRightAction, ETriggerEvent::Triggered, this, &ThisClass::InputLeanRight);
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
		EnhancedInput->BindAction(SelectEmoteAction, ETriggerEvent::Triggered, this, &ThisClass::InputSelectEmote);
		EnhancedInput->BindAction(SelectGestureAction, ETriggerEvent::Triggered, this, &ThisClass::InputSelectGesture);

		InputMantleValue = EnhancedInput->GetBoundActionValue(MantleAction);
		
		OnSetupPlayerInputComponentUpdated.Broadcast();
	}
}

void AALSXTCharacter::OnStanceChanged_Implementation(const FGameplayTag& PreviousStance)
{
	ImpactReaction->RefreshObstacleNavigationPoses();
}

FALSXTCameraShakeSetting AALSXTCharacter::SelectMovementCameraShakeAsset()
{
	FALSXTCameraShakeSetting FoundCameraShakeSetting;
	auto CameraShakeSettings = ALSXTSettings->OverlaySettings.CameraShake.Find(GetOverlayMode());
	if (CameraShakeSettings)
	{
		if (GetStance() == AlsStanceTags::Standing)
		{
			if (GetDesiredGait() == AlsGaitTags::Running)
			{
				FoundCameraShakeSetting = CameraShakeSettings->RunningCameraShake;
				return CameraShakeSettings->RunningCameraShake;
			}
			else if (GetDesiredGait() == AlsGaitTags::Sprinting)
			{
				FoundCameraShakeSetting = CameraShakeSettings->SprintingCameraShake;
				return FoundCameraShakeSetting;
			}
			else
			{
				FoundCameraShakeSetting = CameraShakeSettings->WalkingCameraShake;
				return FoundCameraShakeSetting;
			}
		}
		else
		{
			if (GetDesiredGait() == AlsGaitTags::Running)
			{
				FoundCameraShakeSetting = CameraShakeSettings->CrouchRunningCameraShake;
				return FoundCameraShakeSetting;
			}
			else
			{
				FoundCameraShakeSetting = CameraShakeSettings->CrouchWalkingCameraShake;
				return FoundCameraShakeSetting;
			}
		}
	}
	return FoundCameraShakeSetting;	
}

// TODO: Implement requiring holding down Jump to Mantle and Vault
bool AALSXTCharacter::IsMantlingAllowedToStart_Implementation() const
{
	if (IsLocallyControlled())
	{
		if (ALSXTSettings->LocomotionActionSettings.bHoldJumpToMantle)
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

void AALSXTCharacter::DisableInputMovement(const bool Disable)
{
	bMovementEnabled = !Disable;
	// Disable ? ALSXTCharacterMovement->DisableMovement() : GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	// Disable ? ALSXTCharacterMovement->SetMovementMode(EMovementMode::MOVE_None) : ALSXTCharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AALSXTCharacter::DisableLookAt(const bool Disable)
{

}

void AALSXTCharacter::InputLookMouse(const FInputActionValue& ActionValue)
{
	FVector2D Value{ActionValue.Get<FVector2D>()};

	if (Freelooking == ALSXTFreelookingTags::True)
	{
		Value = FreelookState.LockedLookInput;
	}
	else
	{
		Value = ActionValue.Get<FVector2D>();
	}

	if (Value.Length() > 0.0)
	{
		IdleAnimation->ResetIdleCounterTimer();
	}

	AddControllerPitchInput(Value.Y * LookUpMouseSensitivity);
	PreviousLookInput.Y = Value.Y;
	AddControllerYawInput(Value.X * LookRightMouseSensitivity);
	PreviousLookInput.X = Value.X;
}

void AALSXTCharacter::InputLook(const FInputActionValue& ActionValue)
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
	if (Value.Length() > 0.0)
	{
		IdleAnimation->ResetIdleCounterTimer();
	}
	AddControllerPitchInput(Value.Y * LookUpRate * GetWorld()->GetDeltaSeconds());
	PreviousLookInput.Y = Value.Y;
	AddControllerYawInput(Value.X * LookRightRate * GetWorld()->GetDeltaSeconds());
	PreviousLookInput.X = Value.X;
}

void AALSXTCharacter::InputMove(const FInputActionValue& ActionValue)
{
	if (GetDesiredStatus() == ALSXTStatusTags::Normal && bMovementEnabled)
	{
		const auto Value{ UAlsVector::ClampMagnitude012D(ActionValue.Get<FVector2D>()) };

		if (Value.Length() > 0.0)
		{
			IdleAnimation->ResetIdleCounterTimer();
		}

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

void AALSXTCharacter::InputSprint(const FInputActionValue& ActionValue)
{
	if (CanSprint())
	{
		SetDesiredGait(ActionValue.Get<bool>() ? AlsGaitTags::Sprinting : AlsGaitTags::Running);
	}
}

void AALSXTCharacter::InputWalk()
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

void AALSXTCharacter::InputCrouch()
{
	IdleAnimation->ResetIdleCounterTimer();
	if (GetDesiredStance() == AlsStanceTags::Standing)
	{
		if (CanSlide())
		{
			TryStartSliding(1.3f);
		}
		else {
			SetDesiredStance(AlsStanceTags::Crouching);
		}
	}
	else if (GetDesiredStance() == AlsStanceTags::Crouching)
	{
		SetDesiredStance(AlsStanceTags::Standing);
		if (GetDesiredStatus() != ALSXTStatusTags::Normal)
		{
			SetDesiredStatus(ALSXTStatusTags::Normal);
		}
	}
}

void AALSXTCharacter::InputJump(const FInputActionValue& ActionValue)
{
	if (CanJump() && bMovementEnabled)
	{
		if (ActionValue.Get<bool>())
		{
			IdleAnimation->ResetIdleCounterTimer();
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

void AALSXTCharacter::InputMantle(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		// RefreshMantle();
	}
	else
	{

	}
}

void AALSXTCharacter::InputAim(const FInputActionValue& ActionValue)
{
	if (CanAim())
	{
		SetDesiredAiming(ActionValue.Get<bool>());

		if (ActionValue.Get<bool>())
		{
			SetDesiredRotationMode(AlsRotationModeTags::Aiming);
			IdleAnimation->ResetIdleCounterTimer();
			if (GetDesiredCombatStance() == ALSXTCombatStanceTags::Ready)
			{
				SetDesiredCombatStance(ALSXTCombatStanceTags::Aiming);
			}
			if (IALSXTHeldItemInterface::Execute_IsHoldingAimableItem(this)) {
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
			if (IALSXTHeldItemInterface::Execute_IsHoldingAimableItem(this)) {
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

void AALSXTCharacter::InputToggleAim()
{
	if (IALSXTHeldItemInterface::Execute_IsHoldingAimableItem(this)) 
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

void AALSXTCharacter::InputFocus(const FInputActionValue& ActionValue)
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

void AALSXTCharacter::InputRagdoll()
{
	if (!StopRagdolling())
	{
		IdleAnimation->ResetIdleCounterTimer();
		StartRagdolling();
	}
}

void AALSXTCharacter::InputRoll()
{
	static constexpr auto PlayRate{ 1.3f };
	if(CanRoll() && bMovementEnabled)
	{
		IdleAnimation->ResetIdleCounterTimer();
		StartRolling(PlayRate);
	}
}

void AALSXTCharacter::InputRotationMode()
{
	SetDesiredRotationMode(GetDesiredRotationMode() == AlsRotationModeTags::VelocityDirection
		                       ? AlsRotationModeTags::ViewDirection
		                       : AlsRotationModeTags::VelocityDirection);
}

void AALSXTCharacter::InputViewMode()
{
	const FGameplayTag PreviousViewMode{ GetViewMode() };
	auto DesiredViewMode{ FGameplayTag::EmptyTag };
	DesiredViewMode == (GetViewMode() == AlsViewModeTags::ThirdPerson ? AlsViewModeTags::FirstPerson : AlsViewModeTags::ThirdPerson);
	if (CanSetToViewMode(DesiredViewMode)) 
	{
		IdleAnimation->ResetIdleCounterTimer();
		SetViewMode(GetViewMode() == AlsViewModeTags::ThirdPerson ? AlsViewModeTags::FirstPerson : AlsViewModeTags::ThirdPerson);
		OnViewModeChanged(PreviousViewMode);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AALSXTCharacter::InputSwitchShoulder()
{
	IdleAnimation->ResetIdleCounterTimer();
	Camera->SetRightShoulder(!Camera->IsRightShoulder());
}

void AALSXTCharacter::InputFreelook(const FInputActionValue& ActionValue)
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

void AALSXTCharacter::InputSwitchGripPosition()
{
	if (CanSwitchGripPosition())
	{
		IdleAnimation->ResetIdleCounterTimer();
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

void AALSXTCharacter::InputSwitchForegripPosition()
{
	if (CanSwitchForegripPosition())
	{
		IdleAnimation->ResetIdleCounterTimer();
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
		FALSXTHeldItemState NewHeldItemState = GetHeldItemState();
		NewHeldItemState.GripState.Foregrip.Grip = IALSXTHeldItemInterface::Execute_GetHeldItemForegrip(this);
		SetHeldItemState(NewHeldItemState);
	}
}

void AALSXTCharacter::ApplyDesiredStance()
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
		if ((GetLocomotionAction() == AlsLocomotionActionTags::Rolling && ALSXTSettings->Rolling.bCrouchOnStart) || (GetLocomotionAction() == AlsLocomotionActionTags::Sliding && ALSXTSettings->Sliding.bCrouchOnStart))
		{
			Crouch();
		}
	}
}

// Input Actions

void AALSXTCharacter::InputSelectEmote(const FInputActionValue& ActionValue)
{
	if (CanSelectEmote())
	{
		// 
	}
}

void AALSXTCharacter::InputSelectGesture(const FInputActionValue& ActionValue)
{
	if (CanSelectGesture())
	{
		//
	}
}

bool AALSXTCharacter::CanFreelook_Implementation() const {return true;}

void AALSXTCharacter::ALSXTRefreshRotationInstant(const float TargetYawAngle, const ETeleportType Teleport)
{
	SetRotationInstant(TargetYawAngle, Teleport);
}

void AALSXTCharacter::SetMovementModeLocked(bool bNewMovementModeLocked)
{
	bMovementEnabled = !bNewMovementModeLocked;
	ForceNetUpdate();
}

const float AALSXTCharacter::GetCameraZoom() const
{
	return CameraZoom;
}

void AALSXTCharacter::SetCameraZoom(const float NewCameraZoom)
{
	CameraZoom = NewCameraZoom;
}

void AALSXTCharacter::Crouch(const bool bClientSimulation)
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

void AALSXTCharacter::StartBlendOutPhysicalAnimation()
{
	GetWorld()->GetTimerManager().SetTimer(BlendOutPhysicalAnimationTimerHandle, BlendOutPhysicalAnimationTimerDelegate, 0.01f, true);
}

void AALSXTCharacter::BlendOutPhysicalAnimation()
{
	FALSXTPhysicalAnimationState NewPhysicalAnimationState = GetPhysicalAnimationState();
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

void AALSXTCharacter::EndBlendOutPhysicalAnimation()
{
	GetWorld()->GetTimerManager().ClearTimer(BlendOutPhysicalAnimationTimerHandle);
	FALSXTPhysicalAnimationState NewPhysicalAnimationState;
	NewPhysicalAnimationState.Mode = FGameplayTag::EmptyTag;
	NewPhysicalAnimationState.ProfileName = "CharacterMesh";
	SetPhysicalAnimationState(NewPhysicalAnimationState);
	FALSXTDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
	NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
	NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
	NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;

	if (GetDefensiveModeState().AnticipationMode != ALSXTDefensiveModeTags::Anticipation)
	{
		NewDefensiveModeState.Mode = ALSXTPhysicalAnimationModeTags::None;
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
	SetPhysicalAnimationMode(ALSXTPhysicalAnimationModeTags::None, EmptyNames);
}

void AALSXTCharacter::OnOverlayModeChanged_Implementation(const FGameplayTag& PreviousOverlayMode)
{
	// Super::OnOverlayModeChanged(PreviousOverlayMode);
	ImpactReaction->RefreshBlockingPoses();
	ImpactReaction->RefreshCrowdNavigationPoses();
	RefreshOverlayLinkedAnimationLayer();
	RefreshOverlayObject();
}

void AALSXTCharacter::OnJumped_Implementation()
{
	FALSXTCharacterVoiceParameters CharacterVoiceParams = IALSXTCharacterCustomizationComponentInterface::Execute_GetVoiceParameters(this);
	CharacterSound->PlayActionSound(true, true, true, ALSXTCharacterMovementSoundTags::Jumping, CharacterVoiceParams.Sex, CharacterVoiceParams.Variant, IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(this), ALSXTActionStrengthTags::Medium, IALSXTCharacterInterface::Execute_GetStamina(this));

}

void AALSXTCharacter::OnMantlingStarted_Implementation(const FAlsMantlingParameters& Parameters)
{
	FGameplayTag TypeTag{ FGameplayTag::EmptyTag };
	FALSXTCharacterVoiceParameters CharacterVoiceParams = IALSXTCharacterCustomizationComponentInterface::Execute_GetVoiceParameters(this);

	if (Parameters.MantlingType == EAlsMantlingType::Low)
	{
		TypeTag = ALSXTCharacterMovementSoundTags::MantlingLow;
		ClearOverlayObject();
	}
	else
	{
		TypeTag = ALSXTCharacterMovementSoundTags::MantlingHigh;
	}

	CharacterSound->PlayActionSound(true, true, true, TypeTag, CharacterVoiceParams.Sex, CharacterVoiceParams.Variant, IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(this), ALSXTActionStrengthTags::Medium, IALSXTCharacterInterface::Execute_GetStamina(this));
}

void AALSXTCharacter::OnMantlingEnded_Implementation()
{
	ClearOverlayObject();
}

void AALSXTCharacter::OnRagdollingStarted_Implementation()
{
	FALSXTCharacterVoiceParameters CharacterVoiceParams = IALSXTCharacterCustomizationComponentInterface::Execute_GetVoiceParameters(this);
	RefreshOverlayObject();
	CharacterSound->PlayDamageSound(true, true, true, CharacterVoiceParams.Sex, CharacterVoiceParams.Variant, IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(this), ALSXTAttackMethodTags::Regular, ALSXTActionStrengthTags::Medium, ALSXTImpactFormTags::Blunt, IALSXTCharacterInterface::Execute_GetStamina(this));
}

void AALSXTCharacter::OnRagdollingEnded_Implementation()
{
	RefreshOverlayObject();
}

void AALSXTCharacter::OnSlidingStarted_Implementation()
{
	FALSXTCharacterVoiceParameters CharacterVoiceParams = IALSXTCharacterCustomizationComponentInterface::Execute_GetVoiceParameters(this);
	CharacterSound->PlayActionSound(true, true, true, ALSXTCharacterMovementSoundTags::Sliding, CharacterVoiceParams.Sex, CharacterVoiceParams.Variant, IALSXTCharacterInterface::Execute_GetCharacterOverlayMode(this), ALSXTActionStrengthTags::Medium, IALSXTCharacterInterface::Execute_GetStamina(this));
}

bool AALSXTCharacter::IsHoldingItem_Implementation() const
{
	return (IsValid(OverlaySkeletalMesh) || IsValid(OverlayStaticMesh));
}

bool AALSXTCharacter::IsHoldingAimableItem_Implementation() const
{
	
	return IALSXTHeldItemInterface::Execute_IsHoldingItem(this) && ALSXTSettings->OverlaySettings.AimableOverlayModes.HasTagExact(OverlayMode);
	// return (IsDesiredAiming() && CanAimDownSights() && (GetViewMode() == AlsViewModeTags::FirstPerson) && (GetDesiredCombatStance() != ALSXTCombatStanceTags::Neutral));
}

bool AALSXTCharacter::IsAimingDownSights_Implementation() const
{
	return (IsDesiredAiming() && CanAimDownSights() && (GetViewMode() == AlsViewModeTags::FirstPerson) && (GetDesiredCombatStance() != ALSXTCombatStanceTags::Neutral));
}

FALSXTGlobalGeneralMeshPaintingSettings AALSXTCharacter::GetGlobalGeneralMeshPaintingSettings_Implementation() const
{
	return ALSXTSettings->MeshPainting;
}

USceneCaptureComponent2D* AALSXTCharacter::GetSceneCaptureComponent_Implementation() const
{
	return MeshPaintingSceneCapture;
}

void AALSXTCharacter::GetElementalCondition_Implementation(UPrimitiveComponent* Component, FGameplayTag& ElementalCondition) const
{
	if (Cast<UALSXTPaintableSkeletalMeshComponent>(Component))
	{
		ElementalCondition = Cast<UALSXTPaintableSkeletalMeshComponent>(Component)->GetElementalCondition();
	}
	if (Cast<UALSXTPaintableStaticMeshComponent>(Component))
	{
		ElementalCondition = Cast<UALSXTPaintableStaticMeshComponent>(Component)->GetElementalCondition();
	}
}

void AALSXTCharacter::PaintMesh_Implementation(UPrimitiveComponent* Component, EPhysicalSurface SurfaceType, const FGameplayTag PaintType, FVector Location, float Radius)
{
	if (Cast<UALSXTPaintableSkeletalMeshComponent>(Component))
	{
		Cast<UALSXTPaintableSkeletalMeshComponent>(Component)->PaintMesh(SurfaceType, PaintType, Location, Radius);
	}
	if (Cast<UALSXTPaintableStaticMeshComponent>(Component))
	{
		Cast<UALSXTPaintableStaticMeshComponent>(Component)->PaintMesh(SurfaceType, PaintType, Location, Radius);
	}
}

void AALSXTCharacter::VolumePaintMesh_Implementation(UPrimitiveComponent* Component, EPhysicalSurface SurfaceType, const FGameplayTag PaintType, FVector Origin, FVector Extent)
{
	if (Cast<UALSXTPaintableSkeletalMeshComponent>(Component))
	{
		Cast<UALSXTPaintableSkeletalMeshComponent>(Component)->VolumePaintMesh(SurfaceType, PaintType, Origin, Extent);
	}
	if (Cast<UALSXTPaintableStaticMeshComponent>(Component))
	{
		Cast<UALSXTPaintableStaticMeshComponent>(Component)->VolumePaintMesh(SurfaceType, PaintType, Origin, Extent);
	}
}

void AALSXTCharacter::ResetPaintTypeOnComponent_Implementation(UPrimitiveComponent* Component, const FGameplayTag PaintType)
{
	if (Cast<UALSXTPaintableSkeletalMeshComponent>(Component))
	{
		Cast<UALSXTPaintableSkeletalMeshComponent>(Component)->ResetAllChannels();
	}
	if (Cast<UALSXTPaintableStaticMeshComponent>(Component))
	{
		Cast<UALSXTPaintableStaticMeshComponent>(Component)->ResetAllChannels();
	}
}

void AALSXTCharacter::ResetPaintOnComponent_Implementation(UPrimitiveComponent* Component)
{
	if (Cast<UALSXTPaintableSkeletalMeshComponent>(Component))
	{
		Cast<UALSXTPaintableSkeletalMeshComponent>(Component)->ResetAllChannels();
	}
	if (Cast<UALSXTPaintableStaticMeshComponent>(Component))
	{
		Cast<UALSXTPaintableStaticMeshComponent>(Component)->ResetAllChannels();
	}
}

void AALSXTCharacter::ResetPaintTypeOnAllComponents_Implementation(const FGameplayTag PaintType)
{

}

void AALSXTCharacter::ResetPaintOnAllComponents_Implementation()
{
	
}

// PhysicalAnimation State
void AALSXTCharacter::SetPhysicalAnimationState(const FALSXTPhysicalAnimationState& NewPhysicalAnimationState)
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

void AALSXTCharacter::OnPhysicalAnimationStateChanged(const FALSXTPhysicalAnimationState& PreviousPhysicalAnimationState)
{

}

void AALSXTCharacter::ServerSetPhysicalAnimationState_Implementation(const FALSXTPhysicalAnimationState& NewPhysicalAnimationState)
{
	MulticastSetPhysicalAnimationState(NewPhysicalAnimationState);
}

void AALSXTCharacter::MulticastSetPhysicalAnimationState_Implementation(const FALSXTPhysicalAnimationState& NewPhysicalAnimationState)
{
	SetPhysicalAnimationState(NewPhysicalAnimationState);
}

void AALSXTCharacter::ServerProcessNewPhysicalAnimationState_Implementation(const FALSXTPhysicalAnimationState& NewPhysicalAnimationState)
{
	ProcessNewPhysicalAnimationState(NewPhysicalAnimationState);
}

// void AALSXTCharacter::OnReplicate_PhysicalAnimationState(const FALSXTPhysicalAnimationState& PreviousPhysicalAnimationState)
// {
// 	OnPhysicalAnimationStateChanged(PreviousPhysicalAnimationState);
// }

// Held Item State
void AALSXTCharacter::SetHeldItemState(const FALSXTHeldItemState& NewHeldItemState)
{
	const auto PreviousHeldItemState{ HeldItemState };
	HeldItemState = NewHeldItemState;
	OnHeldItemStateChanged(PreviousHeldItemState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetHeldItemState(NewHeldItemState);
	}
}

void AALSXTCharacter::OnHeldItemStateChanged(const FALSXTHeldItemState& PreviousHeldItemState)
{

}

void AALSXTCharacter::ServerSetHeldItemState_Implementation(const FALSXTHeldItemState& NewHeldItemState)
{
	MulticastSetHeldItemState(NewHeldItemState);
}

void AALSXTCharacter::MulticastSetHeldItemState_Implementation(const FALSXTHeldItemState& NewHeldItemState)
{
	SetHeldItemState(NewHeldItemState);
}

void AALSXTCharacter::ServerProcessNewHeldItemState_Implementation(const FALSXTHeldItemState& NewHeldItemState)
{
	ProcessNewHeldItemState(NewHeldItemState);
}

void AALSXTCharacter::OnReplicate_HeldItemState(const FALSXTHeldItemState& PreviousHeldItemState)
{
	OnHeldItemStateChanged(PreviousHeldItemState);
}


// Breath State
void AALSXTCharacter::SetBreathState(const FALSXTBreathState& NewBreathState)
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

void AALSXTCharacter::ServerSetBreathState_Implementation(const FALSXTBreathState& NewBreathState)
{
	SetBreathState(NewBreathState);
}

void AALSXTCharacter::ClientSetBreathState_Implementation(const FALSXTBreathState& NewBreathState)
{
	SetBreathState(NewBreathState);
}

void AALSXTCharacter::ServerProcessNewBreathState_Implementation(const FALSXTBreathState& NewBreathState)
{
	ProcessNewBreathState(NewBreathState);
}

void AALSXTCharacter::OnReplicate_BreathState(const FALSXTBreathState& PreviousBreathState)
{
	OnBreathStateChanged(PreviousBreathState);
}

void AALSXTCharacter::OnBreathStateChanged_Implementation(const FALSXTBreathState& PreviousBreathState) {}

// ALSXT Pose State
void AALSXTCharacter::SetALSXTPoseState(const FALSXTPoseState& NewALSXTPoseState)
{
	const auto PreviousALSXTPoseState{ ALSXTPoseState };

	ALSXTPoseState = NewALSXTPoseState;

	OnALSXTPoseStateChanged(PreviousALSXTPoseState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetALSXTPoseState(NewALSXTPoseState);
	}
}

void AALSXTCharacter::ServerSetALSXTPoseState_Implementation(const FALSXTPoseState& NewALSXTPoseState)
{
	SetALSXTPoseState(NewALSXTPoseState);
}

void AALSXTCharacter::ServerProcessNewALSXTPoseState_Implementation(const FALSXTPoseState& NewALSXTPoseState)
{
	ProcessNewALSXTPoseState(NewALSXTPoseState);
}

void AALSXTCharacter::OnReplicate_ALSXTPoseState(const FALSXTPoseState& PreviousALSXTPoseState)
{
	OnALSXTPoseStateChanged(PreviousALSXTPoseState);
}

void AALSXTCharacter::OnALSXTPoseStateChanged_Implementation(const FALSXTPoseState& PreviousALSXTPoseState) {}


// Vaulting State
void AALSXTCharacter::SetVaultingState(const FALSXTVaultingState& NewVaultingState)
{
	const auto PreviousVaultingState{ VaultingState };

	VaultingState = NewVaultingState;

	OnVaultingStateChanged(PreviousVaultingState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetVaultingState(NewVaultingState);
	}
}

void AALSXTCharacter::ServerSetVaultingState_Implementation(const FALSXTVaultingState& NewVaultingState)
{
	SetVaultingState(NewVaultingState);
}


void AALSXTCharacter::ServerProcessNewVaultingState_Implementation(const FALSXTVaultingState& NewVaultingState)
{
	ProcessNewVaultingState(NewVaultingState);
}

void AALSXTCharacter::OnReplicate_VaultingState(const FALSXTVaultingState& PreviousVaultingState)
{
	OnVaultingStateChanged(PreviousVaultingState);
}

void AALSXTCharacter::OnVaultingStateChanged_Implementation(const FALSXTVaultingState& PreviousVaultingState) {}

// Stationary Mode State
void AALSXTCharacter::SetStationaryModeState(const FALSXTStationaryModeState& NewStationaryModeState)
{
	const auto PreviousStationaryModeState{ StationaryModeState };

	StationaryModeState = NewStationaryModeState;

	OnStationaryModeStateChanged(PreviousStationaryModeState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetStationaryModeState(NewStationaryModeState);
	}
}

void AALSXTCharacter::ServerSetStationaryModeState_Implementation(const FALSXTStationaryModeState& NewStationaryModeState)
{
	SetStationaryModeState(NewStationaryModeState);
}


void AALSXTCharacter::ServerProcessNewStationaryModeState_Implementation(const FALSXTStationaryModeState& NewStationaryModeState)
{
	ProcessNewStationaryModeState(NewStationaryModeState);
}

void AALSXTCharacter::OnReplicate_StationaryModeState(const FALSXTStationaryModeState& PreviousStationaryModeState)
{
	OnStationaryModeStateChanged(PreviousStationaryModeState);
}

void AALSXTCharacter::OnStationaryModeStateChanged_Implementation(const FALSXTStationaryModeState& PreviousStationaryModeState) {}

// Footprints State
void AALSXTCharacter::ClientSetFootprintsState(const EAlsFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState)
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

void AALSXTCharacter::ResetFootSaturationTimeline_Implementation(const EAlsFootBone& Foot)
{
	GetFootTimeline(Foot)->SetPlaybackPosition(0.0, false, false);
}

void AALSXTCharacter::MulticastSetFootprintsState_Implementation(const EAlsFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState)
{
	SetFootprintsState(Foot, NewFootprintsState);
}

void AALSXTCharacter::SetFootprintsState(const EAlsFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState)
{
	const auto PreviousFootprintsState{ FootprintsState };

	FootprintsState = NewFootprintsState;

	OnFootprintsStateChanged(PreviousFootprintsState);
}

void AALSXTCharacter::ServerSetFootprintsState_Implementation(const EAlsFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState)
{
	MulticastSetFootprintsState(Foot, NewFootprintsState);
}


void AALSXTCharacter::ServerProcessNewFootprintsState_Implementation(const EAlsFootBone& Foot, const FALSXTFootprintsState& NewFootprintsState)
{
	ProcessNewFootprintsState(Foot, NewFootprintsState);
}

void AALSXTCharacter::OnReplicate_FootprintsState(const FALSXTFootprintsState& PreviousFootprintsState)
{
	OnFootprintsStateChanged(PreviousFootprintsState);
}

void AALSXTCharacter::OnFootprintsStateChanged_Implementation(const FALSXTFootprintsState& PreviousFootprintsState) {}

void AALSXTCharacter::InputToggleGait()
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

void AALSXTCharacter::InputToggleCombatReady()
{
	if (CanToggleCombatReady())
	{
		if ((GetDesiredCombatStance() == FGameplayTag::EmptyTag) || (GetDesiredCombatStance() == ALSXTCombatStanceTags::Neutral))
		{
			if (CanBecomeCombatReady())
			{
				SetDesiredCombatStance(ALSXTCombatStanceTags::Ready);
				if (IALSXTHeldItemInterface::Execute_IsHoldingAimableItem(this))
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

void AALSXTCharacter::InputBlock(const FInputActionValue& ActionValue)
{
	FALSXTDefensiveModeState PreviousDefensiveModeState = GetDefensiveModeState();

	if (CanEnterBlockingDefensiveMode())
	{
		if (ActionValue.Get<bool>() == true)
		{
			FALSXTDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
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
			FALSXTDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
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
		FALSXTDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
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


void AALSXTCharacter::InputLeanLeft(const FInputActionValue & ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		if (CanLean())
		{
			SetDesiredLean(ALSXTLeanDirectionTags::Left);
			FALSXTPoseState NewPoseState = GetALSXTPoseState();
			NewPoseState.LeanDirection = ALSXTLeanDirectionTags::Left;
			SetALSXTPoseState(NewPoseState);
		}
	}
	else
	{
		SetDesiredLean(FGameplayTag::EmptyTag);
		FALSXTPoseState NewPoseState = GetALSXTPoseState();
		NewPoseState.LeanDirection = FGameplayTag::EmptyTag;
		SetALSXTPoseState(NewPoseState);
	}
}

void AALSXTCharacter::InputLeanRight(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		if (CanLean())
		{
			SetDesiredLean(ALSXTLeanDirectionTags::Right);
			FALSXTPoseState NewPoseState = GetALSXTPoseState();
			NewPoseState.LeanDirection = ALSXTLeanDirectionTags::Right;
			SetALSXTPoseState(NewPoseState);
		}
	}
	else
	{
		SetDesiredLean(FGameplayTag::EmptyTag);
		FALSXTPoseState NewPoseState = GetALSXTPoseState();
		NewPoseState.LeanDirection = FGameplayTag::EmptyTag;
		SetALSXTPoseState(NewPoseState);
	}
}

bool AALSXTCharacter::CanPerformPrimaryInteraction_Implementation() const
{
	return true;
}

bool AALSXTCharacter::CanPerformSecondaryInteraction_Implementation() const
{
	return true;
}

void AALSXTCharacter::InputSwitchWeaponFirearmStance()
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

void AALSXTCharacter::InputSwitchWeaponReadyPosition()
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

void AALSXTCharacter::OnFirstPersonOverrideChanged_Implementation(float FirstPersonOverride)
{
	//
}

void AALSXTCharacter::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& Unused, float& VerticalLocation)
{
	if (Camera->IsActive())
	{
		Camera->DisplayDebug(Canvas, DebugDisplay, VerticalLocation);
	}

	Super::DisplayDebug(Canvas, DebugDisplay, Unused, VerticalLocation);
}

// First Person Eye Focus

bool AALSXTCharacter::IsFirstPersonEyeFocusActive() const
{
	if (GetViewMode() == AlsViewModeTags::FirstPerson) 
	{
		if (GetDesiredFocus() == ALSXTFocusedTags::True) 
		{
			if (IALSXTHeldItemInterface::Execute_IsHoldingAimableItem(this))
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

void AALSXTCharacter::SetDesiredLean(const FGameplayTag& NewLeanTag)
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

void AALSXTCharacter::ServerSetDesiredLean_Implementation(const FGameplayTag& NewLeanTag)
{
	SetDesiredLean(NewLeanTag);
}

void AALSXTCharacter::SetLean(const FGameplayTag& NewLeanTag)
{

	if (Lean != NewLeanTag)
	{
		IdleAnimation->ResetIdleCounterTimer();
		const auto PreviousLean{ Lean };

		Lean = NewLeanTag;

		OnLeanChanged(PreviousLean);
	}
}

void AALSXTCharacter::OnLeanChanged_Implementation(const FGameplayTag& PreviousLeanTag) {}

// Freelooking

void AALSXTCharacter::IsFreelooking(bool& bIsFreelooking, bool& bIsFreelookingInFirstPerson) const
{

	bIsFreelooking = (GetDesiredFreelooking() == ALSXTFreelookingTags::True) ? true : false;
	bIsFreelookingInFirstPerson = (bIsFreelooking && ((GetViewMode() == AlsViewModeTags::FirstPerson) ? true : false));
}

void AALSXTCharacter::ActivateFreelooking()
{
	IdleAnimation->ResetIdleCounterTimer();
	//PreviousYaw = GetViewState().Rotation.Yaw;
	PreviousYaw = FMath::GetMappedRangeValueClamped(FVector2D(0, 359.998993), FVector2D(0.0, 1.0), GetControlRotation().Yaw);
	//FMath::GetMappedRangeValueClamped(FVector2D(-90,90), FVector2D(0,1), GetViewState().Rotation.Pitch)
	PreviousPitch = FMath::GetMappedRangeValueClamped(FVector2D(89.900002, -89.899994), FVector2D(0.0, 1.0), GetViewState().Rotation.Pitch);
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%f"), GetControlRotation().Yaw));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%f"), PreviousYaw));
	// LockRotation(GetActorRotation().Yaw);
	SetDesiredRotationMode(AlsRotationModeTags::ViewDirection);
	SetDesiredFreelooking(ALSXTFreelookingTags::True);
	FALSXTFreelookState NewFreelookState = GetFreelookState();
	NewFreelookState.Freelooking = ALSXTFreelookingTags::True;
	NewFreelookState.LockedControlRotation = GetControlRotation();
	NewFreelookState.LockedViewState = GetViewState();
	SetFreelookState(NewFreelookState);
}

void AALSXTCharacter::DeactivateFreelooking()
{
	// UnLockRotation();
	SetDesiredFreelooking(ALSXTFreelookingTags::False);
	FALSXTFreelookState NewFreelookState = GetFreelookState();
	NewFreelookState.Freelooking = ALSXTFreelookingTags::False;
	SetFreelookState(NewFreelookState);
	// HeadLookAtState.LockedViewState.PreviousYawAngle = 0.0f;
	// HeadLookAtState.LockedViewState.Rotation = FRotator(0.0f, 0.0f, 0.0f);
	// HeadLookAtState.LockedViewState.YawSpeed = 0.0f;
}

void AALSXTCharacter::SetDesiredFreelooking(const FGameplayTag& NewFreelookingTag)
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

void AALSXTCharacter::ServerSetDesiredFreelooking_Implementation(const FGameplayTag& NewFreelookingTag)
{
	SetDesiredFreelooking(NewFreelookingTag);
}

void AALSXTCharacter::SetFreelooking(const FGameplayTag& NewFreelookingTag)
{

	if (Freelooking != NewFreelookingTag)
	{
		const auto PreviousFreelooking{ Freelooking };

		Freelooking = NewFreelookingTag;
		FALSXTFreelookState NewState = GetFreelookState();

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

void AALSXTCharacter::OnFreelookingChanged_Implementation(const FGameplayTag& PreviousFreelookingTag) 
{
	
}

// Emote

void AALSXTCharacter::SetDesiredEmote(const FGameplayTag& NewEmoteTag)
{
	if (DesiredEmote != NewEmoteTag)
	{
		DesiredEmote = NewEmoteTag;
		Emotes->AddDesiredEmote(NewEmoteTag);

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredEmote, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredEmote(NewEmoteTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredEmote_Implementation(const FGameplayTag& NewEmoteTag)
{
	SetDesiredEmote(NewEmoteTag);
}

void AALSXTCharacter::SetEmote(const FGameplayTag& NewEmoteTag)
{

	if (Emote != NewEmoteTag)
	{
		const auto PreviousEmote{ Emote };

		Gesture = NewEmoteTag;
		Emotes->AddDesiredEmote(NewEmoteTag);
		OnEmoteChanged(PreviousEmote);
	}
}

void AALSXTCharacter::OnEmoteChanged_Implementation(const FGameplayTag& PreviousEmoteeTag) {}

// Gesture

void AALSXTCharacter::SetDesiredGesture(const FGameplayTag& NewGestureTag)
{
	if (DesiredGesture != NewGestureTag)
	{
		DesiredGesture = NewGestureTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredGesture, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredGesture(NewGestureTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredGesture_Implementation(const FGameplayTag& NewGestureTag)
{
	SetDesiredGesture(NewGestureTag);
}

void AALSXTCharacter::SetGesture(const FGameplayTag& NewGestureTag)
{

	if (Gesture != NewGestureTag)
	{
		const auto PreviousGesture{ Gesture };

		Gesture = NewGestureTag;

		OnGestureChanged(PreviousGesture);
	}
}

void AALSXTCharacter::OnGestureChanged_Implementation(const FGameplayTag& PreviousGestureTag) {}

// GestureHand

void AALSXTCharacter::SetDesiredGestureHand(const FGameplayTag& NewGestureHandTag)
{
	if (DesiredGestureHand != NewGestureHandTag)
	{
		DesiredGestureHand = NewGestureHandTag;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredGestureHand, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredGestureHand(NewGestureHandTag);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredGestureHand_Implementation(const FGameplayTag& NewGestureHandTag)
{
	SetDesiredGestureHand(NewGestureHandTag);
}

void AALSXTCharacter::SetGestureHand(const FGameplayTag& NewGestureHandTag)
{

	if (GestureHand != NewGestureHandTag)
	{
		const auto PreviousGestureHand{ GestureHand };

		GestureHand = NewGestureHandTag;

		OnGestureHandChanged(PreviousGestureHand);
	}
}

void AALSXTCharacter::OnGestureHandChanged_Implementation(const FGameplayTag& PreviousGestureHandTag) {}

// Sex

void AALSXTCharacter::SetDesiredSex(const FGameplayTag& NewSexTag)
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

void AALSXTCharacter::ServerSetDesiredSex_Implementation(const FGameplayTag& NewSexTag)
{
	SetDesiredSex(NewSexTag);
}

void AALSXTCharacter::SetSex(const FGameplayTag& NewSexTag)
{

	if (Sex != NewSexTag)
	{
		const auto PreviousSex{ Sex };

		Sex = NewSexTag;

		OnSexChanged(PreviousSex);
	}
}

void AALSXTCharacter::OnSexChanged_Implementation(const FGameplayTag& PreviousSexTag) {}

// LocomotionVariant

void AALSXTCharacter::SetDesiredLocomotionVariant(const FGameplayTag& NewLocomotionVariantTag)
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

void AALSXTCharacter::ServerSetDesiredLocomotionVariant_Implementation(const FGameplayTag& NewLocomotionVariantTag)
{
	SetDesiredLocomotionVariant(NewLocomotionVariantTag);
}

void AALSXTCharacter::SetLocomotionVariant(const FGameplayTag& NewLocomotionVariantTag)
{

	if (LocomotionVariant != NewLocomotionVariantTag)
	{
		const auto PreviousLocomotionVariant{ LocomotionVariant };

		LocomotionVariant = NewLocomotionVariantTag;

		OnLocomotionVariantChanged(PreviousLocomotionVariant);
	}
}

void AALSXTCharacter::OnLocomotionVariantChanged_Implementation(const FGameplayTag& PreviousLocomotionVariantTag) {}

// Injury

void AALSXTCharacter::SetDesiredInjury(const FGameplayTag& NewInjuryTag)
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

void AALSXTCharacter::ServerSetDesiredInjury_Implementation(const FGameplayTag& NewInjuryTag)
{
	SetDesiredInjury(NewInjuryTag);
}

void AALSXTCharacter::SetInjury(const FGameplayTag& NewInjuryTag)
{

	if (Injury != NewInjuryTag)
	{
		const auto PreviousInjury{ Injury };

		Injury = NewInjuryTag;

		OnInjuryChanged(PreviousInjury);
	}
}

void AALSXTCharacter::OnInjuryChanged_Implementation(const FGameplayTag& PreviousInjuryTag) {}

// CombatStance

void AALSXTCharacter::SetDesiredCombatStance(const FGameplayTag& NewCombatStanceTag)
{
	if (DesiredCombatStance != NewCombatStanceTag)
	{
		DesiredCombatStance = NewCombatStanceTag;
		const auto PreviousCombatStance{ CombatStance };

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, DesiredCombatStance, this)

			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				ServerSetDesiredCombatStance(NewCombatStanceTag);
				if (NewCombatStanceTag != ALSXTCombatStanceTags::Neutral)
				{
					if (IALSXTHeldItemInterface::Execute_IsHoldingAimableItem(this))
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
				OnCombatStanceChanged(PreviousCombatStance);
			}
	}
}

void AALSXTCharacter::ServerSetDesiredCombatStance_Implementation(const FGameplayTag& NewCombatStanceTag)
{
	SetDesiredCombatStance(NewCombatStanceTag);
}

void AALSXTCharacter::SetCombatStance(const FGameplayTag& NewCombatStanceTag)
{

	if (CombatStance != NewCombatStanceTag)
	{
		IdleAnimation->ResetIdleCounterTimer();
		const auto PreviousCombatStance{ CombatStance };

		CombatStance = NewCombatStanceTag;

		OnCombatStanceChanged(PreviousCombatStance);
	}
}

void AALSXTCharacter::OnCombatStanceChanged_Implementation(const FGameplayTag& PreviousCombatStanceTag) {}

// WeaponFirearmStance

void AALSXTCharacter::SetDesiredWeaponFirearmStance(const FGameplayTag& NewWeaponFirearmStanceTag)
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

void AALSXTCharacter::ServerSetDesiredWeaponFirearmStance_Implementation(const FGameplayTag& NewWeaponFirearmStanceTag)
{
	SetDesiredWeaponFirearmStance(NewWeaponFirearmStanceTag);
}

void AALSXTCharacter::SetWeaponFirearmStance(const FGameplayTag& NewWeaponFirearmStanceTag)
{

	if (WeaponFirearmStance != NewWeaponFirearmStanceTag)
	{
		IdleAnimation->ResetIdleCounterTimer();
		const auto PreviousWeaponFirearmStance{ WeaponFirearmStance };

		WeaponFirearmStance = NewWeaponFirearmStanceTag;

		OnWeaponFirearmStanceChanged(PreviousWeaponFirearmStance);
	}
}

void AALSXTCharacter::OnWeaponFirearmStanceChanged_Implementation(const FGameplayTag& PreviousWeaponFirearmStanceTag) {}

// WeaponReadyPosition

void AALSXTCharacter::SetDesiredWeaponReadyPosition(const FGameplayTag& NewWeaponReadyPositionTag)
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

void AALSXTCharacter::ServerSetDesiredWeaponReadyPosition_Implementation(const FGameplayTag& NewWeaponReadyPositionTag)
{
	SetDesiredWeaponReadyPosition(NewWeaponReadyPositionTag);
}

void AALSXTCharacter::SetWeaponReadyPosition(const FGameplayTag& NewWeaponReadyPositionTag)
{

	if (WeaponReadyPosition != NewWeaponReadyPositionTag)
	{
		IdleAnimation->ResetIdleCounterTimer();
		const auto PreviousWeaponReadyPosition{ WeaponReadyPosition };

		WeaponReadyPosition = NewWeaponReadyPositionTag;

		OnWeaponReadyPositionChanged(PreviousWeaponReadyPosition);
	}
}

// DefensiveMode

bool AALSXTCharacter::IsInDefensiveMode() const
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

bool AALSXTCharacter::IsInAnticipationMode() const
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

void AALSXTCharacter::SetDesiredDefensiveMode(const FGameplayTag& NewDefensiveModeTag)
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

void AALSXTCharacter::ServerSetDesiredDefensiveMode_Implementation(const FGameplayTag& NewDefensiveModeTag)
{
	SetDesiredDefensiveMode(NewDefensiveModeTag);
}

void AALSXTCharacter::SetDefensiveMode(const FGameplayTag& NewDefensiveModeTag)
{
	if (DefensiveMode != NewDefensiveModeTag)
	{
		IdleAnimation->ResetIdleCounterTimer();
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

void AALSXTCharacter::OnDefensiveModeChanged_Implementation(const FGameplayTag& PreviousDefensiveModeTag) {}

void AALSXTCharacter::SetAimState(const FALSXTAimState& NewAimState)
{
	const auto PreviousAimState{ AimState };

	AimState = NewAimState;

	OnAimStateChanged(PreviousAimState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetAimState(NewAimState);
	}
}

void AALSXTCharacter::ServerSetAimState_Implementation(const FALSXTAimState& NewAimState)
{
	SetAimState(NewAimState);
}


void AALSXTCharacter::ServerProcessNewAimState_Implementation(const FALSXTAimState& NewAimState)
{
	ProcessNewAimState(NewAimState);
}

void AALSXTCharacter::OnReplicate_AimState(const FALSXTAimState& PreviousAimState)
{
	OnAimStateChanged(PreviousAimState);
}

void AALSXTCharacter::OnAimStateChanged_Implementation(const FALSXTAimState& PreviousAimState) {}


//Freelooking


void AALSXTCharacter::SetFreelookState(const FALSXTFreelookState& NewFreelookState)
{
	const auto PreviousFreelookState{ FreelookState };
	FreelookState = NewFreelookState;
	OnFreelookStateChanged(PreviousFreelookState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetFreelookState(NewFreelookState);
	}
}

bool AALSXTCharacter::DoesOverlayObjectUseLeftHandIK_Implementation() const
{
	return false;
}

void AALSXTCharacter::ServerSetFreelookState_Implementation(const FALSXTFreelookState& NewFreelookState)
{
	SetFreelookState(NewFreelookState);
}


void AALSXTCharacter::ServerProcessNewFreelookState_Implementation(const FALSXTFreelookState& NewFreelookState)
{
	ProcessNewFreelookState(NewFreelookState);
}

void AALSXTCharacter::OnReplicate_FreelookState(const FALSXTFreelookState& PreviousFreelookState)
{
	OnFreelookStateChanged(PreviousFreelookState);
}

void AALSXTCharacter::OnFreelookStateChanged_Implementation(const FALSXTFreelookState& PreviousFreelookState) {}

// Head Look At State

bool AALSXTCharacter::CanHeadLookAt_Implementation() const { return (GetDesiredFreelooking() == ALSXTFreelookingTags::False); };

void AALSXTCharacter::SetHeadLookAtState(const FALSXTHeadLookAtState& NewHeadLookAtState)
{
	const auto PreviousHeadLookAtState{ HeadLookAtState };
	HeadLookAtState = NewHeadLookAtState;
	OnHeadLookAtStateChanged(PreviousHeadLookAtState);

	if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	{
		ServerSetHeadLookAtState(NewHeadLookAtState);
	}
}

void AALSXTCharacter::ServerSetHeadLookAtState_Implementation(const FALSXTHeadLookAtState& NewHeadLookAtState)
{
	SetHeadLookAtState(NewHeadLookAtState);
}

void AALSXTCharacter::ServerProcessNewHeadLookAtState_Implementation(const FALSXTHeadLookAtState& NewHeadLookAtState)
{
	ProcessNewHeadLookAtState(NewHeadLookAtState);
}

void AALSXTCharacter::AddHeadLookAtEntry_Implementation(FALSXTHeadLookAtEntry HeadLookAtEntry)
{
	HeadLookAtEntries.Add(HeadLookAtEntry);
}

void AALSXTCharacter::RemoveHeadLookAtEntry_Implementation(FALSXTHeadLookAtEntry HeadLookAtEntry)
{
	HeadLookAtEntries.Remove(HeadLookAtEntry);
}

void AALSXTCharacter::BeginHeadLookAt_Implementation(FALSXTHeadLookAtEntry HeadLookAtEntry)
{
	IALSXTHeadLookAtInterface::Execute_GetBestHeadLookAtEntry(this);
}

void AALSXTCharacter::EndHeadLookAt_Implementation()
{

}

void AALSXTCharacter::OnReplicate_HeadLookAtState(const FALSXTHeadLookAtState& PreviousHeadLookAtState)
{
	OnHeadLookAtStateChanged(PreviousHeadLookAtState);
}

void AALSXTCharacter::OnHeadLookAtStateChanged_Implementation(const FALSXTHeadLookAtState& PreviousHeadLookAtState) {}

//

void AALSXTCharacter::SetSlidingState(const FALSXTSlidingState& NewSlidingState)
{
	const auto PreviousSlidingState{ SlidingState };

	SlidingState = NewSlidingState;

	OnSlidingStateChanged(PreviousSlidingState);

	// if ((GetLocalRole() == ROLE_AutonomousProxy) && IsLocallyControlled())
	// {
	// 	ServerSetSlidingState(NewSlidingState);
	// }
}

void AALSXTCharacter::ServerSetSlidingState_Implementation(const FALSXTSlidingState& NewSlidingState)
{
	// SetSlidingState(NewSlidingState);
	MulticastSetSlidingState(NewSlidingState);
}


void AALSXTCharacter::ServerProcessNewSlidingState_Implementation(const FALSXTSlidingState& NewSlidingState)
{
	ProcessNewSlidingState(NewSlidingState);
}

void AALSXTCharacter::OnReplicate_SlidingState(const FALSXTSlidingState& PreviousSlidingState)
{
	OnSlidingStateChanged(PreviousSlidingState);
}

// Sliding State
void AALSXTCharacter::ClientSetSlidingState(const FALSXTSlidingState& NewSlidingState)
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

void AALSXTCharacter::MulticastSetSlidingState_Implementation(const FALSXTSlidingState& NewSlidingState)
{
	SetSlidingState(NewSlidingState);
}

void AALSXTCharacter::OnSlidingStateChanged_Implementation(const FALSXTSlidingState& PreviousSlidingState) {}

//

void AALSXTCharacter::SetDefensiveModeState(const FALSXTDefensiveModeState& NewDefensiveModeState)
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

void AALSXTCharacter::SetDefensiveModeAnimations(const FALSXTDefensiveModeAnimations& NewDefensiveModeAnimations)
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

void AALSXTCharacter::SetDefensiveModeStateImplementation(const FALSXTDefensiveModeState& NewDefensiveModeState)
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

void AALSXTCharacter::ResetDefensiveModeState()
{
	// Reset but keep Nontage
	FALSXTDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
	FALSXTDefensiveModeState PreviousDefensiveModeState = GetDefensiveModeState();
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

void AALSXTCharacter::ServerSetDefensiveModeState_Implementation(const FALSXTDefensiveModeState& NewDefensiveModeState)
{
	// MulticastSetDefensiveModeState(NewDefensiveModeState);
	SetDefensiveModeState(NewDefensiveModeState);
}


void AALSXTCharacter::ServerProcessNewDefensiveModeState_Implementation(const FALSXTDefensiveModeState& NewDefensiveModeState)
{
	ProcessNewDefensiveModeState(NewDefensiveModeState);
}

void AALSXTCharacter::OnReplicate_DefensiveModeState(const FALSXTDefensiveModeState& PreviousDefensiveModeState)
{
	OnDefensiveModeStateChanged(PreviousDefensiveModeState);
}

// Defensive ModeState
void AALSXTCharacter::ClientSetDefensiveModeState(const FALSXTDefensiveModeState& NewDefensiveModeState)
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

void AALSXTCharacter::MulticastSetDefensiveModeState_Implementation(const FALSXTDefensiveModeState& NewDefensiveModeState)
{
	SetDefensiveModeState(NewDefensiveModeState);
}

void AALSXTCharacter::OnDefensiveModeStateChanged_Implementation(const FALSXTDefensiveModeState& PreviousDefensiveModeState) {}

// Blocking

void AALSXTCharacter::SetDesiredStationaryMode(const FGameplayTag& NewStationaryModeTag)
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

void AALSXTCharacter::ServerSetDesiredStationaryMode_Implementation(const FGameplayTag& NewStationaryModeTag)
{
	SetDesiredStationaryMode(NewStationaryModeTag);
}

void AALSXTCharacter::SetStationaryMode(const FGameplayTag& NewStationaryModeTag)
{

	if (StationaryMode != NewStationaryModeTag)
	{
		const auto PreviousStationaryMode{ StationaryMode };

		StationaryMode = NewStationaryModeTag;

		OnStationaryModeChanged(PreviousStationaryMode);
	}
}

void AALSXTCharacter::OnStationaryModeChanged_Implementation(const FGameplayTag& PreviousStationaryModeTag) {}

// Status

void AALSXTCharacter::SetDesiredStatus(const FGameplayTag& NewStatusTag)
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

void AALSXTCharacter::ServerSetDesiredStatus_Implementation(const FGameplayTag& NewStatusTag)
{
	// SetDesiredStatus(NewStatusTag);
	MulticastSetDesiredStatus(NewStatusTag);
}

void AALSXTCharacter::MulticastSetDesiredStatus_Implementation(const FGameplayTag& NewStatusTag)
{
	SetStatus(NewStatusTag);
}

void AALSXTCharacter::SetStatus(const FGameplayTag& NewStatusTag)
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

void AALSXTCharacter::OnStatusChanged_Implementation(const FGameplayTag& PreviousStatusTag) {}

// Focus

void AALSXTCharacter::SetDesiredFocus(const FGameplayTag& NewFocusTag)
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

void AALSXTCharacter::ServerSetDesiredFocus_Implementation(const FGameplayTag& NewFocusTag)
{
	SetDesiredFocus(NewFocusTag);
}

void AALSXTCharacter::SetFocus(const FGameplayTag& NewFocusTag)
{

	if (Focus != NewFocusTag)
	{
		const auto PreviousFocus{ Focus };

		Focus = NewFocusTag;

		OnFocusChanged(PreviousFocus);
	}
}

void AALSXTCharacter::OnFocusChanged_Implementation(const FGameplayTag& PreviousFocusTag) {}

void AALSXTCharacter::AddCollisionImpulse_Implementation(FVector NewImpulse)
{
	LaunchCharacter(NewImpulse, false, false);
}

// Attack Hit

void AALSXTCharacter::OnStaticMeshAttackCollision_Implementation(FAttackDoubleHitResult Hit)
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

void AALSXTCharacter::OnActorAttackCollision_Implementation(FAttackDoubleHitResult Hit)
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
	CharacterSound->PlayDamageSound(true, true, true, GetDesiredSex(), CharacterCustomization->VoiceParameters.Variant, GetOverlayMode(), ALSXTAttackMethodTags::Regular, Hit.Strength, Hit.DoubleHitResult.ImpactForm, Hit.BaseDamage);
}

FALSXTPhysicalAnimationState AALSXTCharacter::GetCharacterPhysicalAnimationState_Implementation() const
{
	return GetPhysicalAnimationState();
}

FALSXTJigglePhysicsSettings AALSXTCharacter::GetCharacterJigglePhysicsSettings_Implementation() const
{
	FALSXTJigglePhysicsSettings DefualtJigglePhysicsSettings;
	DefualtJigglePhysicsSettings.EnableBellyJigglePhysics = false;
	DefualtJigglePhysicsSettings.EnableBreastsJigglePhysics = true;
	DefualtJigglePhysicsSettings.EnableButtJigglePhysics = true;	
	return DefualtJigglePhysicsSettings;
}

void AALSXTCharacter::SetCharacterPhysicalAnimationState_Implementation(FALSXTPhysicalAnimationState NewPhysicalAnimationState)
{
	SetPhysicalAnimationState(NewPhysicalAnimationState);
}

FGameplayTag AALSXTCharacter::GetCharacterPhysicalAnimationMode_Implementation() const
{
	return GetPhysicalAnimationMode();
}

void AALSXTCharacter::SetCharacterPhysicalAnimationMode_Implementation(const FGameplayTag& NewPhysicalAnimationMode, const TArray<FName>& BelowBoneNames)
{
		SetDesiredPhysicalAnimationMode(NewPhysicalAnimationMode, BelowBoneNames);
}

void AALSXTCharacter::ResetCharacterPhysicalAnimationMode_Implementation()
{
	ResetPhysicalAnimationMode();
}

void AALSXTCharacter::BeginAnticipationTimer()
{
	GetWorld()->GetTimerManager().SetTimer(AnticipationTimerHandle, AnticipationTimerDelegate, 0.1f, true);
}

void AALSXTCharacter::AnticipationTimer()
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

void AALSXTCharacter::EndAnticipationTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(AnticipationTimerHandle);
}

void AALSXTCharacter::BeginFreelookTimer()
{
	if (ALSXTSettings->Freelook.bEnableFreelook)
	{
		GetWorld()->GetTimerManager().SetTimer(FreelookTimerHandle, FreelookTimerDelegate, 0.1f, true);
	}
}

void AALSXTCharacter::FreelookTimer()
{
	FGameplayTag CurrentViewMode = GetViewMode();
	FALSXTFreelookState NewState;

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

void AALSXTCharacter::EndFreelookTimer()
{
	// Clear Freelook Trace Timer
	FALSXTFreelookState EmptyState;
	SetFreelookState(EmptyState);
	GetWorld()->GetTimerManager().ClearTimer(FreelookTimerHandle);
}

// PhysicalAnimationMode

void AALSXTCharacter::SetDesiredPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const TArray<FName>& BoneNames)
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

void AALSXTCharacter::ServerSetDesiredPhysicalAnimationMode_Implementation(const FGameplayTag& NewPhysicalAnimationModeTag, const TArray<FName>& BoneNames)
{
	SetDesiredPhysicalAnimationMode(NewPhysicalAnimationModeTag, BoneNames);
}

void AALSXTCharacter::SetPhysicalAnimationMode(const FGameplayTag& NewPhysicalAnimationModeTag, const TArray<FName>& BoneNames)
{
	if (GetPhysicalAnimationState().Mode != NewPhysicalAnimationModeTag && GetPhysicalAnimationState().AffectedBonesBelow != BoneNames)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("SetPhysicalAnimationMode"));
		const auto PreviousPhysicalAnimationMode{ PhysicalAnimationMode };

		if (NewPhysicalAnimationModeTag == ALSXTPhysicalAnimationModeTags::None || NewPhysicalAnimationModeTag == FGameplayTag::EmptyTag)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ALSXTPhysicalAnimationModeTags::None"));
			PhysicalAnimationMode = ALSXTPhysicalAnimationModeTags::None;
			GetMesh()->SetCollisionProfileName("CharacterMesh");
			PhysicalAnimation->ApplyPhysicalAnimationProfileBelow("pelvis", "Default", true, false);
			GetCapsuleComponent()->SetCapsuleRadius(25);		
			FALSXTPhysicalAnimationState NewPhysicalAnimationState;
			NewPhysicalAnimationState.Mode = FGameplayTag::EmptyTag;
			NewPhysicalAnimationState.ProfileName = "CharacterMesh";
			SetPhysicalAnimationState(NewPhysicalAnimationState);
			FALSXTDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
			NewDefensiveModeState.Mode = ALSXTPhysicalAnimationModeTags::None;
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
		if (NewPhysicalAnimationModeTag == ALSXTPhysicalAnimationModeTags::Navigation)
		{
			GetMesh()->SetCollisionProfileName("PhysicalAnimation");		
			GetCapsuleComponent()->SetCapsuleRadius(14);

			// GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true, false);
			// PhysicalAnimation->ApplyPhysicalAnimationProfileBelow("pelvis", "Hit", false, false);
			FALSXTPhysicalAnimationState NewPhysicalAnimationState;
			NewPhysicalAnimationState.AffectedBonesBelow = BoneNames;
			NewPhysicalAnimationState.Alpha = 0.5;
			NewPhysicalAnimationState.Mode = ALSXTPhysicalAnimationModeTags::Navigation;
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
		if (NewPhysicalAnimationModeTag == ALSXTPhysicalAnimationModeTags::Bump)
		{
			GetMesh()->SetCollisionProfileName("PhysicalAnimation");
			
			// GetMesh()->SetAllBodiesBelowSimulatePhysics(BoneName, true, false);
			GetCapsuleComponent()->SetCapsuleRadius(14);
			FALSXTPhysicalAnimationState NewPhysicalAnimationState;
			NewPhysicalAnimationState.AffectedBonesBelow = BoneNames;
			NewPhysicalAnimationState.Alpha = 0.5;
			NewPhysicalAnimationState.Mode = ALSXTPhysicalAnimationModeTags::Bump;
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
		if (NewPhysicalAnimationModeTag == ALSXTPhysicalAnimationModeTags::Hit)
		{
			GetMesh()->SetCollisionProfileName("PhysicalAnimation");
			GetCapsuleComponent()->SetCapsuleRadius(8);
			
			GetMesh()->SetAllBodiesBelowSimulatePhysics("pelvis", true, false);
			PhysicalAnimation->ApplyPhysicalAnimationProfileBelow("pelvis", "Hit", false, false);
			FALSXTPhysicalAnimationState NewPhysicalAnimationState;
			NewPhysicalAnimationState.AffectedBonesBelow = BoneNames;
			NewPhysicalAnimationState.Alpha = 1.0f;
			NewPhysicalAnimationState.Mode = ALSXTPhysicalAnimationModeTags::Hit;
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

void AALSXTCharacter::ResetPhysicalAnimationMode()
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
	FALSXTPhysicalAnimationState NewPhysicalAnimationState;
	NewPhysicalAnimationState.Mode = ALSXTPhysicalAnimationModeTags::None;
	NewPhysicalAnimationState.ProfileName = "CharacterMesh";
	NewPhysicalAnimationState.AffectedBonesBelow = AffectedBonesBelowNames;
	// NewPhysicalAnimationState.Alpha = 0.0f;
	StartBlendOutPhysicalAnimation();
	SetPhysicalAnimationState(NewPhysicalAnimationState);
	OnPhysicalAnimationModeChanged(PreviousPhysicalAnimationMode);
	FALSXTDefensiveModeState NewDefensiveModeState = GetDefensiveModeState();
	
	NewDefensiveModeState.ObstacleMode = FGameplayTag::EmptyTag;
	NewDefensiveModeState.ObstacleSide = FGameplayTag::EmptyTag;
	NewDefensiveModeState.ObstacleHeight = FGameplayTag::EmptyTag;	
	NewDefensiveModeState.ObstacleTransform = {{0, 0, 0}, {0, 0, 0}, { 0, 0, 0 }};

	if (GetDefensiveModeState().AnticipationMode != ALSXTDefensiveModeTags::Anticipation)
	{
		NewDefensiveModeState.Mode = ALSXTPhysicalAnimationModeTags::None;
		NewDefensiveModeState.AnticipationMode = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationSide = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationHeight = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationForm = FGameplayTag::EmptyTag;
		NewDefensiveModeState.AnticipationTransform = {{0, 0, 0}, {0, 0, 0}, { 0, 0, 0 }};
	}
	
	SetDefensiveModeState(NewDefensiveModeState);
	PhysicalAnimationMode = ALSXTPhysicalAnimationModeTags::None;
}

void AALSXTCharacter::PhysicalAnimationBlendOut() 
{
	FALSXTPhysicalAnimationState NewPhysicalAnimationState = GetPhysicalAnimationState();
	NewPhysicalAnimationState.Alpha = GetPhysicalAnimationState().Alpha - 0.1;
	SetPhysicalAnimationState(NewPhysicalAnimationState);
	if (GetPhysicalAnimationState().Alpha <= 0.0)
	{
		ResetPhysicalAnimationMode();
	}
}

void AALSXTCharacter::OnPhysicalAnimationModeChanged_Implementation(const FGameplayTag& PreviousPhysicalAnimationModeTag) {}

// GripPosition

void AALSXTCharacter::SetDesiredGripPosition(const FGameplayTag& NewGripPositionTag)
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

void AALSXTCharacter::ServerSetDesiredGripPosition_Implementation(const FGameplayTag& NewGripPositionTag)
{
	SetDesiredGripPosition(NewGripPositionTag);
}

void AALSXTCharacter::SetGripPosition(const FGameplayTag& NewGripPositionTag)
{

	if (GripPosition != NewGripPositionTag)
	{
		const auto PreviousGripPosition{ GripPosition };

		GripPosition = NewGripPositionTag;

		OnGripPositionChanged(PreviousGripPosition);
	}
}

void AALSXTCharacter::OnGripPositionChanged_Implementation(const FGameplayTag& PreviousGripPositionTag) {}

// ForegripPosition

const FName AALSXTCharacter::GetSocketNameForForegripPosition(const FGameplayTag& ForegripPositionTag) const
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

const FName AALSXTCharacter::GetSocketNameForGripPosition(const FGameplayTag& ForegripPositionTag) const
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

void AALSXTCharacter::SetDesiredForegripPosition(const FGameplayTag& NewForegripPositionTag)
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

void AALSXTCharacter::ServerSetDesiredForegripPosition_Implementation(const FGameplayTag& NewForegripPositionTag)
{
	SetDesiredForegripPosition(NewForegripPositionTag);
}

void AALSXTCharacter::SetForegripPosition(const FGameplayTag& NewForegripPositionTag)
{

	if (ForegripPosition != NewForegripPositionTag)
	{
		const auto PreviousForegripPosition{ ForegripPosition };

		ForegripPosition = NewForegripPositionTag;

		OnForegripPositionChanged(PreviousForegripPosition);
	}
}

void AALSXTCharacter::OnForegripPositionChanged_Implementation(const FGameplayTag& PreviousForegripPositionTag) {}


// WeaponCarryPosition

void AALSXTCharacter::SetDesiredWeaponCarryPosition(const FGameplayTag& NewWeaponCarryPositionTag)
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

void AALSXTCharacter::ServerSetDesiredWeaponCarryPosition_Implementation(const FGameplayTag& NewWeaponCarryPositionTag)
{
	SetDesiredWeaponCarryPosition(NewWeaponCarryPositionTag);
}

void AALSXTCharacter::SetWeaponCarryPosition(const FGameplayTag& NewWeaponCarryPositionTag)
{

	if (WeaponCarryPosition != NewWeaponCarryPositionTag)
	{
		const auto PreviousWeaponCarryPosition{ WeaponCarryPosition };

		WeaponCarryPosition = NewWeaponCarryPositionTag;

		OnWeaponCarryPositionChanged(PreviousWeaponCarryPosition);
	}
}

void AALSXTCharacter::OnWeaponCarryPositionChanged_Implementation(const FGameplayTag& PreviousWeaponCarryPositionTag) {}

// VaultType

void AALSXTCharacter::SetDesiredVaultType(const FGameplayTag& NewVaultTypeTag)
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

void AALSXTCharacter::ServerSetDesiredVaultType_Implementation(const FGameplayTag& NewVaultTypeTag)
{
	SetDesiredVaultType(NewVaultTypeTag);
}

void AALSXTCharacter::SetVaultType(const FGameplayTag& NewVaultTypeTag)
{

	if (VaultType != NewVaultTypeTag)
	{
		const auto PreviousVaultType{ VaultType };

		VaultType = NewVaultTypeTag;

		OnVaultTypeChanged(PreviousVaultType);
	}
}

void AALSXTCharacter::OnVaultTypeChanged_Implementation(const FGameplayTag& PreviousVaultTypeTag) {}

// WeaponObstruction

void AALSXTCharacter::SetDesiredWeaponObstruction(const FGameplayTag& NewWeaponObstructionTag)
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

void AALSXTCharacter::ServerSetDesiredWeaponObstruction_Implementation(const FGameplayTag& NewWeaponObstructionTag)
{
	SetDesiredWeaponObstruction(NewWeaponObstructionTag);
}

void AALSXTCharacter::SetWeaponObstruction(const FGameplayTag& NewWeaponObstructionTag)
{

	if (WeaponObstruction != NewWeaponObstructionTag)
	{
		const auto PreviousWeaponObstruction{ WeaponObstruction };

		WeaponObstruction = NewWeaponObstructionTag;

		OnWeaponObstructionChanged(PreviousWeaponObstruction);
	}
}

void AALSXTCharacter::OnWeaponObstructionChanged_Implementation(const FGameplayTag& PreviousWeaponObstructionTag) {}

void AALSXTCharacter::UpdateBreathState()
{
	FALSXTBreathState NewBreathState;
	const float Stamina = GetStatusState().CurrentStamina;
	if (this->Implements<UALSXTCharacterInterface>())
	{
		FGameplayTag BreathType = IALSXTCharacterInterface::Execute_GetBreathType(this);
		if (ShouldTransitionBreathState() && BreathState.HoldingBreath != ALSXTHoldingBreathTags::True)
		{
			FALSXTTargetBreathState NewTargetState = CalculateTargetBreathState();
			BreathState.TargetState = NewTargetState;
			NewBreathState = GetBreathState();
			NewBreathState.TargetState = NewTargetState;
			SetBreathState(NewBreathState);
		}
	}
}

bool AALSXTCharacter::ShouldUpdateBreathState() const
{
	FALSXTStatusState StatusState = IALSXTCharacterInterface::Execute_GetStatusState(this);
	float CurrentStamina = IALSXTCharacterInterface::Execute_GetStamina(this);
	return StatusState.CurrentStamina != CurrentStamina;
}

bool AALSXTCharacter::ShouldTransitionBreathState()
{
	return (GetBreathState().CurrentBreathAlpha != GetBreathState().TargetState.Alpha || GetBreathState().CurrentBreathRate != GetBreathState().TargetState.Rate);
}


FALSXTTargetBreathState AALSXTCharacter::CalculateTargetBreathState()
{
	FALSXTTargetBreathState NewTargetBreathState;

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
		float CurrentStaminaConverted = FMath::GetMappedRangeValueClamped(UtilizedStaminaRange, ConversionRange, IALSXTCharacterInterface::Execute_GetStatusState(this).CurrentStamina);
		float PlayRateConverted = FMath::GetMappedRangeValueClamped(ConversionRange, IALSXTCharacterSoundComponentInterface::Execute_GetBreathEffectsSettings(this).BreathAnimationPlayRateRange, CurrentStaminaConverted);
		float BlendConverted = FMath::GetMappedRangeValueClamped(ConversionRange, IALSXTCharacterSoundComponentInterface::Execute_GetBreathEffectsSettings(this).BreathAnimationBlendRange, CurrentStaminaConverted);
		NewTargetBreathState.Alpha = BlendConverted;
		NewTargetBreathState.Rate = PlayRateConverted;
		NewTargetBreathState.TransitionRate = 1.0;
		return NewTargetBreathState;
	}
}

void AALSXTCharacter::TransitionBreathState()
{
	if (BreathState.HoldingBreath == ALSXTHoldingBreathTags::False)
	{
		BreathState.PreviousBreathAlpha = BreathState.CurrentBreathAlpha;
		BreathState.PreviousBreathRate = BreathState.CurrentBreathRate;
		BreathState.CurrentBreathAlpha = BreathState.TargetState.Alpha;
		BreathState.CurrentBreathRate = BreathState.TargetState.Rate;
	}
}

void AALSXTCharacter::OnAIJumpObstacle_Implementation()
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

bool AALSXTCharacter::CanSprint_Implementation() const { return true; }
bool AALSXTCharacter::CanFocus_Implementation() const { return true; }
bool AALSXTCharacter::CanToggleGait_Implementation() const { return true; }
bool AALSXTCharacter::CanLean_Implementation() const { return true; }
bool AALSXTCharacter::CanAimDownSights_Implementation() const { return true; }
bool AALSXTCharacter::CanAim_Implementation() const { return true; }
bool AALSXTCharacter::CanRoll_Implementation() const { return true; }
bool AALSXTCharacter::CanVault_Implementation() const { return true; }
bool AALSXTCharacter::CanSlide_Implementation() const { return true; }
bool AALSXTCharacter::CanWallRun_Implementation() const { return true; }
bool AALSXTCharacter::CanWallJump_Implementation() const { return true; }

void AALSXTCharacter::AIObstacleTrace_Implementation() {}
void AALSXTCharacter::StartVault_Implementation() {}
void AALSXTCharacter::StartWallrun_Implementation() {}
void AALSXTCharacter::OnWeaponReadyPositionChanged_Implementation(const FGameplayTag& PreviousWeaponReadyPositionTag) {}

AALSXTCharacter* AALSXTCharacter::GetCharacter_Implementation()
{
	return this;
}

UALSXTAnimationInstance* AALSXTCharacter::GetCharacterAnimInstance_Implementation() const
{
	return Cast<UALSXTAnimationInstance>(GetMesh()->GetAnimInstance());
}

UALSXTCharacterSettings* AALSXTCharacter::GetCharacterSettings_Implementation() const
{
	return ALSXTSettings;
}

UInputComponent* AALSXTCharacter::GetCharacterInputComponent_Implementation() const
{
	return InputComponent;
}

USkeletalMeshComponent* AALSXTCharacter::GetCharacterMesh_Implementation() const
{
	return GetMesh();
}

FGameplayTag AALSXTCharacter::GetCharacterSex_Implementation() const
{
	return GetDesiredSex();
}

FGameplayTag AALSXTCharacter::GetCharacterStance_Implementation() const
{
	return GetDesiredStance();
}

FGameplayTag AALSXTCharacter::GetCharacterOverlayMode_Implementation() const
{
	return GetOverlayMode();
}

FGameplayTag AALSXTCharacter::GetCharacterCombatStance_Implementation() const
{
	return GetDesiredCombatStance();
}

void AALSXTCharacter::SetCharacterCombatStance_Implementation(const FGameplayTag& NewCombatStance)
{
	return SetDesiredCombatStance(NewCombatStance);
}

FGameplayTag AALSXTCharacter::GetCharacterWeaponReadyPosition_Implementation() const
{
	return GetDesiredWeaponReadyPosition();
}

FGameplayTag AALSXTCharacter::GetCharacterWeaponFirearmStance_Implementation() const
{
	return GetDesiredWeaponFirearmStance();
}

FGameplayTag AALSXTCharacter::GetCharacterInjury_Implementation() const
{
	return GetDesiredInjury();
}

UAlsCameraComponent* AALSXTCharacter::GetCharacterCamera_Implementation() const
{
	return Camera;
}

UALSXTCameraAnimationInstance* AALSXTCharacter::GetCharacterCameraAnimationInstance_Implementation() const
{
	return Cast<UALSXTCameraAnimationInstance>(Camera->GetAnimInstance());
}

FRotator AALSXTCharacter::GetCharacterControlRotation_Implementation() const
{
	return GetControlRotation();
}

FVector AALSXTCharacter::GetCharacterFirstPersonCameraLocation_Implementation() const
{
	return Camera->GetFirstPersonCameraLocation();
}

FALSXTBreathState AALSXTCharacter::GetCharacterBreathState_Implementation() const
{
	return GetBreathState();
}

FALSXTPoseState AALSXTCharacter::GetCharacterPoseState_Implementation() const
{
	return GetALSXTPoseState();
}

FGameplayTag AALSXTCharacter::GetCharacterLocomotionMode_Implementation() const
{
	return GetLocomotionMode();
}

FGameplayTag AALSXTCharacter::GetCharacterGait_Implementation() const
{
	return GetDesiredGait();
}

FGameplayTag AALSXTCharacter::GetCharacterRotationMode_Implementation() const
{
	return GetDesiredRotationMode();
}

FGameplayTag AALSXTCharacter::GetCharacterViewMode_Implementation() const
{
	return GetViewMode();
}

FGameplayTag AALSXTCharacter::GetCharacterLean_Implementation() const
{
	return GetDesiredLean();
}

FALSXTStationaryModeState AALSXTCharacter::GetCharacterStationaryModeState_Implementation() const
{
	return GetStationaryModeState();
}

FALSXTSlidingState AALSXTCharacter::GetCharacterSlidingState_Implementation() const
{
	return GetSlidingState();
}

FALSXTDefensiveModeState AALSXTCharacter::GetCharacterDefensiveModeState_Implementation() const
{
	return GetDefensiveModeState();
}

FALSXTDefensiveModeAnimations AALSXTCharacter::GetCharacterDefensiveModeAnimations_Implementation() const
{
	return GetDefensiveModeAnimations();
}

FALSXTBumpPoseState AALSXTCharacter::GetCrowdNavigationPoseState_Implementation() const
{
	return ImpactReaction->GetCrowdNavigationPoseState();
}

UCapsuleComponent* AALSXTCharacter::GetCharacterCapsuleComponent_Implementation() const
{
	return GetCapsuleComponent();
}

UAlsCharacterMovementComponent* AALSXTCharacter::GetCharacterMovementComponent_Implementation() const
{
	return ALSXTCharacterMovement;
}

bool AALSXTCharacter::IsBlocking_Implementation() const
{
	return IsBlocking();
}

bool AALSXTCharacter::IsCharacterPlayerControlled_Implementation() const
{
	return IsPlayerControlled();
}

bool AALSXTCharacter::GetCharacterFirstPersonFocus_Implementation() const
{
	return IsFirstPersonEyeFocusActive();
}

bool AALSXTCharacter::GetCharacterAimingDownSights_Implementation() const
{
	return IsAimingDownSights();
}

bool AALSXTCharacter::CanEmote_Implementation() const
{
	return true;
}

bool AALSXTCharacter::CanGesture_Implementation() const
{
	return true;
}

FGameplayTag AALSXTCharacter::GetCharacterLocomotionAction_Implementation() const
{
	return GetLocomotionAction();
}

void AALSXTCharacter::SetCharacterLocomotionAction_Implementation(const FGameplayTag& NewLocomotionAction)
{
	SetLocomotionAction(NewLocomotionAction);
}

void AALSXTCharacter::ResetCharacterDefensiveModeState_Implementation()
{
	SetDefensiveMode(FGameplayTag::EmptyTag);
	FALSXTDefensiveModeState EmptyDefensiveModeState;
	SetDefensiveModeState(EmptyDefensiveModeState);
}

FGameplayTag AALSXTCharacter::GetCharacterDefensiveMode_Implementation() const
{
	return DesiredDefensiveMode;
}

void AALSXTCharacter::SetCharacterDefensiveMode_Implementation(const FGameplayTag& NewDefensiveMode)
{
	SetDefensiveMode(NewDefensiveMode);
}

void AALSXTCharacter::SetCharacterStatus_Implementation(const FGameplayTag& NewStatus)
{
	SetDesiredStatus(NewStatus);
}

void AALSXTCharacter::SetCharacterLocomotionVariant_Implementation(const FGameplayTag& NewLocomotionVariant)
{
	SetDesiredLocomotionVariant(NewLocomotionVariant);
}

void AALSXTCharacter::SetCharacterInjury_Implementation(const FGameplayTag& NewInjury)
{
	SetDesiredInjury(NewInjury);
}

void AALSXTCharacter::SetViewportMask_Implementation(const FGameplayTag& EquipmentSlot, const UMaterialInterface* NewViewportMask)
{
	// SetDesiredStatus(NewStatus);
}

void AALSXTCharacter::SetCharacterDefensiveModeState_Implementation(FALSXTDefensiveModeState NewDefensiveModeState)
{
	SetDefensiveModeState(NewDefensiveModeState);
}

void AALSXTCharacter::SetCharacterDefensiveModeAnimations_Implementation(FALSXTDefensiveModeAnimations NewDefensiveModeAnimations)
{
	SetDefensiveModeAnimations(NewDefensiveModeAnimations);
}

void AALSXTCharacter::SetCharacterMovementModeLocked_Implementation(bool NewLocked)
{
	SetMovementModeLocked(NewLocked);
	// NewLocked ? ALSXTCharacterMovement->SetMovementMode(EMovementMode::MOVE_None) : ALSXTCharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AALSXTCharacter::SetCharacterStance_Implementation(const FGameplayTag& NewStance)
{
	SetDesiredStance(NewStance);
}

FGameplayTag AALSXTCharacter::GetCharacterFreelooking_Implementation() const
{
	return GetDesiredFreelooking();
}

FGameplayTag AALSXTCharacter::GetCharacterEmote_Implementation() const
{
	return DesiredEmote;
}

void AALSXTCharacter::SetCharacterRagdoll_Implementation(const bool NewRagdoll)
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

void AALSXTCharacter::SetCharacterEmote_Implementation(const FGameplayTag& NewEmote)
{
	// Emotes->MulticastAddDesiredEmote(NewEmote);
	// SetDesiredEmote(NewEmote);
	Emotes->AddDesiredEmote(NewEmote);
}

void AALSXTCharacter::GetCharacterGesture_Implementation(FGameplayTag& NewGesture, FGameplayTag& NewGestureHand) const
{
	NewGesture = GetDesiredGesture();
	NewGestureHand = GetDesiredGestureHand();
}

void AALSXTCharacter::SetCharacterGesture_Implementation(const FGameplayTag& NewGesture, const FGameplayTag& NewGestureHand)
{
	SetDesiredGestureHand(NewGestureHand);
	SetDesiredGesture(NewGesture);
}


FGameplayTag AALSXTCharacter::GetCharacterForegripPosition_Implementation() const
{
	return GetDesiredForegripPosition();
}

FGameplayTag AALSXTCharacter::GetCharacterWeaponCarryPosition_Implementation() const
{
	return GetDesiredWeaponCarryPosition();
}

FTransform AALSXTCharacter::GetCharacterCurrentForegripTransform_Implementation() const
{
	return GetCurrentForegripTransform();
}

FGameplayTag AALSXTCharacter::GetCharacterVaultType_Implementation() const
{
	return GetDesiredVaultType();
}

FALSXTAimState AALSXTCharacter::GetCharacterAimState_Implementation() const
{
	return GetAimState();
}

FALSXTHeldItemState AALSXTCharacter::GetCharacterHeldItemState_Implementation() const
{
	return GetHeldItemState();
}

FALSXTFreelookState AALSXTCharacter::GetCharacterFreelookState_Implementation() const
{
	return GetFreelookState();
}

FALSXTHeadLookAtState AALSXTCharacter::GetCharacterHeadLookAtState_Implementation() const
{
	return GetHeadLookAtState();
}

FALSXTHeadLookAtEntry AALSXTCharacter::GetBestHeadLookAtEntry_Implementation() const
{
	TArray<FALSXTHeadLookAtEntry> BestHeadLookAtEntries;
	FALSXTHeadLookAtEntry BestHeadLookAtEntry;
	FALSXTHeadLookAtEntry CurrentBestHeadLookAtEntry;

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

FGameplayTag AALSXTCharacter::GetCharacterLocomotionVariant_Implementation() const
{
	return GetDesiredLocomotionVariant();
}

FALSXTFootprintsState AALSXTCharacter::GetCharacterFootprintsState_Implementation() const
{
	return GetFootprintsState();
}

FALSXTFootwearDetails AALSXTCharacter::GetCharacterFootwearDetails_Implementation() const
{
	return GetFootwearDetails();
}

// Character Sound Component Interface Functions
void AALSXTCharacter::PlayBreathEffects_Implementation(const FGameplayTag& StaminaOverride)
{
	CharacterSound->PlayCharacterBreathEffects(StaminaOverride);
	// CharacterSound->MulticastPlayCharacterBreathEffects(StaminaOverride);
}

void AALSXTCharacter::PlayCharacterMovementSound_Implementation(bool AccentSound, bool WeaponSound, UPARAM(meta = (Categories = "Als.Character Movement Sound"))const FGameplayTag& Type, UPARAM(meta = (Categories = "Als.Object Weight"))const FGameplayTag& Weight)
{
	CharacterSound->PlayCharacterMovementSound(AccentSound, WeaponSound, Type, Weight);
}

void AALSXTCharacter::PlayActionSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, const FGameplayTag& Type, const FGameplayTag& SoundSex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina)
{
	CharacterSound->PlayActionSound(MovementSound, AccentSound, WeaponSound, Type, SoundSex, Variant, Overlay, Strength, Stamina);
}

void AALSXTCharacter::PlayAttackSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, const FGameplayTag& SoundSex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina)
{
	CharacterSound->PlayAttackSound(MovementSound, AccentSound, WeaponSound, SoundSex, Variant, Overlay, Strength, AttackMode, Stamina);
}

void AALSXTCharacter::PlayDamageSound_Implementation(bool MovementSound, bool AccentSound, bool WeaponSound, const FGameplayTag& SoundSex, const FGameplayTag& Variant, const FGameplayTag& Overlay, const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage)
{
	CharacterSound->PlayDamageSound(MovementSound, AccentSound, WeaponSound, SoundSex, Variant, Overlay, AttackMethod, Strength, AttackForm, Damage);
}

bool AALSXTCharacter::CanPlayCharacterMovementSound_Implementation() const
{
	return true;
}

bool AALSXTCharacter::CanPlayWeaponMovementSound_Implementation() const
{
	return true;
}

bool AALSXTCharacter::ShouldPlayWeaponMovementSound_Implementation(const FGameplayTag& Type, const FGameplayTag& Strength) const
{
	return true;
}

bool AALSXTCharacter::ShouldPlayMovementAccentSound_Implementation(const FGameplayTag& Type, const FGameplayTag& Weight) const
{
	return true;
}

float AALSXTCharacter::GetCurrentStamina_Implementation() const
{
	return CharacterSound->CurrentStamina;
}

FGameplayTag AALSXTCharacter::GetCurrentStaminaTag_Implementation() const
{
	return CharacterSound->CurrentStaminaTag;
}

FGameplayTag AALSXTCharacter::GetCurrentBreathType_Implementation() const
{
	return CharacterSound->CurrentBreathType;
}


FString AALSXTCharacter::GetCurrentBreathSounds_Implementation() const
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

FString AALSXTCharacter::GetCurrentBreathSound_Implementation() const
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

FString AALSXTCharacter::GetCurrentVocalization_Implementation() const
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

bool AALSXTCharacter::ShouldIdle_Implementation() const
{
	return true;
}

FALSXTIdleState AALSXTCharacter::GetIdleState_Implementation() const
{
	return IdleAnimation->GetIdleState();
}

void AALSXTCharacter::OnEnterStationaryModeBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	FALSXTStationaryModeState NewStationaryModeState = GetStationaryModeState();
	NewStationaryModeState.Mode = NewStationaryModeState.TargetMode;
	NewStationaryModeState.TargetMode = FGameplayTag::EmptyTag;
	SetStationaryModeState(NewStationaryModeState);
}

void AALSXTCharacter::OnSlidingStartedBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->Montage_JumpToSection("Edit", Montage);
}
// void AALSXTCharacter::OnExitStationaryModeBlendOut(UAnimMontage* Montage, bool bInterrupted) {}