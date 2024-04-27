#include "Items/Item.h"
#include "SKH_UE5_Project/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Interface/PickUpInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	WidgetSphere = CreateDefaultSubobject<USphereComponent>(TEXT("WidgetSphere"));
	WidgetSphere->SetupAttachment(GetRootComponent());

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);

	WeaponEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeaponEffect"));
	WeaponEffect->SetupAttachment(RootComponent);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	//델리게이트 초기화는 생성시가아닌 게임실행시가 적절하다
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Runningtime += DeltaTime;

	// 아이템의 상태가 장착전이면 위아래로움직인다.
	if (ItemState == EItemState::EIS_UnEquipped)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(Runningtime * TimeConstant);
}

float AItem::TransformedCosin()
{
	return Amplitude * FMath::Cos(Runningtime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

void AItem::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}

void AItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}

void AItem::DropUp(float DeltaTime)
{
	const float LocationZ = GetActorLocation().Z;
	if (LocationZ < MoveChangeDesiredZ)
	{
		const FVector DeltaLocation = FVector(0.f, 0.f, (DropItemUpSpeed * DeltaTime));
		AddActorWorldOffset(DeltaLocation);
	}
	if (LocationZ > MoveChangeDesiredZ)
	{
		// 최대상승높이값보다 높아지면 값을 false로 변환시킨다.
		isActive = true;
	}
}

void AItem::DropDown(float DeltaTime)
{
	const float LocationZ = GetActorLocation().Z;
	if (LocationZ > DesiredZ)
	{
		const FVector DeltaLocation = FVector(0.f, 0.f, (DropItemDownSpeed * DeltaTime));
		AddActorWorldOffset(DeltaLocation);
	}
}

void AItem::StartLineTracing()
{
	// 라인트레이스에 필요한 임시변수들
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 5000.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult HitResult;
	// 라인트레이스 진행하고 HitResult에 위치저장됨
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	MoveChangeDesiredZ = HitResult.ImpactPoint.Z + 250.f;
	LittleUpDesiredZ = HitResult.ImpactPoint.Z + 70.f;
	DesiredZ = HitResult.ImpactPoint.Z + 50.f;
}

bool AItem::IsOverlapActive()
{
	return isActive&& (GetActorLocation().Z < LittleUpDesiredZ);
}



void AItem::ItemMeshVisible(bool Value)
{
	ItemMesh->SetVisibility(Value);
	//Sphere->SetGenerateOverlapEvents(Value);
}
