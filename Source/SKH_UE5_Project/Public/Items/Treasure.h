#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"



UCLASS()
class SKH_UE5_PROJECT_API ATreasure : public AItem
{
	GENERATED_BODY()

protected:
	// 부모클래스 Item 에서 카피 매크로 부모에 되어있다.
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Treasure Propeerty")
	int32 Gold;

public:
	FORCEINLINE int32 GetGold() const { return Gold; }
};
