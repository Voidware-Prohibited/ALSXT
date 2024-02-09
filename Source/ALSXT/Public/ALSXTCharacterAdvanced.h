// MIT

#pragma once

#include "CoreMinimal.h"
#include "ALSXTCharacter.h"
#include "Interfaces/ALSXTHeadLookAtInterface.h"
#include "Components/Character/ALSXTCombatComponent.h"
#include "Components/Character/ALSXTCharacterSoundComponent.h"
#include "Components/Character/ALSXTCharacterCameraEffectsComponent.h"
#include "ALSXTCharacterAdvanced.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API AALSXTCharacterAdvanced : public AALSXTCharacter, public IALSXTHeadLookAtInterface
{
	GENERATED_BODY()

public:
	AALSXTCharacterAdvanced(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCombatComponent* CombatComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCharacterSoundComponent* CharacterSoundComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCharacterCameraEffectsComponent* CameraEffects;
	
};
