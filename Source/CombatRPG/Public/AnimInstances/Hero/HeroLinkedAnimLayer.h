// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BaseAnimInstance.h"
#include "HeroLinkedAnimLayer.generated.h"

class UHeroAnimInstance;

/**
 * 
 */
UCLASS()
class COMBATRPG_API UHeroLinkedAnimLayer : public UBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UHeroAnimInstance* GetHeroAnimInstance() const;
};
