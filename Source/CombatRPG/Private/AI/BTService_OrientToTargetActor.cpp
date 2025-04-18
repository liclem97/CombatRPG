// liclem97 All Rights Reserved.


#include "AI/BTService_OrientToTargetActor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_OrientToTargetActor::UBTService_OrientToTargetActor()
{
	NodeName = TEXT("Native Orient Rotation To Target Actor");

	INIT_SERVICE_NODE_NOTIFY_FLAGS(); // �˸� ���� �÷��׵��� �⺻������ �ʱ�ȭ

	RotationInterpSpeed = 5.f; // ȸ�� ���� �ӵ�
	Interval = 0.f; // ���� ���� ����
	RandomDeviation = 0.f; // ���� ����

	// �����忡�� InTargetActorKey Ű�� AActor Ÿ�� ��ü�� ����ϵ��� ���� ����
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

// Behavior Tree �ڻ����κ��� �� ��带 �ʱ�ȭ
void UBTService_OrientToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// ������ ������ ��ȿ�� ���, Ÿ�� ���� Ű�� �ش� �����忡�� resolve ��
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		InTargetActorKey.ResolveSelectedKey(*BBAsset); // ���õ� Ű�� �����忡�� �ؼ��Ͽ� ����
	}
}

FString UBTService_OrientToTargetActor::GetStaticDescription() const
{	
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}

void UBTService_OrientToTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject); // �����忡�� Ÿ�� ���� Ű�� ���� ���� �����ͼ� ĳ���� ��

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (OwningPawn && TargetActor)
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);

		OwningPawn->SetActorRotation(TargetRot);
	}
}
