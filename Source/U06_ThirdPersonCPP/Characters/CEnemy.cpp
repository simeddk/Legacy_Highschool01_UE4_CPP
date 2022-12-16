#include "CEnemy.h"
#include "Global.h"
#include "Components/CStatusComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"
#include "Actions/CActionData.h"

ACEnemy::ACEnemy()
{
	//Create SceneComponent
	CHelpers::CreateComponent(this, &NameWidget, "NameWidget", GetMesh());
	CHelpers::CreateComponent(this, &HealthWidget, "HealthWidget", GetMesh());

	//Create ActorComponent
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &State, "State");

	//SkelMesh Settings
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstanceClass;
	CHelpers::GetClass<UAnimInstance>(&animInstanceClass, "/Game/Enemies/ABP_CEnemy");
	GetMesh()->SetAnimInstanceClass(animInstanceClass);

	//Movement Settings
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	//Widget Settings
	TSubclassOf<UCUserWidget_Name> nameWidgetClass;
	CHelpers::GetClass(&nameWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Name.WB_Name_C'");
	NameWidget->SetWidgetClass(nameWidgetClass);
	NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	NameWidget->SetDrawSize(FVector2D(240, 70));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCUserWidget_Health> healthWidgetClass;
	CHelpers::GetClass(&healthWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthWidgetClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemy::BeginPlay()
{
	UMaterialInstanceConstant* bodyMaterial;
	UMaterialInstanceConstant* logoMaterial;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&bodyMaterial, "/Game/Character/Materials/M_Player_Body_Inst");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logoMaterial, "/Game/Character/Materials/M_Player_ChestLogo_Inst");

	BodyMaterial = UMaterialInstanceDynamic::Create(bodyMaterial, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logoMaterial, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Super::BeginPlay();

	HealthWidget->InitWidget();
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetCurrentHealth(), Status->GetMaxHealth());

	NameWidget->SetVisibility(bVisibleNameWidget);
	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(GetName());
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetControllerNameText(GetController()->GetName());

}

void ACEnemy::ChangeBodyColor(FLinearColor InColor)
{
	if (State->IsHittedMode())
	{
		LogoMaterial->SetVectorParameterValue("LogoColor", InColor);
		LogoMaterial->SetScalarParameterValue("IsHitted", 1);

		return;
	}

	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

void ACEnemy::RestoreLogoColor()
{
	FLinearColor color = Action->GetCurrent()->GetEquipmentColor();

	LogoMaterial->SetVectorParameterValue("LogoColor", color);
	LogoMaterial->SetScalarParameterValue("IsHitted", 0);
}


float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Attacker = Cast<ACharacter>(DamageCauser->GetOwner());
	Status->DecreaseHealth(Damage);

	Action->AbortByDamage();

	if (Status->GetCurrentHealth() <= 0.f)
	{
		State->SetDeadMode();
		return Damage;
	}

	State->SetHittedMode();
	return Damage;
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Hitted:	Hitted();	break;
		case EStateType::Dead:		Dead();		break;
	}
}

void ACEnemy::Hitted()
{
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetCurrentHealth(), Status->GetMaxHealth());

	Montages->PlayHitted();
	Status->SetMove();

	//Look At Attacker
	FVector start = GetActorLocation();
	FVector target = Attacker->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));

	//HitBack
	FVector direction = target - start;
	direction.Normalize();
	LaunchCharacter(-direction * Damage * LaunchValue, true, false);

	//Change LogoColor
	ChangeBodyColor(FLinearColor(35.f, 0, 0));
	UKismetSystemLibrary::K2_SetTimer(this, "RestoreLogoColor", 1.f, false);
}

void ACEnemy::Dead()
{
	CheckFalse(State->IsDeadMode());

	//Widget Visible false
	NameWidget->SetVisibility(false);
	HealthWidget->SetVisibility(false);

	//All Weapon Collision Disable
	Action->Dead();

	//Ragdoll
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->GlobalAnimRateScale = 0.f;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	FVector start = GetActorLocation();
	FVector target = Attacker->GetActorLocation();
	FVector direction = target - start;
	direction.Normalize();

	GetMesh()->AddForce(-direction * 2500000);

	//EndDead
	UKismetSystemLibrary::K2_SetTimer(this, "End_Dead", 5.f, false);
}

void ACEnemy::End_Dead()
{
	Action->End_Dead();

	Destroy();
}

