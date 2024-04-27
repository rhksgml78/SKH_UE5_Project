// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "PlayerAnimInstance.generated.h"


UCLASS()
class SKH_UE5_PROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// 초기화, 업데이트
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Deltatime) override;

	UPROPERTY(BlueprintReadOnly) // 클래스 객체 선언
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly) // 무브먼트 컴포넌트 생성
	class UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	// 애니매이션은 캐릭터의 상태를 읽어올수만있다.
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TEnumAsByte<EDeathPose> DeathPose;
};
