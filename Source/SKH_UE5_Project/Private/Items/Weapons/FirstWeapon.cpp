#include "Items/Weapons/FirstWeapon.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Interface/HitInterface.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AFirstWeapon::AFirstWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	// ���ݽ� ��Ʈ������ �ݸ����� ó���� NoCollision���� ����
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	// Ʈ���̽��� ��������Ʈ ������ ����ġ(���� ����Ʈ ��ġ�� BP���� �����ϵ��� ��)
	BoxTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start Point"));
	BoxTraceStartPoint->SetupAttachment(GetRootComponent());
	BoxTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End Point"));
	BoxTraceEndPoint->SetupAttachment(GetRootComponent());

}

void AFirstWeapon::BeginPlay()
{
	Super::BeginPlay();

	// ��������Ʈ ���ε�
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AFirstWeapon::OnBoxOverlap);
}

void AFirstWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	//�������� ���¸� �������� ����
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableSphereCollision();
	PlayEquipSound();
	DeactivateEffect();
}

void AFirstWeapon::DeactivateEffect()
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate(); NiagaraComponent->SetVisibility(false, true);
	}
	if (WidgetSphere)
	{
		WidgetSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WidgetSphere->SetSphereRadius(0.f);
		WidgetSphere->SetVisibility(false, true);
	}
}

void AFirstWeapon::EffectActive()
{
	if (WeaponEffect)
	{
		WeaponEffect->Activate();
	}
}

void AFirstWeapon::EffectDeactive()
{
	if (WeaponEffect)
	{
		WeaponEffect->Deactivate();
	}
}

void AFirstWeapon::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AFirstWeapon::PlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, EquipSound, GetActorLocation()
		);
	}
}

void AFirstWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AFirstWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// SameTag Ignore
	if (GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"))) return;

	if (GetOwner()->ActorHasTag(TEXT("EngageableTarget")) && OtherActor->ActorHasTag(TEXT("EngageableTarget"))) return;

	FHitResult BoxHit;
	BoxTrace(BoxHit);

	// �浹�����̻���� �浹�� ���Ͱ��ִٸ�(Null��ȯ�̾ƴ϶��)
	if (BoxHit.GetActor())
	{
		// ���� ���� ����������� ���� ������ �����ϰ� �ǰݵɼ��ִµ� �̶� �ѹ��� �Ǵ��ϵ��� �Ѵ�.
		if (GetOwner()->ActorHasTag(TEXT("Enemy")) && BoxHit.GetActor()->ActorHasTag(TEXT("Enemy"))) return;

		if (GetOwner()->ActorHasTag(TEXT("EngageableTarget")) && OtherActor->ActorHasTag(TEXT("EngageableTarget"))) return;

		// �ǰ��� ���Ϳ��� ������ ������ ���� ���� �Ѵ�.
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);
		ExcuteGetHit(BoxHit);
		// �ʵ�ý��� 
		CreateFields(BoxHit.ImpactPoint);
	}
}

void AFirstWeapon::ExcuteGetHit(FHitResult& BoxHit)
{
	// �������̽� �ӽ� ������ ����ȯ�Ͽ� �浹������ ���� ���͸� �����Ѵ�(�������̽� ��� �ʿ�)
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		// �������̽����� GetHit �Լ� ��ü�� �������Ʈ ������ ���·� ��ũ�� �����Ͽ��⶧���� ��ӵ� Ŭ�����鿡���� GetHit_Implementation() �Լ��� ȣ���Ͽ� ����ϰ� �ִ�. �ش� �Լ��� �����Ǹ� ���� �����Ǵ�Execute_GetHit �Լ��� ����Ͽ� �ൿ�� �����ϵ��� �缳��
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

void AFirstWeapon::BoxTrace(FHitResult& BoxHit)
{
	// �� ��ǥ�� ������ ��ü�� ���� ������ �ʵ��� ���ó��
	const FVector StartPoint = BoxTraceStartPoint->GetComponentLocation();
	const FVector EndPoint = BoxTraceEndPoint->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	// �ѹ� ��Ʈ���� ���͵�������Ǿ��ִ� �迭�� ���� �ݺ������� �÷��̾� ��Ʈ ����ó�� �� �迭�� �� ����־��ش�(�ߺ�����)
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	// �ڽ� Ʈ���̽� ����
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		StartPoint,
		EndPoint,
		BoxTraceExtent,
		BoxTraceStartPoint->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}

void AFirstWeapon::SetDamage(float ChangeDamage)
{
	Damage = ChangeDamage;
}
