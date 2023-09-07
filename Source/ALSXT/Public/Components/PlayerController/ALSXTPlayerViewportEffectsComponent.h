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

	//FUNCTIONS
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void AddSuppressionEffect(float Amount);

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	void ClearSuppressionEffect();

private:

	// PROPERTIES
	// Timers
	FTimerHandle DOFTraceTimer;
	FTimerHandle RadialBlurEffectTimer;
	FTimerHandle SuppressionEffectFadeOutTimer;
	FTimerHandle BlindnessEffectFadeOutTimer;
	FTimerHandle ConcussionEffectFadeOutTimer;
	FTimerHandle DamageEffectFadeOutTimer;
	FTimerHandle DrunkEffectFadeOutTimer;
	FTimerHandle HighEffectFadeOutTimer;

	// Blendable Indexes
	int32 RadialBlurEffectBlendableIndex;
	int32 SuppressionEffectBlendableIndex;
	int32 DrunkEffectBlendableIndex;
	int32 HighEffectBlendableIndex;
	int32 BlindnessEffectBlendableIndex;
	int32 ConcussionEffectBlendableIndex;
	int32 DeathEffectBlendableIndex;

	// Recovery Scales
	float SuppressionEffectRecoveryScale{ 0.0f };
	float BlindnessEffectRecoveryScale{ 0.0f };
	float ConcussionEffectRecoveryScale{ 0.0f };
	float DamageEffectRecoveryScale{ 0.0f };
	float DrunkEffectRecoveryScale{ 0.0f };
	float HighEffectRecoveryScale{ 0.0f };

};
