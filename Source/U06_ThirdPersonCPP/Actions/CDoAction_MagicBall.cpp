#include "CDoAction_MagicBall.h"
#include "Global.h"
#include "CAim.h"
#include "CThrow.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"


void ACDoAction_MagicBall::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);

	Action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	Action->OnActionTypeChanged.AddDynamic(this, &ACDoAction_MagicBall::ForceAimOff);
}

void ACDoAction_MagicBall::DoAction()
{
	Super::DoAction();

	if (Aim->IsAvaliable())
		CheckFalse(Aim->IsZooming());

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_MagicBall::Begin_DoAction()
{
	Super::Begin_DoAction();

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_ThrowItem");
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();

	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotator));

	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_MagicBall::OnThrowBeginOverlap);
	UGameplayStatics::FinishSpawningActor(throwObject, transform);
}

void ACDoAction_MagicBall::End_DoAction()
{
	Super::End_DoAction();

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_MagicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Aim->Tick(DeltaTime);
}

void ACDoAction_MagicBall::OnAim()
{
	Aim->On();
}

void ACDoAction_MagicBall::OffAim()
{
	Aim->Off();
}

void ACDoAction_MagicBall::OnThrowBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e;
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_MagicBall::ForceAimOff(EActionType InPrevType, EActionType InNewType)
{
	CheckFalse(Aim->IsAvaliable());
	CheckFalse(Aim->IsZooming());
	Aim->Off();
}
