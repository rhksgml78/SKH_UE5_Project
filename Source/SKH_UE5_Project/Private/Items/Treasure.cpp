#include "Items/Treasure.h"
#include "Components/SphereComponent.h"
#include "Interface/PickUpInterface.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->AddGold(this);
		SpawnPickupSound();
		Destroy();
	}
}
