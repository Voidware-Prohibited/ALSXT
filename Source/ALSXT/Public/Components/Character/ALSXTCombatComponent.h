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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Als Character Example", Meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> PrimaryAction;

	virtual void InputPrimaryAction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	UAlsCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	FTargetHitResultEntry CurrentTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FLinearColor HighlightColor { 1.0f, 0.0f, 1.0f, 1.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FName HighlightMaterialParameterName { "Highlight" };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FVector	TraceAreaHalfSize { 400.0f, 400.0f, 150.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (Units = "cm", AllowPrivateAccess))
	float MaxInitialLockDistance { 1000.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (Units = "cm", AllowPrivateAccess))
	float MaxLockDistance { 1000.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	bool UnlockWhenTargetIsObstructed { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstructionTraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	bool DebugMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	float DebugDuration { 4.0f };

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALSXT|Target Lock")
	void GetTargetableOverlayModes(TArray<FGameplayTag>& TargetableOverlayModes) const;

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	float GetAngle(FVector Target);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Category = "ALS|Als Character")
	void TryTraceForTargets();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void TraceForTargets(TArray<FTargetHitResultEntry>& Targets);

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	bool IsTartgetObstructed();

	UFUNCTION(BlueprintImplementableEvent, Category = "ALSXT|Target Lock")
	void OnTargetObstructed();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void GetClosestTarget();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void SetCurrentTarget(const FTargetHitResultEntry& NewTarget);

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void ClearCurrentTarget();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void DisengageAllTargets();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void GetTargetLeft();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void GetTargetRight();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void RotatePlayerToTarget(FTargetHitResultEntry Target);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanAttack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FALSXTCombatState CombatState;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void Attack(const FGameplayTag& AttackType, const FGameplayTag& Strength, float BaseDamage, float PlayRate = 1.0f);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	UAnimMontage* SelectAttackMontage(const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, float BaseDamage);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	UALSXTCombatSettings* SelectAttackSettings();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnAttackStarted(const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float& BaseDamage);

	// Desired Attack

private:
	bool IsAttackAllowedToStart(const UAnimMontage* Montage) const;
	
	void StartAttack(const FGameplayTag& AttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, float BaseDamage, float PlayRate, float TargetYawAngle);

	FTimerHandle TargetTraceTimerHandle;
	FTimerDelegate TargetTraceTimerDelegate;

	TArray<UMaterialInstanceDynamic*> TargetDynamicMaterials;

	UFUNCTION(Server, Reliable)
	void ServerStartAttack(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartAttack(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	void StartAttackImplementation(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	void RefreshAttack(float DeltaTime);

	void RefreshAttackPhysics(float DeltaTime);

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void StopAttack();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnAttackEnded();
};
