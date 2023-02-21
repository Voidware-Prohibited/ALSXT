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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	bool DebugMode;
	
	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	UAlsCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	FTargetHitResultEntry CurrentTarget;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALSXT|Target Lock")
	void GetTargetableOverlayModes(TArray<FGameplayTag>& TargetableOverlayModes) const;

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	float GetAngle(FVector Target);

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void TraceForTargets(bool DisplayDebug, float DebugDuration, TArray<FTargetHitResultEntry>& Targets);

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void GetClosestTarget();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void DisengageAllTargets();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void GetTargetLeft();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void GetTargetRight();

	UFUNCTION(BlueprintCallable, Category = "ALSXT|Target Lock")
	void RotatePlayerToTarget(FTargetHitResultEntry Target);

};
