// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Settings/ALSXTIdleAnimationSettings.h"
#include "ALSXTIdleAnimationComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UALSXTIdleAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UALSXTIdleAnimationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameters", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPrivateAccess))
	FALSXTALSXTGeneralIdleAnimationSettings IdleAnimationSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (AllowPrivateAccess))
	float IdleCounterCurrent{ 0.0f };

	float IdleCounterTarget{ 0.0f };
	FTimerHandle IdleCounterTimerHandle;
	FTimerDelegate IdleCounterTimerDelegate;
	FTimerHandle CameraRotationTimerHandle;
	FTimerDelegate CameraRotationTimerDelegate;
	FVector CameraOffset{ FVector::ZeroVector };

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Parameters")
	UALSXTIdleAnimationSettings* SelectIdleSettings();

	void StartIdleCounterTimer();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void IdleCounterTimer();

	void ResetIdleCounterTimer();

	void StartCameraRotationTimer();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void CameraRotationTimer();

	void ResetCameraRotationTimer();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void StartIdle();

	UFUNCTION(BlueprintCallable, Category = "Parameters")
	void StopIdle();
};
