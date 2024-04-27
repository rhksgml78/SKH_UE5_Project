#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraSystem;
class UNiagaraComponent;

enum class EItemState : uint8
{
	EIS_UnEquipped UMETA(DisplayName = "UnEquipped"),
	EIS_Equipped UMETA(DisplayName = "Equipped")
};

UCLASS()
class SKH_UE5_PROJECT_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();
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

	//델리게이트
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	// 무기의 상태
	EItemState ItemState = EItemState::EIS_UnEquipped;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* WidgetSphere;

	// 이펙트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	class UNiagaraComponent* WeaponEffect;

	// 사운드
	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;



	/* 드랍된 아이템(소울,회복포션)과 관련된 데이터 */

	// 라인트레이스 결과 데이터 저장용 변수
	float DesiredZ;
	float LittleUpDesiredZ;
	float MoveChangeDesiredZ;

	// 소환시 아래로 하강할 속도
	UPROPERTY(EditAnywhere, Category = "Item Properties")
	float DropItemDownSpeed = -100.f;
	UPROPERTY(EditAnywhere, Category = "Item Properties")
	float DropItemUpSpeed = 300.f;

	// 첫소환시 위로 상승하도록 하는 변수
	bool isActive = false;
	
	// 아이템 드랍시 상승 하강 함수
	void DropUp(float DeltaTime);
	void DropDown(float DeltaTime);

	// 라인 트레이싱 함수
	void StartLineTracing();

	// 오버랩 실행 조건문
	bool IsOverlapActive();

	/* 드랍된 아이템과 관련된 데이터 끝 */



private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Runningtime;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

public:
	FORCEINLINE UStaticMeshComponent* GetMesh() { return ItemMesh; }
	FORCEINLINE void ItemMeshVisible(bool Value);

};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}