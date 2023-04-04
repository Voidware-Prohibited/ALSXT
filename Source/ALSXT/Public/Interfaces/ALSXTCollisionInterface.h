#pragma once

#include "NativeGameplayTags.h"
#include "Utility/ALSXTStructs.h"
#include "State/ALSXTImpactReactionState.h"
#include "ALSXTCollisionInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTCollisionInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTCollisionInterface {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface")
	void GetActorMass(float& Mass);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface")
	void GetActorVelocity(float& Velocity);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface")
	void GetActorThreatPoint(FVector& ThreatPoint);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Parameters")
	UPARAM(meta = (Categories = "Als.Impact Form")) FGameplayTag GetCurrentBumpForm();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface")
	bool CanReceiveImpulse();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Impact Reaction")
    void GetAnticipationInfo(const FGameplayTag& Form, FVector& AnticipationPoint);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface")
	void GetActorPhysicsStates(FGameplayTag& PhysicalMaterial, const TArray<FGameplayTag>& PhysicalStates);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface")
	void GetActorPhysicsDamageInfo(FGameplayTag& DamageType, float& PhysicsBaseDamage, float& PhysicsDamageFrequency);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface")
	void OnActorAttackCollision(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface")
	void OnActorImpactCollision(FDoubleHitResult Hit);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Combat")
    FALSXTImpactReactionState GetImpactReactionState();
};