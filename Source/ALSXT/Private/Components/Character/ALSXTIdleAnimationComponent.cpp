// MIT


#include "Components/Character/ALSXTIdleAnimationComponent.h"

// Sets default values for this component's properties
UALSXTIdleAnimationComponent::UALSXTIdleAnimationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UALSXTIdleAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AALSXTCharacter>(GetOwner());
	if (Character)
	{
		Camera = Character->Camera;
	}
}


// Called every frame
void UALSXTIdleAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UALSXTIdleAnimationSettings* UALSXTIdleAnimationComponent::SelectIdleSettings_Implementation()
{
	return nullptr;
}

void UALSXTIdleAnimationComponent::StartIdleCounterTimer()
{
	GetWorld()->GetTimerManager().SetTimer(IdleCounterTimerHandle, IdleCounterTimerDelegate, 0.01f, true);
}

void UALSXTIdleAnimationComponent::IdleCounterTimer()
{
	IdleCounterCurrent = IdleCounterCurrent + 0.01;
	if (IdleCounterCurrent >= IdleCounterTarget)
	{
		StartIdle();
		ResetIdleCounterTimer();
	}
	// ...
}

void UALSXTIdleAnimationComponent::ResetIdleCounterTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(IdleCounterTimerHandle);
	IdleCounterCurrent = 0.0f;
	IdleCounterTarget = 0.0f;
}

void UALSXTIdleAnimationComponent::StartCameraRotationTimer()
{
	GetWorld()->GetTimerManager().SetTimer(CameraRotationTimerHandle, CameraRotationTimerDelegate, 0.01f, true);
}

void UALSXTIdleAnimationComponent::CameraRotationTimer()
{
	// ...
}

void UALSXTIdleAnimationComponent::ResetCameraRotationTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(CameraRotationTimerHandle);
	CameraOffset = FVector::ZeroVector;
}

void UALSXTIdleAnimationComponent::StartIdle()
{
	// ..
}

void UALSXTIdleAnimationComponent::StopIdle()
{
	// ..
}