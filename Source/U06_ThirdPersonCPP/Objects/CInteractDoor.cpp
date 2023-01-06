#include "CInteractDoor.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"

ACInteractDoor::ACInteractDoor()
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

void ACInteractDoor::BeginPlay()
{
	Super::BeginPlay();
	
	Box->SetHiddenInGame(bHiddenCollision);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACInteractDoor::OnBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACInteractDoor::OnEndOverlap);

	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->OnInteractEvent.AddDynamic(this, &ACInteractDoor::Interact);
}

void ACInteractDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpening)
		Opening(DeltaTime);

	if (bClosed)
		Closing(DeltaTime);
}

void ACInteractDoor::Interact()
{
	CheckNull(Player);

	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(Player);

	float dot = GetActorForwardVector() | camera->GetForwardVector();
	Direction = FMath::Sign(dot);
	DottedDegree = Direction * MaxDegree;

	if (bClosed)
	{
		bClosed = false;
		bClosing = false;
		bOpening = true;
	}
	else
	{
		bClosed = true;
		bClosing = true;
		bOpening = false;
	}
}

void ACInteractDoor::Opening(float DeltaTime)
{
	CurrentDegree = Direction * Speed * DeltaTime;

	if (FMath::IsNearlyEqual(Door->GetRelativeRotation().Yaw, DottedDegree, 2.f))
	{
		bOpening = false;
		Door->SetRelativeRotation(FRotator(0, DottedDegree, 0));
	}
	else if (bOpening)
	{
		Door->AddRelativeRotation(FRotator(0, CurrentDegree, 0));
	}
}

void ACInteractDoor::Closing(float DeltaTime)
{
	CurrentDegree =  (Door->GetRelativeRotation().Yaw > 0.f  ? -Speed : Speed) * DeltaTime;

	if (FMath::IsNearlyZero(Door->GetRelativeRotation().Yaw, 2.f))
	{
		bClosing = false;
		Door->SetRelativeRotation(FRotator::ZeroRotator);
	}
	else if (bClosing)
	{
		Door->AddRelativeRotation(FRotator(0, CurrentDegree, 0));
	}
}

void ACInteractDoor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<ACPlayer>(OtherActor);
}

void ACInteractDoor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = nullptr;
}

