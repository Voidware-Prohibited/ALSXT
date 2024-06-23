// MIT


#include "Components/Character/ALSXTGestureComponent.h"

// Sets default values for this component's properties
UALSXTGestureComponent::UALSXTGestureComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UALSXTGestureComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = IALSXTCharacterInterface::Execute_GetCharacter(GetOwner());
	AlsCharacter = Cast<AAlsCharacter>(GetOwner());
	
}


// Called every frame
void UALSXTGestureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Gesture

void UALSXTGestureComponent::AddDesiredGesture(const FGameplayTag& Gesture, const FGameplayTag& GestureHand)
{
	if (CanGesture())
	{
		if (Character->GetLocalRole() == ROLE_AutonomousProxy)
		{
			ServerAddDesiredGesture(Gesture, GestureHand);
		}
	}
}

void UALSXTGestureComponent::ServerAddDesiredGesture_Implementation(const FGameplayTag& Gesture, const FGameplayTag& GestureHand)
{
	AddDesiredGesture(Gesture, GestureHand);
}

void UALSXTGestureComponent::AddGesture(const FGameplayTag& Gesture, const FGameplayTag& GestureHand)
{
	if (IsValid(GestureSettings) && CanGesture())
	{
		FALSXTGestureMontages* FoundMontages = GestureSettings->Gestures.Find(Gesture);
		if (GestureHand == ALSXTHandTags::Left && IsValid(FoundMontages->LeftMontage))
		{
			Character->GetMesh()->GetAnimInstance()->Montage_Play(FoundMontages->LeftMontage);
			OnGesture(Gesture, GestureHand);
		}
		if (GestureHand == ALSXTHandTags::Right && IsValid(FoundMontages->RightMontage))
		{
			Character->GetMesh()->GetAnimInstance()->Montage_Play(FoundMontages->RightMontage);
			OnGesture(Gesture, GestureHand);
		}
	}
}

void UALSXTGestureComponent::OnGesture_Implementation(const FGameplayTag& Gesture, const FGameplayTag& GestureHand) {}