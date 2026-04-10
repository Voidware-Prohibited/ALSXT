// MIT

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Actors/AlsxtOverlayObjectAttachmentBase.h"
#include "Components/Character/AlsxtProceduralRecoilAnimComponent.h"
#include "Components/Mesh/AlsxtPaintableSkeletalMeshComponent.h"
#include "Components/Mesh/AlsxtPaintableStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/AlsxtFirearmInterface.h"
#include "Interfaces/AlsxtMeshPaintingInterface.h"
#include "Interfaces/AlsxtOverlayObjectInterface.h"
#include "Settings/AlsxtOverlayObjectSettings.h"
#include "SmartObjectComponent.h"
#include "AlsxtOverlayObjectActor.generated.h"

USTRUCT(BlueprintType)
struct FAlsxtOverlayObjectAttachmentGroup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attachments")
	TArray<TSubclassOf<AAlsxtOverlayObjectAttachmentBase>> GroupTemplates;

	UPROPERTY(BlueprintReadOnly, Category = "Attachments")
	TArray<AAlsxtOverlayObjectAttachmentBase*> Group;

	UPROPERTY(BlueprintReadOnly, Category = "Attachments")
	int Index = 0;

	FAlsxtProceduralRecoilPlayerControllerMagnitudes GetInputScale()
	{
		FAlsxtProceduralRecoilPlayerControllerMagnitudes PlayerControllerMagnitudes;

		if(Group.Num() == 0)
		{
			return PlayerControllerMagnitudes;
		}

		if(const auto Attachment = Group[Index])
		{
			PlayerControllerMagnitudes = Attachment->RecoilAnimScale;
		}
		
		return PlayerControllerMagnitudes;
	}

	void CycleGroup()
	{
		SetVisibility(false);
		Index++;
		Index = Index > Group.Num() - 1 ? 0 : Index;
		SetVisibility(true);
	}

	void SetVisibility(bool bVisible)
	{
		if(!Group[Index])
		{
			return;
		}
		
		Group[Index]->SetVisibility(bVisible);
	}
};

UCLASS()
class ALSXT_API AAlsxtOverlayObjectActor : public AActor, public IAlsxtMeshPaintingInterface, public IAlsxtOverlayObjectInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAlsxtOverlayObjectActor();

#if WITH_EDITOR
	// Override the PostEditChangeProperty function
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY()
	USceneComponent* Pivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USmartObjectComponent* SmartObjectComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* FireMontage;

	void OnMeshAssetChanged();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Data used to initialize the Ability System Component. (Can be found in "AbilitySystemData.h")
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability System", Meta = (ShowOnlyInnerProperties))
	TSoftObjectPtr<UAlsxtAbilitySystemInitializationDataAsset> AbilitySystemInitializationData;

	UPROPERTY(EditAnywhere, Category = "Ability System")
	
	UAbilitySystemComponent* AbilitySystemComponent;

	void InitializeAbilitySystem();

	// This event is fired after Ability System Component initialization is finished.
	UFUNCTION(BlueprintNativeEvent)
	void PostInitializeAbilitySystem();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UAlsxtPaintableStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UAlsxtPaintableSkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	USceneCaptureComponent2D* SceneCapture;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowedClasses = "/Script/Engine.SkeletalMesh, /Script/Engine.StaticMesh"))
	TSoftObjectPtr<UObject> MeshAsset;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSoftObjectPtr<UAlsxtOverlayObjectSettingsDataAsset> OverlayObjectSettings;

	// This Gameplay Tag is used to retrieve Settings from the Overlay Object Lookup Table 
	UPROPERTY(EditAnywhere, Category = "Settings")
	FGameplayTag OverlayObjectTag;

	virtual USceneCaptureComponent2D* GetSceneCaptureComponent_Implementation() const override;

	UFUNCTION(BlueprintCallable)
	void SetMesh(UObject* NewMeshAsset);

	UFUNCTION(BlueprintCallable)
	void ModifyAttachment(int Index);
	
	UFUNCTION(BlueprintCallable)
	FAlsxtProceduralRecoilPlayerControllerMagnitudes GetAnimScaleFromGroup(int Index);
	
	void SetAttachmentsVisibility(bool bVisibility);
	
	void SetupAttachments();

	// Implement the IAbilitySystemInterface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimSequence* SkeletalMeshBasePose;

	// Gun offset in mesh space 
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FVector PositionOffset = FVector::ZeroVector;
	
	// Gun offset in mesh space
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FVector FirearmPivotOffset = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FTransform PointAim;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FTransform EditingOffset;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSoftObjectPtr<UAlsxtProceduralRecoilSettingsDataAsset> ProceduralRecoilSettings;

	UPROPERTY(VisibleInstanceOnly, Replicated, Category = "Settings")
	FAlsxtProceduralRecoilPlayerControllerMagnitudes PlayerControllerMagnitudes;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachments")
	TArray<FAlsxtOverlayObjectAttachmentGroup> Attachments;
};
