// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "State/AlsxtFootstepState.h"
#include "AlsxtFootprintComponent.generated.h"


class UTimelineComponent;
enum class EAlsFootBone : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UAlsxtFootprintComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAlsxtFootprintComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Als Character")
	void OnFootprintsStateChanged(const FAlsxtFootprintsState& PreviousFootprintsState);

	UFUNCTION(BlueprintCallable, Category = "Settings|Als Character|Footstep State", Meta = (AutoCreateRefTerm = "Foot"))
	void ClientSetFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State", Meta = (AutoCreateRefTerm = "Foot"))
	void MulticastSetFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState);

	UFUNCTION(BlueprintCallable, Category = "Settings|Als Character|Footstep State", Meta = (ForceAsFunction, AutoCreateRefTerm = "Foot"))
	void UpdateFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintStatePhase& Target, const float& Alpha);

	UFUNCTION(BlueprintCallable, Category = "ALS|State|Footstep", Meta = (AutoCreateRefTerm = "Foot"))
	void StartFootSaturation(const EAlsFootBone& Foot, const FAlsxtFootprintStatePhase& TargetState);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Settings|Als Character|Footstep State", Meta = (AutoCreateRefTerm = "Foot"))
	void ResetFootSaturationTimeline(const EAlsFootBone& Foot);

	UFUNCTION(BlueprintCallable, Category = "ALS|State|Footstep", Meta = (AutoCreateRefTerm = "Foot"))
	UTimelineComponent* GetFootTimeline(const EAlsFootBone& Foot);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Als Character|Footstep State", ReplicatedUsing = "OnReplicate_FootprintsState", Meta = (AllowPrivateAccess))
	FAlsxtFootprintsState FootprintsState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* LeftFootSaturationTimelineCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* RightFootSaturationTimelineCurve;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	FAlsxtFootwearDetails GetFootwearDetails() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Movement System")
	const FAlsxtFootprintsState& GetFootprintsState() const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFootprintsState"))
	void SetFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState);

	UFUNCTION(BlueprintCallable, Category = "ALS|Als Character", Meta = (AutoCreateRefTerm = "NewFootprintsState"))
	FAlsxtFootprintsState ProcessNewFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState);

	UFUNCTION(Server, Reliable)
	void ServerProcessNewFootprintsState(const EAlsFootBone& Foot, const FAlsxtFootprintsState& NewFootprintsState);

	UFUNCTION(BlueprintCallable, Category = "Settings|Als Character|Footstep State", Meta = (ForceAsFunction, AutoCreateRefTerm = "Foot"))
	void SetFootprintNewSurface(const FAlsxtFootprintsState& UpdatedTargetState, const FAlsxtFootprintsState& NewFootprintsState, const EAlsFootBone& Foot);
		
private:
	UFUNCTION()
	void OnReplicate_FootprintsState(const FAlsxtFootprintsState& PreviousFootprintsState);

	UTimelineComponent* LeftFootSaturationTimeline;
	UTimelineComponent* RightFootSaturationTimeline;
};
