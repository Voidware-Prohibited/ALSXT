// Copyright (C) 2026 Uriel Ballinas, VOIDWARE Prohibited. All rights reserved.
// This software is licensed under the MIT License (LICENSE.md).


#include "AlsxtOverlayObjectActor.h"
#include "Actors/AlsxtOverlayObjectAttachmentBase.h"


/**
* @file AlsxtOverlayObjectAcotr.cpp
* @brief Base Actor class for in-world Overlay Object pickups
*/

/**
* @brief Constructor for AlsxtOverlayObjectActor adding Meshes, and Ability System Component
*/
AAlsxtOverlayObjectActor::AAlsxtOverlayObjectActor()
{
	// If the NetUpdateFrequency is too low, there will be a delay on Ability activation / Effect application on the client.
	SetNetUpdateFrequency(100.0f);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));

	StaticMeshComponent = CreateDefaultSubobject<UAlsxtPaintableStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->bEditableWhenInherited = true;
	StaticMeshComponent->SetupAttachment(Pivot);

	SkeletalMeshComponent = CreateDefaultSubobject<UAlsxtPaintableSkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->bEditableWhenInherited = true;
	SkeletalMeshComponent->SetupAttachment(Pivot);

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture"));

	// Create the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
 
	// Set Replication Mode to Mixed for NPCs.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	SmartObjectComponent = CreateDefaultSubobject<USmartObjectComponent>(TEXT("Smart Object Component"));
}

#if WITH_EDITOR
void AAlsxtOverlayObjectActor::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	// Get the name of the property that was changed
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Call your custom function
	// OnMeshAssetChanged(PropertyName);
	if (PropertyName == "MeshAsset")
	{
		OnMeshAssetChanged();
	}
	
	// Call the base class version
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void AAlsxtOverlayObjectActor::OnMeshAssetChanged()
{
	if (UStaticMesh* StaticMesh = Cast<UStaticMesh>(MeshAsset.Get()))
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
	}
	else if (USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(MeshAsset.Get()))
	{
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh, false);
	}
}

// Called when the game starts or when spawned
void AAlsxtOverlayObjectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAlsxtOverlayObjectActor::InitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		// Shouldn't happen, but if it is, return an error.
		return;
	}
	
	// Call the function on "Custom Ability System Component" to set up references and Init data. (Client)
	// AbilitySystemComponent->InitializeAbilitySystemData(AbilitySystemInitializationData, this, this);

	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAlsxtMovementAttributeSet::GetMovementSpeedMultiplierAttribute()).AddUObject(this, &ThisClass::MovementSpeedMultiplierChanged);
	
	PostInitializeAbilitySystem();
}

void AAlsxtOverlayObjectActor::PostInitializeAbilitySystem_Implementation()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
}

// Called every frame
void AAlsxtOverlayObjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAlsxtOverlayObjectActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AAlsxtOverlayObjectActor, PlayerControllerMagnitudes, COND_SkipOwner);
}

void AAlsxtOverlayObjectActor::SetupAttachments()
{
	for(int i = 0; i < Attachments.Num(); i++)
	{
		for(auto Template : Attachments[i].GroupTemplates)
		{
			AAlsxtOverlayObjectAttachmentBase* Attachment = nullptr;

			if (Template)
			{
				Attachment = GetWorld()->SpawnActor<AAlsxtOverlayObjectAttachmentBase>(Template, GetActorLocation(), GetActorRotation());
				Attachment->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				Attachment->SetActorRelativeLocation(Attachment->ParentTransform.GetLocation());
				Attachment->SetActorRelativeRotation(Attachment->ParentTransform.GetRotation());
				Attachment->SetVisibility(false);
			}

			Attachments[i].Group.Add(Attachment);
		}
	}
}

UAbilitySystemComponent* AAlsxtOverlayObjectActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

USceneCaptureComponent2D* AAlsxtOverlayObjectActor::GetSceneCaptureComponent_Implementation() const
{
	return SceneCapture;
}

void AAlsxtOverlayObjectActor::SetMesh(UObject* NewMeshAsset)
{
	if (!NewMeshAsset) return;

	if (UStaticMesh* StaticMesh = Cast<UStaticMesh>(NewMeshAsset))
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
	}
	else if (USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(NewMeshAsset))
	{
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh, false);
	}
}

void AAlsxtOverlayObjectActor::ModifyAttachment(int Index)
{
	if(Index > Attachments.Num() - 1)
	{
		return;
	}
	
	auto& Group = Attachments[Index];
	Group.CycleGroup();
}

FAlsxtProceduralRecoilPlayerControllerMagnitudes AAlsxtOverlayObjectActor::GetAnimScaleFromGroup(int Index)
{
	FAlsxtProceduralRecoilPlayerControllerMagnitudes NewScale;
	if(Index > Attachments.Num() - 1)
	{
		return NewScale;
	}
	
	auto& Group = Attachments[Index];
	if(const auto Attachment = Group.Group[Group.Index])
	{
		NewScale = Attachment->RecoilAnimScale;
	}

	return NewScale;
}

void AAlsxtOverlayObjectActor::SetAttachmentsVisibility(bool bVisibility)
{
	for(auto Group : Attachments)
	{
		Group.SetVisibility(bVisibility);
	}
}