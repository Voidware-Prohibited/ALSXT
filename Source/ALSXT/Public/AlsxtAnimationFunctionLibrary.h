// MIT

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "AlsxtAnimationFunctionLibrary.generated.h"

struct FAnimationStateMachineReference;
struct FAnimUpdateContext;
/**
 * 
 */
UCLASS(Experimental)
class ALSXT_API UAlsxtAnimationFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "State Machine", meta=(BlueprintThreadSafe, DefaultToSelf = "Node", AutoCreateRefTerm = "UpdateContext, Node", AdvancedDisplay = "4"))
	static void AlsxtRefreshStandingMovement(const FAnimUpdateContext& UpdateContext, const FAnimationStateMachineReference& Node);

	UFUNCTION(BlueprintCallable, Category = "State Machine", meta=(BlueprintThreadSafe, DefaultToSelf = "Node", AutoCreateRefTerm = "UpdateContext, Node", AdvancedDisplay = "4"))
	static void AlsxtRefreshCrouchingMovement(const FAnimUpdateContext& UpdateContext, const FAnimationStateMachineReference& Node);

	UFUNCTION(BlueprintCallable, Category = "State Machine", meta=(BlueprintThreadSafe, DefaultToSelf = "Node", AutoCreateRefTerm = "UpdateContext, Node", AdvancedDisplay = "4"))
	static void AlsxtRefreshDynamicTransitions(const FAnimUpdateContext& UpdateContext, const FAnimationStateMachineReference& Node);
};
