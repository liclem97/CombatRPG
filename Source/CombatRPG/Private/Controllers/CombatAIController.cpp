// liclem97 All Rights Reserved.


#include "Controllers/CombatAIController.h"

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
}

void ACombatAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
}
