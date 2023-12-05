// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsAnimationInstance.h"
#include "State/AlsFeetState.h"
#include "State/ALSXTStatusState.h"
#include "State/ALSXTBreathState.h"
#include "State/ALSXTDefensiveModeState.h"
#include "State/ALSXTBumpPoseState.h"
#include "ALSXTCharacter.h"
#include "Utility/ALSXTGameplayTags.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Settings/ALSXTAnimationInstanceSettings.h"
#include "ALSXTAnimationInstance.generated.h"

class UALSXTLinkedAnimationInstance;
class AALSXTCharacter;

UCLASS()
class ALSXT_API UALSXTAnimationInstance : public UAlsAnimationInstance
{
	GENERATED_BODY()

	friend UALSXTLinkedAnimationInstance;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UALSXTAnimationInstanceSettings> ALSXTSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	TObjectPtr<AALSXTCharacter> ALSXTCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTStaminaThresholdSettings StaminaThresholdSettings;

	UPROPERTY(BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTCharacterBreathEffectsSettings CharacterBreathEffectsSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Freelooking{ALSXTFreelookingTags::False};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Sex{ALSXTSexTags::Male};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionVariant{ALSXTLocomotionVariantTags::Default};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Injury{ALSXTInjuryTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag CombatStance{ALSXTCombatStanceTags::Neutral};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponFirearmStance{ALSXTWeaponFirearmStanceTags::Regular};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponReadyPosition{ALSXTWeaponReadyPositionTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTStatusState StatusState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTBreathState BreathState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTAimState AimState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTFreelookState FreelookState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTBumpPoseState CrowdNavigationPoseState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTBumpPoseState BumpPoseState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTDefensiveModeState DefensiveModeState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag DefensiveMode{ALSXTDefensiveModeTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag StationaryMode{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag HitReaction{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag HitSurface{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag BumpReaction{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag BumpSurface{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag HoldingBreath{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag PhysicalAnimationMode{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Gesture{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag GestureHand{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag ReloadingType{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag ForegripPosition{ALSXTForegripPositionTags::Default};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FTransform ForegripTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag FirearmFingerAction {ALSXTFirearmFingerActionTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag FirearmFingerActionHand {FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactType{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponCarryPosition{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag FirearmSightLocation{ALSXTFirearmFingerActionTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag VaultType {FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponObstruction {FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool DoesOverlayObjectUseLeftHandIK{ false };

public:

	UALSXTAnimationInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaTime) override;

	virtual void NativePostEvaluateAnimation() override;

	// Core

protected:
	UFUNCTION(BlueprintPure, Category = "ALS|Als Animation Instance",
		Meta = (BlueprintProtected, BlueprintThreadSafe, ReturnDisplayName = "Setting"))
	UALSXTAnimationInstanceSettings* GetALSXTSettingsUnsafe() const;

protected:
	// Spine Rotation
	virtual bool IsSpineRotationAllowed() override;

	// Rotate In Place
	virtual bool IsRotateInPlaceAllowed() override;

	// Turn In Place

	virtual bool IsTurnInPlaceAllowed() override;

	void UpdateStatusState();
	void UpdateBreathState();
	bool ShouldUpdateBreathState() const;
	bool ShouldTransitionBreathState();
	FALSXTTargetBreathState CalculateTargetBreathState();
	void SetTargetBreathState(FALSXTTargetBreathState NewTargetBreathState);
	void TransitionBreathState();
};

inline UALSXTAnimationInstanceSettings* UALSXTAnimationInstance::GetALSXTSettingsUnsafe() const
{
	return ALSXTSettings;
}