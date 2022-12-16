#include "CActionComponent.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CActionData.h"
#include "Actions/CEquipment.h"
#include "Actions/CAttachment.h"
#include "Actions/CDoAction.h"
#include "GameFramework/Character.h"

UCActionComponent::UCActionComponent()
{

}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(character, &Datas[i]);
	}
}

void UCActionComponent::SetUnarmedMode()
{
	if (!!Datas[(int32)Type])
		Datas[(int32)Type]->GetEquipment()->Unequip();

	Datas[(int32)EActionType::Unaremd]->GetEquipment()->Equip();

	ChangeType(EActionType::Unaremd);
}

void UCActionComponent::SetFistMode()
{
	SetMode(EActionType::Fist);
}

void UCActionComponent::SetOneHandMode()
{
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetMagicBallMode()
{
	SetMode(EActionType::MagicBall);
}

void UCActionComponent::SetWarpMode()
{
	SetMode(EActionType::Warp);
}

void UCActionComponent::SetMagicStormMode()
{
	SetMode(EActionType::MagicStorm);
}

void UCActionComponent::SetMode(EActionType InNewType)
{
	if (Type == InNewType)
	{
		SetUnarmedMode();
		return;
	}
	else if (IsUnarmedMode() == false)
	{
		if (!!Datas[(int32)Type] && !!Datas[(int32)Type]->GetEquipment())
			Datas[(int32)Type]->GetEquipment()->Unequip();
	}

	if (!!Datas[(int32)InNewType] && !!Datas[(int32)Type]->GetEquipment())
		Datas[(int32)InNewType]->GetEquipment()->Equip();

	ChangeType(InNewType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType prevType = Type;
	Type = InNewType;

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, Type);
}


void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* doAction = Datas[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->DoAction();
	}
}

void UCActionComponent::DoOnAim()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			action->OnAim();
	}
}

void UCActionComponent::DoOffAim()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			action->OffAim();
	}
}

void UCActionComponent::Dead()
{
	OffAllCollisios();
}

void UCActionComponent::End_Dead()
{
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!Datas[i] && !!Datas[i]->GetAttachment())
			Datas[i]->GetAttachment()->Destroy();

		if (!!Datas[i] && !!Datas[i]->GetEquipment())
			Datas[i]->GetEquipment()->Destroy();

		if (!!Datas[i] && !!Datas[i]->GetDoAction())
			Datas[i]->GetDoAction()->Destroy();
	}
}

void UCActionComponent::AbortByDamage()
{
	CheckNull(Datas[(int32)Type]);
	CheckTrue(IsUnarmedMode());

	Datas[(int32)Type]->GetEquipment()->Begin_Equip();
	Datas[(int32)Type]->GetEquipment()->End_Equip();

	Datas[(int32)Type]->GetDoAction()->Abort();
}

void UCActionComponent::OffAllCollisios()
{
	for (UCAction* data : Datas)
	{
		if (data == nullptr)
			continue;

		if (data->GetAttachment() == nullptr)
			continue;

		data->GetAttachment()->OffCollision();
	}
}
