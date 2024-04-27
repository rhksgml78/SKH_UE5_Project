#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"



UCLASS()
class SKH_UE5_PROJECT_API ATreasure : public AItem
{
	GENERATED_BODY()

protected:
	// �θ�Ŭ���� Item ���� ī�� ��ũ�� �θ� �Ǿ��ִ�.
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Treasure Propeerty")
	int32 Gold;

public:
	FORCEINLINE int32 GetGold() const { return Gold; }
};
