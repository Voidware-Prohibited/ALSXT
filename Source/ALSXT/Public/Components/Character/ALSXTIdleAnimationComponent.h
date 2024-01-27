// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSXTCharacter.h"
#include "Settings/ALSXTIdleAnimationSettings.h"
#include "State/ALSXTStatusState.h"
#include "ALSXTIdleAnimationComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UALSXTIdleAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSXTIdleAnimationComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FRotator PreviousControlRotation;
	bool bIsIdle;

	TArray<TObjectPtr<UAnimMontage>> PreviousMontages;

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	TArray<FIdleAnimation> SelectIdleAnimations(const FGameplayTag& Sex, const FGameplayTag& Stance, const FGameplayTag& Overlay, const FGameplayTag& Injury, const FGameplayTag& CombatStance);

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	UAnimMontage* GetNewIdleAnimation(TArray<FIdleAnimation> IdleAnimations);

	void SetNewAnimation(UAnimMontage* Animation, int NoRepeats);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTALSXTGeneralIdleAnimationSettings IdleAnimationSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess))
	FALSXTStatusState StatusState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Settings", Meta = (AllowPrivateAccess))
	UAnimMontage* CurrentIdleMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Settings", Meta = (AllowPrivateAccess))
	float IdleCounterCurrent{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Settings", Meta = (AllowPrivateAccess))
	float CurrentTimeBetweenAnimations{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Settings", Meta = (AllowPrivateAccess))
	float TargetTimeBetweenAnimations{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Settings", Meta = (AllowPrivateAccess))
	float IdleCounterTarget{ 0.0f };

	FTimerHandle IdleCounterTimerHandle;
	FTimerHandle DelayBetweenAnimationsTimerHandle;
	FTimerDelegate DelayBetweenAnimationsTimerDelegate;
	FTimerDelegate IdleCounterTimerDelegate;
	FTimerHandle CameraRotationTimerHandle;
	FTimerDelegate CameraRotationTimerDelegate;
	FVector CameraOffset{ FVector::ZeroVector };

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	bool ShouldIdle();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	UALSXTIdleAnimationSettings* SelectIdleSettings();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	bool IsPlayerIdle();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void SetPlayerIdle(bool NewIdle);

	bool IsPlayerInputIdle();

	void StartIdleCounterTimer();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void IdleCounterTimer();

	UFUNCTION(BlueprintCallable, Category = "Timerc")
	void ResetIdleCounterTimer();

	void StartCameraRotationTimer();

	void StartDelayBetweenAnimationsTimer(float InitialDelay);
	
	void DelayBetweenAnimationsTimer();

	void ResetDelayBetweenAnimationsTimer();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void CameraRotationTimer();

	void ResetCameraRotationTimer();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void StartIdle();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void StopIdle();
};
