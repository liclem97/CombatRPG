// liclem97 All Rights Reserved.


#include "Characters/EnemyCharacter.h"

#include "Components/Combat/EnemyCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false; // ĳ���Ͱ� ��Ʈ�ѷ��� ȸ�� ���⿡ ���� ȸ���ϴ� ����
	GetCharacterMovement()->bOrientRotationToMovement = true; // ĳ���Ͱ� �̵� �������� ȸ��
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f; // �ȴ� ���� ���� �� �󸶳� ������ ���������� ����

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");
}
