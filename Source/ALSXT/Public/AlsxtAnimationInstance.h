// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsAnimationInstance.h"
#include "State/AlsFeetState.h"
#include "State/AlsxtPoseState.h"
#include "State/AlsxtStatusState.h"
#include "State/AlsxtBreathState.h"
#include "State/AlsxtDefensiveModeState.h"
#include "State/AlsxtBumpPoseState.h"
#include "State/AlsxtHeadLookAtState.h"
#include "Settings/AlsxtHeldItemSettings.h"
#include "Settings/AlsxtStationaryModeSettings.h"
#include "AlsxtCharacter.h"
#include "Utility/AlsxtGameplayTags.h"
#include "Utility/AlsxtAimableOverlayObjectGameplayTags.h"
#include "Interfaces/AlsxtCharacterInterface.h"
#include "Settings/AlsxtAnimationInstanceSettings.h"
#include "Settings/AlsxtCharacterBreathEffectsSettings.h"
#include "AbilitySystem/AbilitySystemComponent/AlsxtAbilitySystemComponent.h"
#include "AlsxtAnimationInstance.generated.h"

class UAlsxtLinkedAnimationInstance;
class AAlsxtCharacter;

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTControlRigInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALSXT")
	bool bObstacleNavigation{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALSXT")
	bool bCombatStance{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALSXT")
	bool bEnableLeftHandIK{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALSXT")
	bool bHeadLookAt{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALSXT")
	bool bFreelooking{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALSXT")
	FTransform LeftHandTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALSXT")
	FTransform SightTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALSXT")
	FTransform TargetTransform;

};

UCLASS()
class ALSXT_API UAlsxtAnimationInstance : public UAlsAnimationInstance
{
	GENERATED_BODY()

	friend UAlsxtLinkedAnimationInstance;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAlsxtAnimationInstanceSettings> ALSXTSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	TObjectPtr<AAlsxtCharacter> ALSXTCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtAbilitySystemComponent> AlsxtAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtAnimationParametersState AnimationParametersState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTJigglePhysicsSettings JigglePhysicsSettings;

	UPROPERTY(BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtStaminaThresholdSettings StaminaThresholdSettings;

	UPROPERTY(BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtCharacterBreathEffectsSettings CharacterBreathEffectsSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Freelooking{ALSXTFreelookingTags::False};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Sex{ALSXTSexTags::Male};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag LocomotionVariant{ALSXTLocomotionVariantTags::Default};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Injury{AlsxtStatusEffectLocomotionVariantTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Readiness{AlsxtReadinessTags::Relaxed};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag CombatStance{ALSXTCombatStanceTags::Neutral};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponFirearmStance{ALSXTWeaponFirearmStanceTags::Regular};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponReadyPosition{ALSXTWeaponReadyPositionTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FAlsxtPoseState ALSXTPoseState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	FAlsStandingState AlsxtCrouchingState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtStatusState StatusState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtBreathState BreathState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtHeldItemSettings HeldItemSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtHeldItemState HeldItemState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtAimState AimState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtStationaryModeState StationaryModeState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtFreelookState FreelookState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtHeadLookAtState HeadLookAtState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtBumpPoseState CrowdNavigationPoseState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtBumpPoseState BumpPoseState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtDefensiveModeState DefensiveModeState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FAlsxtDefensiveModeAnimations DefensiveModeAnimations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag DefensiveMode{ FGameplayTag::EmptyTag };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Lean{ FGameplayTag::EmptyTag };

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
	FGameplayTag FirearmFingerAction {AlsxtFirearmFingerActionTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag FirearmFingerActionHand {FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag ImpactType{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponCarryPosition{FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag FirearmSightLocation{AlsxtFirearmFingerActionTags::None};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag VaultType {FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag WeaponObstruction {FGameplayTag::EmptyTag};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool DoesOverlayObjectUseLeftHandIK{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsxtRecoilState RecoilState;

protected:
	virtual void RefreshAlsxtPose();

public:

	UAlsxtAnimationInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaTime) override;

	virtual void NativePostEvaluateAnimation() override;

protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

	void AlsxtPlayQueuedTransitionAnimation();

	UFUNCTION(BlueprintCallable, Category = "ALS|Linked Animation Instance", Meta = (BlueprintThreadSafe))
	void AlsxtRefreshCrouchingMovement();

	UFUNCTION(BlueprintCallable, Category = "ALS|Linked Animation Instance", Meta = (BlueprintThreadSafe))
	void AlsxtRefreshStandingMovement();

	UFUNCTION(BlueprintCallable, Category = "ALS|Linked Animation Instance", Meta = (BlueprintThreadSafe))
	void AlsxtRefreshDynamicTransitions();

	// Core

protected:
	UFUNCTION(BlueprintPure, Category = "ALS|Als Animation Instance",
		Meta = (BlueprintProtected, BlueprintThreadSafe, ReturnDisplayName = "Setting"))
	UAlsxtAnimationInstanceSettings* GetALSXTSettingsUnsafe() const;

protected:
	// Spine Rotation
	virtual bool IsSpineRotationAllowed() override;

	// Rotate In Place
	virtual bool IsRotateInPlaceAllowed() override;

	// Turn In Place

	virtual bool IsTurnInPlaceAllowed() override;

	void UpdateStatusState();

	UFUNCTION(BlueprintPure, Category = "ALSXT|Animation Instance", Meta = (BlueprintThreadSafe, ReturnDisplayName = "ALSXT Ability System Component"))
	TSoftObjectPtr<UAlsxtAbilitySystemComponent> GetAlsxtAbilitySystemComponent() const;

	float GetBreathingRate() const;

	UFUNCTION(BlueprintPure, Category = "ALSXT|Animation Instance", Meta = (BlueprintThreadSafe, ReturnDisplayName = "Rig Input"))
	FALSXTControlRigInput GetALSXTControlRigInput() const;

private:
	FVector2f GetAlsxtRelativeAccelerationAmount() const;
};

inline UAlsxtAnimationInstanceSettings* UAlsxtAnimationInstance::GetALSXTSettingsUnsafe() const
{
	return ALSXTSettings;
}