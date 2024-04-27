#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	// PrimaryComponentTick = �������� ������Ʈ ƽ ������Ʈ���ƴ� Ŭ������ Tick�� ����� �����̹Ƿ� false ����
	PrimaryComponentTick.bCanEverTick = false;

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	// ü���� - ���� ���� �ʵ��� Clamp �Լ��� ��� 0�� �ִ� ü�°����� ���� ���� ��Ų��.
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

}

void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

void UAttributeComponent::UseSkillStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

float UAttributeComponent::GetChangePercent()
{
	return Change / MaxChange;
}

float UAttributeComponent::GetSkill1CooltimePercent()
{
	return Skill1Cooltime / Skill1MaxCooltime;
}

float UAttributeComponent::GetSkill2CooltimePercent()
{
	return Skill2Cooltime / Skill2MaxCooltime;
}

float UAttributeComponent::GetSkill3CooltimePercent()
{
	return Skill3Cooltime / Skill3MaxCooltime;
}

float UAttributeComponent::GetSkill4CooltimePercent()
{
	return Skill4Cooltime / Skill4MaxCooltime;
}

float UAttributeComponent::GetSkill5CooltimePercent()
{
	return Skill5Cooltime / Skill5MaxCooltime;
}

bool UAttributeComponent::IsAlive()
{
	// �ܼ��ϰ� ü���� ���� 0���� ũ�� True�� ��ȯ
	return Health > 0.f;
}

void UAttributeComponent::AddGold(int32 AmountOfGold)
{
	Gold += AmountOfGold;
}

void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
	Souls += NumberOfSouls;
}

void UAttributeComponent::AddHealth(int32 AddHealthCount)
{
	Health = FMath::Clamp(Health + AddHealthCount, 0.f, MaxHealth);
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ���� ���׹̳��� �ִ뽺�׹̳����� ������� ������ �����ϵ��� �Ѵ�. �÷��̾��� HUD�� ������Ʈ�Ǵ°��� �÷��̾� Ŭ�������� ���� �������

}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

void UAttributeComponent::RegenChange(float DeltaTime)
{
	Change = FMath::Clamp(Change + ChangeRegenRate * DeltaTime, 0.f, MaxChange);
}

void UAttributeComponent::UnRegenChange(float DeltaTime)
{
	Change = FMath::Clamp(Change - ChangeRegenRate * DeltaTime, 0.f, MaxChange);
}

void UAttributeComponent::UnRegenSkill1Cooltime(float DeltaTime)
{
	Skill1Cooltime = FMath::Clamp(Skill1Cooltime - CooltimeRegen * DeltaTime, 0.f, Skill1MaxCooltime);
}

void UAttributeComponent::UnRegenSkill2Cooltime(float DeltaTime)
{
	Skill2Cooltime = FMath::Clamp(Skill2Cooltime - CooltimeRegen * DeltaTime, 0.f, Skill2MaxCooltime);
}

void UAttributeComponent::UnRegenSkill3Cooltime(float DeltaTime)
{
	Skill3Cooltime = FMath::Clamp(Skill3Cooltime - CooltimeRegen * DeltaTime, 0.f, Skill3MaxCooltime);
}

void UAttributeComponent::UnRegenSkill4Cooltime(float DeltaTime)
{
	Skill4Cooltime = FMath::Clamp(Skill4Cooltime - CooltimeRegen * DeltaTime, 0.f, Skill4MaxCooltime);
}

void UAttributeComponent::UnRegenSkill5Cooltime(float DeltaTime)
{
	Skill5Cooltime = FMath::Clamp(Skill5Cooltime - CooltimeRegen * DeltaTime, 0.f, Skill5MaxCooltime);
}

