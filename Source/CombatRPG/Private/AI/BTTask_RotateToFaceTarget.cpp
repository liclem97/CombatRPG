// liclem97 All Rights Reserved.


#include "AI/BTTask_RotateToFaceTarget.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
	NodeName = TEXT("Native Rotate to Face Target Actor");
	AnglePrecision = 10.f;
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
