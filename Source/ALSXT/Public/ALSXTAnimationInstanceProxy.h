#pragma once

#include "Animation/AnimInstanceProxy.h"
#include "ALSXTAnimationInstanceProxy.generated.h"

class UALSXTAnimationInstance;
class UALSXTLinkedAnimationInstance;

USTRUCT()
struct ALSXT_API FALSXTAnimationInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

	// This allows UAlsAnimationInstance and UAlsLinkedAnimationInstance to access some protected members of FAnimInstanceProxy.

	friend UALSXTAnimationInstance;
	friend UALSXTLinkedAnimationInstance;

public:
	FALSXTAnimationInstanceProxy() = default;

	explicit FALSXTAnimationInstanceProxy(UAnimInstance* AnimationInstance);

protected:
	virtual void PostUpdate(UAnimInstance* AnimationInstance) const override;
};
