// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AlsCharacter.h"
#include "ALSXTCharacter.h"
#include "NativeGameplayTags.h"
#include "Settings/ALSXTGestureSettings.h"
#include "ALSXTGestureComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UALSXTGestureComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSXTGestureComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	TObjectPtr<UALSXTGestureSettings> GestureSettings;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Settings")
	bool CanGesture();
	
	// Desired Gesture

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Gesture", Meta = (AutoCreateRefTerm = "NewGestureTag"))
	void AddDesiredGesture(const FGameplayTag& Gesture, const FGameplayTag& GestureHand);

private:
	UFUNCTION(Server, Reliable)
	void ServerAddDesiredGesture(const FGameplayTag& Gesture, const FGameplayTag& GestureHand);

	// Emote

private:
	void AddGesture(const FGameplayTag& Gesture, const FGameplayTag& GestureHand);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Gesture")
	void OnGesture(const FGameplayTag& Gesture, const FGameplayTag& GestureHand);
};
