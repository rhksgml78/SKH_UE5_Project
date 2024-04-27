// Fill out your copyright notice in the Description page of Project Settings.
#include "Item_Test.h"
#include "SKH_UE5_Project/DebugMacros.h"
#include "Components/SphereComponent.h"

AItem_Test::AItem_Test()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
}

void AItem_Test::BeginPlay()
{
	Super::BeginPlay();

	//델리게이트 초기화는 생성시가아닌 게임실행시가 적절하다
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem_Test::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem_Test::OnSphereEndOverlap);
}

float AItem_Test::TransformedCosin()
{
	return Amplitude * FMath::Cos(Runningtime * TimeConstant);
}

void AItem_Test::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Orange, OtherActorName);
	}
}

void AItem_Test::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Cyan, OtherActorName);
	}
}



float AItem_Test::TransformedSin()
{
	return Amplitude * FMath::Sin(Runningtime * TimeConstant);
}

void AItem_Test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Runningtime += DeltaTime;
	

}
 
