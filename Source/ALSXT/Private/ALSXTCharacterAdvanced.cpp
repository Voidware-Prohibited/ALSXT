// MIT


#include "ALSXTCharacterAdvanced.h" 	
#include "Math/Vector.h"

AALSXTCharacterAdvanced::AALSXTCharacterAdvanced(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	Combat = CreateDefaultSubobject<UALSXTCombatComponent>(TEXT("Combat"));
	AddOwnedComponent(Combat);

	AcrobaticActions = CreateDefaultSubobject<UALSXTAcrobaticActionComponent>(TEXT("Acrobatic Actions"));
	AddOwnedComponent(AcrobaticActions);
	
	CameraEffects = CreateDefaultSubobject<UALSXTCharacterCameraEffectsComponent>(TEXT("Camera Effects"));
	AddOwnedComponent(CameraEffects);
}


//Head Look At Interface
void AALSXTCharacterAdvanced::IsInFrontOf_Implementation(bool& IsInFrontOf, FVector LookAtActorLocation) const
{
	IsInFrontOf =  FVector::DotProduct(LookAtActorLocation, ALSXTMesh->GetForwardVector()) < 0.0f;
}