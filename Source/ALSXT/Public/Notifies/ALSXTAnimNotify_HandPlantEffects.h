#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Engine/DataAsset.h"
#include "Engine/EngineTypes.h"
#include "Notifies/AlsAnimNotify_FootstepEffects.h"
#include "ALSXTAnimNotify_HandPlantEffects.generated.h"

enum EPhysicalSurface : int;
struct FHitResult;
class USoundBase;


USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHandPlantSoundSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	EAlsFootstepSoundSpawnMode SpawnMode{ EAlsFootstepSoundSpawnMode::SpawnAtTraceHitLocation };
};

USTRUCT(BlueprintType)
struct ALSXT_API FALSXTHandPlantEffectSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FALSXTHandPlantSoundSettings Sound;

public:
#if WITH_EDITOR
	void PostEditChangeProperty(const FPropertyChangedEvent& ChangedEvent);
#endif
};

UCLASS(Blueprintable, BlueprintType)
class ALSXT_API UALSXTHandPlantEffectsSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TEnumAsByte<ECollisionChannel> SurfaceTraceChannel{ECC_Visibility};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float SurfaceTraceDistance{50.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", DisplayName = "Hand Left Y Axis")
	FVector3f HandLeftYAxis{ 0.0f, 0.0f, 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", DisplayName = "Hand Left Z Axis")
	FVector3f HandLeftZAxis{ 1.0f, 0.0f, 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", DisplayName = "Hand Right Y Axis")
	FVector3f HandRightYAxis{ 0.0f, 0.0f, 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", DisplayName = "Hand Right Z Axis")
	FVector3f HandRightZAxis{ -1.0f, 0.0f, 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ForceInlineRow))
	TMap<TEnumAsByte<EPhysicalSurface>, FALSXTHandPlantEffectSettings> Effects;

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& ChangedEvent) override;
#endif
};

UCLASS(DisplayName = "Als Footstep Effects Animation Notify",
	AutoExpandCategories = ("Settings|Sound", "Settings|Decal", "Settings|Particle System"))
class ALSXT_API UALSXTAnimNotify_HandPlantEffects : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UALSXTHandPlantEffectsSettings> HandPlantEffectsSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	EAlsFootBone HandBone{ EAlsFootBone::Left };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	uint8 bSkipEffectsWhenInAir : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound")
	uint8 bSpawnSound : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (ClampMin = 0, ForceUnits = "x"))
	float SoundVolumeMultiplier{1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound", Meta = (ClampMin = 0, ForceUnits = "x"))
	float SoundPitchMultiplier{1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Sound")
	uint8 bIgnoreHandPlantSoundBlockCurve : 1 {false};

public:
	virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITOR
	virtual void OnAnimNotifyCreatedInEditor(FAnimNotifyEvent& NotifyEvent) override;
#endif

	virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence,
	                    const FAnimNotifyEventReference& NotifyEventReference) override;

private:
	void SpawnSound(USkeletalMeshComponent* Mesh, const FALSXTHandPlantSoundSettings& SoundSettings,
	                const FVector& FootstepLocation, const FQuat& FootstepRotation) const;

};
