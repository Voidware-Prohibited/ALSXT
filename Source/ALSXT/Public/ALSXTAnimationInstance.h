// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AlsAnimationInstance.h"
#include "Utility/ALSXTGameplayTags.h"
#include "ALSXTAnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UALSXTAnimationInstance : public UAlsAnimationInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	FGameplayTag Sex{ALSXTSexTags::Male};
	
};
