// liclem97 All Rights Reserved.


#include "Controllers/CombatAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
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

	SetGenericTeamId(FGenericTeamId(1)); // 적의 팀 ID는 1
}

ETeamAttitude::Type ACombatAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
		
	// 감지된 컨트롤러의 GenericTeamAgentInterface를 가져옴
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());
	
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		return ETeamAttitude::Hostile; // 인터페이스가 유효하고 서로 컨트롤러의 Team ID가 다르면 적대 상태 반환
	}

	return ETeamAttitude::Friendly; // 우호 상태 반환
}

void ACombatAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor) // 성공적으로 감지되고 액터가 유효함
	{
		if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor); // 블랙보드 키 설정
		}
	}
}
