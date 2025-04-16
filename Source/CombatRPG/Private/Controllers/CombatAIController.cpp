// liclem97 All Rights Reserved.


#include "Controllers/CombatAIController.h"

#include "Navigation/CrowdFollowingComponent.h"

#include "CombatDebugHelper.h"

ACombatAIController::ACombatAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		Debug::Print(TEXT("CrowdFollowingComponent Valid"), FColor::Green);
	}
}
