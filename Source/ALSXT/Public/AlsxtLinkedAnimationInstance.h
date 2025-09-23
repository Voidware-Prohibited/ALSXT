// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlsxtCharacter.h"
#include "AlsLinkedAnimationInstance.h"
#include "Chooser.h"
#include "AlsxtLinkedAnimationInstance.generated.h"

class UAlsxtAnimationAssetsSettings;
/**
 * 
 */
UCLASS()
class ALSXT_API UAlsxtLinkedAnimationInstance : public UAlsLinkedAnimationInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	TObjectPtr<UAlsxtAnimationInstance> ALSXTParent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	TObjectPtr<AAlsxtCharacter> ALSXTCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess))
	FAlsxtAnimationInstanceAssets AnimationAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess))
	TObjectPtr<UChooserTable> ChooserTable;
	
public:
	UAlsxtLinkedAnimationInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, BlueprintCallable, Category = "Animation|Chooser")
	UChooserTable* GetChooserTable();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animation|Chooser")
	UAlsxtAnimationAssetsSettings* EvaluateChooserTable();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	void UpdateAnimationAssets();

protected:
	// Be very careful when using this function to read your custom variables using the property access system. It is
	// safe to use this function to read variables that change only inside UALSXTAnimationInstance::NativeUpdateAnimation()
	// because it is guaranteed that this function will be called before parallel animation evaluation. Reading
	// variables that change in other functions can be dangerous because they can be changed in the game thread
	// at the same time as being read in the worker thread, which can lead to undefined behavior or even a crash.
	UFUNCTION(BlueprintPure, Category = "ALSXT|Linked Animation Instance",
		Meta = (BlueprintThreadSafe, ReturnDisplayName = "Parent"))
	UAlsxtAnimationInstance* GetALSXTParent() const;
};

inline UAlsxtAnimationInstance* UAlsxtLinkedAnimationInstance::GetALSXTParent() const
{
	return ALSXTParent.Get();
}
