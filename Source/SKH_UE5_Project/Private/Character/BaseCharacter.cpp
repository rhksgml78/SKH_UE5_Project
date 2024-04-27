#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/FirstWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "NiagaraSystem.h"
#include "Materials/MaterialInterface.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 속성 생성
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// 캡슐 컴포넌트 세팅
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Die_Implementation()
{
	Tags.Add(FName("Dead"));
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayAttackMontage2()
{
	return PlayRandomMontageSection(AttackMontage2, AttackMontageSections2);
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::DodgeEnd()
{
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::PlayHitLargeReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && LargeHitReactMontage)
	{
		AnimInstance->Montage_Play(LargeHitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, LargeHitReactMontage);
	}
}

void ABaseCharacter::PlayDeathReactMontage(const FName& SectionName)
{
	// 사망시의 몽타주를 재생시킨다.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
}

void ABaseCharacter::SetAttackTypeNormal()
{
	AttackType = EAttackType::EAT_Normal;
}

void ABaseCharacter::SetAttackTypeStrong()
{
	AttackType = EAttackType::EAT_Strong;
}

void ABaseCharacter::PlayMontageSection(UAnimMontage*  Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
	}
	AnimInstance->Montage_JumpToSection(SectionName, Montage);
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	// 배열이 없을경우 -1 반환
	if (SectionNames.Num() <= 0) return -1;

	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);

	return Selection;
}

void ABaseCharacter::SetDamageLow()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SetDamage(AttackDamageLow);
		//UE_LOG(LogTemp, Warning, TEXT("SetDamage : %f"), AttackDamageLow);
	}

}

void ABaseCharacter::SetDamageMiddle()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SetDamage(AttackDamageMiddle);
		//UE_LOG(LogTemp, Warning, TEXT("SetDamage : %f"), AttackDamageMiddle);
	}
}

void ABaseCharacter::SetDamageHigh()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SetDamage(AttackDamageHigh);
		//UE_LOG(LogTemp, Warning, TEXT("SetDamage : %f"), AttackDamageHigh);
	}
}

int32 ABaseCharacter::PlayDeathMontage()
{
	return PlayRandomMontageSection(DeathMontage, DeathMontageSections);
}

void ABaseCharacter::PlayDodgeMontage()
{
	if (DodgeMontage)
	{
		PlayMontageSection(DodgeMontage, FName("Default"));
	}
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget == nullptr) return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToSelf = (Location - CombatTargetLocation).GetSafeNormal();

	TargetToSelf *= WarpTargetDistance;

	return CombatTargetLocation + TargetToSelf;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		// 거리에따라 사운드 크기가다른 함수는 AtLocation
		// 거리에상관없이 일관적인 사운드는 2D Sound
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticle && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			ImpactPoint
		);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	// 속성 컴포넌트가 있을때
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	// 피격당한 방향에따라 몽타주 재생시키기위해서으 DotProduct를 사용 해야 한다. DotProduct는 액터의 포워드 벡터와 피격당한 위치의 방향벡터가 필요하다.
	const FVector Forward = GetActorForwardVector();
	// Z위치값은 액터의 위치기준으로
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theata)
	// |Forward| = 1, |Tohit| = 1, so Forward * Tohit = cos(teata)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// cos(theta)의 역코사인(arc - cos)을 취하여 theta구하기
	double Theta = FMath::Acos(CosTheta);

	// 라디안 각도인 Theta를 디그리(도)단위로 변환
	Theta = FMath::RadiansToDegrees(Theta);

	// 여기까지 각도는 모두 양수로 표현된다. 때문에 계산된 각도가 음수인지 양수인지를 판단할 식이 필요하다. Cross Product 를 사용한다 (오른손 좌표계) 단, 언리얼엔진은 왼손좌표계이므로 반대가된다!
	// 크로스 프로덕트의 포인트(Z)가 아래방향이면 음수이다
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		// 즉 Z가 0보다작으면 음수로 변환시켜준다
		Theta *= -1.f;
	}

	// 재생할 몽타주는 기본 FormBack 으로 해두고 아래의 조건문을 체크하여 각도(Theta)의 값이 해당사항이있다면 섹션의 이름을 바꿀것이다. 때문에 else 문 없이 진행할 수 있다.
	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}

	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}

	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	// 피격직후 몬스터가 살아있을 경우
	//if (Attributes->IsAlive())
	//{
	//	PlayHitReactMontage(Section);
	//}
	
	//else
	//{
	//	// 피격당했을때와는 다르게 피격방향의 반대로 죽는다.
	//	if (Section == FName("FromFront")) DeathPose = EDeathPose::EDP_DeathBack;

	//	if (Section == FName("FromBack")) DeathPose = EDeathPose::EDP_DeathFront;

	//	if (Section == FName("FromLeft")) DeathPose = EDeathPose::EDP_DeathRight;

	//	if (Section == FName("FromRight")) DeathPose = EDeathPose::EDP_DeathLeft;

	//	// 몬스터의 체력이 0보다 작으면 사망 몽타주 재생
	//	PlayDeathReactMontage(Section);

	//	if (HealthBarWidget) HealthBarWidget->SetVisibility(false);
	//}
}

FName ABaseCharacter::DirectionalHitReactName(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	if (Theta >= -45.f && Theta < 45.f)
	{
		return FName("FromFront");
	}

	else if (Theta >= -135.f && Theta < -45.f)
	{
		return FName("FromLeft");
	}

	else if (Theta >= 45.f && Theta < 135.f)
	{
		return FName("FromRight");
	}

	else return FName("FromBack");
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
