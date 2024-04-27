#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/FirstWeapon.h"
#include "Animation/AnimMontage.h"
#include "Components/AttributeComponent.h"
#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Items/HealthPotion.h"
#include "Character/BaseCharacter.h"
#include "Enemy/Enemy.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	//��Ʈ�� �������� �޾��ֱ�
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	//PlayerOverlay->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	// �θ��� Tick ���X �ش�Ŭ������ ƽ�����
	//Super::Tick(DeltaTime); 

	if (Attributes && PlayerOverlay)
	{
		if (Attributes->GetStamina() < Attributes->GetMaxStamina())
		{
			// ���׹̳� ����ȸ��
			Attributes->RegenStamina(DeltaTime);

			// ȸ���� ���׹̳ʷ� HUD ����
			PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
		}
		if (Attributes->GetChange() < Attributes->GetMaxChange() && ChangeType == ECharacterChangeType::ECCY_Human)
		{
			// ���Ű����� ��������
			Attributes->RegenChange(DeltaTime);

			// ���Ű����� HUD ����
			PlayerOverlay->SetChangeBarPercent(Attributes->GetChangePercent());
		}
		if (Attributes->GetChange() > 0 && ChangeType == ECharacterChangeType::ECCY_Dragon)
		{
			// ���Ű����� ���Ӱ���
			Attributes->UnRegenChange(DeltaTime);

			// ���Ű����� HUD ����
			PlayerOverlay->SetChangeBarPercent(Attributes->GetChangePercent());
		}

		// ���� �������� Ǯ�϶� ���Ľ�Ƽ�� �����̵���
		if (Attributes->GetChange() == Attributes->GetMaxChange())
		{
			PlayerOverlay->SetChangeProgressOpacity();
		}

		if (ChangeType == ECharacterChangeType::ECCY_Human)
		{
			PlayerOverlay->IsHumanType = true;
		}		
		if (ChangeType == ECharacterChangeType::ECCY_Dragon)
		{
			PlayerOverlay->IsHumanType = false;
		}
		
		// ��ų ��Ÿ�� ����
		SkillCoolTimeRegen(DeltaTime);
	}
}

void APlayerCharacter::SkillCoolTimeRegen(float DeltaTime)
{
	if (Attributes->GetSkill1CooltimePercent() > 0.f)
	{
		Attributes->UnRegenSkill1Cooltime(DeltaTime);
		PlayerOverlay->SetSkillBar1Percent(Attributes->GetSkill1CooltimePercent());
	}
	else if (Attributes->GetSkill1CooltimePercent() <= 0.f)
	{
		PlayerOverlay->SetNum1On();
	}
		
	if (Attributes->GetSkill2CooltimePercent() > 0.f)
	{
		Attributes->UnRegenSkill2Cooltime(DeltaTime);
		PlayerOverlay->SetSkillBar2Percent(Attributes->GetSkill2CooltimePercent());
	}
	else if (Attributes->GetSkill2CooltimePercent() <= 0.f)
	{
		PlayerOverlay->SetNum2On();
	}

	if (Attributes->GetSkill3CooltimePercent() > 0.f)
	{
		Attributes->UnRegenSkill3Cooltime(DeltaTime);
		PlayerOverlay->SetSkillBar3Percent(Attributes->GetSkill3CooltimePercent());
	}
	else if (Attributes->GetSkill3CooltimePercent() <= 0.f)
	{
		PlayerOverlay->SetNum3On();
	}

	if (Attributes->GetSkill4CooltimePercent() > 0.f)
	{
		Attributes->UnRegenSkill4Cooltime(DeltaTime);
		PlayerOverlay->SetSkillBar4Percent(Attributes->GetSkill4CooltimePercent());
	}
	else if (Attributes->GetSkill4CooltimePercent() <= 0.f)
	{
		PlayerOverlay->SetNum4On();
	}

	if (Attributes->GetSkill5CooltimePercent() > 0.f)
	{
		Attributes->UnRegenSkill5Cooltime(DeltaTime);
		PlayerOverlay->SetSkillBar5Percent(Attributes->GetSkill5CooltimePercent());
	}
	else if (Attributes->GetSkill5CooltimePercent() <= 0.f)
	{
		PlayerOverlay->SetNum5On();
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/*Enhanced Input Binding*/
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);

		EnhancedInputComponent->BindAction(PressEkeyAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PressE);

		EnhancedInputComponent->BindAction(Press1keyAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Press1);

		EnhancedInputComponent->BindAction(Press2keyAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Press2);

		EnhancedInputComponent->BindAction(Press3keyAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Press3);

		EnhancedInputComponent->BindAction(Press4keyAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Press4);

		EnhancedInputComponent->BindAction(Press5keyAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Press5);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);

		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dodge);

		EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Wheel);
	}


	// �Լ� ���ε� (������ �׼�)
	//PlayerInputComponent->BindAxis(FName("MoveForward"), this, &APlayerCharacter::MoveForward);
	//PlayerInputComponent->BindAxis(FName("MoveRight"), this, &APlayerCharacter::MoveRight);
	//PlayerInputComponent->BindAxis(FName("Turn"), this, &APlayerCharacter::Turn);
	//PlayerInputComponent->BindAxis(FName("LookUp"), this, &APlayerCharacter::LookUp);
	//PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &APlayerCharacter::Jump);
	//PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &APlayerCharacter::PressE);
	//PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &APlayerCharacter::Attack);
	//PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &APlayerCharacter::Dodge);
}

void APlayerCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

void APlayerCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Attributes->IsAlive())
	{
		ActionState = EActionState::EAS_HitReaction;
		// ���͸� Enemy�� ����ȯ
		AEnemy* HitterEnemy = Cast<AEnemy>(Hitter);
		if (HitterEnemy)
		{
			// ���� ������ ���Ÿ���� ��� 
			if (HitterEnemy->GetAttackType() == EAttackType::EAT_Normal)
			{
				PlayHitReactMontage(DirectionalHitReactName(Hitter->GetActorLocation()));
			}

			// ���� ������ ��Ʈ��Ÿ���� ���
			else if (HitterEnemy->GetAttackType() == EAttackType::EAT_Strong)
			{
				PlayHitLargeReactMontage(DirectionalHitReactName(Hitter->GetActorLocation()));
			}
		}


		//int32 RandomNum = FMath::RandRange(0, 1);
		//if (RandomNum == 0)
		//{
		//	//PlayHitReactMontage(DirectionalHitReactName(ImpactPoint));
		//	PlayHitReactMontage(DirectionalHitReactName(Hitter->GetActorLocation()));
		//}
		//else
		//{
		//	//PlayHitLargeReactMontage(DirectionalHitReactName(ImpactPoint));
		//	PlayHitLargeReactMontage(DirectionalHitReactName(Hitter->GetActorLocation()));
		//}

	}
	if (!Attributes->IsAlive())
	{
		// ������ ü�� �̾��ٸ� �ǰݹ����� �̸��� �޾Ƽ��ǰݴ��������ʹ� �ٸ��� �ǰݹ����� �ݴ�� �״´�.
		//FName Section = DirectionalHitReactName(ImpactPoint);
		FName Section = DirectionalHitReactName(Hitter->GetActorLocation());

		if (Section == FName("FromFront")) DeathPose = EDeathPose::EDP_DeathBack;

		if (Section == FName("FromBack")) DeathPose = EDeathPose::EDP_DeathFront;

		if (Section == FName("FromLeft")) DeathPose = EDeathPose::EDP_DeathRight;

		if (Section == FName("FromRight")) DeathPose = EDeathPose::EDP_DeathLeft;

		PlayDeathReactMontage(Section);
		Die_Implementation();
	}

	// �ǰݽ� ���� ��ƼŬ ���
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void APlayerCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void APlayerCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && PlayerOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		PlayerOverlay->SetSouls(Attributes->GetSouls());
	}
}

void APlayerCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && PlayerOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		PlayerOverlay->SetGold(Attributes->GetGold());
	}
}

void APlayerCharacter::AddHealth(AHealthPotion* HealthPotion)
{
	if (Attributes && PlayerOverlay)
	{
		Attributes->AddHealth(HealthPotion->GetAddHealth());
		PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// �÷��̾��� ��Ʈ�ѷ� Ȯ��
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// ����ý��� Ȯ��
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// ����ý��ۿ� �÷��̾����ؽ�Ʈ �߰�
			Subsystem->AddMappingContext(PlayerContext, 0);
		}
	}
	// ������ �±� ����
	Tags.Add(FName("EngageableTarget"));

	// HUD �ʱ�ȭ
	InitializePlayerOverlay();
	// ���Ű����� HUD ����
	PlayerOverlay->SetChangeBarPercent(Attributes->GetChangePercent());
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// ����Ȥ�� �����߿��� �̵�����������
	if (ActionState != EActionState::EAS_Unoccupied) return;

	// �Էµ� ���� 2D���ͷ� ����
	const FVector2D MovementVector = Value.Get<FVector2D>();
	// ��Ʈ�ѷ��� ȸ���� ����
	const FRotator Rotation = Controller->GetControlRotation();
	// �÷��̾�� Ű�Է¿� ���� Yaw�� ȸ���� ������
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void APlayerCharacter::Wheel(const FInputActionValue& Value)
{
	const float WheelValue = Value.Get<float>() * -10.f;
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + WheelValue, 200.f, 800.f);
}

