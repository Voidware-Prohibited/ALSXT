// Fill out your copyright notice in the Description page of Project Settings.


#include "ALSXTLinkedAnimationInstance.h"
#include "ALSXTCharacter.h"
#include "Utility/AlsMacros.h"

UALSXTLinkedAnimationInstance::UALSXTLinkedAnimationInstance()
{
	RootMotionMode = ERootMotionMode::IgnoreRootMotion;
	bUseMainInstanceMontageEvaluationData = true;
}

void UALSXTLinkedAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ALSXTParent = Cast<UALSXTAnimationInstance>(GetSkelMeshComponent()->GetAnimInstance());
	ALSXTCharacter = Cast<AALSXTCharacter>(GetOwningActor());

#if WITH_EDITOR
	if (!GetWorld()->IsGameWorld())
	{
		// Use default objects for editor preview.

		if (!IsValid(ALSXTParent))
		{
			ALSXTParent = GetMutableDefault<UALSXTAnimationInstance>();
		}

		if (!IsValid(ALSXTCharacter))
		{
			ALSXTCharacter = GetMutableDefault<AALSXTCharacter>();
		}
	}
#endif
}

void UALSXTLinkedAnimationInstance::NativeBeginPlay()
{
	ALS_ENSURE_MESSAGE(IsValid(ALSXTParent),
					TEXT("%s (%s) should only be used as a linked animation instance within the %s animation blueprint!"),
					ALS_GET_TYPE_STRING(UALSXTLinkedAnimationInstance), *GetClass()->GetName(),
					ALS_GET_TYPE_STRING(UALSXTAnimationInstance));

	Super::NativeBeginPlay();
}