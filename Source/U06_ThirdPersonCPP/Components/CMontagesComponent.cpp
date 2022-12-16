#include "CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DataTable == nullptr)
	{
		CLog::Print("DataTable is not set");
		return;
	}

	TArray< FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);

	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		}
	}

}

void UCMontagesComponent::PlayRoll()
{
	PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackStep()
{
	PlayAnimMontage(EStateType::BackStep);
}

void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InStateType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());
	CheckNull(character);

	FMontageData* data = Datas[(int32)InStateType];
	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRate, data->StartSection);
	}

}
