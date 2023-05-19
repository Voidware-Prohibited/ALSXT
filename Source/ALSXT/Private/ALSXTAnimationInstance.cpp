// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSXTAnimationInstance.h"
#include "ALSXTCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/ALSXTCharacterInterface.h"

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
	FirearmFingerAction = ALSXTCharacter->GetFirearmFingerAction();
	FirearmFingerActionHand = ALSXTCharacter->GetFirearmFingerActionHand();
	WeaponCarryPosition = ALSXTCharacter->GetWeaponCarryPosition();
	FirearmSightLocation = ALSXTCharacter->GetFirearmSightLocation();
	VaultType = ALSXTCharacter->GetVaultType();
	FreelookState = ALSXTCharacter->GetFreelookState();

	if (UKismetSystemLibrary::DoesImplementInterface(ALSXTCharacter, UALSXTCharacterInterface::StaticClass()))
	{
		CrowdNavigationPoseState = IALSXTCharacterInterface::Execute_GetCrowdNavigationPoseState(ALSXTCharacter);
		BumpPoseState = IALSXTCharacterInterface::Execute_GetBumpPoseState(ALSXTCharacter);
	}

	DefensiveModeState = ALSXTCharacter->GetDefensiveModeState();
	WeaponObstruction = ALSXTCharacter->GetWeaponObstruction();

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