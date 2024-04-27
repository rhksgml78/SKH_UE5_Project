// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// ���� �÷��̾� ĳ���ͷ� ĳ����(����ȯ)
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter) // ĳ������ �����ߴٸ�
	{
		// �����Ʈ ����
		PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float Deltatime)
{
	Super::NativeUpdateAnimation(Deltatime);

	// ĳ������ �ӵ� ������Ʈ(Kismet���̺귯�� �����ʿ�)
	if (PlayerCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);

		IsFalling = PlayerCharacterMovement->IsFalling();
		CharacterState = PlayerCharacter->GetCharacterState();
		ActionState = PlayerCharacter->GetActionState();
		DeathPose = PlayerCharacter->GetDeathPose();
	}

}
