// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Chaos/ChaosEngineInterface.h"
#include "ImpactReactionComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSXT_API UImpactReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UImpactReactionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly, Category = "ALS|Als Character", Meta = (AllowPrivateAccess))
	AALSXTCharacter* Character{ Cast<AALSXTCharacter>(GetOwner()) };

	AAlsCharacter* AlsCharacter{ Cast<AAlsCharacter>(GetOwner()) };

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ALS|Movement System")
	bool CanReact();

	// Desired BumpReaction

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Impact Reaction", Meta = (AutoCreateRefTerm = "NewBumpReactionTag"))
		void AddDesiredBumpReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType);

private:
	UFUNCTION(Server, Reliable)
		void ServerAddDesiredBumpReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType);

	// BumpReaction

private:
	void AddBumpReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Impact Reaction")
		void OnBumpReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType);

	// Desired HitReaction

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Impact Reaction", Meta = (AutoCreateRefTerm = "NewHitReactionTag"))
		void AddDesiredHitReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType);

private:
	UFUNCTION(Server, Reliable)
		void ServerAddDesiredHitReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType);

	// HitReaction

private:
	void AddHitReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "ALS|Impact Reaction")
		void OnHitReaction(const FGameplayTag& ImpactForm, const FGameplayTag& ImpactLocation, const FGameplayTag& ImpactSide, const FGameplayTag& ImpactStrength, EPhysicalSurface SurfaceType);

		
};
