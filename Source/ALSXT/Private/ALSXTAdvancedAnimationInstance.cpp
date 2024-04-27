// MIT


#include "ALSXTAdvancedAnimationInstance.h"
#include "ALSXTAdvancedAnimationInstanceProxy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSXTAdvancedAnimationInstance)

UALSXTAdvancedAnimationInstance::UALSXTAdvancedAnimationInstance()
{
	// RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
}

void UALSXTAdvancedAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

// 	ALSXTCharacter = Cast<AALSXTCharacter>(GetOwningActor());
// 
// #if WITH_EDITOR
// 	if (!GetWorld()->IsGameWorld() && !IsValid(ALSXTCharacter))
// 	{
// 		// Use default objects for editor preview.
// 
// 		ALSXTCharacter = GetMutableDefault<AALSXTCharacter>();
// 	}
// #endif
}

void UALSXTAdvancedAnimationInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// ALS_ENSURE(IsValid(ALSXTSettings));
	// ALS_ENSURE(IsValid(ALSXTCharacter));
	// if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	// {
	// 	StaminaThresholdSettings = IALSXTCharacterInterface::Execute_GetCharacterSettings(GetOwningActor())->StatusSettings.StaminaThresholdSettings;
	// 	CharacterBreathEffectsSettings = IALSXTCharacterInterface::Execute_GetCharacterSettings(GetOwningActor())->BreathEffects;
	// }
}

void UALSXTAdvancedAnimationInstance::NativeUpdateAnimation(const float DeltaTime)
{

	Super::NativeUpdateAnimation(DeltaTime);

	// if (!IsValid(ALSXTCharacter))
	// {
	// 	return;
	// }
	// 
	// if (!GetOwningActor()->Implements<UALSXTCharacterInterface>())
	// {
	// 	return;
	// }
	// 
	// RefreshALSXTPose();
	// 
	// if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	// {
	// 	// Freelooking = IALSXTCharacterInterface::Execute_GetCharacterFreelooking(GetOwningActor());
	// }
}

void UALSXTAdvancedAnimationInstance::NativeThreadSafeUpdateAnimation(const float DeltaTime)
{

	Super::NativeThreadSafeUpdateAnimation(DeltaTime);

}

void UALSXTAdvancedAnimationInstance::NativePostEvaluateAnimation()
{

	Super::NativePostEvaluateAnimation();

	bPendingUpdate = false;
}

FAnimInstanceProxy* UALSXTAdvancedAnimationInstance::CreateAnimInstanceProxy()
{
	return new FALSXTAdvancedAnimationInstanceProxy{ this };
}

void UALSXTAdvancedAnimationInstance::RefreshALSXTPose()
{
	Super::RefreshALSXTPose();
}