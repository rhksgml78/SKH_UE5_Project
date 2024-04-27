#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"


UCLASS()
class SKH_UE5_PROJECT_API ASoul : public AItem
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	//µ®∏Æ∞‘¿Ã∆Æ
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 Souls;

public:
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE void SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }
};
