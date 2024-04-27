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

	// �Ӽ� ����
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// ĸ�� ������Ʈ ����
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
	// ������� ��Ÿ�ָ� �����Ų��.
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
	// �迭�� ������� -1 ��ȯ
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
		// �Ÿ������� ���� ũ�Ⱑ�ٸ� �Լ��� AtLocation
		// �Ÿ���������� �ϰ����� ����� 2D Sound
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
	// �Ӽ� ������Ʈ�� ������
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	// �ǰݴ��� ���⿡���� ��Ÿ�� �����Ű�����ؼ��� DotProduct�� ��� �ؾ� �Ѵ�. DotProduct�� ������ ������ ���Ϳ� �ǰݴ��� ��ġ�� ���⺤�Ͱ� �ʿ��ϴ�.
	const FVector Forward = GetActorForwardVector();
	// Z��ġ���� ������ ��ġ��������
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theata)
	// |Forward| = 1, |Tohit| = 1, so Forward * Tohit = cos(teata)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// cos(theta)�� ���ڻ���(arc - cos)�� ���Ͽ� theta���ϱ�
	double Theta = FMath::Acos(CosTheta);

	// ���� ������ Theta�� ��׸�(��)������ ��ȯ
	Theta = FMath::RadiansToDegrees(Theta);

	// ������� ������ ��� ����� ǥ���ȴ�. ������ ���� ������ �������� ��������� �Ǵ��� ���� �ʿ��ϴ�. Cross Product �� ����Ѵ� (������ ��ǥ��) ��, �𸮾����� �޼���ǥ���̹Ƿ� �ݴ밡�ȴ�!
	// ũ�ν� ���δ�Ʈ�� ����Ʈ(Z)�� �Ʒ������̸� �����̴�
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		// �� Z�� 0���������� ������ ��ȯ�����ش�
		Theta *= -1.f;
	}

	// ����� ��Ÿ�ִ� �⺻ FormBack ���� �صΰ� �Ʒ��� ���ǹ��� üũ�Ͽ� ����(Theta)�� ���� �ش�������ִٸ� ������ �̸��� �ٲܰ��̴�. ������ else �� ���� ������ �� �ִ�.
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

	// �ǰ����� ���Ͱ� ������� ���
	//if (Attributes->IsAlive())
	//{
	//	PlayHitReactMontage(Section);
	//}
	
	//else
	//{
	//	// �ǰݴ��������ʹ� �ٸ��� �ǰݹ����� �ݴ�� �״´�.
	//	if (Section == FName("FromFront")) DeathPose = EDeathPose::EDP_DeathBack;

	//	if (Section == FName("FromBack")) DeathPose = EDeathPose::EDP_DeathFront;

	//	if (Section == FName("FromLeft")) DeathPose = EDeathPose::EDP_DeathRight;

	//	if (Section == FName("FromRight")) DeathPose = EDeathPose::EDP_DeathLeft;

	//	// ������ ü���� 0���� ������ ��� ��Ÿ�� ���
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
