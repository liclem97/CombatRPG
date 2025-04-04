// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/CharacterAnimInstance.h"
#include "HeroAnimInstance.generated.h"

class AHeroCharacter;

/**
 * 
 */
UCLASS()
class COMBATRPG_API UHeroAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
	AHeroCharacter* OwningHeroCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxStateThreshold = 5.f; // 설정된 시간 이후 캐릭터가 Relax 상태로 진입함

	float IdleElpasedTime; // 캐릭터가 Idle 상태인 시간

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Rifle")
	float YawOffset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Rifle")
	float Lean;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Rifle")
	float AO_Yaw;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Rifle")
	float AO_Pitch;
};
