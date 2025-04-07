// liclem97 All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"

#include "AbilitySystem/Abilities/EnemyGameplayAbility.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

// ASC�� �� ���� �����÷��� �����Ƽ�� �ο���
void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	if (!EnemyCombatAbilties.IsEmpty())
	{
		for (const TSubclassOf<UEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilties)
		{
			if (!AbilityClass) continue;

			FGameplayAbilitySpec AbilitySpec(AbilityClass); // �����Ƽ ��� ����
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;

			InASCToGive->GiveAbility(AbilitySpec); // ������� �����Ƽ ������ ASC�� �ο�
		}
	}
}
