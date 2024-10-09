// MIT

#include "Components/Character/ALSXTGestureComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UALSXTGestureComponent::UALSXTGestureComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}

void UALSXTGestureComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Parameters;
	Parameters.bIsPushBased = true;

	Parameters.Condition = COND_SkipOwner;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, CurrentGestureMontage, Parameters)
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
	if (IALSXTCharacterInterface::Execute_CanGesture(GetOwner()))
	{
		if (Character->GetLocalRole() == ROLE_AutonomousProxy)
		{
			ServerAddDesiredGesture(Gesture, GestureHand);
		}
		else
		{
			AddGesture(Gesture, GestureHand);
		}
	}
}

void UALSXTGestureComponent::ExitGesture(bool Immediate)
{
	if (Immediate)
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Stop(0.5f, CurrentGestureMontage);
	}
	else
	{
		Character->GetMesh()->GetAnimInstance()->Montage_JumpToSection(GestureSettings->ExitSectionName, CurrentGestureMontage);
	}
}

void UALSXTGestureComponent::ServerAddDesiredGesture_Implementation(const FGameplayTag& Gesture, const FGameplayTag& GestureHand)
{
	AddDesiredGesture(Gesture, GestureHand);
}

void UALSXTGestureComponent::AddGesture(const FGameplayTag& Gesture, const FGameplayTag& GestureHand)
{
	if (IsValid(GestureSettings) && IALSXTCharacterInterface::Execute_CanGesture(GetOwner()))
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