#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Character/CharacterTypes.h"
#include "BaseCharacter.generated.h"

class AFirstWeapon;
class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;
class UNiagaraSystem;
class UMaterialInterface;

UCLASS()
class SKH_UE5_PROJECT_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void DisableCapsule();
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);

	// 충돌 콜라이더 세팅용 함수
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	// 공격 함수
	virtual void Attack();

	// 사망 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Die();
	//virtual void Die();

	// 몽타주 재생 함수
	void PlayHitReactMontage(const FName& SectionName);
	void PlayHitLargeReactMontage(const FName& SectionName);
	void PlayDeathReactMontage(const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayAttackMontage2();
	virtual int32 PlayDeathMontage();
	virtual void PlayDodgeMontage();
	void StopAttackMontage();

	void DisableMeshCollision();

	// 사망시 포즈
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TEnumAsByte<EDeathPose> DeathPose;

	/* Death Effect */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Death)
	float DeathEffectDelay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Death)
	UNiagaraSystem* NiagaraDeathEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Death)
	UMaterialInterface* DissolveMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Death)
	FLinearColor EffectBaseColor;
	/* Death Effect End */

	// 모션워핑 관련
	UPROPERTY(EditAnywhere, Category = Combat)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = Combat)
	double WarpTargetDistance = 75.f;

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	// 공격 데미지
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackDamageLow = 5.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackDamageMiddle = 10.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackDamageHigh = 15.f;

	// 피격 데미지 함수
	virtual void HandleDamage(float DamageAmount);

	// 피격 방향 게산 함수
	void DirectionalHitReact(const FVector& ImpactPoint);
	FName DirectionalHitReactName(const FVector& ImpactPoint);

	// 현재 캐릭터의 상태를 체크하는 함수
	virtual bool CanAttack(); // 공격할수 있는지

	// 노티파이
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	AFirstWeapon* EquippedWeapon;

	// 직접 작성한 속성(체력 등)의 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;

	// 체력바 위젯
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	// 무기 장착할 소켓 이름 지정
	UPROPERTY(EditAnywhere, Category = Combat)
	FName WeaponSocketName = "WeaponSocket";

	UPROPERTY(EditAnywhere, Category = Combat)
	FName MagicWeaponSocketName = "MagicWeaponSocket";

	// 공격 타입 관련
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackType")
	EAttackType AttackType = EAttackType::EAT_Normal;

	UFUNCTION(BlueprintCallable)
	void SetAttackTypeNormal();

	UFUNCTION(BlueprintCallable)
	void SetAttackTypeStrong();

private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	// 피격사운드 재생용
	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase* HitSound;

	// 피격 파티클
	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem* HitParticle;

	// 애니메이션 몽타주 관련
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* AttackMontage2;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* LargeHitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<FName> AttackMontageSections2;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<FName> DeathMontageSections;

public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }
	FORCEINLINE int32 GetDeathMontageSections() { return DeathMontageSections.Num(); }
	FORCEINLINE UAnimMontage* GetAttackMontage() { return AttackMontage; }
	FORCEINLINE UAnimMontage* GetAttackMontage2() { return AttackMontage2; }
	FORCEINLINE EAttackType GetAttackType() { return AttackType; }

	UFUNCTION(BlueprintCallable)
	void SetDamageLow();

	UFUNCTION(BlueprintCallable)
	void SetDamageMiddle();

	UFUNCTION(BlueprintCallable)
	void SetDamageHigh();
};
