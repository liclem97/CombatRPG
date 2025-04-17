// liclem97 All Rights Reserved.


#include "Controllers/CombatAIController.h"

#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "CombatDebugHelper.h"

// PathFollowingComponent를 UPathFollowingComponent에서 UCrowdFollowingComponent로 변경함
ACombatAIController::ACombatAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Debug::Print(TEXT("CrowdFollowingComponent Valid"), FColor::Green);
	}

	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true; // 적 탐지
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false; // 아군 탐지
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false; // 중립 탐지
	AISenseConfig_Sight->SightRadius = 5000.f; // 목표 인식 최대 시야 거리
	AISenseConfig_Sight->LoseSightRadius = 0.f; // 이미 본 타깃을 시야에서 놓치게 되는 거리
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f; // AI 시야 각도

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);
}

void ACombatAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
}
