// MIT

#pragma once

#include "CoreMinimal.h"
#include "ALSXTAnimationInstance.h"
#include "ALSXTCharacterAdvanced.h"
#include "ALSXTAdvancedLinkedAnimationInstance.h"
#include "Settings/ALSXTFirearmSettings.h"
#include "ALSXTAdvancedAnimationInstance.generated.h"

class UALSXTAdvancedLinkedAnimationInstance;
class AALSXTCharacterAdvanced;

UCLASS()
class ALSXT_API UALSXTAdvancedAnimationInstance : public UALSXTAnimationInstance
{
	GENERATED_BODY()

	friend UALSXTAdvancedLinkedAnimationInstance;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (AllowPrivateAccess))
	TObjectPtr<AALSXTCharacterAdvanced> ALSXTCharacterAdvanced;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UALSXTAdvancedAnimationInstanceSettings> ALSXTAdvancedSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FALSXTFirearmAimState FirearmAimState;

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
