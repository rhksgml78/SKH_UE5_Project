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

	// 지오메트리컬렉션 컴포넌트를 루트로지정
	SetRootComponent(GeometryCollection);

	// 지오메트리 컬렉션 컴포넌트의 상세 지정하기
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	// 우선 모든 콜리전과 충돌을 제외시켜두고
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// 폰하고만 충돌하도록 다시 설정
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
	// 트레저클래스의 멤버가 0보다클때 즉 배열이 있을때
	if (World && TreasureClasses.Num() > 0)
	{

		UC_GameInstance* GameInstance = Cast<UC_GameInstance>(GetGameInstance());

		FString ItemName = GameInstance->GetItemName(UKismetMathLibrary::RandomIntegerInRange(0, GameInstance->GetAllItemNum() - 1));

		FVector Location = GetActorLocation();
		//FVector Location = GeometryCollection->GetComponentLocation();
		
		Location.Z += 50.f;

		// 지정된 배열중 랜덤한 아이템 드랍
		const int32 RandomDrop = FMath::RandRange(0, TreasureClasses.Num() - 1);
		World->SpawnActor<ATreasure>(TreasureClasses[RandomDrop], Location, GetActorRotation());
	}
}

