// MIT


#include "ALSXTCharacterAdvanced.h" 	
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AlsCameraComponent.h"
#include "Components/Character/ALSXTCharacterCameraEffectsComponent.h"
#include "Components/Character/ALSXTEmoteComponent.h"
#include "Components/Character/ALSXTGestureComponent.h"
#include "Math/Vector.h"

AALSXTCharacterAdvanced::AALSXTCharacterAdvanced(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	Combat = CreateDefaultSubobject<UALSXTCombatComponent>(TEXT("Combat"));
	AddOwnedComponent(Combat);

	AcrobaticActions = CreateDefaultSubobject<UALSXTAcrobaticActionComponent>(TEXT("Acrobatic Actions"));
	AddOwnedComponent(AcrobaticActions);

	Emotes = CreateDefaultSubobject<UALSXTEmoteComponent>(TEXT("Emotes"));
	AddOwnedComponent(Emotes);

	Gestures = CreateDefaultSubobject<UALSXTGestureComponent>(TEXT("Gestures"));
	AddOwnedComponent(Gestures);
	
	CameraEffects = CreateDefaultSubobject<UALSXTCharacterCameraEffectsComponent>(TEXT("Camera Effects"));
	AddOwnedComponent(CameraEffects);
}

void AALSXTCharacterAdvanced::SetupPlayerInputComponent(UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);

	auto* EnhancedInput{ Cast<UEnhancedInputComponent>(Input) };
	if (IsValid(EnhancedInput))
	{
		EnhancedInput->BindAction(AcrobaticAction, ETriggerEvent::Triggered, this, &ThisClass::InputAcrobaticAction);
		EnhancedInput->BindAction(PrimaryActionAction, ETriggerEvent::Triggered, this, &ThisClass::InputPrimaryAction);
		EnhancedInput->BindAction(SecondaryActionAction, ETriggerEvent::Triggered, this, &ThisClass::InputSecondaryAction);
		EnhancedInput->BindAction(SwitchTargetLeftAction, ETriggerEvent::Triggered, this, &ThisClass::InputSwitchTargetLeft);
		EnhancedInput->BindAction(SwitchTargetRightAction, ETriggerEvent::Triggered, this, &ThisClass::InputSwitchTargetRight);
		// EnhancedInput->BindAction(PrimaryInteractionAction, ETriggerEvent::Triggered, this, &ThisClass::InputPrimaryInteraction);
		// EnhancedInput->BindAction(SecondaryInteractionAction, ETriggerEvent::Triggered, this, &ThisClass::InputSecondaryInteraction);
		// EnhancedInput->BindAction(SelectEmoteAction, ETriggerEvent::Triggered, this, &ThisClass::InputSelectEmote);
		// EnhancedInput->BindAction(SelectGestureAction, ETriggerEvent::Triggered, this, &ThisClass::InputSelectGesture);

		OnSetupPlayerInputComponentUpdated.Broadcast();
	}
}

void AALSXTCharacterAdvanced::OnDesiredAimingChanged_Implementation(bool bPreviousDesiredAiming)
{
	if (bDesiredAiming)
	{
		if (Execute_GetTargetableOverlayModes(this).HasTag(OverlayMode) && (DesiredCombatStance.MatchesTag(ALSXTCombatStanceTags::Aiming) || DesiredCombatStance.MatchesTag(ALSXTCombatStanceTags::Ready)))
		{
			Combat->GetClosestTarget();
		}
	}
	else
	{
		Combat->DisengageAllTargets();
	}
}

bool AALSXTCharacterAdvanced::CanPerformPrimaryAction_Implementation() const
{
	return true;
}

bool AALSXTCharacterAdvanced::CanPerformSecondaryAction_Implementation() const
{
	return true;
}

// Input Actions

// Acrobatic Action Component
void AALSXTCharacterAdvanced::InputAcrobaticAction(const FInputActionValue& ActionValue)
{
	if (CanPerformAcrobaticAction() && bMovementEnabled && GetLocomotionAction() != AlsLocomotionActionTags::Acrobatic)
	{
		AcrobaticActions->TryAcrobaticAction();
	}
}

void AALSXTCharacterAdvanced::InputReload()
{
	// 
}

void AALSXTCharacterAdvanced::InputReloadWithRetention()
{
	// 
}

//Combat Component
void AALSXTCharacterAdvanced::InputSwitchTargetLeft()
{
	Combat->GetTargetLeft();
}

void AALSXTCharacterAdvanced::InputSwitchTargetRight()
{
	Combat->GetTargetRight();
}

void AALSXTCharacterAdvanced::BeginHoldBreathTimer()
{
	if (ALSXTSettings->Freelook.bEnableFreelook)
	{
		GetWorld()->GetTimerManager().SetTimer(HoldBreathTimerHandle, HoldBreathTimerDelegate, 0.1f, true);
	}
}

