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

	// 피격당한 액터를 한번만 피격당하도록 예외처리할 배열 외부(플레이어)에서 접근할수 있도록 퍼블릭에서 생성
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

	// 박스 충돌체와 충돌 판정할 델리게이트
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ExcuteGetHit(FHitResult& BoxHit);

	// 오브젝트 파괴를 위한 필드 시스템(C++생성, BP정의)
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	bool bShowBoxDebug = false;

	// 무기장착시 필요한 음향
	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	USoundBase* EquipSound;

	// 무기 콜라이더
	UPROPERTY(VisibleAnywhere, Category = "Weapon Property")
	UBoxComponent* WeaponBox;

	// 충돌에 사용하는 트레이스
	UPROPERTY(VisibleAnywhere, Category = "Trace")
	USceneComponent* BoxTraceStartPoint;

	UPROPERTY(VisibleAnywhere, Category = "Trace")
	USceneComponent* BoxTraceEndPoint;

	// 무기의 공격력 설정하기
	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	float Damage = 20;

public:
	// 무기의 타입 설정하기
	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	EItemTypes WeaponType = EItemTypes::EIT_None;

	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

	UFUNCTION(BlueprintCallable)
	void SetDamage(float ChangeDamage);

	UFUNCTION(BlueprintCallable)
	float GetDamage() { return Damage; }
};
