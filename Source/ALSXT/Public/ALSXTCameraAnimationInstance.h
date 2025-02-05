// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsCameraAnimationInstance.h"
#include "Utility/ALSXTGameplayTags.h"
#include "Settings/ALSXTCameraAnimationInstanceSettings.h"
#include "ALSXTCharacter.h"
#include "Interfaces/ALSXTCharacterInterface.h"
#include "Interfaces/ALSXTIdleAnimationComponentInterface.h"
#include "ALSXTCameraAnimationInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFirstPersonOverrideChanged, float, FirstPersonOverride);

class AALSXTCharacter;
// class UALSXTCameraComponent;
class UAlsCameraComponent;


UCLASS()
class ALSXT_API UALSXTCameraAnimationInstance : public UAlsCameraAnimationInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		TObjectPtr<AALSXTCharacter> ALSXTCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		TObjectPtr<UAlsCameraComponent> ALSXTCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bIsFocusActive {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	float CameraZoom {0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	bool bIsIdleCameraRotationActive {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	float IdleCameraRotation {0.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		FGameplayTag Overlay {
		AlsOverlayModeTags::Default
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		FGameplayTag Freelooking {
		ALSXTFreelookingTags::False
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		FGameplayTag Sex {
		ALSXTSexTags::Male
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		FGameplayTag LocomotionVariant {
		ALSXTLocomotionVariantTags::Default
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		FGameplayTag Injury {
		ALSXTInjuryTags::None
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		FGameplayTag CombatStance {
		ALSXTCombatStanceTags::Neutral
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		FGameplayTag WeaponFirearmStance {
		ALSXTWeaponFirearmStanceTags::Regular
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		FGameplayTag WeaponReadyPosition {
		ALSXTWeaponReadyPositionTags::None
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess))
	UALSXTGeneralCameraAnimationInstanceSettings* GeneralCameraAnimationInstanceSettings{nullptr};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess))
	UALSXTCameraAnimationInstanceSettings* CameraAnimationInstanceSettings{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTCameraAnimationInstanceState CameraAnimationInstanceState;

	virtual void OnFirstPersonOverrideChangedEvent();
	float FirstPersonOverride{ GetCurveValue("FirstPersonOverride") };
	float PreviousFirstPersonOverride{ GetCurveValue("FirstPersonOverride") };
	FTimerHandle FirstPersonOverrideHandle;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnFirstPersonOverrideChanged OnFirstPersonOverrideChanged;
	
};
