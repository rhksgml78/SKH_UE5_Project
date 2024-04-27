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
	UInputAction* MovementAction; // 전후좌우이동

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction; // 시야회전

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction; // 점프

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* PressEkeyAction; // E키 상호작용

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Press1keyAction; // 1키 상호작용

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Press2keyAction; // 2키 상호작용

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Press3keyAction; // 3키 상호작용

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Press4keyAction; // 4키 상호작용

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* Press5keyAction; // 5키 상호작용

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction; // 공격

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction; // 회피

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* WheelAction; // 스프링암거리조절

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Wheel(const FInputActionValue& Value);

	// 인풋 콜백 함수
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


	// 재정의 함수
	virtual void Attack() override;
	virtual void Die_Implementation() override;

	bool HasEnoughStamina();

	// 몽타주 관련
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

	// 사망시 블루프린트에서 호출할 함수
	UFUNCTION(BlueprintCallable)
	void ReSpawn();

	// 무기관련
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	AFirstWeapon* SwordWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	AFirstWeapon* MagicWeapon;

private:
	// 플레이어 컴포넌트 관련
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

	// 플레이어 상태 관련
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
	// 언리얼 인라인 매크로 FORCEINLINE
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
