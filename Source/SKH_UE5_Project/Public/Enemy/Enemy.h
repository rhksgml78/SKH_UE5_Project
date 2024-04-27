#pragma once
#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;

UCLASS()
class SKH_UE5_PROJECT_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemy();

	/* AActor */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/* AActor End */

	/* Interface */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/* Interface End  */

protected:
	/* AActor */
	virtual void BeginPlay() override;
	/* AActor End */

	/* ABaseCharacter */
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;
	virtual int32 PlayDeathMontage() override;
	virtual void Die_Implementation() override;
	/* ABaseCharacter End */


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EEnemyState EnemyState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyType")
	EEnemyTypes EnemyTypes = EEnemyTypes::ET_Normal;

private:
	/* AI Behavior */
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* TargetTemp, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	bool HalfHealthOver();

	// 스폰 관련
	void SpawnDefaultWeapon();

	void SpawnSoul();

	void SapwnHealthPotion();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing; // 콜백 U폰센싱컴포넌트

	// 멤버변수들 엔진에서 수정 가능
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFirstWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = Combat)
	double CombatRadius = 750.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	double AcceptanceRadius = 50.f;

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navi")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navi")
	TArray<AActor*> PatrolTargets; 

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 300.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navi")
	float PatrolWaitMinTime = 2.f;

	UPROPERTY(EditAnywhere, Category = "AI Navi")
	float PatrolWaitMaxTime = 5.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrolSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.0f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChaseSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeeathLifeSpan = 5.f;
	/* AI Behavior End */

	UPROPERTY(EditAnywhere, Category = combat)
	TSubclassOf<class ASoul> SoulClass;

	UPROPERTY(EditAnywhere, Category = combat)
	TSubclassOf<class AHealthPotion> HealthPotionClass;
};
