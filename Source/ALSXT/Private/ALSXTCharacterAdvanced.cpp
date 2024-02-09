// MIT


#include "ALSXTCharacterAdvanced.h"

AALSXTCharacterAdvanced::AALSXTCharacterAdvanced(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	CombatComponent = CreateDefaultSubobject<UALSXTCombatComponent>(TEXT("Combat Component"));
	AddOwnedComponent(CombatComponent);

	CharacterSoundComponent = CreateDefaultSubobject<UALSXTCharacterSoundComponent>(TEXT("Character Sound Component"));
	AddOwnedComponent(CharacterSoundComponent);
	
	CameraEffects = CreateDefaultSubobject<UALSXTCharacterCameraEffectsComponent>(TEXT("Camera Effects"));
	AddOwnedComponent(CameraEffects);
}