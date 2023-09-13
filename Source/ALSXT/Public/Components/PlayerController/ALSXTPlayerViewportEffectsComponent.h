// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSXTCharacter.h"
#include "Components/PostProcessComponent.h"
#include "Settings/ALSXTCameraEffectsSettings.h"
#include "ALSXTPlayerViewportEffectsComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UALSXTPlayerViewportEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSXTPlayerViewportEffectsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//PROPERTIES
	UPROPERTY(BlueprintReadOnly, Category = "ALSXT", Meta = (AllowPrivateAccess))
	APlayerController* PlayerController {Cast<APlayerController>(GetOwner())};

	UPROPERTY(BlueprintReadOnly, Category = "ALSXT", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character {nullptr};

	UPROPERTY(BlueprintReadOnly, Category = "ALSXT", Meta = (AllowPrivateAccess))
	UAlsCameraComponent* Camera {nullptr};

	UPROPERTY(BlueprintReadOnly, Category = "ALSXT", Meta = (AllowPrivateAccess))
	APlayerCameraManager* CameraManager {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTGeneralCameraEffectsSettings GeneralCameraEffectsSettings;
	
	UPROPERTY(BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	UPostProcessComponent* PostProcessComponent{ nullptr };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentRadialBlurAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentDrunkEffectAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentHighEffectAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentSuppressionAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentBlindnessEffectAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentConcussionEffectAmount{ 0.0f };

	UPROPERTY(BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentDamageEffectAmount{ 0.0f };

	//FUNCTIONS
	// Drunk Effect
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void AddDrunkEffect(float Amount, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void ResetDrunkEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutDrunkEffect(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutDrunkEffect();

	// High Effect
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void AddHighEffect(float Amount, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void ResetHighEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutHighEffect(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutHighEffect();

	// Suppression Effect
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void AddSuppressionEffect(float Amount, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void ResetSuppressionEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutSuppressionEffect(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutSuppressionEffect();

	// Blindness Effect
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void AddBlindnessEffect(float Amount, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void ResetBlindnessEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutBlindnessEffect(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutBlindnessEffect();

	// Concussion Effect
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void AddConcussionEffect(float Amount, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void ResetConcussionEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutConcussionEffect(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutConcussionEffect();


	// Damage Effect
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void AddDamageEffect(float Amount, float RecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void ResetDamageEffect();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void BeginFadeOutDamageEffect(float NewRecoveryScale, float NewRecoveryDelay);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void FadeOutDamageEffect();

	// Death Effect
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void AddDeathEffect(float Amount);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void ResetDeathEffect();


	// On Effect Changed
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnDrunkEffectChanged(float NewMagnitude);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnHighEffectChanged(float NewMagnitude);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnSuppressionEffectChanged(float NewMagnitude);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnBlindnessEffectChanged(float NewMagnitude);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnConcussionEffectChanged(float NewMagnitude);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	void OnDamageEffectChanged(float NewMagnitude);

private:

	// PROPERTIES
	// Timer Handles
	FTimerHandle DepthOfFieldTraceTimer;
	FTimerHandle RadialBlurEffectTimer;
	FTimerHandle DrunkEffectFadeOutTimer;
	FTimerHandle HighEffectFadeOutTimer;
	FTimerHandle SuppressionEffectFadeOutTimer;
	FTimerHandle BlindnessEffectFadeOutTimer;
	FTimerHandle ConcussionEffectFadeOutTimer;
	FTimerHandle DamageEffectFadeOutTimer;

	// Material Indexes
	int32 RadialBlurEffectBlendableIndex;
	int32 DrunkEffectBlendableIndex;
	int32 HighEffectBlendableIndex;
	int32 SuppressionEffectBlendableIndex;
	int32 BlindnessEffectBlendableIndex;
	int32 ConcussionEffectBlendableIndex;
	int32 DamageEffectBlendableIndex;
	int32 DeathEffectBlendableIndex;

	// Recovery (Fade-Out) Scales
	float RecoveryScale{ 0.0f };
	float DrunkEffectRecoveryScale{ 0.0f };
	float HighEffectRecoveryScale{ 0.0f };
	float SuppressionEffectRecoveryScale{ 0.0f };
	float BlindnessEffectRecoveryScale{ 0.0f };
	float ConcussionEffectRecoveryScale{ 0.0f };
	float DamageEffectRecoveryScale{ 0.0f };

	//FUNCTIONS
	// Depth Of Field
	void DepthOfFieldTrace();
	void SetRadialBlurEffect();

};
