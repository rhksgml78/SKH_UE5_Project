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

	//��������Ʈ �ʱ�ȭ�� �����ð��ƴ� ���ӽ���ð� �����ϴ�
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Runningtime += DeltaTime;

	// �������� ���°� �������̸� ���Ʒ��ο����δ�.
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
		// �ִ��³��̰����� �������� ���� false�� ��ȯ��Ų��.
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
	// ����Ʈ���̽��� �ʿ��� �ӽú�����
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 5000.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult HitResult;
	// ����Ʈ���̽� �����ϰ� HitResult�� ��ġ�����
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
