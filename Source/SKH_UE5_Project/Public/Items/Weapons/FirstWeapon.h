#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "FirstWeapon.generated.h"

class USoundBase;
class UBoxComponent;

UCLASS()
class SKH_UE5_PROJECT_API AFirstWeapon : public AItem
{
	GENERATED_BODY()
public:
	AFirstWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void DeactivateEffect();
	void EffectActive();
	void EffectDeactive();
	void DisableSphereCollision();
	void PlayEquipSound();
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	// �ǰݴ��� ���͸� �ѹ��� �ǰݴ��ϵ��� ����ó���� �迭 �ܺ�(�÷��̾�)���� �����Ҽ� �ֵ��� �ۺ����� ����
	TArray<AActor*> IgnoreActors;

	/* Dissolve Effect */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Death)
	float DeathEffectDelay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Death)
	UNiagaraSystem* NiagaraDeathEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Death)
	UMaterialInterface* DissolveMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Death)
	FLinearColor EffectBaseColor;
	/* Dissolve Effect End */
	
protected:
	virtual void BeginPlay() override;

	// �ڽ� �浹ü�� �浹 ������ ��������Ʈ
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ExcuteGetHit(FHitResult& BoxHit);

	// ������Ʈ �ı��� ���� �ʵ� �ý���(C++����, BP����)
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	bool bShowBoxDebug = false;

	// ���������� �ʿ��� ����
	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	USoundBase* EquipSound;

	// ���� �ݶ��̴�
	UPROPERTY(VisibleAnywhere, Category = "Weapon Property")
	UBoxComponent* WeaponBox;

	// �浹�� ����ϴ� Ʈ���̽�
	UPROPERTY(VisibleAnywhere, Category = "Trace")
	USceneComponent* BoxTraceStartPoint;

	UPROPERTY(VisibleAnywhere, Category = "Trace")
	USceneComponent* BoxTraceEndPoint;

	// ������ ���ݷ� �����ϱ�
	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	float Damage = 20;

public:
	// ������ Ÿ�� �����ϱ�
	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	EItemTypes WeaponType = EItemTypes::EIT_None;

	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

	UFUNCTION(BlueprintCallable)
	void SetDamage(float ChangeDamage);

	UFUNCTION(BlueprintCallable)
	float GetDamage() { return Damage; }
};
