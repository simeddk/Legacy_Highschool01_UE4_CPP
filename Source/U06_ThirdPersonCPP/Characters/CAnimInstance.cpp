#include "CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(owner);

	UCActionComponent* actionComp =  CHelpers::GetComponent<UCActionComponent>(owner);
	CheckNull(actionComp);

	actionComp->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(owner);

	Speed = owner->GetVelocity().Size2D();
	Direction = CalculateDirection(owner->GetVelocity(), owner->GetControlRotation());
}

void UCAnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}