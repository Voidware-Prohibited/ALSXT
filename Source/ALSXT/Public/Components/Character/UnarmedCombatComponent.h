// MIT

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "InputAction.h"
#include "ALSXTCharacter.h"
#include "AlsCharacter.h"
#include "AlsCameraComponent.h"
#include "Components/ActorComponent.h"
#include "State/ALSXTUnarmedCombatState.h"
#include "UnarmedCombatComponent.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALSXT_API UUnarmedCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUnarmedCombatComponent();

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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanUnarmedAttack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Meta = (AllowPrivateAccess))
	FALSXTUnarmedCombatState UnarmedCombatState;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void UnarmedAttack(const FGameplayTag& UnarmedAttackType, const FGameplayTag& Strength, float BaseDamage, float PlayRate = 1.0f);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	UAnimMontage* SelectUnarmedAttackMontage(const FGameplayTag& UnarmedAttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, float BaseDamage);

	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	UALSXTUnarmedCombatSettings* SelectUnarmedAttackSettings();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	void OnUnarmedAttackStarted(const FGameplayTag& UnarmedAttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, const float& BaseDamage);

	// Desired UnarmedAttack

private:
	bool IsUnarmedAttackAllowedToStart(const UAnimMontage* Montage) const;
	
	void StartUnarmedAttack(const FGameplayTag& UnarmedAttackType, const FGameplayTag& Stance, const FGameplayTag& Strength, float BaseDamage, float PlayRate, float TargetYawAngle);

	UFUNCTION(Server, Reliable)
	void ServerStartUnarmedAttack(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartUnarmedAttack(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	void StartUnarmedAttackImplementation(UAnimMontage* Montage, float PlayRate, float StartYawAngle, float TargetYawAngle);

	void RefreshUnarmedAttack(float DeltaTime);

	void RefreshUnarmedAttackPhysics(float DeltaTime);

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character")
	void StopUnarmedAttack();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Als Character")
	void OnUnarmedAttackEnded();
};
