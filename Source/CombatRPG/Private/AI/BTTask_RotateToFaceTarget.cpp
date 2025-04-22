// liclem97 All Rights Reserved.


#include "AI/BTTask_RotateToFaceTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = TEXT("Native Rotate to Face Target Actor");
	AnglePrecision = 10.f; // 회전 정밀도
	RotationInterpSpeed = 5.f;

	bNotifyTick = true; 
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;

	INIT_TASK_NODE_NOTIFY_FLAGS(); // 알림 관련 플래그들을 기본값으로 초기화

	// 블랙보드에서 InTargetToFaceKey 키가 AActor 타입 객체만 허용하도록 필터 설정
	InTargetToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetToFaceKey), AActor::StaticClass());
}

// Behavior Tree 자산으로부터 이 노드를 초기화
void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// 블랙보드 에셋을 기준으로 InTargetToFaceKey를 초기화함
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{	
		InTargetToFaceKey.ResolveSelectedKey(*BBAsset);
	}
}

// FRotateToFaceTargetTaskMemeory의 크기를 반환
uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToFaceTargetTaskMemeory);
}

// 노드의 설명을 설정함
FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{	
	const FString KeyDescription = InTargetToFaceKey.SelectedKeyName.ToString();

	return 	FString::Printf(TEXT("Smoothly rotate to face %s until the angle precision: %s is reached"), *KeyDescription, *FString::SanitizeFloat(AnglePrecision));
}

// 노드 실행 함수
EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* ActorObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetToFaceKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject); // 블랙보드에서 TargetActor 가져오기

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn(); // 현재 AI가 조종 중인 Pawn 가져오기

	// 노드 메모리 캐스팅 및 유효성 검사
	FRotateToFaceTargetTaskMemeory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemeory>(NodeMemory);
	check(Memory);

	// 메모리에 현재 대상 정보 저장
	Memory->OwningPawn = OwningPawn;
	Memory->TargetActor = TargetActor;
	
	// 타겟이나 소유자가 유효하지 않으면 실패
	if (!Memory->IsValid())
	{
		return EBTNodeResult::Failed;
	}

	// 이미 타겟을 바라보고 있다면 즉시 성공 처리
	if (HasReachedAnglePercision(OwningPawn, TargetActor))
	{
		Memory->Reset(); // 메모리 초기화
		return EBTNodeResult::Succeeded;
	}

	// 회전이 필요하므로 TickTask 호출을 위한 InProgress 반환
	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 노드 메모리 접근
	FRotateToFaceTargetTaskMemeory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemeory>(NodeMemory);
	if (!Memory->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed); // 타겟이나 소유자가 유효하지 않으면 실패 처리
	}

	// 회전 완료되었는지 확인
	if (HasReachedAnglePercision(Memory->OwningPawn.Get(), Memory->TargetActor.Get()))
	{
		Memory->Reset(); // 메모리 초기화
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); // 작업 성공으로 종료
	}
	else
	{	
		// LookAt 회전 계산 (현재 위치 → 타겟 위치)
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Memory->OwningPawn->GetActorLocation(), Memory->TargetActor->GetActorLocation());
		
		// 현재 회전 → 목표 회전으로 선형 보간 (자연스럽게 회전하도록)
		const FRotator TargetRot = FMath::RInterpTo(Memory->OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);
		
		Memory->OwningPawn->SetActorRotation(TargetRot);
	}
}

// 회전 완료 조건 검사 함수 - 목표를 일정 각도 이하로 정확히 바라보고 있는지 판단
bool UBTTask_RotateToFaceTarget::HasReachedAnglePercision(APawn* QueryPawn, AActor* TargetActor) const
{
	const FVector OwnerForward = QueryPawn->GetActorForwardVector();

	// 소유자 → 타겟 방향 벡터 (정규화)
	const FVector OwnerToTargetNormalized = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

	// 두 벡터 사이의 코사인 각도 계산 (DotProduct)
	const float DotResult = FVector::DotProduct(OwnerForward, OwnerToTargetNormalized);

	// 각도 차이로 변환 (acos → degree)
	const float AngleDiff = UKismetMathLibrary::DegAcos(DotResult);

	// AnglePrecision 이하로 차이가 나면 회전 완료로 간주
	return AngleDiff <= AnglePrecision;
}
