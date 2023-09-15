// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSXTCharacter.h"
#include "Components/PostProcessComponent.h"
#include "Settings/ALSXTCameraEffectsSettings.h"
#include "ALSXTCharacterCameraEffectsComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UALSXTCharacterCameraEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSXTCharacterCameraEffectsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character {Cast<AALSXTCharacter>(GetOwner())};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTGeneralCameraEffectsSettings GeneralCameraEffectsSettings;
	
	UPROPERTY(BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	UPostProcessComponent* PostProcessComponent{ nullptr };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentRadialBlurAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentSuppressionAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentBlindnessEffectAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentDamageEffectAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentConcussionEffectAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentDrunkEffectAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentHighEffectAmount{ 0.0f };

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void AddDrunkEffect(float NewMagnitude, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnDrunkEffectChanged(float NewMagnitude);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ResetDrunkEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutDrunkEffect(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutDrunkEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void AddHighEffect(float NewMagnitude, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnHighEffectChanged(float NewMagnitude);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ResetHighEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutHighEffect(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutHighEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void AddSuppression(float NewMagnitude, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnSuppressionEffectChanged(float NewMagnitude);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ResetSuppression();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutSuppression(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutSuppression();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void AddBlindnessEffect(float NewMagnitude, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnBlindnessEffectChanged(float NewMagnitude);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ResetBlindnessEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutBlindnessEffect(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutBlindnessEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void AddConcussionEffect(float NewMagnitude, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnConcussionEffectChanged(float NewMagnitude);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ResetConcussionEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutConcussionEffect(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutConcussionEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void AddDamageEffect(float NewMagnitude, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnDamageEffectChanged(float NewMagnitude);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ResetDamageEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutDamageEffect(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutDamageEffect();

private:
	// Timer Handles
	FTimerHandle CameraEffectsTraceTimer;
	FTimerHandle RadialBlurTimer;
	FTimerHandle DrunkEffectFadeOutTimer;
	FTimerHandle HighEffectFadeOutTimer;
	FTimerHandle SuppressionFadeOutTimer;
	FTimerHandle BlindnessEffectFadeOutTimer;
	FTimerHandle ConcussionEffectFadeOutTimer;
	FTimerHandle DamageEffectFadeOutTimer;

	// Material Indexes
	int32 RadialBlurBlendableIndex;
	int32 DrunkEffectBlendableIndex;
	int32 HighEffectBlendableIndex;
	int32 SuppressionBlendableIndex;
	int32 BlindnessEffectBlendableIndex;
	int32 ConcussionEffectBlendableIndex;
	int32 DamageEffectBlendableIndex;
	int32 DeathEffectBlendableIndex;

	// Recovery (Fade-Out) Scales
	float RecoveryScale {0.0f};
	float DrunkEffectRecoveryScale{ 0.0f };
	float HighEffectRecoveryScale{ 0.0f };
	float SuppressionEffectRecoveryScale{ 0.0f };
	float BlindnessEffectRecoveryScale{ 0.0f };
	float ConcussionEffectRecoveryScale{ 0.0f };
	float DamageEffectRecoveryScale{ 0.0f };

	void Initialize();
	void CameraEffectsTrace();
	void SetRadialBlur();
	void SetDepthOfField();
};
