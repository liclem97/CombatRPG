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
	
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile; // 인터페이스가 유효하고 자신의 Team ID보다 작으면 적대
	}

	return ETeamAttitude::Friendly; // 우호 상태 반환
}

void ACombatAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{	
		// bool에 따른 군중 시뮬레이션 상태 설정
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);
		
		// 회피 품질 설정
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1:	CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);	 break;
		case 2:	CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);	 break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);	 break;
		default:
			break;
		}

		CrowdComp->SetAvoidanceGroup(1); // AI가 속한 회피 그룹 설정
		CrowdComp->SetGroupsToAvoid(1); // 어떤 그룹을 회피할지 지정
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange); // 충돌 쿼리 범위 설정
	}
}

void ACombatAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{	
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor"))) // 타겟이 유효하지 않을 때만 실행
		{
			if (Stimulus.WasSuccessfullySensed() && Actor) // 성공적으로 감지되고 액터가 유효함
			{
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor); // 블랙보드 키 설정
			}
		}
	}
}
