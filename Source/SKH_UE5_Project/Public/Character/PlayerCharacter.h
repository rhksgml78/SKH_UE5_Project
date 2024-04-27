#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interface/PickUpInterface.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;
class ASoul;
class ATreasure;
class UPlayerOverlay;

class UInputMappingContext;
class UInputAction;

UCLASS()
class SKH_UE5_PROJECT_API APlayerCharacter : public ABaseCharacter, public IPickUpInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	void SkillCoolTimeRegen(float DeltaTime);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

	/* Interface */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;
	virtual void AddHealth(AHealthPotion* HealthPotion) override;
	/* Interface End  */
	
protected:
	virtual void BeginPlay() override;

	/*Enhanced Input*/
	// IMC
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* PlayerContext;

	// IA
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction; // �����¿��̵�

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction; // �þ�ȸ��

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction; // ����

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* PressEkeyAction; // EŰ ��ȣ�ۿ�

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Press1keyAction; // 1Ű ��ȣ�ۿ�

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Press2keyAction; // 2Ű ��ȣ�ۿ�

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Press3keyAction; // 3Ű ��ȣ�ۿ�

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Press4keyAction; // 4Ű ��ȣ�ۿ�

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Press5keyAction; // 5Ű ��ȣ�ۿ�

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction; // ����

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction; // ȸ��

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* WheelAction; // �������ϰŸ�����

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Wheel(const FInputActionValue& Value);

	// ��ǲ �ݹ� �Լ�
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void PressE();
	void Press1();
	void Press2();
	void Press3();
	void Press4();
	void Press5();
	void Dodge();


	// ������ �Լ�
	virtual void Attack() override;
	virtual void Die_Implementation() override;

	bool HasEnoughStamina();

	// ��Ÿ�� ����
	void EquipWeapon(AFirstWeapon* Weapon);
	void PlayEquipMontage(const FName& SectionName);
	void PlayWeaponChangeMontage(const FName& SectionName);
	void PlaySkillMontage(const FName& SectionName);
	virtual void AttackEnd() override;
	virtual void DodgeEnd() override;
	virtual bool CanAttack() override; 
	bool CanDisarm();
	void Disarm();
	bool CanArm();
	void Arm();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttackWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UFUNCTION(BlueprintCallable)
	void WeaponChange();

	// ����� �������Ʈ���� ȣ���� �Լ�
	UFUNCTION(BlueprintCallable)
	void ReSpawn();

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	AFirstWeapon* SwordWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	AFirstWeapon* MagicWeapon;

private:
	// �÷��̾� ������Ʈ ����
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* WeaponChangeMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* SwordSkillMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* MagicSkillMontage;

	// �÷��̾� ���� ����
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterChangeType ChangeType = ECharacterChangeType::ECCY_Human;

	void InitializePlayerOverlay();
	void SetHUDHealth();

	bool IsUnoccupied();

	// HUD
	UPROPERTY(EditAnywhere)
	UPlayerOverlay* PlayerOverlay;

public:
	// �𸮾� �ζ��� ��ũ�� FORCEINLINE
	FORCEINLINE ECharacterState GetCharacterState() { return CharacterState; }
	FORCEINLINE EActionState GetActionState() { return ActionState; }

	UFUNCTION(BlueprintCallable)
	void ShowOverlay();	
	UFUNCTION(BlueprintCallable)
	void HideOverlay();
	UFUNCTION(BlueprintCallable)
	void SetOverlayHuman();
	UFUNCTION(BlueprintCallable)
	void SetOverlayDragon();
};
