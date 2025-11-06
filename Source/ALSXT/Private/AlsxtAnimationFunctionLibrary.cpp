// MIT

#include "AlsxtAnimationFunctionLibrary.h"

#include "AlsxtAnimationInstanceProxy.h"
#include "AlsxtLinkedAnimationInstance.h"
#include "CoreTypes.h"
#include "Runtime/AnimGraphRuntime/Public/AnimationStateMachineLibrary.h"
#include "Runtime/Engine/Classes/Animation/AnimExecutionContext.h"
#include "Runtime/Engine/Classes/Animation/AnimNode_StateMachine.h"


void UAlsxtAnimationFunctionLibrary::AlsxtRefreshStandingMovement(const FAnimUpdateContext& UpdateContext, const FAnimationStateMachineReference& Node)
{
	Node.CallAnimNodeFunction<FAnimNode_StateMachine>(
		TEXT("AlsxtRefreshStandingMovement"),
		[&UpdateContext](FAnimNode_StateMachine& StateMachineNode)
		{
			if (UAnimInstance* BaseAnimInstance = Cast<UAnimInstance>(UpdateContext.GetContext()->AnimInstanceProxy->GetAnimInstanceObject()))
			{
				if (UAlsxtLinkedAnimationInstance* AlsxtLinkedAnimInstance = Cast<UAlsxtLinkedAnimationInstance>(BaseAnimInstance))
				{
					AlsxtLinkedAnimInstance->AlsxtRefreshStandingMovement();
				}
				else
				{
					// The cast failed, the AnimInstance is not of the expected type
					UE_LOG(LogTemp, Warning, TEXT("Failed to cast AnimInstance to UAlsxtLinkedAnimationInstance!"));
				}
			}
		});
}

void UAlsxtAnimationFunctionLibrary::AlsxtRefreshCrouchingMovement(const FAnimUpdateContext& UpdateContext,	const FAnimationStateMachineReference& Node)
{
	Node.CallAnimNodeFunction<FAnimNode_StateMachine>(
		TEXT("AlsxtRefreshCrouchingMovement"),
		[&UpdateContext](FAnimNode_StateMachine& StateMachineNode)
		{
			if (UAnimInstance* BaseAnimInstance = Cast<UAnimInstance>(UpdateContext.GetContext()->AnimInstanceProxy->GetAnimInstanceObject()))
			{
				if (UAlsxtLinkedAnimationInstance* AlsxtLinkedAnimInstance = Cast<UAlsxtLinkedAnimationInstance>(BaseAnimInstance))
				{
					AlsxtLinkedAnimInstance->AlsxtRefreshCrouchingMovement();
				}
				else
				{
					// The cast failed, the AnimInstance is not of the expected type
					UE_LOG(LogTemp, Warning, TEXT("Failed to cast AnimInstance to UAlsxtLinkedAnimationInstance!"));
				}
			}
		});
}

void UAlsxtAnimationFunctionLibrary::AlsxtRefreshDynamicTransitions(const FAnimUpdateContext& UpdateContext,
	const FAnimationStateMachineReference& Node)
{
	Node.CallAnimNodeFunction<FAnimNode_StateMachine>(
		TEXT("AlsxtRefreshDynamicTransitions"),
		[&UpdateContext](FAnimNode_StateMachine& StateMachineNode)
		{
			if (UAnimInstance* BaseAnimInstance = Cast<UAnimInstance>(UpdateContext.GetContext()->AnimInstanceProxy->GetAnimInstanceObject()))
			{
				if (UAlsxtLinkedAnimationInstance* AlsxtLinkedAnimInstance = Cast<UAlsxtLinkedAnimationInstance>(BaseAnimInstance))
				{
					AlsxtLinkedAnimInstance->AlsxtRefreshDynamicTransitions();
				}
				else
				{
					// The cast failed, the AnimInstance is not of the expected type
					UE_LOG(LogTemp, Warning, TEXT("Failed to cast AnimInstance to UAlsxtLinkedAnimationInstance!"));
				}
			}
		});
}
