#include "ALSXTAnimationInstanceProxy.h"
#include "ALSXTAnimationInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSXTAnimationInstanceProxy)

FALSXTAnimationInstanceProxy::FALSXTAnimationInstanceProxy(UAnimInstance* AnimationInstance): FAnimInstanceProxy{AnimationInstance} {}

void FALSXTAnimationInstanceProxy::PostUpdate(UAnimInstance* AnimationInstance) const
{
	FAnimInstanceProxy::PostUpdate(AnimationInstance);

	// Epic does not allow to override the UAnimInstance::PostUpdateAnimation()
	// function in child classes, so we have to resort to this workaround.

	auto* ALSXTAnimationInstance{Cast<UALSXTAnimationInstance>(AnimationInstance)};
	if (IsValid(ALSXTAnimationInstance))
	{
		ALSXTAnimationInstance->NativePostUpdateAnimation();
	}
}
