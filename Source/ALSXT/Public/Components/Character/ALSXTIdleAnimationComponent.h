// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Replicated, Category = "State", Meta = (AllowPrivateAccess))
	bool bIsIdle;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "State", Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<UAnimMontage>> PreviousMontages;

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	TArray<FIdleAnimation> SelectIdleAnimations(const FGameplayTag& Sex, const FGameplayTag& Stance, const FGameplayTag& Overlay, const FGameplayTag& Injury, const FGameplayTag& CombatStance);

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	UAnimMontage* GetNewIdleAnimation(TArray<FIdleAnimation> IdleAnimations);

	void SetNewAnimation(UAnimMontage* Animation, int NoRepeats);

	UFUNCTION(Server, Reliable)
	void ServerSetNewAnimation(UAnimMontage* Animation, int NoRepeats);

	void SetNewAnimationImplementation(UAnimMontage* Animation, int NoRepeats);

private:
	UFUNCTION(Client, Reliable)
	void ClientSetIdleState(const FALSXTIdleState& NewIdleState);

	UFUNCTION(Server, Reliable)
	void ServerSetIdleState(const FALSXTIdleState& NewIdleState);

	UFUNCTION()
	void OnReplicate_IdleState(const FALSXTIdleState& PreviousIdleState);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnIdleStateChanged(const FALSXTIdleState& PreviousIdleState);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Meta = (AllowPrivateAccess), Transient)
	FALSXTIdleState IdleState;

	// Breath State
	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTIdleState& GetIdleState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewIdleState"))
	void SetIdleState(const FALSXTIdleState& NewIdleState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewIdleState"))
	FALSXTIdleState ProcessNewIdleState(const FALSXTIdleState& NewIdleState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewIdleState(const FALSXTIdleState& NewIdleState);

	UPROPERTY(BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess))
	UAnimInstance* AnimInstance{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTALSXTGeneralIdleAnimationSettings IdleAnimationSettings;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess))
	FALSXTStatusState StatusState;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Replicated, Category = "State", Meta = (AllowPrivateAccess))
	UAnimMontage* CurrentIdleMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Replicated, Category = "State", Meta = (AllowPrivateAccess))
	float IdleCounterCurrent{ 0.0f };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Replicated, Category = "State", Meta = (AllowPrivateAccess))
	float CurrentTimeBetweenAnimations{ 0.0f };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Replicated, Category = "State", Meta = (AllowPrivateAccess))
	float TargetTimeBetweenAnimations{ 0.0f };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Replicated, Category = "State", Meta = (AllowPrivateAccess))
	float IdleCounterTarget{ 0.0f };

	FTimerHandle PreCountIdleCounterTimerHandle;
	FTimerHandle IdleCounterTimerHandle;
	FTimerDelegate IdleCounterTimerDelegate;
	FTimerHandle DelayBetweenAnimationsTimerHandle;
	FTimerDelegate DelayBetweenAnimationsTimerDelegate;
	FTimerHandle GazingCounterTimerHandle;
	FTimerDelegate GazingCounterTimerDelegate;
	FTimerHandle CameraRotationTimerHandle;
	FTimerDelegate CameraRotationTimerDelegate;
	FVector CameraOffset{ FVector::ZeroVector };

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Parameters")
	void SetIdleCounterTarget();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	bool IsPlayerIdle();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Parameters")
	void SetPlayerIdle(bool NewIdle);

	bool IsPlayerInputIdle();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Parameters")
	void ServerStartIdleCounterTimer();

	void StartIdleCounterTimer();

	void StartIdleCounterTimerImplementation();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void IdleCounterTimer();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void GazeCounterTimer();

	UFUNCTION(BlueprintCallable, Category = "Timerc")
	void ResetIdleCounterTimer();

	void StartCameraRotationTimer();

	void StartDelayBetweenAnimationsTimer(float InitialDelay);
	
	void DelayBetweenAnimationsTimer();

	void ResetDelayBetweenAnimationsTimer();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void CameraRotationTimer();

	void ResetCameraRotationTimer();

	UFUNCTION(Server, Reliable)
	void ServerStartIdle();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartIdle();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void StartIdle();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void StartIdleImplementation();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void StopIdle();
};

inline const FALSXTIdleState& UALSXTIdleAnimationComponent::GetIdleState() const
{
	return IdleState;
}