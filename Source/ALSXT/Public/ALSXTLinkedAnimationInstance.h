// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ALSXTCharacter.h"
#include "AlsLinkedAnimationInstance.h"
#include "ALSXTLinkedAnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UALSXTLinkedAnimationInstance : public UAlsLinkedAnimationInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		TObjectPtr<UALSXTAnimationInstance> ALSXTParent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
		TObjectPtr<AALSXTCharacter> ALSXTCharacter;
	
public:
	UALSXTLinkedAnimationInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

protected:
	// Be very careful when using this function to read your custom variables using the property access system. It is
	// safe to use this function to read variables that changes only inside UAlsAnimationInstance::NativeUpdateAnimation()
	// because it is guaranteed that this function will be called before parallel animation evaluation. Reading variables
	// that changes in other functions can be dangerous because the can be changed in the game thread at the same
	// time as being read in the worker thread, which can lead to undefined behavior or even a crash. If you're not
	// sure what you're doing, then it's better to access your custom variables through the "Parent" variable.
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Linked Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	UALSXTAnimationInstance* GetALSXTParentUnsafe() const;
};

inline UALSXTAnimationInstance* UALSXTLinkedAnimationInstance::GetALSXTParentUnsafe() const
{
	return ALSXTParent;
}
