#include "ALSXTAdvancedAnimationInstanceProxy.h"
#include "ALSXTAnimationInstanceProxy.h"
#include "ALSXTAdvancedAnimationInstance.h"
#include "ALSXTAnimationInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSXTAdvancedAnimationInstanceProxy)

FALSXTAdvancedAnimationInstanceProxy::FALSXTAdvancedAnimationInstanceProxy(UAnimInstance* AnimationInstance): FALSXTAnimationInstanceProxy{AnimationInstance} {}

void FALSXTAdvancedAnimationInstanceProxy::PostUpdate(UAnimInstance* AnimationInstance) const
{
	FALSXTAnimationInstanceProxy::PostUpdate(AnimationInstance);

	// Epic does not allow to override the UAnimInstance::PostUpdateAnimation()
	// function in child classes, so we have to resort to this workaround.

	auto* ALSXTAdvancedAnimationInstance{Cast<UALSXTAdvancedAnimationInstance>(AnimationInstance)};
	if (IsValid(ALSXTAdvancedAnimationInstance))
	{
		ALSXTAdvancedAnimationInstance->NativePostUpdateAnimation();
	}
}
