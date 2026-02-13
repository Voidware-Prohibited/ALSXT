#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagContainer.h"
#include "Settings/AlsxtFootstepEffectSettings.h"
#include "State/AlsxtFootstepState.h"
#include "Notifies/AlsAnimNotify_FootstepEffects.h"
#include "Settings/AlsxtVertexSettings.h"
#include "AlsxtAnimNotify_FootstepEffects.generated.h"

class USoundBase;
class UMaterialInterface;
class UNiagaraSystem;

UCLASS(DisplayName = "ALSXT Footstep Effects Animation Notify",
	AutoExpandCategories = ("Settings|Sound", "Settings|Decal", "Settings|Particle System"), Meta = (ShowWorldContextPin))
class ALSXT_API UAlsxtAnimNotify_FootstepEffects : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Values", Meta = (AllowPrivateAccess))
	TWeakObjectPtr<UPhysicalMaterial> PreviewSurface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Values", Meta = (AllowPrivateAccess))
	FGameplayTag PreviewFootwearType;

public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	UALSXTFootstepEffectsSettings* FootstepEffectsSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	bool bSpawnSound{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	bool bSpawnDecal{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	bool bSpawnParticleSystem{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	EAlsFootBone FootBone {EAlsFootBone::Left};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound", Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
	float SoundVolumeMultiplier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound", Meta = (AllowPrivateAccess, ClampMin = 0, ForceUnits = "x"))
	float SoundPitchMultiplier{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
	bool bSkipEffectsWhenInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
	EALSXTFootstepSoundType SoundType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sound", Meta = (AllowPrivateAccess))
	bool bIgnoreFootstepSoundBlockCurve;

	UFUNCTION(BlueprintCallable, Category = "Settings", Meta = (AutoCreateRefTerm = "NewALSXTFootstepEffectsSettings"))
	void SetFootstepEffectsSettings(UALSXTFootstepEffectsSettings* NewALSXTFootstepEffectsSettings, float NewSoundVolumeMultiplier, float NewSoundPitchMultiplier, EAlsFootBone NewFootBone, bool bNewSkipEffectsWhenInAir, bool bNewSpawnSound, EALSXTFootstepSoundType NewFootstepSoundType, bool bNewIgnoreFootstepSoundBlockCurve, bool bNewSpawnDecal, bool bNewSpawnParticleSystem);

	FHitResult HitResult;

	FAlsxtFootprintsState CurrentFootprintsState;
};
