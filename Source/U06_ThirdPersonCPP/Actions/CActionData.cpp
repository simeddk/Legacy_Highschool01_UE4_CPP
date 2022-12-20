#include "CActionData.h"
#include "Global.h"
#include "CEquipment.h"
#include "CAttachment.h"
#include "CDoAction.h"
#include "GameFramework/Character.h"

void UCActionData::BeginPlay(ACharacter* InOwnerCharacter, UCAction** OutAction)
{
	FTransform transform;

	ACAttachment* Attachment = nullptr;
	if (!!AttachmentClass)
	{
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
		Attachment->SetActorLabel(GetLabelName(InOwnerCharacter, "Attachment"));
		UGameplayStatics::FinishSpawningActor(Attachment, transform);
	}

	ACEquipment* Equipment = nullptr;
	if (!!EquipmentClass)
	{
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		Equipment->SetData(EquipmentData);
		Equipment->SetColor(EquipmentColor);
		Equipment->SetActorLabel(GetLabelName(InOwnerCharacter, "Equipment"));
		UGameplayStatics::FinishSpawningActor(Equipment, transform);

		if (!!Attachment)
		{
			Equipment->OnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnEquip);
			Equipment->OnUnequipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}
	
	ACDoAction* DoAction = nullptr;
	if (!!DoActionClass)
	{
		DoAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, transform, InOwnerCharacter);
		DoAction->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		DoAction->SetDatas(DoActionDatas);
		DoAction->SetActorLabel(GetLabelName(InOwnerCharacter, "DoAction"));
		UGameplayStatics::FinishSpawningActor(DoAction, transform);

		if (!!Equipment)
		{
			DoAction->SetEquipped(Equipment->IsEquipped());
		}

		if (!!Attachment)
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentEndOverlap);
		}
	}

	*OutAction = NewObject<UCAction>();
	(*OutAction)->Attachment = Attachment;
	(*OutAction)->Equipment = Equipment;
	(*OutAction)->DoAction = DoAction;
	(*OutAction)->EquipmentColor = EquipmentColor;
}


FString UCActionData::GetLabelName(ACharacter* InOwnerCharacter, FString InName)
{
	FString label;
	label.Append(InOwnerCharacter->GetActorLabel());
	label.Append("_");
	label.Append(InName);
	label.Append("_");
	label.Append(GetName().Replace(L"DA", L""));

	return label;
}