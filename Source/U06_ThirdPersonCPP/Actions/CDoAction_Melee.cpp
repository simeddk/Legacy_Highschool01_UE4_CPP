#include "CDoAction_Melee.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/Character.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();

	CheckFalse(Datas.Num() > 0);

	if (bCanCombo == true)
	{
		bCanCombo = false;
		bSucceed = true;
		return;
	}

	CheckFalse(State->IsIdleMode());

	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bSucceed);
	bSucceed = false;

	OwnerCharacter->StopAnimMontage();

	ComboCount++;
	ComboCount = FMath::Clamp<int32>(ComboCount, 0, Datas.Num() - 1);

	OwnerCharacter->PlayAnimMontage(Datas[ComboCount].AnimMontage, Datas[ComboCount].PlayRate, Datas[ComboCount].StartSection);
	Datas[ComboCount].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage();
	ComboCount = 0;

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);

	//다단히트 방지
	for (const ACharacter* other : HittedCharacters)
	{
		if (other == InOtherCharacter)
			return;
	}
	HittedCharacters.Add(InOtherCharacter);


	//히트 스탑
	float hitStop = Datas[ComboCount].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreGlobalTimeDilation", 2e-2f * hitStop, false);
	}

	//파티클 재생
	UParticleSystem* hitEffect = Datas[ComboCount].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[ComboCount].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	//카메라 쉐이크
	TSubclassOf<UCameraShake> shakeClass = Datas[ComboCount].ShakeClass;
	if (!!shakeClass)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!!controller)
			controller->PlayerCameraManager->PlayCameraShake(shakeClass);
	}


	//대미지 주기
	FDamageEvent damageEvent;
	InOtherCharacter->TakeDamage(Datas[ComboCount].Power, damageEvent, InAttacker->GetController(), InAttackCauser);
}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackCauser, InOtherCharacter);

	HittedCharacters.Empty();
}

void ACDoAction_Melee::RestoreGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}
