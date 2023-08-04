// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSXTAnimationInstance.h"
#include "ALSXTCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "ALS/Public/Utility/AlsMacros.h"
#include "Stats/Stats.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSXTAnimationInstance)

UALSXTAnimationInstance::UALSXTAnimationInstance()
{
	RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
}

void UALSXTAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ALSXTCharacter = Cast<AALSXTCharacter>(GetOwningActor());

#if WITH_EDITOR
	if (!GetWorld()->IsGameWorld() && !IsValid(ALSXTCharacter))
	{
		// Use default objects for editor preview.

		ALSXTCharacter = GetMutableDefault<AALSXTCharacter>();
	}
#endif
}

void UALSXTAnimationInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ALS_ENSURE(IsValid(ALSXTSettings));
	ALS_ENSURE(IsValid(ALSXTCharacter));
}

void UALSXTAnimationInstance::NativeUpdateAnimation(const float DeltaTime)
{

	Super::NativeUpdateAnimation(DeltaTime);

	if (!IsValid(ALSXTCharacter))
	{
		return;
	}

	Freelooking = ALSXTCharacter->GetDesiredFreelooking();
	Sex = ALSXTCharacter->GetDesiredSex();
	DefensiveMode = ALSXTCharacter->GetDesiredDefensiveMode();
	LocomotionVariant = ALSXTCharacter->GetDesiredLocomotionVariant();
	Injury = ALSXTCharacter->GetDesiredInjury();
	CombatStance = ALSXTCharacter->GetDesiredCombatStance();
	WeaponFirearmStance = ALSXTCharacter->GetDesiredWeaponFirearmStance();
	WeaponReadyPosition = ALSXTCharacter->GetDesiredWeaponReadyPosition();
	StationaryMode = ALSXTCharacter->GetStationaryMode();
	HoldingBreath = ALSXTCharacter->GetHoldingBreath();
	PhysicalAnimationMode = ALSXTCharacter->GetPhysicalAnimationMode();
	Gesture = ALSXTCharacter->GetGesture();
	GestureHand = ALSXTCharacter->GetGestureHand();
	ReloadingType = ALSXTCharacter->GetReloadingType();
	ForegripPosition = ALSXTCharacter->GetForegripPosition();
	FirearmFingerAction = ALSXTCharacter->GetFirearmFingerAction();
	FirearmFingerActionHand = ALSXTCharacter->GetFirearmFingerActionHand();
	WeaponCarryPosition = ALSXTCharacter->GetWeaponCarryPosition();
	FirearmSightLocation = ALSXTCharacter->GetFirearmSightLocation();
	ForegripTransform = ALSXTCharacter->GetCurrentForegripTransform();
	VaultType = ALSXTCharacter->GetVaultType();
	AimState = ALSXTCharacter->GetAimState();
	FreelookState = ALSXTCharacter->GetFreelookState();
	DoesOverlayObjectUseLeftHandIK = ALSXTCharacter->DoesOverlayObjectUseLeftHandIK();

	if (UKismetSystemLibrary::DoesImplementInterface(ALSXTCharacter, UALSXTCharacterInterface::StaticClass()))
	{
		CrowdNavigationPoseState = IALSXTCharacterInterface::Execute_GetCrowdNavigationPoseState(ALSXTCharacter);
		BumpPoseState = IALSXTCharacterInterface::Execute_GetBumpPoseState(ALSXTCharacter);
	}

	DefensiveModeState = ALSXTCharacter->GetDefensiveModeState();
	WeaponObstruction = ALSXTCharacter->GetWeaponObstruction();

}

void UALSXTAnimationInstance::NativeThreadSafeUpdateAnimation(const float DeltaTime)
{

	Super::NativeThreadSafeUpdateAnimation(DeltaTime);

	if (!IsValid(ALSXTSettings) || !IsValid(ALSXTCharacter))
	{
		return;
	}
}

void UALSXTAnimationInstance::NativePostEvaluateAnimation()
{

	Super::NativePostEvaluateAnimation();

	if (!IsValid(ALSXTSettings) || !IsValid(ALSXTCharacter))
	{
		return;
	}

	bPendingUpdate = false;
}



bool UALSXTAnimationInstance::IsSpineRotationAllowed()
{
	return ALSXTCharacter->GetRotationMode() == AlsRotationModeTags::Aiming;
	//return ALSXTCharacter->GetRotationMode() == AlsRotationModeTags::Aiming && ALSXTCharacter->GetLocomotionState().bRotationLocked == false;
}

bool UALSXTAnimationInstance::IsRotateInPlaceAllowed()
{
	return (ALSXTCharacter->GetRotationMode() == AlsRotationModeTags::Aiming || ALSXTCharacter->GetViewMode() == AlsViewModeTags::FirstPerson);
}

bool UALSXTAnimationInstance::IsTurnInPlaceAllowed()
{
	return (ALSXTCharacter->GetRotationMode() == AlsRotationModeTags::ViewDirection && ALSXTCharacter->GetViewMode() != AlsViewModeTags::FirstPerson);
}