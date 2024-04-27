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

	//��������Ʈ
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	// ������ ����
	EItemState ItemState = EItemState::EIS_UnEquipped;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* WidgetSphere;

	// ����Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	class UNiagaraComponent* WeaponEffect;

	// ����
	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;



	/* ����� ������(�ҿ�,ȸ������)�� ���õ� ������ */

	// ����Ʈ���̽� ��� ������ ����� ����
	float DesiredZ;
	float LittleUpDesiredZ;
	float MoveChangeDesiredZ;

	// ��ȯ�� �Ʒ��� �ϰ��� �ӵ�
	UPROPERTY(EditAnywhere, Category = "Item Properties")
	float DropItemDownSpeed = -100.f;
	UPROPERTY(EditAnywhere, Category = "Item Properties")
	float DropItemUpSpeed = 300.f;

	// ù��ȯ�� ���� ����ϵ��� �ϴ� ����
	bool isActive = false;
	
	// ������ ����� ��� �ϰ� �Լ�
	void DropUp(float DeltaTime);
	void DropDown(float DeltaTime);

	// ���� Ʈ���̽� �Լ�
	void StartLineTracing();

	// ������ ���� ���ǹ�
	bool IsOverlapActive();

	/* ����� �����۰� ���õ� ������ �� */



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