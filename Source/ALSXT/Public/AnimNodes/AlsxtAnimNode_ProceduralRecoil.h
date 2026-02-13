// MIT

#pragma once

#include "GameplayTagContainer.h"
#include "BoneControllers/AnimNode_ModifyBone.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "Utility/AlsxtGameplayTags.h"
#include "AlsxtAnimNode_ProceduralRecoil.generated.h"

class UAlsxtProceduralRecoilAnimComponent;

UENUM(BlueprintType)
enum ERecoilAxesOrder
{
	XYZ UMETA(DisplayName = "XYZ"),
	XZY UMETA(DisplayName = "XZY"),
	YXZ UMETA(DisplayName = "YXZ"),
	YZX UMETA(DisplayName = "YZX"),
	ZXY UMETA(DisplayName = "ZXY"),
	ZYX UMETA(DisplayName = "ZYX")
};

USTRUCT(BlueprintInternalUseOnly)
struct ALSXT_API FAlsxtAnimNode_RecoilAnimationNode : public FAnimNode_SkeletalControlBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category=SkeletalControl)
	FBoneReference BoneToModify;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Translation, meta=(PinHiddenByDefault))
	FVector TranslationScale{FVector::OneVector};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Rotation, meta=(PinHiddenByDefault))
	FVector RotationScale{FVector::OneVector};

	UPROPERTY(EditAnywhere, Category=SkeletalControl, meta=(PinHiddenByDefault))
	TEnumAsByte<ERecoilAxesOrder> AxesOrder{XYZ};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=UpdateMode, meta=(PinShownByDefault))
	bool bUseAutoUpdate{true};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Translation, meta=(PinHiddenByDefault))
	FVector Translation{FVector::OneVector};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Rotation, meta=(PinHiddenByDefault))
	FRotator Rotation{FRotator::ZeroRotator};
	
	UPROPERTY(EditAnywhere, Category=Translation)
	TEnumAsByte<enum EBoneControlSpace> TranslationSpace{BCS_BoneSpace};
	
	UPROPERTY(EditAnywhere, Category=Rotation)
	TEnumAsByte<enum EBoneControlSpace> RotationSpace{BCS_BoneSpace};

	UPROPERTY(EditAnywhere, Category=Debug)
	bool bDrawDebugRecoilPivot{false};

	UPROPERTY(EditAnywhere, Category=Debug, meta = (EditCondition="bDrawDebugRecoilPivot"))
	float DebugPivotSphereRadius{3.f};

	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output,
	                                               TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;

	virtual bool HasPreUpdate() const override { return bUseAutoUpdate; }

	virtual void PreUpdate(const UAnimInstance* InAnimInstance) override;

private:
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;

	FTransform ComputeRecoilTransform();

	FGameplayTag Readiness {AlsxtReadyStanceTags::Relaxed};
	
	bool bIsInitialized{false};

	TWeakObjectPtr<UAlsxtProceduralRecoilAnimComponent> RecoilComponent;
	FTransform RecoilTransform{FTransform::Identity};
};