// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NativeGameplayTags.h"
#include "ALSXTCharacter.h"
#include "AlsCharacter.h"
#include "Utility/ALSXTStructs.h"
#include "Engine/EngineTypes.h"
#include "Utility/ALSXTEnums.h"
#include "TargetLockComponent.generated.h"

class AAlsCharacter;
class AALSXTCharacter;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetLockComponent();

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

	UPROPERTY(BlueprintReadWrite, AdvancedDisplay, Category = "Target Lock", Meta = (AllowPrivateAccess))
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

	void TryTraceForTargets();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	float GetAngle(FVector Target);

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	bool IsTartgetObstructed();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void TraceForTargets(TArray<FTargetHitResultEntry>& Targets);

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

private:

	FTimerHandle TraceTimerHandle;
	FTimerDelegate TraceTimerDelegate;

};