void AALSXTCharacterAdvanced::HoldBreathTimer()
{

}

void AALSXTCharacterAdvanced::EndHoldBreathTimer()
{
	// Clear Attack Trace Timer
	// FALSXTFreelookState EmptyState;
	// SetFreelookState(EmptyState);
	GetWorld()->GetTimerManager().ClearTimer(HoldBreathTimerHandle);
}

// Interface Functions

//Camera Effects Component Interface
UAlsCameraComponent* AALSXTCharacterAdvanced::GetCameraComponent_Implementation() const
{
	return Camera;
}

UALSXTCharacterCameraEffectsComponent* AALSXTCharacterAdvanced::GetCameraEffectsComponent_Implementation() const
{
	return CameraEffects;
}

void AALSXTCharacterAdvanced::SetRadialBlurEffect_Implementation(float Amount)
{
	CameraEffects->SetRadialBlur(Amount);
}

void AALSXTCharacterAdvanced::ResetRadialBlurEffect_Implementation()
{
	CameraEffects->ResetRadialBlur();
}

void AALSXTCharacterAdvanced::SetFocusEffect_Implementation(bool NewFocus)
{
	CameraEffects->SetFocusEffect(NewFocus);
}

void AALSXTCharacterAdvanced::AddDrunkEffect_Implementation(float Magnitude, float Length, float FadeInLength, float FadeOutLength)
{
	CameraEffects->AddDrunkEffect(Magnitude, Length);
}

void AALSXTCharacterAdvanced::ResetDrunkEffect_Implementation()
{
	CameraEffects->ResetDrunkEffect();
}

void AALSXTCharacterAdvanced::AddHighEffect_Implementation(float Magnitude, float Length, float FadeInLength, float FadeOutLength)
{
	CameraEffects->AddHighEffect(Magnitude, Length);
}

void AALSXTCharacterAdvanced::ResetHighEffect_Implementation()
{
	CameraEffects->ResetHighEffect();
}

void AALSXTCharacterAdvanced::AddSuppressionEffect_Implementation(float Magnitude, float PostDelay)
{
	CameraEffects->AddSuppression(Magnitude, PostDelay);
}

void AALSXTCharacterAdvanced::ResetSuppressionEffect_Implementation()
{
	CameraEffects->ResetSuppression();
}

void AALSXTCharacterAdvanced::AddBlindnessEffect_Implementation(float Magnitude, float Length, float FadeOutLength)
{
	CameraEffects->AddBlindnessEffect(Magnitude, Length);
}

void AALSXTCharacterAdvanced::ResetBlindnessEffect_Implementation()
{
	CameraEffects->ResetBlindnessEffect();
}

void AALSXTCharacterAdvanced::AddProjectileFlyByEffect_Implementation(USoundBase* Sound, FVector Location, FRotator Rotation)
{
	//
}

void AALSXTCharacterAdvanced::AddConcussionEffect_Implementation(float Magnitude, float Length, float FadeInLength, float FadeOutLength)
{
	CameraEffects->AddConcussionEffect(Magnitude, Length);
}

void AALSXTCharacterAdvanced::ResetConcussionEffect_Implementation()
{
	CameraEffects->ResetConcussionEffect();
}

void AALSXTCharacterAdvanced::AddDamageEffect_Implementation(float Damage, const FGameplayTag& DamageType, const FHitResult& HitResult, float PostDelay)
{
	CameraEffects->AddDamageEffect(Damage, PostDelay);
}

void AALSXTCharacterAdvanced::ResetDamageEffect_Implementation()
{
	CameraEffects->ResetDamageEffect();
}

void AALSXTCharacterAdvanced::AddNearDeathEffect_Implementation(float Damage, const FGameplayTag& DamageType)
{
	//
}

void AALSXTCharacterAdvanced::ResetNearDeathEffect_Implementation()
{
	//
}

void AALSXTCharacterAdvanced::AddDeathEffect_Implementation(float Damage, const FGameplayTag& DamageType, const FHitResult& HitResult, float PostDelay)
{
	//
}

void AALSXTCharacterAdvanced::ResetDeathEffect_Implementation()
{
	//
}

//Head Look At Interface
void AALSXTCharacterAdvanced::IsInFrontOf_Implementation(bool& IsInFrontOf, FVector LookAtActorLocation) const
{
	IsInFrontOf = FVector::DotProduct(LookAtActorLocation, ALSXTMesh->GetForwardVector()) < 0.0f;
}

//Combat Interface
void AALSXTCharacterAdvanced::BeginCombatAttackCollisionTrace_Implementation(FALSXTCombatAttackTraceSettings TraceSettings)
{
	AALSXTCharacter::BeginAttackCollisionTrace(TraceSettings);
}

void AALSXTCharacterAdvanced::EndCombatAttackCollisionTrace_Implementation()
{
	AALSXTCharacter::EndAttackCollisionTrace();
}