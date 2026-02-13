// MIT

#pragma once

#include "CoreMinimal.h"
#include "Components/Character/AlsxtProceduralRecoilAnimComponent.h"
#include "UObject/Object.h"
#include "AlsxtOverlayObjectAttachmentBase.generated.h"

/**
 * 
 */
UCLASS()
class ALSXT_API AAlsxtOverlayObjectAttachmentBase : public AActor
{
	GENERATED_BODY()

public:
	AAlsxtOverlayObjectAttachmentBase();

	void SetVisibility(bool bVisible);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment Base")
	FTransform ParentTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil Anim Scale")
	FAlsxtProceduralRecoilPlayerControllerMagnitudes RecoilAnimScale;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment Base")
	UStaticMeshComponent* Mesh;
};
