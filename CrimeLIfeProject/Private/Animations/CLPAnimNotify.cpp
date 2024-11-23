// Crime Life Project, All Rights NOT Reserved.

#include "Animations/CLPAnimNotify.h"

void UCLPAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& InEventReference)
{
	OnNotified.Broadcast(MeshComp);

	Super::Notify(MeshComp, Animation, InEventReference);
}
