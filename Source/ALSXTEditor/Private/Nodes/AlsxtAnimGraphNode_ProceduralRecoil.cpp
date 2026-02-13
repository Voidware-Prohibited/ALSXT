// MIT

#include "Nodes/AlsxtAnimGraphNode_ProceduralRecoil.h"

FText UAlsxtAnimGraphNode_ProceduralRecoil::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("ALSXT Procedural Recoil"));
}

FText UAlsxtAnimGraphNode_ProceduralRecoil::GetTooltipText() const
{
	return FText::FromString(TEXT("Applies weapon recoil to the specified bone."));
}

FText UAlsxtAnimGraphNode_ProceduralRecoil::GetControllerDescription() const
{
	return FText::FromString(TEXT("ALSXT Procedural Recoil AnimGraph Node"));
}