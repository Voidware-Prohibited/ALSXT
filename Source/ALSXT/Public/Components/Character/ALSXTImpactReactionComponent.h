// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utility/ALSXTStructs.h"
#include "ALSXTCharacter.h"
#include "AlsCharacter.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h" 
#include "Settings/ALSXTImpactReactionSettings.h"
#include "State/ALSXTImpactReactionState.h" 
#include "Components/TimelineComponent.h"
#include "ALSXTImpactReactionComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Physics), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UALSXTImpactReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSXTImpactReactionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTGeneralImpactReactionSettings ImpactReactionSettings;

	FALSXTImpactReactionParameters ImpactReactionParameters;

	void ObstacleTrace();

	/*Curve float reference*/
	UPROPERTY(EditAnywhere, Category = "Impact Reaction Timeline")
	UCurveFloat* CurveFloat;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetStamina();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanReact();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldFall();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldPerformResponse();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	UNiagaraSystem* GetImpactReactionParticle(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	TSubclassOf<AActor> GetImpactReactionParticleActor(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	USoundBase* GetImpactReactionSound(FDoubleHitResult Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTImpactReactionState ImpactReactionState;

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void BumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void ImpactReaction(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void AttackReaction(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void SyncedAttackReaction(FAttackDoubleHitResult Hit, const FGameplayTag& Overlay, const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const FGameplayTag& AttackMode, int Index);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void BodyFallReaction(FAttackDoubleHitResult Hit);

protected:
	// Parameters
	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectBumpReactionMontage(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FAnticipationPose SelectAnticipationMontage(const FGameplayTag& Strength, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FAnticipationPose SelectDefensiveMontage(const FGameplayTag& Strength, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health);	

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectAttackReactionMontage(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectImpactReactionMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FSyncedAttackAnimation GetSyncedMontage(const FGameplayTag& Overlay, FAttackDoubleHitResult Hit, const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const FGameplayTag& AttackMode, int Index);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectFallMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectFallenPose(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectGetUpMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectResponseMontage(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UALSXTImpactReactionSettings* SelectImpactReactionSettings(const FGameplayTag& Location);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UALSXTAttackReactionSettings* SelectAttackReactionSettings(const FGameplayTag& Location);

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	bool DebugMode{ false };


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldSpawnParticleActor(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldSpawnParticleActorModeration(FDoubleHitResult Hit);

	// Main Sequence

	void SyncedReaction();

	void Fall();

	void FallLand();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void GeUp();

	void AttackResponse();

	// Hooks 

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnSpawnParticleActor(const FDoubleHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnAttackReactionStarted(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnImpactReactionStarted(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnSyncedReactionStarted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnSyncedReactionEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnFallStarted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnFallLand();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnGetUpStarted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnGetUpEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnResponseStarted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnResponseEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnABumpReactionFinished(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnAttackReactionFinished(FAttackDoubleHitResult Hit, const FGameplayTag& AttackMode);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnImpactReactionFinished(FDoubleHitResult Hit);

	// Desired UnarmedAttack

private:
	FTimeline ImpactTimeline;

	FTimerHandle TimeSinceLastRecoveryTimerHandle;
	float TimeSinceLastRecovery;

	FTimerHandle TimeSinceLastResponseTimerHandle;
	float TimeSinceLastResponse;

	UFUNCTION()
	void ImpactTimelineUpdate(float Value);

	bool IsImpactReactionAllowedToStart(const UAnimMontage* Montage) const;

	void StartBumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	void StartAttackReaction(FAttackDoubleHitResult Hit);

	void StartSyncedAttackReaction(FAttackDoubleHitResult Hit);
	
	void StartImpactReaction(FDoubleHitResult Hit);

	void StartFall(FDoubleHitResult Hit);

	void StartGetUp(FDoubleHitResult Hit);

	void StartResponse(FAttackDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttackReaction(FAttackDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackReaction(FAttackDoubleHitResult Hit);

	UFUNCTION(Server, Reliable)
	void ServerImpactReaction(FDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastImpactReaction(FDoubleHitResult Hit);

	UFUNCTION(Server, Reliable)
	void ServerStartImpactReaction(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartImpactReaction(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	void StartImpactReactionImplementation(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawnParticleActor(FDoubleHitResult Hit, TSubclassOf<AActor> ParticleActor);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnParticleActor(FDoubleHitResult Hit, TSubclassOf<AActor> ParticleActor);

	void SpawnParticleActorImplementation(FDoubleHitResult Hit, TSubclassOf<AActor> ParticleActor);

	void RefreshImpactReaction(float DeltaTime);

	void RefreshImpactReactionPhysics(float DeltaTime);

public:
	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopImpactReaction();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Hooks")
	void OnImpactReactionEnded();

};
