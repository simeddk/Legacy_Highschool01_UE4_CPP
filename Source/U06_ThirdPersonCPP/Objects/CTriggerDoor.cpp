#include "CTriggerDoor.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ACTriggerDoor::ACTriggerDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Scene, "Scene");
	CHelpers::CreateComponent(this, &Box, "Box", Scene);
	CHelpers::CreateComponent(this, &DoorFrame, "DoorFrame", Scene);
	CHelpers::CreateComponent(this, &Door, "Door", DoorFrame);

	UStaticMesh* doorFrameMesh;
	CHelpers::GetAsset<UStaticMesh>(&doorFrameMesh, "StaticMesh'/Game/DoorMesh/Props/SM_DoorFrame.SM_DoorFrame'");
	DoorFrame->SetStaticMesh(doorFrameMesh);

	UStaticMesh* door;
	CHelpers::GetAsset<UStaticMesh>(&door, "StaticMesh'/Game/DoorMesh/Props/SM_Door.SM_Door'");
	Door->SetStaticMesh(door);
	Door->SetRelativeLocation(FVector(0, 45, 0));

	Box->InitBoxExtent(FVector(150, 50, 100));
	Box->SetRelativeLocation(FVector(0, 0, 100));
	Box->SetCollisionProfileName("Trigger");
}

void ACTriggerDoor::BeginPlay()
{
	Super::BeginPlay();

	Box->SetHiddenInGame(bHiddenCollision);
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACTriggerDoor::OnBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACTriggerDoor::OnEndOverlap);

	OnTriggerDoorOpen.AddUFunction(this, "Open");
	OnTriggerDoorClose.AddUFunction(this, "Close");
}

void ACTriggerDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator current = Door->GetRelativeRotation();

	if (bOpen)
		Door->SetRelativeRotation(FMath::Lerp(current, FRotator(0, MaxDegreeDirection, 0), Speed));
	else
		Door->SetRelativeRotation(FMath::Lerp(current, FRotator::ZeroRotator, Speed));
}

void ACTriggerDoor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);
	CheckTrue(OtherActor == this);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);

	if (OnTriggerDoorOpen.IsBound())
		OnTriggerDoorOpen.Broadcast(character);
}

void ACTriggerDoor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);
	CheckTrue(OtherActor == this);

	if (OnTriggerDoorClose.IsBound())
		OnTriggerDoorClose.Broadcast();
}

void ACTriggerDoor::Open(ACharacter* InCharacter)
{
	FVector doorForward =  GetActorForwardVector();
	FVector characterForward = InCharacter->GetActorForwardVector();

	float direction = FMath::Sign(doorForward | characterForward);
	MaxDegreeDirection = MaxDegree * direction;

	bOpen = true;
}

void ACTriggerDoor::Close()
{
	bOpen = false;
}

