// liclem97 All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"

#include "AbilitySystem/Abilities/EnemyGameplayAbility.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

// ASC에 적 전용 게임플레이 어빌리티를 부여함
void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

	if (!EnemyCombatAbilties.IsEmpty())
	{
		for (const TSubclassOf<UEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilties)
		{
			if (!AbilityClass) continue;

			FGameplayAbilitySpec AbilitySpec(AbilityClass); // 어빌리티 사양 생성
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;

			InASCToGive->GiveAbility(AbilitySpec); // 만들어진 어빌리티 스펙을 ASC에 부여
		}
	}
}
