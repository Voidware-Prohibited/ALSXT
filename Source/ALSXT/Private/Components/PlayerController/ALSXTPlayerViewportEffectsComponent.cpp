// MIT


#include "Components/PlayerController/ALSXTPlayerViewportEffectsComponent.h"
#include "Interfaces/ALSXTControllerVFXInterface.h"
#include "Engine/Scene.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UALSXTPlayerViewportEffectsComponent::UALSXTPlayerViewportEffectsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UALSXTPlayerViewportEffectsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->GetLocalRole() != ROLE_AutonomousProxy)
	{
		return;
	}

	if (GeneralCameraEffectsSettings.bEnableEffects)
	{
		Character = IALSXTControllerVFXInterface::Execute_GetCharacter(GetOwner());
		Camera = IALSXTControllerVFXInterface::Execute_GetCamera(GetOwner());
	}
	
}


// Called every frame
void UALSXTPlayerViewportEffectsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UALSXTPlayerViewportEffectsComponent::AddSuppressionEffect(float Amount) { }

void UALSXTPlayerViewportEffectsComponent::ClearSuppressionEffect() { }

