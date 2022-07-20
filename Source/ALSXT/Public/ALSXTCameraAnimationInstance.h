// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsCameraAnimationInstance.h"
#include "Utility/ALSXTGameplayTags.h"
#include "ALSXTCameraAnimationInstance.generated.h"

/**
 * 
 */
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

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
};
