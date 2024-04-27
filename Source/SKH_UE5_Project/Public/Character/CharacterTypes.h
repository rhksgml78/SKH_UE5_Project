#pragma once

UENUM(BlueprintType)//언리얼엔진노출용 매크로
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "EquippedOneHandedWeapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "EquippedTwoHandedWeapon"),
	ECS_EquippedMagicWeapon UMETA(DisplayName = "EquippedMagicWeapon")
};

UENUM(BlueprintType)
enum class ECharacterChangeType : uint8
{
	ECCY_Human UMETA(DisplayName = "Human"),
	ECCY_Dragon UMETA(DisplayName = "Dragon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "EquippingWeapon"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum EDeathPose
{
	EDP_DeathLeft UMETA(DisplayName = "DeathLeft"),
	EDP_DeathRight UMETA(DisplayName = "DeathRight"),
	EDP_DeathFront UMETA(DisplayName = "DeathFront"),
	EDP_DeathBack UMETA(DisplayName = "DeathBack"),
	EDP_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")
};

UENUM(BlueprintType)
enum class EEnemyTypes : uint8
{
	ET_Normal UMETA(DisplayName = "Normal"),
	ET_Special UMETA(DisplayName = "Special"),
	ET_Boss UMETA(DisplayName = "Boss")
};

// 아이템 타입 
UENUM(BlueprintType)
enum class EItemTypes : uint8
{
	EIT_None  UMETA(DisplayName = "None"),
	EIT_OneHnadWeapon  UMETA(DisplayName = "OneHandWeapon"),
	EIT_OneMagicWeapon  UMETA(DisplayName = "OneMagicWeapon")
};

// 아이템 타입 
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_Normal  UMETA(DisplayName = "Normal"),
	EAT_Strong UMETA(DisplayName = "Strong")
};