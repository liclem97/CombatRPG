// liclem97 All Rights Reserved.


#include "Characters/EnemyCharacter.h"

#include "Components/Combat/EnemyCombatComponent.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "CombatDebugHelper.h"

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

UPawnCombatComponent* AEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void AEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull()) return;

	// �񵿱� �ε� ��û
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda( // �ε��� �Ϸ�Ǹ� ���� �Լ��� �����
			[this]()
			{
				if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(CombatAbilitySystemComponent);

					Debug::Print(TEXT("Enemy Start Up Data Loaded"), FColor::Green);
				}
			}
		)
	);
}
