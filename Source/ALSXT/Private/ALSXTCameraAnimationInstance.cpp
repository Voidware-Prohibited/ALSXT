// MIT


#include "ALSXTCameraAnimationInstance.h"
#include "AlsCameraComponent.h"
#include "AlsCharacter.h"
#include "ALSXTCharacter.h"

void UALSXTCameraAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ALSXTCharacter = Cast<AALSXTCharacter>(GetOwningActor());
	ALSXTCamera = Cast<UAlsCameraComponent>(GetSkelMeshComponent());

#if WITH_EDITOR
	if (!GetWorld()->IsGameWorld())
	{
		// Use default objects for editor preview.

		if (!IsValid(ALSXTCharacter))
		{
			ALSXTCharacter = GetMutableDefault<AALSXTCharacter>();
		}

		if (!IsValid(ALSXTCamera))
		{
			ALSXTCamera = GetMutableDefault<UAlsCameraComponent>();
		}
	}
#endif
}

void UALSXTCameraAnimationInstance::NativeUpdateAnimation(const float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!IsValid(ALSXTCharacter) || !IsValid(ALSXTCamera))
	{
		return;
	}

	Overlay = ALSXTCharacter->GetOverlayMode();
	Freelooking = ALSXTCharacter->GetDesiredFreelooking();
	Sex = ALSXTCharacter->GetDesiredSex();
	LocomotionVariant = ALSXTCharacter->GetDesiredLocomotionVariant();
	Injury = ALSXTCharacter->GetDesiredInjury();
	CombatStance = ALSXTCharacter->GetDesiredCombatStance();
	WeaponFirearmStance = ALSXTCharacter->GetDesiredWeaponFirearmStance();
	WeaponReadyPosition = ALSXTCharacter->GetDesiredWeaponReadyPosition();
}
