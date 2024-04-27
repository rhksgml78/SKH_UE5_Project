#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "HealthPotion.generated.h"

UCLASS()
class SKH_UE5_PROJECT_API AHealthPotion : public AItem
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	//µ®∏Æ∞‘¿Ã∆Æ
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "HealthProperties")
	int32 AddHealth;

public:
	FORCEINLINE int32 GetAddHealth() const { return AddHealth; }
	FORCEINLINE void SetAddHealthScale(int32 NumberOfHealth) { AddHealth = NumberOfHealth; }
};