void APlayerCharacter::MoveForward(float Value)
{
	// ����Ȥ�� �����߿��� �̵�����������
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller && (Value != 0.f))
	{
		//FVector Forward = GetActorForwardVector();
		//AddMovementInput(Forward, Value);

		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	// ����Ȥ�� �����߿��� �̵�����������
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller && (Value != 0.f))
	{
		//FVector Right = GetActorRightVector();
		//AddMovementInput(Right, Value);

		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerCharacter::PressE()
{
	AFirstWeapon* OverlappingWeapon = Cast<AFirstWeapon>(OverlappingItem);

	if (OverlappingWeapon) // �������� ���������ִٸ�
	{
		// ������ ���Ⱑ ������� �ش繫�⸦ �����Ѵ�.
		if (EquippedWeapon == nullptr)
		{
			// �����Ҷ� ������ Ÿ�Կ� ���� �ҵ峪 ���� ���Ⱑ Null �� �ƴϰ� �ȴ�.
			EquipWeapon(OverlappingWeapon);
		}
		// ������ ���Ⱑ �ְ�
		else 
		{
			// �ҵ������ ���� ������ ������Ÿ���� ���ڵ��ϰ��
			if (SwordWeapon == nullptr && OverlappingWeapon->WeaponType == EItemTypes::EIT_OneHnadWeapon)
			{
				//OverlappingWeapon->AttachMeshToSocket(GetMesh(), FName("Holster"));

				OverlappingWeapon->ItemMeshVisible(false);
				SwordWeapon = OverlappingWeapon;

				OverlappingWeapon->Equip(GetMesh(), FName("Holster"), this, this);

				SwordWeapon->EffectDeactive();
			}
			// ���������� ���� ������ ������Ÿ���� �������ϰ��
			if (MagicWeapon == nullptr && OverlappingWeapon->WeaponType == EItemTypes::EIT_OneMagicWeapon)
			{
				//OverlappingWeapon->AttachMeshToSocket(GetMesh(), FName("Holster"));

				OverlappingWeapon->ItemMeshVisible(false);
				MagicWeapon = OverlappingWeapon;

				OverlappingWeapon->Equip(GetMesh(), FName("Holster"), this, this);

				MagicWeapon->EffectDeactive();
			}
		}
	}

	else  // �������� �������̾��ٸ� �����۱�ü
	{
		// �������� ���Ⱑ �ְ� �ҵ�� ���� ���� ��ΰ� �ִٸ�
		if (SwordWeapon && MagicWeapon)
		{
			PlayWeaponChangeMontage(FName("WeaponChange"));
		}
		/*
		������ �ڵ� ���� ���� �׼�
		if (CanDisarm()) Disarm();
		else if (CanArm()) Arm();
		*/
	}







	if (PlayerOverlay->GetVisibility() == ESlateVisibility::Hidden)
	{
		PlayerOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerCharacter::Press1()
{
	if (CanAttack() && PlayerOverlay && Attributes)
	{
		if (Attributes->GetSkill1CooltimePercent() <= 0.f)
		{
			PlaySkillMontage(FName("Attack1"));
			ActionState = EActionState::EAS_Attacking;

			Attributes->UseSkill1();
			PlayerOverlay->SetSkillBar1Percent(Attributes->GetSkill1CooltimePercent());

			// ��ų�� �ڽ�Ʈ��ŭ ���׹̳� ����ϰ� ����
			Attributes->UseStamina(Attributes->GetSkill1Cost());
			PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());

			// SetNum
			PlayerOverlay->SetNum1Off();
		}
	}
}

void APlayerCharacter::Press2()
{
	//if (CanAttack())
	//{
	//	PlaySkillMontage(FName("Attack2"));
	//	ActionState = EActionState::EAS_Attacking;
	//}
	if (CanAttack() && PlayerOverlay && Attributes)
	{
		if (Attributes->GetSkill2CooltimePercent() <= 0.f)
		{
			PlaySkillMontage(FName("Attack2"));
			ActionState = EActionState::EAS_Attacking;

			Attributes->UseSkill2();
			PlayerOverlay->SetSkillBar2Percent(Attributes->GetSkill2CooltimePercent());

			// ��ų�� �ڽ�Ʈ��ŭ ���׹̳� ����ϰ� ����
			Attributes->UseStamina(Attributes->GetSkill2Cost());
			PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
			// SetNum
			PlayerOverlay->SetNum2Off();
		}
	}
}

void APlayerCharacter::Press3()
{
	if (CanAttack() && PlayerOverlay && Attributes)
	{
		if (Attributes->GetSkill3CooltimePercent() <= 0.f)
		{
			PlaySkillMontage(FName("Attack3"));
			ActionState = EActionState::EAS_Attacking;

			Attributes->UseSkill3();
			PlayerOverlay->SetSkillBar3Percent(Attributes->GetSkill3CooltimePercent());

			// ��ų�� �ڽ�Ʈ��ŭ ���׹̳� ����ϰ� ����
			Attributes->UseStamina(Attributes->GetSkill3Cost());
			PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
			// SetNum
			PlayerOverlay->SetNum3Off();
		}
	}
}

void APlayerCharacter::Press4()
{
	if (CanAttack() && PlayerOverlay && Attributes)
	{
		if (Attributes->GetSkill4CooltimePercent() <= 0.f)
		{
			PlaySkillMontage(FName("Attack4"));
			ActionState = EActionState::EAS_Attacking;

			Attributes->UseSkill4();
			PlayerOverlay->SetSkillBar4Percent(Attributes->GetSkill4CooltimePercent());

			// ��ų�� �ڽ�Ʈ��ŭ ���׹̳� ����ϰ� ����
			Attributes->UseStamina(Attributes->GetSkill4Cost());
			PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
			// SetNum
			PlayerOverlay->SetNum4Off();
		}
	}
}

void APlayerCharacter::Press5()
{
	if (CanAttack() && PlayerOverlay && Attributes)
	{
		if (Attributes->GetSkill5CooltimePercent() <= 0.f)
		{
			PlaySkillMontage(FName("Attack5"));
			ActionState = EActionState::EAS_Attacking;

			Attributes->UseSkill5();
			PlayerOverlay->SetSkillBar5Percent(Attributes->GetSkill5CooltimePercent());

			// ��ų�� �ڽ�Ʈ��ŭ ���׹̳� ����ϰ� ����
			Attributes->UseStamina(Attributes->GetSkill5Cost());
			PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
			// SetNum
			PlayerOverlay->SetNum5Off();
		}
	}
}

void APlayerCharacter::Dodge()
{
	// �⺻���¿����� ������ �����ϵ���
	if (ActionState != EActionState::EAS_Unoccupied && (!HasEnoughStamina())) return;

	if (Attributes && PlayerOverlay && HasEnoughStamina())
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());

		// �÷��̾� HUD ���׹̳� ����
		PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());

		// ��Ÿ�� ����Ŀ� ���¸� ������ ����
		PlayDodgeMontage();
		ActionState = EActionState::EAS_Dodge;
	}
}

