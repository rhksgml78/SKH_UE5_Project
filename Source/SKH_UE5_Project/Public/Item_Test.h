// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Test.generated.h"

class USphereComponent;

UCLASS()
class SKH_UE5_PROJECT_API AItem_Test : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem_Test();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCosin();

	template<typename T>
	T Avg(T First, T Second);

	//µ®∏Æ∞‘¿Ã∆Æ
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Runningtime;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;


};

template<typename T>
inline T AItem_Test::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
