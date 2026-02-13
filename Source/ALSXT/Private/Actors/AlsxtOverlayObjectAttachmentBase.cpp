// MIT


#include "Actors/AlsxtOverlayObjectAttachmentBase.h"

AAlsxtOverlayObjectAttachmentBase::AAlsxtOverlayObjectAttachmentBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAlsxtOverlayObjectAttachmentBase::SetVisibility(bool bVisible)
{
	if(Mesh)
	{
		Mesh->SetVisibility(bVisible, false);
	}
}