void APlayerCharacter::Attack()
{
	if (CanAttack())
	{
		if (CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon)
		{
			PlayAttackMontage();
		}
		else if (CharacterState == ECharacterState::ECS_EquippedMagicWeapon)
		{
			PlayAttackMontage2();
		}
		ActionState = EActionState::EAS_Attacking;
	}
}

void APlayerCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
	PlayerOverlay->SetVisibility(ESlateVisibility::Hidden);
}

bool APlayerCharacter::HasEnoughStamina()
{
	// ���׹̳ʰ� 0���� Ŭ��� ��, ���׹̳ʰ� �ִٸ�
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

void APlayerCharacter::EquipWeapon(AFirstWeapon* Weapon)
{
	if (Weapon->WeaponType == EItemTypes::EIT_OneHnadWeapon)
	{
		Weapon->Equip(GetMesh(), WeaponSocketName, this, this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		SwordWeapon = Weapon;
		PlayerOverlay->SetSwordSkill();
	}
	else if (Weapon->WeaponType == EItemTypes::EIT_OneMagicWeapon)
	{
		Weapon->Equip(GetMesh(), MagicWeaponSocketName, this, this);
		CharacterState = ECharacterState::ECS_EquippedMagicWeapon;
		MagicWeapon = Weapon;
		PlayerOverlay->SetMagicSkill();
	}

	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void APlayerCharacter::PlayEquipMontage(const FName& SectionName)
{
	ActionState = EActionState::EAS_EquippingWeapon;
	UAnimInstance* AnimInstance2 = GetMesh()->GetAnimInstance();
	if (AnimInstance2 && EquipMontage)
	{
		AnimInstance2->Montage_Play(EquipMontage);
		AnimInstance2->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void APlayerCharacter::PlayWeaponChangeMontage(const FName& SectionName)
{
	ActionState = EActionState::EAS_EquippingWeapon;
	UAnimInstance* AnimInstance2 = GetMesh()->GetAnimInstance();
	if (AnimInstance2 && WeaponChangeMontage)
	{
		AnimInstance2->Montage_Play(WeaponChangeMontage);
		AnimInstance2->Montage_JumpToSection(SectionName, WeaponChangeMontage);
	}
}

void APlayerCharacter::WeaponChange()
{
	// �������� ���Ⱑ �ְ�
	if (EquippedWeapon)
	{
		// �������� ������ Ÿ���� ���ڵ�
		if (EquippedWeapon->WeaponType == EItemTypes::EIT_OneHnadWeapon)
		{
			MagicWeapon->EffectActive();
			EquippedWeapon = MagicWeapon;
			EquippedWeapon->ItemMeshVisible(true);
			SwordWeapon->ItemMeshVisible(false);
			CharacterState = ECharacterState::ECS_EquippedMagicWeapon;
			EquippedWeapon->Equip(GetMesh(), MagicWeaponSocketName, this, this);

			PlayerOverlay->SetMagicSkill();
		}

		else if (EquippedWeapon->WeaponType == EItemTypes::EIT_OneMagicWeapon)
		{
			MagicWeapon->EffectDeactive();
			EquippedWeapon = SwordWeapon;
			EquippedWeapon->ItemMeshVisible(true);
			MagicWeapon->ItemMeshVisible(false);
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			EquippedWeapon->Equip(GetMesh(), WeaponSocketName, this, this);

			PlayerOverlay->SetSwordSkill();
		}
	}
}

void APlayerCharacter::PlaySkillMontage(const FName& SectionName)
{
	ActionState = EActionState::EAS_EquippingWeapon;
	UAnimInstance* AnimInstance2 = GetMesh()->GetAnimInstance();
	if (AnimInstance2 && SwordSkillMontage)
	{
		if (CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon && SwordSkillMontage)
		{
			AnimInstance2->Montage_Play(SwordSkillMontage);
			AnimInstance2->Montage_JumpToSection(SectionName, SwordSkillMontage);
		}

		else if (CharacterState == ECharacterState::ECS_EquippedMagicWeapon && MagicSkillMontage)
		{
			AnimInstance2->Montage_Play(MagicSkillMontage);
			AnimInstance2->Montage_JumpToSection(SectionName, MagicSkillMontage);
		}
	}
}

void APlayerCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
	// �ݸ����� Ȱ��ȭ�Ǵ� ��Ƽ���̿��� ���⿡�� �ѹ� ��Ʈ���� ���͸� ����ó���ϴ� �迭�� ��� ����ֵ����Ͽ� �������ݿ� �ٽ� ��Ʈ������ �����ϵ��� �Ѵ�!
	EquippedWeapon->IgnoreActors.Empty();
}

void APlayerCharacter::DodgeEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool APlayerCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool APlayerCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

void APlayerCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
}

bool APlayerCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

void APlayerCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
}

void APlayerCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("Holster"));
		EquippedWeapon->ItemMeshVisible(false);
		ActionState = EActionState::EAS_EquippingWeapon;
	}
}

