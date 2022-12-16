#include "CAttachment.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"

ACAttachment::ACAttachment()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	GetComponents<UShapeComponent>(ShapeComponents);
	for (UShapeComponent* shape : ShapeComponents)
	{
		shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
	}

	OffCollision();

	Super::BeginPlay();
	
}

void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent
	(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName
	);
}

void ACAttachment::AttachToCollision(USceneComponent* InComponent, FName InSocketName)
{
	InComponent->AttachToComponent
	(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		InSocketName
	);
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	if (OnAttachmentBeginOverlap.IsBound())
	{
		ACharacter* character =  Cast<ACharacter>(OtherActor);
		CheckNull(character);

		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, character);
	}
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OnAttachmentEndOverlap.IsBound())
	{
		ACharacter* character = Cast<ACharacter>(OtherActor);
		CheckNull(character);

		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, this, character);
	}
}

void ACAttachment::OnCollision()
{
	for (UShapeComponent* shape : ShapeComponents)
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACAttachment::OffCollision()
{
	for (UShapeComponent* shape : ShapeComponents)
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
