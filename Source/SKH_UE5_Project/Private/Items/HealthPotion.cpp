#include "Items/HealthPotion.h"
#include "Interface/PickUpInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void AHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ��ȯ �� �۵��ϵ���
	if (!isActive) DropUp(DeltaTime);
	if (isActive) DropDown(DeltaTime);
}

void AHealthPotion::BeginPlay()
{
	Super::BeginPlay();

	// �θ��� ����Ʈ���̽� �Լ� ����
	StartLineTracing();
}

void AHealthPotion::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickupInterface && IsOverlapActive())
	{
		PickupInterface->AddHealth(this);

		SpawnPickupSystem();
		SpawnPickupSound();

		Destroy();
	}
}
