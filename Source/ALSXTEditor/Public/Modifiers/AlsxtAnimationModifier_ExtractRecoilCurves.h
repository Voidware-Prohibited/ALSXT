// Copyright (C) 2026 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "Curves/CurveVector.h"
#include "Settings/AlsxtAnimationModifierExtractCurvesSettings.h"
#include "AlsxtAnimationModifier_ExtractRecoilCurves.generated.h"

UCLASS(Blueprintable)
class ALSXTEDITOR_API UAlsxtAnimationModifier_ExtractRecoilCurves : public UAnimationModifier
{
	GENERATED_BODY()

public:
	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	virtual void OnRevert_Implementation(UAnimSequence* AnimationSequence) override;

	void SaveCurve(UPackage* Package, UCurveVector* Curve, const FString& PackagePath);

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSoftObjectPtr<UAlsxtAnimationModifierExtractCurvesSettings> AnimationAssetExportSettings;

	/** Bone which will be captured */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FName BoneName;

	/** Name of procedural animation. Note: _Rot and _Loc postfixes are added automatically */
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ToolTip="Semi, Burst or Auto"))
	FName AnimationAssetName;

	/** Weapon name */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FString OverlayObjectName;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float AnimationSampleRate = 60.f;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ToolTip="Map Curves to -1.0 to 1.0"))
	bool bNormalizeCurves = true;
	
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ToolTip="Add Curves To Animation Asset"))
	bool bAddCurvesToAnimationAsset = false;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition="bAddCurvesToAnimationAsset"))
	UAnimSequence* ReferenceAnimationSequence = nullptr;

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (EditCondition="bAddCurvesToAnimationAsset"))
	FName CurveName;
	
private:
	/** Bone transform is returned in root bone space */
	FTransform ExtractPose(UAnimSequence* Animation, const FBoneContainer& BoneContainer,
									FCompactPoseBoneIndex CompactPoseBoneIndex, double Time);

	float NormalizeValue(float Value, float MaxValue) const;
	
	void ExtractCurveAssets(UAnimSequence* AnimationSequence);
	void ExtractAnimCurves(UAnimSequence* AnimationSequence);

	void AddCurveKey(TArray<FRichCurveKey>& Keys, float Time, float Value);
	void AddCurve(UAnimSequence* Animation, FName Name, const TArray<FRichCurveKey>& Keys);
	void RemoveCurve(UAnimSequence* Animation, FName Name);
	FName GetCurveName(FString Type, FString Axis) const;
};