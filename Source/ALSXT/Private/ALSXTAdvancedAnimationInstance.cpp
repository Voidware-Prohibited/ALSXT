// MIT


#include "ALSXTAdvancedAnimationInstance.h"
#include "ALS/Public/Utility/AlsMacros.h"
#include "ALSXTAdvancedAnimationInstanceProxy.h"
#include "Interfaces/ALSXTFirearmInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSXTAdvancedAnimationInstance)

UALSXTAdvancedAnimationInstance::UALSXTAdvancedAnimationInstance()
{
	RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
}

void UALSXTAdvancedAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

// 	ALSXTCharacterAdvanced = Cast<AALSXTCharacterAdvanced>(GetOwningActor());
// 
#if WITH_EDITOR
	if (!GetWorld()->IsGameWorld() && !IsValid(ALSXTCharacterAdvanced))
	{
		// Use default objects for editor preview.
		ALSXTCharacterAdvanced = GetMutableDefault<AALSXTCharacterAdvanced>();
	}
#endif
}

void UALSXTAdvancedAnimationInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// ALS_ENSURE(IsValid(ALSXTAdvancedSettings));
	ALS_ENSURE(IsValid(ALSXTCharacterAdvanced));
	// if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	// {
	// 	StaminaThresholdSettings = IALSXTCharacterInterface::Execute_GetCharacterSettings(GetOwningActor())->StatusSettings.StaminaThresholdSettings;
	// 	CharacterBreathEffectsSettings = IALSXTCharacterInterface::Execute_GetCharacterSettings(GetOwningActor())->BreathEffects;
	// }
}

void UALSXTAdvancedAnimationInstance::NativeUpdateAnimation(const float DeltaTime)
{

	Super::NativeUpdateAnimation(DeltaTime);

	if (!GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{
		return;
	}
	
	RefreshALSXTPose();


	if (GetOwningActor()->Implements<UALSXTCharacterInterface>())
	{

	}

	if (GetOwningActor()->Implements<UALSXTFirearmInterface>())
	{
		FirearmAimState = IALSXTFirearmInterface::Execute_GetFirearmAimState(GetOwningActor());		
	}
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