// MIT

#pragma once

#include "CoreMinimal.h"
#include "AlsxtAnimationInstance.h"
#include "AlsxtCharacterAdvanced.h"
#include "AlsxtAdvancedLinkedAnimationInstance.h"
#include "Settings/AlsxtFirearmSettings.h"
#include "AlsxtAdvancedAnimationInstance.generated.h"

class UAlsxtAdvancedAnimationInstanceSettings;
class UAlsxtAdvancedLinkedAnimationInstance;
class AAlsxtCharacterAdvanced;

UCLASS()
class ALSXT_API UAlsxtAdvancedAnimationInstance : public UAlsxtAnimationInstance
{
	GENERATED_BODY()

	friend UAlsxtAdvancedLinkedAnimationInstance;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	TObjectPtr<AAlsxtCharacterAdvanced> ALSXTCharacterAdvanced;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAlsxtAdvancedAnimationInstanceSettings> ALSXTAdvancedSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsxtFirearmAimState FirearmAimState;

private:
	void RefreshAlsxtPose();

public:

	UAlsxtAdvancedAnimationInstance();

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
	UAlsxtAdvancedAnimationInstanceSettings* GetALSXTAdvancedSettingsUnsafe() const;
	
};

inline UAlsxtAdvancedAnimationInstanceSettings* UAlsxtAdvancedAnimationInstance::GetALSXTAdvancedSettingsUnsafe() const
{
	return ALSXTAdvancedSettings;
}
