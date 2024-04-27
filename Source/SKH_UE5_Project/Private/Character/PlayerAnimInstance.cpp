// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 폰을 플레이어 캐릭터로 캐스팅(형변환)
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter) // 캐스팅이 성공했다면
	{
		// 무브먼트 저장
		PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float Deltatime)
{
	Super::NativeUpdateAnimation(Deltatime);

	// 캐릭터의 속도 업데이트(Kismet라이브러리 포함필요)
	if (PlayerCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);

		IsFalling = PlayerCharacterMovement->IsFalling();
		CharacterState = PlayerCharacter->GetCharacterState();
		ActionState = PlayerCharacter->GetActionState();
		DeathPose = PlayerCharacter->GetDeathPose();
	}

}
