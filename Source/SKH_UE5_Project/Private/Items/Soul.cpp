#include "Items/Soul.h"
#include "Interface/PickUpInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ��ȯ �� �۵��ϵ���
	if (!isActive) DropUp(DeltaTime);
	if (isActive) DropDown(DeltaTime);
}

void ASoul::BeginPlay()
{
	Super::BeginPlay();

	// �θ��� ����Ʈ���̽� �Լ� ����
	StartLineTracing();
}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickupInterface && IsOverlapActive())
	{
		PickupInterface->AddSouls(this);

		SpawnPickupSystem();
		SpawnPickupSound();

		Destroy();
	}
}
