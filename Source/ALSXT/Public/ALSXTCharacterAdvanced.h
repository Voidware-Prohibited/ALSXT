// MIT

#pragma once

#include "CoreMinimal.h"
#include "ALSXTCharacter.h"
#include "Interfaces/ALSXTHeadLookAtInterface.h"
#include "Interfaces/ALSXTAcrobaticActionComponentInterface.h"
#include "Components/Character/ALSXTCombatComponent.h"
#include "Components/Character/ALSXTCharacterSoundComponent.h"
#include "Components/Character/ALSXTAcrobaticActionComponent.h"
#include "Components/Character/ALSXTCharacterCameraEffectsComponent.h"
#include "ALSXTCharacterAdvanced.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API AALSXTCharacterAdvanced : public AALSXTCharacter, public IALSXTHeadLookAtInterface, public IALSXTAcrobaticActionComponentInterface
{
	GENERATED_BODY()

public:
	AALSXTCharacterAdvanced(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input Actions", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> AcrobaticAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCombatComponent* Combat;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTAcrobaticActionComponent* AcrobaticActions;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCharacterCameraEffectsComponent* CameraEffects;

	//Head Look At Interface
	void IsInFrontOf_Implementation(bool& IsInFrontOf, FVector LookAtActorLocation) const override;
	
};
