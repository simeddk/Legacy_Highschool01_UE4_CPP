#include "CDoAction_MagicStorm.h"
#include "Global.h"
#include "Actions/CAttachment.h"
#include "Characters/CEnemy.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"

void ACDoAction_MagicStorm::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* actor : OwnerCharacter->Children)
	{
		if (actor->IsA<ACAttachment>() && actor->GetActorLabel().Contains("MagicStorm"))
		{
			Box = CHelpers::GetComponent<UBoxComponent>(actor);
			break;
		}
	}
}

void ACDoAction_MagicStorm::DoAction()
{
	CheckFalse(State->IsIdleMode());
	CheckTrue(bActivating);

	bActivating = true;

	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_MagicStorm::Begin_DoAction()
{
	Angle = OwnerCharacter->GetActorForwardVector().Rotation().Yaw;
	Particle = UGameplayStatics::SpawnEmitterAttached(Datas[0].Effect, Box, NAME_None);
	Particle->SetRelativeLocation(Datas[0].EffectTransform.GetLocation());
	Particle->SetRelativeScale3D(Datas[0].EffectTransform.GetScale3D());

	ACAttachment* attachment = Cast<ACAttachment>(Box->GetOwner());
	attachment->OnCollision();

	UKismetSystemLibrary::K2_SetTimer(this, "Hitted", DPS, true);
}

void ACDoAction_MagicStorm::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();

	FTimerDynamicDelegate timerDelegate;
	timerDelegate.BindUFunction(this, "Finish");
	UKismetSystemLibrary::K2_SetTimerDelegate(timerDelegate, ActivationTime, false);
}

void ACDoAction_MagicStorm::Abort()
{
}

void ACDoAction_MagicStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Todo. ¿©±â
	Box->SetWorldLocation(OwnerCharacter->GetActorLocation());
}

void ACDoAction_MagicStorm::Hitted()
{
	FDamageEvent e;

	for (ACharacter* character : HittedCharacters)
	{
		ACEnemy* enemy = Cast<ACEnemy>(character);
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(enemy);
		
		if (!!enemy &&
			enemy->IsPendingKill() == false &&
			state->IsDeadMode() == false)
		{
			enemy->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
		}
	}
}

void ACDoAction_MagicStorm::Finish()
{
	if (!!Particle)
		Particle->DestroyComponent();

	bActivating = false;

	ACAttachment* attachment = Cast<ACAttachment>(Box->GetOwner());
	attachment->OffCollision();

	UKismetSystemLibrary::K2_ClearTimer(this, "Hitted");
}

void ACDoAction_MagicStorm::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	HittedCharacters.AddUnique(InOtherCharacter);
}

void ACDoAction_MagicStorm::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	HittedCharacters.Remove(InOtherCharacter);
}
