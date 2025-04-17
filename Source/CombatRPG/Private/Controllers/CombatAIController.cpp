// liclem97 All Rights Reserved.


#include "Controllers/CombatAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "CombatDebugHelper.h"

// PathFollowingComponent�� UPathFollowingComponent���� UCrowdFollowingComponent�� ������
ACombatAIController::ACombatAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Debug::Print(TEXT("CrowdFollowingComponent Valid"), FColor::Green);
	}

	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true; // �� Ž��
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false; // �Ʊ� Ž��
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false; // �߸� Ž��
	AISenseConfig_Sight->SightRadius = 5000.f; // ��ǥ �ν� �ִ� �þ� �Ÿ�
	AISenseConfig_Sight->LoseSightRadius = 0.f; // �̹� �� Ÿ���� �þ߿��� ��ġ�� �Ǵ� �Ÿ�
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f; // AI �þ� ����

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1)); // ���� �� ID�� 1
}

ETeamAttitude::Type ACombatAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);
		
	// ������ ��Ʈ�ѷ��� GenericTeamAgentInterface�� ������
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());
	
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() != GetGenericTeamId())
	{
		return ETeamAttitude::Hostile; // �������̽��� ��ȿ�ϰ� ���� ��Ʈ�ѷ��� Team ID�� �ٸ��� ���� ���� ��ȯ
	}

	return ETeamAttitude::Friendly; // ��ȣ ���� ��ȯ
}

void ACombatAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor) // ���������� �����ǰ� ���Ͱ� ��ȿ��
	{
		if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor); // ������ Ű ����
		}
	}
}