void APlayerCharacter::AttackWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("WeaponSocket"));
		EquippedWeapon->ItemMeshVisible(true);
		ActionState = EActionState::EAS_EquippingWeapon;
	}
}

void APlayerCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerCharacter::ReSpawn()
{
	// �±� �缳��
	Tags.Empty(); // ����
	Tags.Add(FName("EngageableTarget"));

	// HUD �缳���ϱ� 
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
		if (PlayerHUD)
		{
			PlayerOverlay = PlayerHUD->GetPlayerOverlay();
			if (PlayerOverlay && Attributes)
			{
				Attributes->SetHealth(Attributes->GetMaxHealth());
				PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				PlayerOverlay->SetStaminaBarPercent(1.f);
			}
		}
	}

	// �������� �ٽ� �����Ű��
	ShowOverlay();

	// �÷��̾� ���� �缳���ϱ�
	ActionState = EActionState::EAS_Unoccupied;
	ChangeType = ECharacterChangeType::ECCY_Human;

	// �ݶ��̴� �ٽ� �ѱ�
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APlayerCharacter::InitializePlayerOverlay()
{
	// HUD�� �����ϱ�
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
		if (PlayerHUD)
		{
			PlayerOverlay = PlayerHUD->GetPlayerOverlay();
			if (PlayerOverlay && Attributes)
			{
				PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				PlayerOverlay->SetStaminaBarPercent(1.f);
				PlayerOverlay->SetChangeBarPercent(0.f);
				PlayerOverlay->SetGold(0);
				PlayerOverlay->SetSouls(0);
			}
		}
	}
	HideOverlay();
}

void APlayerCharacter::SetHUDHealth()
{
	if (PlayerOverlay && Attributes)
	{
		PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

bool APlayerCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void APlayerCharacter::ShowOverlay()
{
	PlayerOverlay->SetVisibility(ESlateVisibility::Visible);
}

void APlayerCharacter::HideOverlay()
{
	PlayerOverlay->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerCharacter::SetOverlayHuman()
{
	PlayerOverlay->OverlaySetHuman();
}

void APlayerCharacter::SetOverlayDragon()
{
	PlayerOverlay->OverlaySetDragon();
}
