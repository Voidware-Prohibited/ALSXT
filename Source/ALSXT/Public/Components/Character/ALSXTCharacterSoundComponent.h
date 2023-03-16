// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NativeGameplayTags.h"
#include "Components/AudioComponent.h"
#include "ALSXTCharacter.h"
#include "AlsCharacter.h"
#include "Utility/ALSXTStructs.h"
#include "Settings/ALSXTCharacterSoundSettings.h"
#include "ALSXTCharacterSoundComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UALSXTCharacterSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSXTCharacterSoundComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Als Character")
	UALSXTCharacterSoundSettings* SelectCharacterSoundSettings();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Als Character")
	bool ShouldPlayDeathSoundModeration();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FName VoiceSocketName { "head" };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FVector2D ActionSoundDelay { 1.0f, 2.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FVector2D AttackSoundDelay { 1.0f, 2.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	FVector2D DamageSoundDelay { 1.0f, 2.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	bool DebugMode { false };

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool CanPlayActionSound();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool CanPlayAttackSound();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action Sound")
	bool CanPlayDamageSound();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Vitals")
	float GetStamina();

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FGameplayTag ConvertStaminaToStaminaTag(const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTCharacterActionSound SelectActionSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTCharacterActionSound SelectAttackSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTCharacterDamageSound SelectDamageSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Overlay, const FGameplayTag& Form, const float Damage);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	FALSXTCharacterDamageSound SelectDeathSound(UALSXTCharacterSoundSettings* Settings, const FGameplayTag& Overlay, const FGameplayTag& Form, const float Damage);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	void PlayActionSound(const FGameplayTag& Overlay, const FGameplayTag& Strength, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	void PlayAttackSound(const FGameplayTag& Overlay, const FGameplayTag& Strength, const FGameplayTag& AttackMode, const float Stamina);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	void PlayDamageSound(const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);

	UFUNCTION(BlueprintCallable, Category = "Action Sound")
	void PlayDeathSound(const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);

private:
	FTimerHandle TimeSinceLastActionSoundTimer;
	float TimeSinceLastActionSound;
	FTimerHandle TimeSinceLastAttackSoundTimer;
	float TimeSinceLastAttackSound;
	FTimerHandle TimeSinceLastDamageSoundTimer;
	float TimeSinceLastDamageSound;
	float TargetActionSoundDelay{ 1.0f };
	float CurrentActionSoundDelay{ 10.0f };
	float TargetAttackSoundDelay{ 1.0f };
	float CurrentAttackSoundDelay{ 10.0f };
	float TargetDamageSoundDelay{ 1.0f };
	float CurrentDamageSoundDelay{ 10.0f };
	FALSXTCharacterActionSound LastCharacterActionSound;
	FALSXTCharacterActionSound LastCharacterAttackSound;
	FALSXTCharacterDamageSound LastCharacterDamageSound;

	bool ShouldPlayActionSound(const FGameplayTag& Strength, const float Stamina);
	bool ShouldPlayAttackSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const float Stamina);
	bool ShouldPlayDamageSound(const FGameplayTag& AttackMethod, const FGameplayTag& Strength, const FGameplayTag& AttackForm, const float Damage);	
	void StartTimeSinceLastActionSoundTimer(const float Delay);
	void IncrementTimeSinceLastActionSound();
	void ResetTimeSinceLastActionSoundTimer();
	void StartTimeSinceLastAttackSoundTimer(const float Delay);
	void IncrementTimeSinceLastAttackSound();
	void ResetTimeSinceLastAttackSoundTimer();
	void StartTimeSinceLastDamageSoundTimer(const float Delay);
	void IncrementTimeSinceLastDamageSound();
	void ResetTimeSinceLastDamageSoundTimer();
	void PlaySound(FALSXTCharacterSound Sound);

	UFUNCTION(Server, Reliable)
	void ServerPlaySound(FALSXTCharacterSound Sound);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySound(FALSXTCharacterSound Sound);
};
