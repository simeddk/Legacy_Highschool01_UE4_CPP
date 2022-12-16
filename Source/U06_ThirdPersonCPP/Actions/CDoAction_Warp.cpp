#include "CDoAction_Warp.h"
#include "Global.h"
#include "CAttachment.h"
#include "Characters/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CBehaviorComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "AIController.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* actor : OwnerCharacter->Children)
	{
		if (actor->IsA<ACAttachment>() && actor->GetActorLabel().Contains("Warp"))
		{
			Decal = CHelpers::GetComponent<UDecalComponent>(actor);
			break;
		}
	}
}

void ACDoAction_Warp::DoAction()
{
	Super::DoAction();

	CheckFalse(*bEquipped);
	CheckFalse(State->IsIdleMode());

	if (GetPlayerUsing())
	{
		FRotator rotator;
		CheckFalse(GetDecalLocation(Location, rotator));
	}
	else
	{
		AAIController* controller = OwnerCharacter->GetController<AAIController>();
		UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
		
		Location = behavior->GetWarpLocation();
		Decal->SetVisibility(false);
	}

	State->SetActionMode();
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
}

void ACDoAction_Warp::Begin_DoAction()
{
	Super::Begin_DoAction();

	FTransform transform = Datas[0].EffectTransform;

	UGameplayStatics::SpawnEmitterAttached
	(
		Datas[0].Effect,
		OwnerCharacter->GetMesh(),
		NAME_None,
		transform.GetLocation(),
		FRotator(transform.GetRotation()),
		transform.GetScale3D()
	);
}

void ACDoAction_Warp::End_DoAction()
{
	Super::End_DoAction();

	Location += FVector(0, 0, 88);
	OwnerCharacter->SetActorLocation(Location/*, true*/);
	Location = FVector::ZeroVector;

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(*bEquipped);

	if (GetPlayerUsing() == nullptr)
	{
		Decal->SetVisibility(false);
		return;
	}

	FVector location;
	FRotator rotator;
	if (GetDecalLocation(location, rotator) == true)
	{
		Decal->SetVisibility(true);
		Decal->SetWorldLocation(location);
		Decal->SetWorldRotation(rotator);
	}
	else
		Decal->SetVisibility(false);
}

bool ACDoAction_Warp::GetDecalLocation(FVector& OutLocation, FRotator& OutRotator)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

	FHitResult hitResult;
	if (controller->GetHitResultUnderCursorForObjects(objectTypes, true, hitResult))
	{
		FVector location = hitResult.Location;
		location.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		OutLocation = location;
		OutRotator = hitResult.ImpactNormal.Rotation();

		return true;
	}

	return false;
}

ACPlayer* ACDoAction_Warp::GetPlayerUsing()
{
	return Cast<ACPlayer>(OwnerCharacter);
}
