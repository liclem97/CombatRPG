// liclem97 All Rights Reserved.


#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"

#include "AbilitySystem/Abilities/CombatGameplayAbility.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

// ASC에 히어로 전용 게임플레이 어빌리티를 부여 함
void UDataAsset_HeroStartUpData::GiveToAbilitySystemComponent(UCombatAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

    for (const FHeroAbilitySet& AbilitySet : HeroStartUpAbilitySets)
    {
        if (!AbilitySet.IsValid()) continue;

        FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant); // 어빌리티 사양 생성
        AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

        InASCToGive->GiveAbility(AbilitySpec); // 만들어진 어빌리티 스펙을 ASC에 부여
    }
}
