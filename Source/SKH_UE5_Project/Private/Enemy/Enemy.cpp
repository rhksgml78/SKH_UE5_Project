#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapons/FirstWeapon.h"
#include "Items/Soul.h"
#include "Items/HealthPotion.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// �޽� ������Ʈ ����
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	// ������ ������Ʈ ����
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("UPawnSensingComponent"));
	PawnSensing->SightRadius = 4000.f; // �þ߰Ÿ�
	PawnSensing->SetPeripheralVisionAngle(45.f); // �þ߰�

	// ü�¹� ����
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	// ĳ���� ������ ���� �ʱ� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	EnemyState = EEnemyState::EES_Patrolling;
	StartPatrolling();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���ǻ��´� 0(����) 1(����) 2(����)
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	// ������ ��븦 Ÿ������ �����ϰ� �������� ����
	CombatTarget = EventInstigator->GetPawn();

	if (IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
		ChaseTarget();
	}
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (!IsDead()) ShowHealthBar();

	// �ǰݽ� �ʱ�ȭ �ϰų� ����� �Ұ͵�
	ClearPatrolTimer();
	ClearAttackTimer();
	StopAttackMontage();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	if (IsInsideAttackRadius())
	{
		if (!IsDead()) StartAttackTimer();
	}

	if (Attributes->IsAlive())
	{
		// ������ ü���� �����ִٸ� �ǰݹ��� ����� ��Ÿ�� ���
		//PlayHitReactMontage(DirectionalHitReactName(ImpactPoint));
		PlayHitReactMontage(DirectionalHitReactName(Hitter->GetActorLocation()));
	}
	if (!Attributes->IsAlive())
	{
		if (GetDeathMontageSections() < 1)
		{
			// ������ ü�� �̾��ٸ� �ǰݹ����� �̸��� �޾Ƽ��ǰݴ��������ʹ� �ٸ��� �ǰݹ����� �ݴ�� �״´�.
			//FName Section = DirectionalHitReactName(ImpactPoint);
			FName Section = DirectionalHitReactName(Hitter->GetActorLocation());

			if (Section == FName("FromFront")) DeathPose = EDeathPose::EDP_DeathBack;

			if (Section == FName("FromBack")) DeathPose = EDeathPose::EDP_DeathFront;

			if (Section == FName("FromLeft")) DeathPose = EDeathPose::EDP_DeathRight;

			if (Section == FName("FromRight")) DeathPose = EDeathPose::EDP_DeathLeft;

			PlayDeathReactMontage(Section);
		}
		else
		{
			PlayDeathMontage();
		}

		Die_Implementation();
	}

	// �ǰݽ� ���� ��ƼŬ ���
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// ���ӽ����Ҷ��� ���� ��Ʈ�ѷ� ���� �־��ش�
	EnemyController = Cast<AAIController>(GetController());

	// ������ ����� �Լ� ���ε�
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	
	// ó������ÿ��� ü�¹ٸ� �Ⱥ��̵��� ����
	HideHealthBar();
	MoveToTarget(PatrolTarget);

	// �⺻���� ������ �ٷ� ����
	SpawnDefaultWeapon();

	// ������ �±� ����
	Tags.Add(FName("Enemy"));
}

void AEnemy::Attack()
{
	// �θ�Ŭ�������� �����ѱ���̾����Ƿ� Super ���� X
	// Super::Attack()
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}

	if (CombatTarget == nullptr) return;

	EnemyState = EEnemyState::EES_Engaged;
	if (HalfHealthOver())
	{
		PlayAttackMontage();
	}
	if (!HalfHealthOver())
	{
		if (GetAttackMontage2())
		{
			PlayAttackMontage2();
		}
		else
		{
			PlayAttackMontage();
		}
	}
}

bool AEnemy::CanAttack()
{
	bool bCanAttack = IsInsideAttackRadius() && !IsAttacking() && !IsEngaged() && !IsDead();

	return bCanAttack;
}

