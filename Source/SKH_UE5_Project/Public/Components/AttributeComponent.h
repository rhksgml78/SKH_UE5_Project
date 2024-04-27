#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKH_UE5_PROJECT_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RegenStamina(float DeltaTime);
	void RegenChange(float DeltaTime);
	void UnRegenChange(float DeltaTime);
	void UnRegenSkill1Cooltime(float DeltaTime);
	void UnRegenSkill2Cooltime(float DeltaTime);
	void UnRegenSkill3Cooltime(float DeltaTime);
	void UnRegenSkill4Cooltime(float DeltaTime);
	void UnRegenSkill5Cooltime(float DeltaTime);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxChange = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Change = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 HealthPotionScale;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 10.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate = 10.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float ChangeRegenRate = 2.f;

	// 스킬관련
	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill1MaxCooltime = 20.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill1Cooltime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill2MaxCooltime = 20.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill2Cooltime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill3MaxCooltime = 20.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill3Cooltime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill4MaxCooltime = 20.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill4Cooltime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill5MaxCooltime = 20.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill5Cooltime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill1Cost = 10.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill2Cost = 10.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill3Cost = 10.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill4Cost = 10.f;

	UPROPERTY(EditAnywhere, Category = "Skills")
	float Skill5Cost = 10.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float CooltimeRegen = 2.5f;

public:
	// 데미지 전달하기
	void ReceiveDamage(float Damage);

	// 스테미나 소모하기
	void UseStamina(float StaminaCost);
	void UseSkillStamina(float StaminaCost);

	// 현재 체력의 백분률값 전달하기
	float GetHealthPercent();
	// 현재 스테미너 백분률값 전달하기
	float GetStaminaPercent();

	// 현재 변신용 게이지 백분률값 전달하기
	UFUNCTION(BlueprintCallable)
	float GetChangePercent();

	// 쿨타임 백분률값 전달하기
	float GetSkill1CooltimePercent();
	float GetSkill2CooltimePercent();
	float GetSkill3CooltimePercent();
	float GetSkill4CooltimePercent();
	float GetSkill5CooltimePercent();

	// 캐릭터의 사망 여부를 판단
	bool IsAlive();

	void AddGold(int32 AmountOfGold);
	void AddSouls(int32 NumberOfSouls);
	void AddHealth(int32 AddHealthCount);

	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE int32 GetHealthPotionScale() const { return HealthPotionScale; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE float GetChange() const { return Change; }
	FORCEINLINE float GetMaxChange() const { return MaxChange; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE void SetHealth(float Value) { Health = Value; }

	FORCEINLINE void UseSkill1() { Skill1Cooltime = Skill1MaxCooltime; }
	FORCEINLINE void UseSkill2() { Skill2Cooltime = Skill2MaxCooltime; }
	FORCEINLINE void UseSkill3() { Skill3Cooltime = Skill3MaxCooltime; }
	FORCEINLINE void UseSkill4() { Skill4Cooltime = Skill4MaxCooltime; }
	FORCEINLINE void UseSkill5() { Skill5Cooltime = Skill5MaxCooltime; }
	FORCEINLINE float GetSkill1Cost() const { return Skill1Cost; }
	FORCEINLINE float GetSkill2Cost() const { return Skill2Cost; }
	FORCEINLINE float GetSkill3Cost() const { return Skill3Cost; }
	FORCEINLINE float GetSkill4Cost() const { return Skill4Cost; }
	FORCEINLINE float GetSkill5Cost() const { return Skill5Cost; }
};
