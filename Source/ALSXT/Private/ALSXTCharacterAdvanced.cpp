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
		// EnhancedInput->BindAction(PrimaryInteractionAction, ETriggerEvent::Triggered, this, &ThisClass::InputPrimaryInteraction);
		// EnhancedInput->BindAction(SecondaryInteractionAction, ETriggerEvent::Triggered, this, &ThisClass::InputSecondaryInteraction);
		// EnhancedInput->BindAction(SelectEmoteAction, ETriggerEvent::Triggered, this, &ThisClass::InputSelectEmote);
		// EnhancedInput->BindAction(SelectGestureAction, ETriggerEvent::Triggered, this, &ThisClass::InputSelectGesture);

		OnSetupPlayerInputComponentUpdated.Broadcast();
	}
}

void AALSXTCharacterAdvanced::InputAcrobaticAction(const FInputActionValue& ActionValue)
{
	if (CanPerformAcrobaticAction() && bMovementEnabled && GetLocomotionAction() != AlsLocomotionActionTags::Acrobatic)
	{
		AcrobaticActions->TryAcrobaticAction();
	}
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