void AEnemy::HandleDamage(float DamageAmount)
{
	// ����ϴ� �θ��� �Լ��� ���� �����Ͽ� �������� ����
	Super::HandleDamage(DamageAmount);
	// �Ӽ� ������Ʈ�� ������
	if (Attributes && HealthBarWidget)
	{
		// ��ӵ� �Լ������� �ｺ�������� �ۼ��������� �����ϴ� �۾��� �Ѵ�.
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

int32 AEnemy::PlayDeathMontage()
{
	const int32 Selection = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}

void AEnemy::Die_Implementation()
{
	Super::Die_Implementation();

	EnemyState = EEnemyState::EES_Dead;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ClearAttackTimer();
	SetLifeSpan(DeeathLifeSpan);
	DisableCapsule();
	HideHealthBar();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	// ���� ������ �ҿ�or������ ���� ��Ų��.
	if (SoulClass && HealthPotionClass)
	{
		int32 RandomSpawn = FMath::RandRange(0, 1);
		switch (RandomSpawn)
		{
		case 0:
			SpawnSoul();
			break;
		case 1:
			SapwnHealthPotion();
			break;
		default:
			break;
		}

	}
	else if (HealthPotionClass)
	{
		SapwnHealthPotion();
	}
	else if (SoulClass)
	{
		SpawnSoul();
	}

}

void AEnemy::CheckPatrolTarget()
{
	// PatrolRadius = 200.f ��Ÿ��ȿ� �������� ����
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		// ��ǥ Ÿ���� ����
		PatrolTarget = ChoosePatrolTarget();

		// �ݹ� Ÿ�̸Ӹ� ����� �̵����(��, �������ð��� ������ �Ŀ� �ൿ�� �����)
		const float RandomWaitTime = FMath::RandRange(PatrolWaitMinTime, PatrolWaitMaxTime);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, RandomWaitTime);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
	}

	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	}

	else if (CanAttack())
	{
		ClearAttackTimer();
		StartAttackTimer();
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	// ü�¹� �����
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

bool AEnemy::HalfHealthOver()
{
	return Attributes->GetHealthPercent() > 0.5f;
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	// Ÿ�̸��ڵ��� ����Ͽ� �ش� Ŭ������ �Լ��� ������ �ð��� ���� ��Ű���� �Ѵ�.
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;

	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();

	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;

	// AI �׺���õ� �͵� �߰�
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	FNavPathSharedPtr NavPath;
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;

	// �ߺ����� ó���� ���� �迭 ����
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 MaxNumPatrolTarget = ValidTargets.Num();
	if (MaxNumPatrolTarget > 0)
	{
		const int32 RandomSelectTarget = FMath::RandRange(0, MaxNumPatrolTarget - 1);
		
		//�Լ�������ǰ� ���ǿ� �°Եȴٸ� ������ Ÿ���� ��ȯ��Ų��.
		return ValidTargets[RandomSelectTarget];
	}

	// ���� ���ǿ� ���� �ʴٸ� nullptr ��ȯ ��Ű��
	return nullptr;
}

void AEnemy::SpawnDefaultWeapon()
{
	// ��������
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AFirstWeapon* DefaultWeapon = World->SpawnActor<AFirstWeapon>(WeaponClass);

		//DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		DefaultWeapon->Equip(GetMesh(), WeaponSocketName, this, this);
		EquippedWeapon = DefaultWeapon;
		// ���������ϰ� ������ ������ �������� ����
		EquippedWeapon->SetDamage(AttackDamageMiddle);
		
		//EquippedWeapon->GetActorLocation();
		//EquippedWeapon->SetActorLocation(CombatTarget->GetActorLocation());
	}
}

void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass)
	{
		const FVector SpawnLocation = GetActorLocation();
	
		ASoul* SpawnSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
		if (SpawnSoul)
		{
			SpawnSoul->SetSouls(Attributes->GetSouls());
			SpawnSoul->SetOwner(this);
		}
	}
}

void AEnemy::SapwnHealthPotion()
{
	UWorld* World = GetWorld();
	if (World && HealthPotionClass)
	{
		const FVector SpawnLocation = GetActorLocation();

		AHealthPotion* SpawnHealthPotion = World->SpawnActor<AHealthPotion>(HealthPotionClass, SpawnLocation, GetActorRotation());
		if (SpawnHealthPotion)
		{
			SpawnHealthPotion->SetAddHealthScale(Attributes->GetHealthPotionScale());
			SpawnHealthPotion->SetOwner(this);
		}
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget = EnemyState != EEnemyState::EES_Dead && EnemyState != EEnemyState::EES_Chasing && EnemyState < EEnemyState::EES_Attacking && SeenPawn->ActorHasTag(FName("EngageableTarget"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}
