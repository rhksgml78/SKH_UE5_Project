#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Breakable/C_GameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));

	// ������Ʈ���÷��� ������Ʈ�� ��Ʈ������
	SetRootComponent(GeometryCollection);

	// ������Ʈ�� �÷��� ������Ʈ�� �� �����ϱ�
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	// �켱 ��� �ݸ����� �浹�� ���ܽ��ѵΰ�
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// ���ϰ� �浹�ϵ��� �ٽ� ����
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBroken) return;

	bBroken = true;


	UWorld* World = GetWorld();
	// Ʈ����Ŭ������ ����� 0����Ŭ�� �� �迭�� ������
	if (World && TreasureClasses.Num() > 0)
	{

		UC_GameInstance* GameInstance = Cast<UC_GameInstance>(GetGameInstance());

		FString ItemName = GameInstance->GetItemName(UKismetMathLibrary::RandomIntegerInRange(0, GameInstance->GetAllItemNum() - 1));

		FVector Location = GetActorLocation();
		//FVector Location = GeometryCollection->GetComponentLocation();
		
		Location.Z += 50.f;

		// ������ �迭�� ������ ������ ���
		const int32 RandomDrop = FMath::RandRange(0, TreasureClasses.Num() - 1);
		World->SpawnActor<ATreasure>(TreasureClasses[RandomDrop], Location, GetActorRotation());
	}
}

