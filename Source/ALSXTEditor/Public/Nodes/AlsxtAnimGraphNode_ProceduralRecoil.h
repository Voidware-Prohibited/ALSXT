// MIT

#pragma once

#include "AnimGraphNode_SkeletalControlBase.h"
#include "ALSXTEditor.h"
#include "AnimNodes/AlsxtAnimNode_ProceduralRecoil.h"
#include "AlsxtAnimGraphNode_ProceduralRecoil.generated.h"

UCLASS(meta=(Keywords = "Procedural Recoil"))
class ALSXTEDITOR_API UAlsxtAnimGraphNode_ProceduralRecoil : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Settings)
	FAlsxtAnimNode_RecoilAnimationNode Node;

public:
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	// End of UEdGraphNode interface

protected:
	// UAnimGraphNode_SkeletalControlBase interface
	virtual FText GetControllerDescription() const override;
	virtual const FAnimNode_SkeletalControlBase* GetNode() const override { return &Node; }
	// End of UAnimGraphNode_SkeletalControlBase interface

};