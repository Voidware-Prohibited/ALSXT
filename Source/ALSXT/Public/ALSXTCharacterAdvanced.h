// MIT

#pragma once

#include "CoreMinimal.h"
#include "ALSXTCharacter.h"
#include "Interfaces/ALSXTCharacterCameraEffectsComponentInterface.h"
#include "Interfaces/ALSXTHeadLookAtInterface.h"
#include "Interfaces/ALSXTAcrobaticActionComponentInterface.h"
#include "Interfaces/ALSXTCombatInterface.h"
#include "Interfaces/ALSXTFirearmInterface.h"
#include "AlsCameraComponent.h"
#include "Components/Character/ALSXTCharacterCameraEffectsComponent.h"
#include "Components/Character/ALSXTCombatComponent.h"
#include "Components/Character/ALSXTCharacterSoundComponent.h"
#include "Components/Character/ALSXTAcrobaticActionComponent.h"
#include "Components/Character/ALSXTCharacterCameraEffectsComponent.h"
#include "ALSXTCharacterAdvanced.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API AALSXTCharacterAdvanced : public AALSXTCharacter, public IALSXTCharacterCameraEffectsComponentInterface, public IALSXTHeadLookAtInterface, public IALSXTAcrobaticActionComponentInterface, public IALSXTTargetLockInterface, public IALSXTCombatInterface, public IALSXTFirearmInterface
{
	GENERATED_BODY()

public:
	AALSXTCharacterAdvanced(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* Input) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> PrimaryInteractionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SecondaryInteractionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input Actions", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> AcrobaticAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SelectEmoteAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (DisplayThumbnail = false))
	TObjectPtr<UInputAction> SelectGestureAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCombatComponent* Combat;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTAcrobaticActionComponent* AcrobaticActions;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTEmoteComponent* Emotes;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTGestureComponent* Gestures;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	class UALSXTCharacterCameraEffectsComponent* CameraEffects;

	//Character Camera Effects Component Interface
	UAlsCameraComponent* GetCameraComponent_Implementation() const override;

	UALSXTCharacterCameraEffectsComponent* GetCameraEffectsComponent_Implementation() const override;

	//Head Look At Interface
	void IsInFrontOf_Implementation(bool& IsInFrontOf, FVector LookAtActorLocation) const override;

	// Combat Interface
	void BeginCombatAttackCollisionTrace_Implementation(FALSXTCombatAttackTraceSettings TraceSettings) override;

	void EndCombatAttackCollisionTrace_Implementation() override;

private:
	void InputAcrobaticAction(const FInputActionValue& ActionValue);

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Als|Input Actions")
	bool CanPerformAcrobaticAction() const;
	
};
