#pragma once

#include "ALSXTAdvancedAnimationInstance.h"
#include "ALSXTAnimationInstanceProxy.h"
#include "ALSXTAnimationInstance.h"
#include "ALSXTAdvancedAnimationInstanceProxy.generated.h"

class UALSXTAdvancedAnimationInstance;
class UALSXTAdvancedLinkedAnimationInstance;

USTRUCT()
struct ALSXT_API FALSXTAdvancedAnimationInstanceProxy : public FALSXTAnimationInstanceProxy
{
	GENERATED_BODY()

	// This allows UAlsAnimationInstance and UAlsLinkedAnimationInstance to access some protected members of FAnimInstanceProxy.

	friend UALSXTAdvancedAnimationInstance;
	friend UALSXTAdvancedLinkedAnimationInstance;

public:
	FALSXTAdvancedAnimationInstanceProxy() = default;

	explicit FALSXTAdvancedAnimationInstanceProxy(UAnimInstance* AnimationInstance);

protected:
	virtual void PostUpdate(UAnimInstance* AnimationInstance) const override;
};
