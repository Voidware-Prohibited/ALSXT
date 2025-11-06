// MIT


#include "AlsxtAdvancedAnimationInstance.h"
#include "ALS/Public/Utility/AlsMacros.h"
#include "AlsxtAdvancedAnimationInstanceProxy.h"
#include "Interfaces/AlsxtFirearmInterface.h"

UAlsxtAdvancedAnimationInstance::UAlsxtAdvancedAnimationInstance()
{
	RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
}

void UAlsxtAdvancedAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

// 	ALSXTCharacterAdvanced = Cast<AALSXTCharacterAdvanced>(GetOwningActor());
// 
#if WITH_EDITOR
	if (!GetWorld()->IsGameWorld() && !IsValid(ALSXTCharacterAdvanced))
	{
		// Use default objects for editor preview.
		ALSXTCharacterAdvanced = GetMutableDefault<AAlsxtCharacterAdvanced>();
	}
#endif
}

void UAlsxtAdvancedAnimationInstance::NativeBeginPlay()
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

void UAlsxtAdvancedAnimationInstance::NativeUpdateAnimation(const float DeltaTime)
{

	Super::NativeUpdateAnimation(DeltaTime);

	if (!GetOwningActor()->Implements<UAlsxtCharacterInterface>())
	{
		return;
	}
	
	RefreshAlsxtPose();


	if (GetOwningActor()->Implements<UAlsxtCharacterInterface>())
	{

	}

	if (GetOwningActor()->Implements<UAlsxtFirearmInterface>())
	{
		FirearmAimState = IAlsxtFirearmInterface::Execute_GetFirearmAimState(GetOwningActor());		
	}
}

void UAlsxtAdvancedAnimationInstance::NativeThreadSafeUpdateAnimation(const float DeltaTime)
{

	Super::NativeThreadSafeUpdateAnimation(DeltaTime);

}

void UAlsxtAdvancedAnimationInstance::NativePostEvaluateAnimation()
{

	Super::NativePostEvaluateAnimation();

	bPendingUpdate = false;
}

FAnimInstanceProxy* UAlsxtAdvancedAnimationInstance::CreateAnimInstanceProxy()
{
	return new FAlsxtAdvancedAnimationInstanceProxy{ this };
}

void UAlsxtAdvancedAnimationInstance::RefreshAlsxtPose()
{
	Super::RefreshAlsxtPose();
}