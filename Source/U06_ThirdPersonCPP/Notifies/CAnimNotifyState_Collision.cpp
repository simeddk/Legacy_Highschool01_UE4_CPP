#include "CAnimNotifyState_Collision.h"
#include "Global.h"
#include "Actions/CAttachment.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction_Melee.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	ACDoAction_Melee* melee =  Cast<ACDoAction_Melee>(actionComp->GetCurrent()->GetDoAction());
	if (!!melee)
	{
		FString collisionName = melee->GetCollisionName();
		actionComp->GetCurrent()->GetAttachment()->OnCollision(collisionName);
	}
	else
		actionComp->GetCurrent()->GetAttachment()->OnCollision();
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(actionComp);

	actionComp->GetCurrent()->GetAttachment()->OffCollision();
}

