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

	// 메시 컴포넌트 세팅
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	// 폰센싱 컴포넌트 세팅
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("UPawnSensingComponent"));
	PawnSensing->SightRadius = 4000.f; // 시야거리
	PawnSensing->SetPeripheralVisionAngle(45.f); // 시야각

	// 체력바 위젯
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	// 캐릭터 움직임 관련 초기 설정
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

	// 적의상태는 0(순찰) 1(추적) 2(공격)
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
	// 공격한 상대를 타겟으로 지정하고 추적으로 변경
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

	// 피격시 초기화 하거나 멈춰야 할것들
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
		// 몬스터의 체력이 남아있다면 피격방향 계산후 몽타주 재생
		//PlayHitReactMontage(DirectionalHitReactName(ImpactPoint));
		PlayHitReactMontage(DirectionalHitReactName(Hitter->GetActorLocation()));
	}
	if (!Attributes->IsAlive())
	{
		if (GetDeathMontageSections() < 1)
		{
			// 몬스터의 체력 이없다면 피격방향의 이름을 받아서피격당했을때와는 다르게 피격방향의 반대로 죽는다.
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

	// 피격시 사운드 파티클 재생
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 게임시작할때에 적의 컨트롤러 값을 넣어준다
	EnemyController = Cast<AAIController>(GetController());

	// 폰센싱 실행용 함수 바인딩
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	
	// 처음실행시에는 체력바를 안보이도록 세팅
	HideHealthBar();
	MoveToTarget(PatrolTarget);

	// 기본무기 생성후 바로 장착
	SpawnDefaultWeapon();

	// 판정용 태그 설정
	Tags.Add(FName("Enemy"));
}

void AEnemy::Attack()
{
	// 부모클래스에서 정의한기능이없으므로 Super 선언 X
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
	// 상속하는 부모의 함수를 먼저 실행하여 데미지를 전달
	Super::HandleDamage(DamageAmount);
	// 속성 컴포넌트가 있을때
	if (Attributes && HealthBarWidget)
	{
		// 상속된 함수에서는 헬스바위젯의 퍼센테이지를 조절하는 작업을 한다.
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

	// 적이 죽을때 소울or포션을 스폰 시킨다.
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
	// PatrolRadius = 200.f 사거리안에 들어왔을떄 실행
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		// 목표 타겟을 설정
		PatrolTarget = ChoosePatrolTarget();

		// 콜백 타이머를 사용한 이동사용(즉, 정해진시간의 딜레이 후에 행동이 실행됨)
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
	// 체력바 숨기기
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
	// 타이머핸들을 사용하여 해당 클래스의 함수를 랜덤한 시간에 실행 시키도록 한다.
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

	// AI 네비관련된 것들 추가
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	FNavPathSharedPtr NavPath;
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;

	// 중복방지 처리를 위한 배열 생성
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
		
		//함수가실행되고 조건에 맞게된다면 임의의 타겟을 반환시킨다.
		return ValidTargets[RandomSelectTarget];
	}

	// 위의 조건에 맞지 않다면 nullptr 반환 시키기
	return nullptr;
}

void AEnemy::SpawnDefaultWeapon()
{
	// 무기장착
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AFirstWeapon* DefaultWeapon = World->SpawnActor<AFirstWeapon>(WeaponClass);

		//DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		DefaultWeapon->Equip(GetMesh(), WeaponSocketName, this, this);
		EquippedWeapon = DefaultWeapon;
		// 무기장착하고 별도로 설정한 데미지값 세팅
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
