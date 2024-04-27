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
	// 공격시 히트판정할 콜리전은 처음에 NoCollision으로 지정
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	// 트레이스용 씬컴포넌트 생성및 어태치(각각 포인트 위치는 BP에서 지정하도록 함)
	BoxTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start Point"));
	BoxTraceStartPoint->SetupAttachment(GetRootComponent());
	BoxTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End Point"));
	BoxTraceEndPoint->SetupAttachment(GetRootComponent());

}

void AFirstWeapon::BeginPlay()
{
	Super::BeginPlay();

	// 델리게이트 바인딩
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AFirstWeapon::OnBoxOverlap);
}

void AFirstWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	//아이템의 상태를 장착으로 변경
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

	// 충돌지점이생겼고 충돌된 액터가있다면(Null반환이아니라면)
	if (BoxHit.GetActor())
	{
		// 만일 여러 적이있을경우 위의 조건을 무시하고 피격될수있는데 이때 한번더 판단하도록 한다.
		if (GetOwner()->ActorHasTag(TEXT("Enemy")) && BoxHit.GetActor()->ActorHasTag(TEXT("Enemy"))) return;

		if (GetOwner()->ActorHasTag(TEXT("EngageableTarget")) && OtherActor->ActorHasTag(TEXT("EngageableTarget"))) return;

		// 피격한 액터에게 데미지 전달을 가장 먼저 한다.
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);
		ExcuteGetHit(BoxHit);
		// 필드시스템 
		CreateFields(BoxHit.ImpactPoint);
	}
}

void AFirstWeapon::ExcuteGetHit(FHitResult& BoxHit)
{
	// 인터페이스 임시 변수에 형변환하여 충돌지점이 생긴 액터를 저장한다(인터페이스 헤더 필요)
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		// 인터페이스에서 GetHit 함수 자체를 블루프린트 재지정 형태로 매크로 설정하였기때문에 상속된 클래스들에서는 GetHit_Implementation() 함수를 호출하여 사용하고 있다. 해당 함수가 생성되며 같이 생성되는Execute_GetHit 함수를 사용하여 행동을 실행하도록 재설정
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

void AFirstWeapon::BoxTrace(FHitResult& BoxHit)
{
	// 각 좌표를 저장할 객체는 값이 변하지 않도록 상수처리
	const FVector StartPoint = BoxTraceStartPoint->GetComponentLocation();
	const FVector EndPoint = BoxTraceEndPoint->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	// 한번 히트당한 액터들이저장되어있는 배열을 범위 반복문으로 플레이어 히트 예외처리 한 배열에 다 집어넣어준다(중복방지)
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	// 박스 트레이싱 진행
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
