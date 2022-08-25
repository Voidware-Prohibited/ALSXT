// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsCameraAnimationInstance.h"
#include "Utility/ALSXTGameplayTags.h"
// #include "ALSXTCharacter.h"
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
