#include "CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Engine/TriggerVolume.h"

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriggerVolume::StaticClass(), actors);
	CheckFalse(actors.Num() > 0);

	for (AActor* triggerVolume : actors)
	{
		triggerVolume->OnActorBeginOverlap.AddDynamic(this, &UCFeetComponent::OnActorBeginOverlap);
		triggerVolume->OnActorEndOverlap.AddDynamic(this, &UCFeetComponent::OnActorEndOverlap);
	}
}

void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance;
	FRotator leftRotator;
	Trace(LeftSocket, leftDistance, leftRotator);

	float rightDistance;
	FRotator rightRotator;
	Trace(RightSocket, rightDistance, rightRotator);

	float shouldBeDown = FMath::Min(leftDistance, rightDistance);

	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, shouldBeDown, DeltaTime, InterpSpeed);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - shouldBeDown), DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - shouldBeDown), DeltaTime, InterpSpeed);

	Data.LeftRotator = UKismetMathLibrary::RInterpTo(Data.LeftRotator, leftRotator, DeltaTime, InterpSpeed);
	Data.RightRotator = UKismetMathLibrary::RInterpTo(Data.RightRotator, rightRotator, DeltaTime, InterpSpeed);
}

void UCFeetComponent::Trace(FName InSocketName, float& OutDistance, FRotator& OutRotator)
{
	OutDistance = 0.f;

	FVector socketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName);
	FVector start = FVector(socketLocation.X, socketLocation.Y, OwnerCharacter->GetActorLocation().Z);

	float lineLenghth = start.Z - CapsuleHalfHeight - AdditionalDistance;
	FVector end = FVector(socketLocation.X, socketLocation.Y, lineLenghth);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true,
		ignoreActors,
		DrawDebugType,
		hitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red
	);

	CheckFalse(hitResult.IsValidBlockingHit());

	float depthUnderGround = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	OutDistance = FootOffset + depthUnderGround - AdditionalDistance;

	FVector normal = hitResult.ImpactNormal;
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), hitResult.ImpactPoint, hitResult.ImpactPoint + normal * 100, 10, FLinearColor::Blue);

	float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);

	roll = FMath::Clamp(roll, -15.f, 15.f);
	pitch = FMath::Clamp(pitch, -30.f, 30.f);

	OutRotator = FRotator(pitch, 0.f, roll);

}

void UCFeetComponent::OnActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	CheckNull(Cast<ACharacter>(OtherActor));
	bIKMode = true;
}

void UCFeetComponent::OnActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	CheckNull(Cast<ACharacter>(OtherActor));
	bIKMode = false;
}

