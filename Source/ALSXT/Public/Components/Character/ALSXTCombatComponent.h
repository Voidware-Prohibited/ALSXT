// MIT

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "InputAction.h"
#include "ALSXTCharacter.h"
#include "AlsCharacter.h"
#include "AlsCameraComponent.h"
#include "Components/ActorComponent.h"
#include "State/ALSXTCombatState.h"
#include "ALSXTCombatComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALSXT_API UALSXTCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UALSXTCombatComponent();

	UFUNCTION()
		virtual void SetupInputComponent(class UEnhancedInputComponent* PlayerInputComponent);
	//virtual void SetupPlayerInputComponent(UInputComponent* Input) override;

	UEnhancedInputComponent* EnhancedInputComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> PrimaryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> SecondaryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Input", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> BlockAction;


	virtual void InputPrimaryAction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	UPROPERTY(BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess))
	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };

	UPROPERTY(BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess))
	UAlsCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Lock", Meta = (AllowPrivateAccess))
	FTargetHitResultEntry CurrentTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTGeneralCombatSettings CombatSettings;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Target Lock")
	void GetTargetableOverlayModes(TArray<FGameplayTag>& TargetableOverlayModes) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetStamina();

	UFUNCTION(BlueprintCallable, Category = "Target Lock")
	float GetAngle(FVector Target);

	UFUNCTION(BlueprintCallable, Category = "Target Lock")
	void TryTraceForTargets();

	UFUNCTION(BlueprintCallable, Category = "Target Lock")
	void TraceForTargets(TArray<FTargetHitResultEntry>& Targets);

	UFUNCTION(BlueprintCallable, Category = "Target Lock")
	bool IsTartgetObstructed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Target Lock")
	void OnTargetObstructed();

	UFUNCTION(BlueprintCallable, Category = "Target Lock")
	void GetClosestTarget();

	UFUNCTION(BlueprintCallable, Category = "Target Lock")
	void SetCurrentTarget(const FTargetHitResultEntry& NewTarget);

	UFUNCTION(BlueprintCallable, Category = "Target Lock")
	void ClearCurrentTarget();

	UFUNCTION(BlueprintCallable, Category = "Target Lock")
	void DisengageAllTargets();

	UFUNCTION(BlueprintCallable, Category = "Target Lock")
	void GetTargetLeft();

	UFUNCTION(BlueprintCallable, Category = "Target Lock")
	void GetTargetRight();

	UFUNCTION(BlueprintCallable, Category = "Target Lock")
	void RotatePlayerToTarget(FTargetHitResultEntry Target);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanAttack();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanGrapple();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanBeGrappled();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanThrow();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanBeThrown();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanPerformTakedown();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanBeTakenDown();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanPerformSpecialAttack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FALSXTCombatState CombatState;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	AActor* TraceForPotentialAttackTarget(float Distance);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack(const FGameplayTag& AttackType, const FGameplayTag& Strength, float BaseDamage, float PlayRate = 1.0f);

	FALSXTCombatParameters CombatParameters;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	void DetermineAttackMethod(FGameplayTag& AttackMethod, const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float BaseDamage, const AActor* Target);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UAnimMontage* SelectAttackMontage(const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, float BaseDamage);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	void GetSyncedAttackMontageInfo(FSyncedActionMontageInfo& SyncedActionMontageInfo, const FGameplayTag& AttackType, int32 Index);

	UFUNCTION(BlueprintNativeEvent, Category = "Parameters")
	UALSXTCombatSettings* SelectAttackSettings();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Hooks")
	void OnAttackStarted(const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float& BaseDamage);

	// Desired Attack

private:
	bool IsAttackAllowedToStart(const UAnimMontage* Montage) const;
	
	void StartAttack(const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, float BaseDamage, float PlayRate, float TargetYawAngle);

	void StartSyncedAttack(const FGameplayTag& Overlay, const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float BaseDamage, const float PlayRate, const float TargetYawAngle, int Index);

	FTimerHandle TargetTraceTimerHandle;
	FTimerDelegate TargetTraceTimerDelegate;

	TArray<UMaterialInstanceDynamic*> TargetDynamicMaterials;

	FTimerHandle LastTargetsTimerHandle;

	TArray<FLastTargetEntry> LastSeenTargets;

	TArray<FLastTargetEntry> LastTargets;

	FTimerHandle TimeSinceLastBlockTimerHandle;
	float TimeSinceLastBlock;

	FTimerHandle ConsecutiveHitsTimerHandle;
	int ConsecutiveHits;

	UFUNCTION(Server, Reliable)
	void ServerStartAttack(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartAttack(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	void StartAttackImplementation(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	void RefreshAttack(float DeltaTime);

	void RefreshAttackPhysics(float DeltaTime);

public:
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopAttack();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Hooks")
	void OnAttackEnded();
};
