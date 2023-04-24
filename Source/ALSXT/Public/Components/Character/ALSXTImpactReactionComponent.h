// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utility/ALSXTStructs.h"
#include "ALSXTCharacter.h"
#include "AlsCharacter.h"
#include "Animation/AnimInstance.h"
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
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	UPARAM(meta = (Categories = "Als.Impact Form")) FGameplayTag GetCurrentBumpForm();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	float GetDynamicImpactFallenMinimumTime();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	float GetDynamicAttackFallenMinimumTime();

	float GetImpactFallenMinimumTime();

	float GetAttackFallenMinimumTime();

	void StartClutchImpactPointTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void ClutchImpactPointTimer();

	void StartImpactFallenTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void ImpactFallenTimer();

	void StartAttackFallenTimer();

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void AttackFallenTimer();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess))
	UAnimInstance* AnimInstance{ nullptr };

	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };
	FALSXTImpactReactionParameters ImpactReactionParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTGeneralImpactReactionSettings ImpactReactionSettings;

	void ObstacleTrace();

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FALSXTImpactReactionState& GetImpactReactionState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewImpactReactionState"))
	void SetImpactReactionState(const FALSXTImpactReactionState& NewImpactReactionState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewImpactReactionState"))
	FALSXTImpactReactionState ProcessNewImpactReactionState(const FALSXTImpactReactionState& NewImpactReactionState);

	UFUNCTION(NetMulticast, Reliable)
	void ServerSetDefensiveModeState(const FALSXTDefensiveModeState& NewDefensiveModeState);

	UFUNCTION(Server, Unreliable)
	void ServerProcessNewImpactReactionState(const FALSXTImpactReactionState& NewImpactReactionState);

private:
	void AnticipationTrace();

	UFUNCTION(Server, Unreliable)
	void ServerSetImpactReactionState(const FALSXTImpactReactionState& NewImpactReactionState);

	UFUNCTION()
	void OnReplicate_ImpactReactionState(const FALSXTImpactReactionState& PreviousImpactReactionState);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	bool ShouldRecieveVelocityDamage();

	UFUNCTION(BlueprintCallable, Category = "Vitals")
	float GetBaseVelocityDamage();

	// BlendOut Delegates

	FOnMontageBlendingOutStarted OnCrowdNavigationReactionBlendOutDelegate;
	FOnMontageEnded OnCrowdNavigationReactionEndedDelegate;

	FOnMontageBlendingOutStarted OnBumpReactionBlendOutDelegate;
	FOnMontageEnded OnBumpReactionEndedDelegate;
	FOnMontageBlendingOutStarted OnImpactReactionBlendOutDelegate;
	FOnMontageEnded OnImpactReactionEndedDelegate;
	FOnMontageBlendingOutStarted OnAttackReactionBlendOutDelegate;
	FOnMontageEnded OnAttackReactionEndedDelegate;
	FOnMontageBlendingOutStarted OnSyncedAttackReactionBlendOutDelegate;
	FOnMontageEnded OnSyncedAttackReactionEndedDelegate;
	FOnMontageBlendingOutStarted OnClutchImpactPointBlendOutDelegate;
	FOnMontageEnded OnClutchImpactPointEndedDelegate;
	FOnMontageBlendingOutStarted OnBumpFallBlendOutDelegate;
	FOnMontageEnded OnBumpFallEndedDelegate;
	FOnMontageBlendingOutStarted OnImpactFallBlendOutDelegate;
	FOnMontageEnded OnImpactFallEndedDelegate;
	FOnMontageBlendingOutStarted OnAttackFallBlendOutDelegate;
	FOnMontageEnded OnAttackFallEndedDelegate;
	FOnMontageBlendingOutStarted OnSyncedAttackFallBlendOutDelegate;
	FOnMontageEnded OnSyncedAttackFallEndedDelegate;
	FOnMontageBlendingOutStarted OnBraceForImpactBlendOutDelegate;
	FOnMontageEnded OnBraceForImpactEndedDelegate;
	FOnMontageBlendingOutStarted OnBumpFallGetupBlendOutDelegate;
	FOnMontageEnded OnBumpFallGetupEndedDelegate;
	FOnMontageBlendingOutStarted OnImpactFallGetupBlendOutDelegate;
	FOnMontageEnded OnImpactFallGetupEndedDelegate;
	FOnMontageBlendingOutStarted OnAttackFallGetupBlendOutDelegate;
	FOnMontageEnded OnAttackFallGetupEndedDelegate;
	FOnMontageBlendingOutStarted OnSyncedAttackFallGetUpBlendOutDelegate;
	FOnMontageEnded OnSyncedAttackFallGetUpEndedDelegate;
	FOnMontageBlendingOutStarted OnImpactResponseBlendOutDelegate;
	FOnMontageEnded OnImpactResponseEndedDelegate;
	FOnMontageBlendingOutStarted OnAttackResponseBlendOutDelegate;
	FOnMontageEnded OnAttackResponseEndedDelegate;

	void OnCrowdNavigationReactionBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnBumpReactionBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnImpactReactionBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnAttackReactionBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnSyncedAttackReactionBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnClutchImpactPointBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnBumpFallBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnImpactFallBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnAttackFallBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnSyncedAttackFallBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnBraceForImpactBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnBumpFallGetupBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnImpactFallGetupBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnAttackFallGetupBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnSyncedAttackFallGetUpBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnImpactResponseBlendOut(UAnimMontage* Montage, bool bInterrupted);
	void OnAttackResponseBlendOut(UAnimMontage* Montage, bool bInterrupted);

	void OnCrowdNavigationReactionEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnBumpReactionEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnImpactReactionEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnAttackReactionEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnSyncedAttackReactionEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnClutchImpactPointEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnBumpFallEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnImpactFallEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnAttackFallEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnSyncedAttackFallEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnBraceForImpactEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnBumpFallGetupEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnImpactFallGetupEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnAttackFallGetupEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnSyncedAttackFallGetUpEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnImpactResponseEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnAttackResponseEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnImpactReactionStateChanged(const FALSXTImpactReactionState& PreviousImpactReactionState);

	// Settings
private:
	bool IsSyncedAnticipationReactionAllowedToStart(const UAnimMontage* Montage) const;
	bool IsDefensiveReactionAllowedToStart(const UAnimMontage* Montage) const;
	bool IsCrowdNavigationReactionAllowedToStart(const UAnimMontage* Montage) const;
	bool IsBumpReactionAllowedToStart(const UAnimMontage* Montage) const;
	bool IsImpactReactionAllowedToStart(const UAnimMontage* Montage) const;
	bool IsAttackReactionAllowedToStart(const UAnimMontage* Montage) const;
	bool IsSyncedAttackReactionAllowedToStart(const UAnimMontage* Montage) const;
	bool IsClutchImpactPointAllowedToStart(const UAnimSequenceBase* Montage) const;
	bool IsBumpFallAllowedToStart(const UAnimMontage* Montage) const;
	bool IsImpactFallAllowedToStart(const UAnimMontage* Montage) const;
	bool IsAttackFallAllowedToStart(const UAnimMontage* Montage) const;
	bool IsSyncedAttackFallAllowedToStart(const UAnimMontage* Montage) const;
	bool IsImpactResponseAllowedToStart(const UAnimMontage* Montage) const;
	bool IsAttackResponseAllowedToStart(const UAnimMontage* Montage) const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldSpawnParticleActor(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldSpawnParticleActorModeration(FDoubleHitResult Hit);

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanReact();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanPerformSyncedAnticipationReaction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanPerformDefensiveReaction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanClutchImpactPoint();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldPerformSyncedAnticipationReaction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldPerformDefensiveReaction();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldClutchImpactPoint();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldBumpFall();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldImpactFall();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldAttackFall();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldSyncedAttackFall();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldPerformImpactResponse();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool ShouldPerformAttackResponse();

	// Parameters
private:
	FTimeline ImpactTimeline;
	FImpactReactionAnimation LastImpactReactionAnimation;
	FAttackReactionAnimation LastAttackReactionAnimation;
	FSyncedAttackAnimation LastSyncedAttackReactionAnimation;
	FTimerHandle TimeSinceLastRecoveryTimerHandle;
	float TimeSinceLastRecovery;
	FTimerHandle TimeSinceLastResponseTimerHandle;
	float TimeSinceLastResponse;
	FTimerHandle ClutchImpactPointTimerHandle;
	FTimerDelegate ClutchImpactPointTimerDelegate;
	FTimerHandle ImpactFallenTimerHandle;
	FTimerDelegate ImpactFallenTimerDelegate;
	FTimerHandle AttackFallenTimerHandle;
	FTimerDelegate AttackFallenTimerDelegate;

	UFUNCTION()
	void ImpactTimelineUpdate(float Value);

protected:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	FGameplayTag HealthToHealthTag(float Health);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	FGameplayTag LocationToImpactSide(FVector Location);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FBumpReactionAnimation SelectBumpReactionMontage(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FBumpReactionAnimation SelectCrowdNavigationReactionMontage(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	UPARAM(meta = (Categories = "Als.Defensive Mode")) FGameplayTag DetermineDefensiveMode(const FGameplayTag& Form);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	UPARAM(meta = (Categories = "Als.Defensive Mode")) FGameplayTag DetermineDefensiveModeFromCharacter(const FGameplayTag& Form, const FGameplayTag& CombatStance);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	UPARAM(meta = (Categories = "Als.Defensive Mode")) FGameplayTag DetermineDefensiveModeFromAttackingCharacter(const FGameplayTag& Form, const FGameplayTag& CombatStance);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FAnticipationPose SelectImpactAnticipationMontage(UPARAM(meta = (Categories = "Als.Impact Velocity")) const FGameplayTag& Velocity, UPARAM(meta = (Categories = "Als.Stance")) const FGameplayTag& Stance, UPARAM(meta = (Categories = "Als.Impact Side")) const FGameplayTag& Side, UPARAM(meta = (Categories = "Als.Impact Form")) const FGameplayTag& Form, UPARAM(meta = (Categories = "Als.Health")) const FGameplayTag& Health);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FAnticipationPose SelectAttackAnticipationMontage(UPARAM(meta = (Categories = "Als.Combat Stance")) const FGameplayTag& CharacterCombatStance, UPARAM(meta = (Categories = "Als.Action Strength")) const FGameplayTag& Strength, UPARAM(meta = (Categories = "Als.Stance")) const FGameplayTag& Stance, UPARAM(meta = (Categories = "Als.Impact Side")) const FGameplayTag& Side, UPARAM(meta = (Categories = "Als.Impact Form")) const FGameplayTag& Form, UPARAM(meta = (Categories = "Als.Health")) const FGameplayTag& Health);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FAnticipationPose SelectDefensiveMontage(UPARAM(meta = (Categories = "Als.Action Strength")) const FGameplayTag& Strength, UPARAM(meta = (Categories = "Als.Stance")) const FGameplayTag& Stance, UPARAM(meta = (Categories = "Als.Impact Side")) const FGameplayTag& Side, UPARAM(meta = (Categories = "Als.Impact Form")) const FGameplayTag& Form, UPARAM(meta = (Categories = "Als.Health")) const FGameplayTag& Health);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FAttackReactionAnimation SelectAttackReactionMontage(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FImpactReactionAnimation SelectImpactReactionMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FSyncedAttackAnimation GetSyncedMontage(int Index);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FClutchImpactLocationAnimation SelectClutchImpactPointMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FAnticipationPose SelectSteadyMontage(const FGameplayTag& Side);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FFallenAnimation SelectImpactFallAnimations(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FFallenAnimation SelectAttackFallAnimations(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FActionMontageInfo SelectImpactFallMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FActionMontageInfo SelectAttackFallMontage(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectImpactFallenPose(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectAttackFallenPose(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FActionMontageInfo SelectImpactGetUpMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FActionMontageInfo SelectAttackGetUpMontage(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FResponseAnimation SelectImpactResponseMontage(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	FResponseAnimation SelectAttackResponseMontage(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	UALSXTImpactReactionSettings* SelectImpactReactionSettings();

public:
	UPROPERTY(EditAnywhere, Category = "Impact Reaction Timeline")
	UCurveFloat* CurveFloat;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetStamina();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	FGameplayTag GetCharacterVelocity();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	UNiagaraSystem* GetImpactReactionParticle(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	UNiagaraSystem* GetBodyFallParticle(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	TSubclassOf<AActor> GetImpactReactionParticleActor(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	FALSXTCharacterSound GetImpactReactionSound(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	FALSXTCharacterSound GetAttackReactionSound(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	FALSXTCharacterSound GetBodyFallSound(FDoubleHitResult Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTImpactReactionState ImpactReactionState;

	// Entry Events
public:
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
	void ClutchImpactPoint(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void BumpFall();
	
	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void ImpactFall(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void ImpactFallIdle(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void AttackFall(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void AttackFallIdle(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void SyncedAttackFall(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void SyncedAttackFallIdle(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void BraceForImpact();

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void ImpactGetUp(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void AttackGetUp(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void SyncedAttackGetUp(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void ImpactResponse(FDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void AttackResponse(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Impact Reaction")
	void BodyFallReaction(FDoubleHitResult Hit);

private:

	void StartBumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	void StartCrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	void StartSyncedAnticipationReaction(FVector AnticipationPoint);

	void StartDefensiveReaction(const FGameplayTag& Velocity, const FGameplayTag& Side, const FGameplayTag& Form, FVector AnticipationPoint);

	void StartImpactReaction(FDoubleHitResult Hit);
	
	void StartAttackReaction(FAttackDoubleHitResult Hit);

	void StartSyncedAttackReaction(int32 Index);

	void StartClutchImpactPoint(FDoubleHitResult Hit);

	void StartImpactFall(FDoubleHitResult Hit);

	void StartImpactFallIdle(FDoubleHitResult Hit);

	void StartAttackFall(FAttackDoubleHitResult Hit);

	void StartAttackFallIdle(FAttackDoubleHitResult Hit);

	void StartSyncedAttackFall(int32 Index);

	void StartSyncedAttackFallIdle(int32 Index);

	void StartBraceForImpact();

	void StartImpactFallLand(FDoubleHitResult Hit);

	void StartAttackFallLand(FAttackDoubleHitResult Hit);

	void StartSyncedAttackFallLand(int32 Index);

	void StartImpactGetUp(FDoubleHitResult Hit);

	void StartAttackGetUp(FAttackDoubleHitResult Hit);

	void StartSyncedAttackGetUp(int32 Index);

	void StartImpactResponse(FDoubleHitResult Hit);

	void StartAttackResponse(FAttackDoubleHitResult Hit);

	// RPCs

	UFUNCTION(Server, Reliable)
	void ServerCrouch();

	UFUNCTION(Server, Reliable)
	void ServerGetUp();

	UFUNCTION(Server, Reliable)
	void ServerBumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastBumpReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(Server, Reliable)
	void ServerCrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCrowdNavigationReaction(const FGameplayTag& Gait, const FGameplayTag& Side, const FGameplayTag& Form);

	UFUNCTION(Server, Reliable)
	void ServerSyncedAnticipationReaction(FVector AnticipationPoint);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSyncedAnticipationReaction(FVector AnticipationPoint);

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
	void ServerClutchImpactPoint(FDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastClutchImpactPoint(FDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerImpactFall(FDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastImpactFall(FDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerImpactFallIdle(FDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastImpactFallIdle(FDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttackFall(FAttackDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackFall(FAttackDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttackFallIdle(FAttackDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackFallIdle(FAttackDoubleHitResult Hit);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSyncedAttackFall(int32 Index);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSyncedAttackFall(int32 Index);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSyncedAttackFallIdle(int32 Index);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSyncedAttackFallIdle(int32 Index);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBraceForImpact();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastBraceForImpact();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerImpactFallLand(FDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastImpactFallLand(FDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttackFallLand(FAttackDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackFallLand(FAttackDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSyncedAttackFallLand(int32 Index);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSyncedAttackFallLand(int32 Index);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerImpactGetUp(FDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastImpactGetUp(FDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttackGetUp(FAttackDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackGetUp(FAttackDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSyncedAttackGetUp(int32 Index);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSyncedAttackGetUp(int32 Index);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerImpactResponse(FDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastImpactResponse(FDoubleHitResult Hit);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttackResponse(FAttackDoubleHitResult Hit);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackResponse(FAttackDoubleHitResult Hit);

	// Start RPCs

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartBumpReaction(FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartBumpReaction(FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartCrowdNavigationReaction(FActionMontageInfo Montage, USoundBase* Audio);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartCrowdNavigationReaction(FActionMontageInfo Montage, USoundBase* Audio);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartSyncedAnticipationReaction(FVector AnticipationPoint);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartSyncedAnticipationReaction(FVector AnticipationPoint);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartDefensiveReaction(FActionMontageInfo Montage, USoundBase* Audio, FVector AnticipationPoint);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartDefensiveReaction(FActionMontageInfo Montage, USoundBase* Audio, FVector AnticipationPoint);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartImpactReaction(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartImpactReaction(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartAttackReaction(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartAttackReaction(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartSyncedAttackReaction(FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartSyncedAttackReaction(FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartClutchImpactPoint(UAnimSequenceBase* Pose, FVector ImpactPoint);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartClutchImpactPoint(UAnimSequenceBase* Pose, FVector ImpactPoint);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartImpactFall(FDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartImpactFall(FDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartImpactFallIdle(FDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartImpactFallIdle(FDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartAttackFall(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartAttackFall(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartAttackFallIdle(UAnimMontage* Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartAttackFallIdle(UAnimMontage* Montage);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartSyncedAttackFall(FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartSyncedAttackFall(FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartSyncedAttackFallIdle(FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartSyncedAttackFallIdle(FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartBraceForImpact(UAnimMontage* Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartBraceForImpact(UAnimMontage* Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartImpactFallLand(FDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartImpactFallLand(FDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartAttackFallLand(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartAttackFallLand(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartSyncedAttackFallLand(FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartSyncedAttackFallLand(FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartImpactGetUp(FDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartImpactGetUp(FDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartAttackGetUp(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartAttackGetUp(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartSyncedAttackGetUp(FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartSyncedAttackGetUp(FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartImpactResponse(FDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartImpactResponse(FDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartAttackResponse(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartAttackResponse(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	// Particle Actor RPCs

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawnParticleActor(FDoubleHitResult Hit, TSubclassOf<AActor> ParticleActor);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnParticleActor(FDoubleHitResult Hit, TSubclassOf<AActor> ParticleActor);

	// Implementations

	void StartSyncedAnticipationReactionImplementation(FActionMontageInfo Montage, FVector AnticipationPoint);

	void StartDefensiveReactionImplementation(FActionMontageInfo Montage, USoundBase* Audio, FVector AnticipationPoint);

	void StartCrowdNavigationReactionImplementation(FActionMontageInfo Montage, USoundBase* Audio);

	void StartBumpReactionImplementation(FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	void StartImpactReactionImplementation(FDoubleHitResult Hit, UAnimMontage* Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	void StartAttackReactionImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage, TSubclassOf<AActor> ParticleActor, UNiagaraSystem* Particle, USoundBase* Audio);

	void StartSyncedAttackReactionImplementation(FActionMontageInfo Montage);

	void StartClutchImpactPointImplementation(UAnimSequenceBase* Montage, FVector ImpactPoint);

	void StartImpactFallImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage);

	void StartImpactFallIdleImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage);

	void StartAttackFallImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	void StartAttackFallIdleImplementation(UAnimMontage* Montage);

	void StartSyncedAttackFallImplementation(FActionMontageInfo Montage);

	void StartSyncedAttackFallIdleImplementation(FActionMontageInfo Montage);

	void StartBraceForImpactImplementation(UAnimMontage* Montage);

	void StartImpactFallLandImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage);

	void StartAttackFallLandImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	void StartSyncedAttackFallLandImplementation(FActionMontageInfo Montage);

	void StartImpactGetUpImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage);

	void StartAttackGetUpImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	void StartSyncedAttackGetUpImplementation(FActionMontageInfo Montage);

	void StartImpactResponseImplementation(FDoubleHitResult Hit, FActionMontageInfo Montage);

	void StartAttackResponseImplementation(FAttackDoubleHitResult Hit, FActionMontageInfo Montage);

	void SpawnParticleActorImplementation(FDoubleHitResult Hit, TSubclassOf<AActor> ParticleActor);

	void RefreshSyncedAnticipationReaction(float DeltaTime);

	void RefreshSyncedAnticipationReactionPhysics(float DeltaTime);

	void RefreshDefensiveReaction(float DeltaTime);

	void RefreshDefensiveReactionPhysics(float DeltaTime);

	void RefreshCrowdNavigationReaction(float DeltaTime);

	void RefreshCrowdNavigationReactionPhysics(float DeltaTime);

	void RefreshBumpReaction(float DeltaTime);

	void RefreshBumpReactionPhysics(float DeltaTime);

	void RefreshImpactReaction(float DeltaTime);

	void RefreshImpactReactionPhysics(float DeltaTime);

	void RefreshAttackReaction(float DeltaTime);

	void RefreshAttackReactionPhysics(float DeltaTime);

	void RefreshSyncedAttackReaction(float DeltaTime);

	void RefreshSyncedAttackReactionPhysics(float DeltaTime);

	void RefreshBumpFallReaction(float DeltaTime);

	void RefreshBumpFallReactionPhysics(float DeltaTime);

	void RefreshImpactFallReaction(float DeltaTime);

	void RefreshImpactFallReactionPhysics(float DeltaTime);

	void RefreshAttackFallReaction(float DeltaTime);

	void RefreshAttackFallReactionPhysics(float DeltaTime);

	void RefreshSyncedAttackFallReaction(float DeltaTime);

	void RefreshSyncedAttackFallReactionPhysics(float DeltaTime);

public:
	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopSyncedAnticipationReaction();

	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopDefensiveReaction();

	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopCrowdNavigationReaction();

	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopBumpReaction();

	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopImpactReaction();

	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopAttackReaction();

	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopSyncedAttackReaction();

	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopBumpFallReaction();

	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopImpactFallReaction();

	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopAttackFallReaction();

	UFUNCTION(BlueprintCallable, Category = "Hooks")
	void StopSyncedAttackFallReaction();

protected:
	// Hooks 

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnSpawnParticleActor(const FDoubleHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnSyncedAnticipationReactionStarted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnSyncedAnticipationReactionEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnDefensiveReactionStarted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnDefensiveReactionEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnCrowdNavigationReactionStarted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnCrowdNavigationReactionEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnBumpReactionStarted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnBumpReactionEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnImpactReactionStarted(FDoubleHitResult Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Hooks")
	void OnImpactReactionEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnAttackReactionStarted(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnAttackReactionEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnSyncedAttackReactionStarted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnSyncedAttackReactionEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnFallStarted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnFallLand();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnBumpFallEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnImpactFallEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnAttackFallEnded();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnSyncedAttackFallEnded();

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

};

inline const FALSXTImpactReactionState& UALSXTImpactReactionComponent::GetImpactReactionState() const
{
	return ImpactReactionState;
}