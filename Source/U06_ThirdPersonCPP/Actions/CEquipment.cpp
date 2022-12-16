#include "CEquipment.h"
#include "Global.h"
#include "Characters/ICharacter.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	Super::BeginPlay();
}

void ACEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (!!Data.AnimMontage)
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRate, Data.StartSection);
	else
	{
		Begin_Equip();
		End_Equip();
	}

	if (Data.bPawnControl == true)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	IICharacter* characterInterface = Cast<IICharacter>(OwnerCharacter);
	CheckNull(characterInterface);
	characterInterface->ChangeBodyColor(Color);
}

void ACEquipment::Begin_Equip_Implementation()
{
	if (OnEquipmentDelegate.IsBound())
		OnEquipmentDelegate.Broadcast();
}

void ACEquipment::End_Equip_Implementation()
{
	State->SetIdleMode();

	bEquipped = true;
}

void ACEquipment::Unequip_Implementation()
{
	bEquipped = false;

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	if (OnUnequipmentDelegate.IsBound())
		OnUnequipmentDelegate.Broadcast();
}