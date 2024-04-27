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

	// �浹 �ݶ��̴� ���ÿ� �Լ�
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	// ���� �Լ�
	virtual void Attack();

	// ��� �Լ�
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Die();
	//virtual void Die();

	// ��Ÿ�� ��� �Լ�
	void PlayHitReactMontage(const FName& SectionName);
	void PlayHitLargeReactMontage(const FName& SectionName);
	void PlayDeathReactMontage(const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayAttackMontage2();
	virtual int32 PlayDeathMontage();
	virtual void PlayDodgeMontage();
	void StopAttackMontage();

	void DisableMeshCollision();

	// ����� ����
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

	// ��ǿ��� ����
	UPROPERTY(EditAnywhere, Category = Combat)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = Combat)
	double WarpTargetDistance = 75.f;

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	// ���� ������
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackDamageLow = 5.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackDamageMiddle = 10.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackDamageHigh = 15.f;

	// �ǰ� ������ �Լ�
	virtual void HandleDamage(float DamageAmount);

	// �ǰ� ���� �Ի� �Լ�
	void DirectionalHitReact(const FVector& ImpactPoint);
	FName DirectionalHitReactName(const FVector& ImpactPoint);

	// ���� ĳ������ ���¸� üũ�ϴ� �Լ�
	virtual bool CanAttack(); // �����Ҽ� �ִ���

	// ��Ƽ����
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	AFirstWeapon* EquippedWeapon;

	// ���� �ۼ��� �Ӽ�(ü�� ��)�� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;

	// ü�¹� ����
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	// ���� ������ ���� �̸� ����
	UPROPERTY(EditAnywhere, Category = Combat)
	FName WeaponSocketName = "WeaponSocket";

	UPROPERTY(EditAnywhere, Category = Combat)
	FName MagicWeaponSocketName = "MagicWeaponSocket";

	// ���� Ÿ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackType")
	EAttackType AttackType = EAttackType::EAT_Normal;

	UFUNCTION(BlueprintCallable)
	void SetAttackTypeNormal();

	UFUNCTION(BlueprintCallable)
	void SetAttackTypeStrong();

private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	// �ǰݻ��� �����
	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase* HitSound;

	// �ǰ� ��ƼŬ
	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem* HitParticle;

	// �ִϸ��̼� ��Ÿ�� ����
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
