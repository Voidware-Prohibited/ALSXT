#pragma once

#include "NativeGameplayTags.h"
#include "Utility/ALSXTStructs.h"
#include "State/ALSXTImpactReactionState.h"
#include "Settings/ALSXTElementalInteractionSettings.h"
#include "ALSXTCollisionInterface.generated.h"

UINTERFACE(Blueprintable)
class UALSXTCollisionInterface : public UInterface {
	GENERATED_BODY()
};

class ALSXT_API IALSXTCollisionInterface {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Settings")
	UALSXTImpactReactionSettings* SelectImpactReactionSettings();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Settings")
	UALSXTElementalInteractionSettings* SelectElementalInteractionSettings();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Parameters")
	void GetActorMass(float& Mass);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Parameters")
	void GetActorVelocity(FVector& Velocity);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Parameters")
	void GetActorThreatPoint(FVector& ThreatPoint);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Collision Interface|Parameters")
	UPARAM(meta = (Categories = "Als.Impact Form")) FGameplayTag GetCurrentBumpForm();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Parameters")
	FALSXTImpactReactionState GetImpactReactionState();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Parameters")
	void GetAnticipationInfo(FGameplayTag& Velocity, FGameplayTag& Form, FVector& AnticipationPoint);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Parameters")
	void GetActorPhysicsStates(FGameplayTag& PhysicalMaterial, const TArray<FGameplayTag>& PhysicalStates);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Parameters")
	void GetActorPhysicsDamageInfo(FGameplayTag& DamageType, float& PhysicsBaseDamage, float& PhysicsDamageFrequency);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|State")
	bool Blocking();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanReact();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanClutchImpactPoint();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanGetUp();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanAttackFall();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanReceiveImpulse();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanReceiveSpecialAttacks();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanReceiveUniqueAttacks();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanBeGrappled();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanBeThrown();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanBeRagdolled();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanBeTakenDown();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool CanBeKnockedDown();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool WillDamageKnockDownPlayer(float Damage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool WillDamageKillPlayer(float Damage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldSpawnParticleActor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldSpawnParticleActorModeration();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldClutchImpactPoint();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldStabilize();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldImpactFall();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldAttackFall();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldSyncedAttackFall();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldPerformAttackResponse();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldGetUp();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldPerformCrowdNavigationResponse();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldPerformCrowdNavigationReaction();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldCrowdNavigationFall();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldPerformImpactResponse();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Criteria")
	bool ShouldPerformDefensiveReaction();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Events")
	void PerformSurfaceInteraction(const FGameplayTag& PhysicalMaterial);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Events")
	void ActorBumpCollision(FDoubleHitResult Hit);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Hooks")
	void OnActorAttackCollision(FAttackDoubleHitResult Hit);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Hooks")
	void OnActorImpactCollision(FDoubleHitResult Hit);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Hooks")
	FGameplayTag GetCharacterPhysicalAnimationMode();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Collision Interface|Hooks")
	void SetCharacterPhysicalAnimationMode(const FGameplayTag& PhysicalAnimationmode, FName BelowBoneName);
};