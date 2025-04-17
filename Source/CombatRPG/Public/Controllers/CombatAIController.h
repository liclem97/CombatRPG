// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class COMBATRPG_API ACombatAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACombatAIController(const FObjectInitializer& ObjectInitializer);

protected:
	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight; // AI ½Ã¾ß
};
