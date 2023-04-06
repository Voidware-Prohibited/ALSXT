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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	UPARAM(meta = (Categories = "Als.Impact Form")) FGameplayTag GetCurrentBumpForm();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTGeneralImpactReactionSettings ImpactReactionSettings;

	FALSXTImpactReactionParameters ImpactReactionParameters;

	void ObstacleTrace();

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTImpactReactionState& GetImpactReactionState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewImpactReactionState"))
	void SetImpactReactionState(const FALSXTImpactReactionState& NewImpactReactionState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewImpactReactionState"))
	FALSXTImpactReactionState ProcessNewImpactReactionState(const FALSXTImpactReactionState& NewImpactReactionState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewImpactReactionState(const FALSXTImpactReactionState& NewImpactReactionState);

private:
	UFUNCTION(Server, Unreliable)
	void ServerSetImpactReactionState(const FALSXTImpactReactionState& NewImpactReactionState);

	UFUNCTION()
	void OnReplicate_ImpactReactionState(const FALSXTImpactReactionState& PreviousImpactReactionState);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	bool ShouldRecieveVelocityDamage();

	UFUNCTION(BlueprintCallable, Category = "Vitals")
	float GetBaseVelocityDamage();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnImpactReactionStateChanged(const FALSXTImpactReactionState& PreviousImpactReactionState);

public:
	/*Curve float reference*/
	UPROPERTY(EditAnywhere, Category = "Impact Reaction Timeline")
	UCurveFloat* CurveFloat;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetStamina();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	FGameplayTag GetCharacterVelocity();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanReact();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanPerformAnticipationReaction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanPerformDefensiveReaction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanGraspImpactPoint();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldPerformAnticipationReaction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldPerformDefensiveReaction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldGraspImpactPoint();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldFall();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldPerformResponse();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	UNiagaraSystem* GetImpactReactionParticle(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	UNiagaraSystem* GetBodyFallParticle(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	TSubclassOf<AActor> GetImpactReactionParticleActor(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	USoundBase* GetImpactReactionSound(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	USoundBase* GetBodyFallSound(FDoubleHitResult Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTImpactReactionState ImpactReactionState;

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void AnticipationReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void SyncedAnticipationReaction(FVector AnticipationPoint);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void DefensiveReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void CrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void BumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void ImpactReaction(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void AttackReaction(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void SyncedAttackReaction(int Index);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void ImpactFall();

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void SyncedAttackFall();

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void BumpFall();

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void BodyFallReaction(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void GetUp();

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void SyncedAttackGetUp();

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void AttackResponse();

protected:
	// Parameters
	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FBumpReactionAnimation SelectBumpReactionMontage(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FBumpReactionAnimation SelectCrowdNavigationReactionMontage(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FAnticipationPose SelectAnticipationMontage(const FGameplayTag& Strength, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FAnticipationPose SelectDefensiveMontage(const FGameplayTag& Strength, const FGameplayTag& Side, const FGameplayTag& Form, const FGameplayTag& Health);	

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FAttackReactionAnimation SelectAttackReactionMontage(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FImpactReactionAnimation SelectImpactReactionMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FSyncedAttackAnimation GetSyncedMontage(int Index);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FImpactReactionAnimation SelectClaspImpactPointMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FAnticipationPose SelectSteadyMontage(const FGameplayTag& Side);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FActionMontageInfo SelectImpactFallMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FActionMontageInfo SelectAttackFallMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectImpactFallenPose(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectAttackFallenPose(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FActionMontageInfo SelectImpactGetUpMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FActionMontageInfo SelectAttackGetUpMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FResponseAnimation SelectImpactResponseMontage(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	FResponseAnimation SelectAttackResponseMontage(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UALSXTImpactReactionSettings* SelectImpactReactionSettings();

	// Settings
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldSpawnParticleActor(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldSpawnParticleActorModeration(FDoubleHitResult Hit);

	// Main Sequence

	void SyncedReaction();

	void Fall();

	void FallLand();

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

	FImpactReactionAnimation LastImpactReactionAnimation;
	FAttackReactionAnimation LastAttackReactionAnimation;
	FSyncedAttackAnimation LastSyncedAttackReactionAnimation;

	UFUNCTION()
	void ImpactTimelineUpdate(float Value);

	bool IsImpactReactionAllowedToStart(const UAnimMontage* Montage) const;

	void StartBumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	void StartCrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	void StartAnticipationReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint);

	void StartDefensiveReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint);

	void StartImpactReaction(FDoubleHitResult Hit);
	
	void StartAttackReaction(FAttackDoubleHitResult Hit);

	void StartSyncedAttackReaction(int32 Index);

	void StartClutchImpactPoint(FDoubleHitResult Hit);

	void StartImpactFall(FDoubleHitResult Hit);

	void StartAttackFall(FAttackDoubleHitResult Hit);

	void StartBraceForImpact();

	void StartImpactGetUp(FDoubleHitResult Hit);

	void StartAttackGetUp(FAttackDoubleHitResult Hit);

	void StartImpactResponse(FDoubleHitResult Hit);

	void StartAttackResponse(FAttackDoubleHitResult Hit);

	UFUNCTION(Server, Reliable)
	void ServerBumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastBumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(Server, Reliable)
	void ServerCrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(Server, Reliable)
	void ServerAnticipationReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAnticipationReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint);

	UFUNCTION(Server, Reliable)
	void ServerDefensiveReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDefensiveReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint);

	UFUNCTION(Server, Reliable)
	void ServerImpactReaction(FDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastImpactReaction(FDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttackReaction(FAttackDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackReaction(FAttackDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSyncedAttackReaction(int32 Index);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSyncedAttackReaction(int32 Index);

	UFUNCTION(Server, Reliable, WithValidation)
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

inline const FALSXTImpactReactionState& UALSXTImpactReactionComponent::GetImpactReactionState() const
{
	return ImpactReactionState;
}