// MIT

#pragma once

#include "CoreMinimal.h"
#include "ALSXTAnimationInstance.h"
#include "ALSXTAdvancedLinkedAnimationInstance.h"
#include "ALSXTAdvancedAnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API UALSXTAdvancedAnimationInstance : public UALSXTAnimationInstance
{
	GENERATED_BODY()

	friend UALSXTAdvancedLinkedAnimationInstance;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UALSXTAdvancedAnimationInstanceSettings> ALSXTAdvancedSettings;

private:
	void RefreshALSXTPose();

public:

	UALSXTAdvancedAnimationInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaTime) override;

	virtual void NativePostEvaluateAnimation() override;

protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;

	// Core

protected:
	UFUNCTION(BlueprintPure, Category = "ALS|Als Animation Instance",
		Meta = (BlueprintProtected, BlueprintThreadSafe, ReturnDisplayName = "Setting"))
	UALSXTAdvancedAnimationInstanceSettings* GetALSXTAdvancedSettingsUnsafe() const;
	
};

inline UALSXTAdvancedAnimationInstanceSettings* UALSXTAdvancedAnimationInstance::GetALSXTAdvancedSettingsUnsafe() const
{
	return ALSXTAdvancedSettings;
}
