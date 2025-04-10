// liclem97 All Rights Reserved.


#include "AbilitySystem/GEExacCalc/GEExacCalc_DamageTaken.h"

#include "AbilitySystem/CombatAttributeSet.h"
#include "CombatGameplayTags.h"

#include "CombatDebugHelper.h"

struct FCombatDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FCombatDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, DamageTaken, Target, false)
	}
};

static const FCombatDamageCapture& GetCombatDamageCapture()
{
	static FCombatDamageCapture CombatDamageCapture;
	return CombatDamageCapture;
}

UGEExacCalc_DamageTaken::UGEExacCalc_DamageTaken()
{	
	/* 매크로 없이 캡처하는 방법 */
	//FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
	//	UCombatAttributeSet::StaticClass(),
	//	GET_MEMBER_NAME_CHECKED(UCombatAttributeSet, AttackPower)
	//);

	//FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
	//	AttackPowerProperty,
	//	EGameplayEffectAttributeCaptureSource::Source,
	//	false
	//	);
	///***
	//InSpanShot == true > 
	//	* GE가 처음 적용될 때 해당 속성값을 한 번만 캡쳐
	//	* 이후 해당 GE가 유지되는 동안, 속성값이 바뀌어도 영향을 받지 않음
	//	* 주로 버프나 디버프, 초기 데미지 계산 등에서 사용

	//InSnapShot == false >
	//	* GE가 평가(evaluation)될 때마다 실시간으로 해당 속성값을 참조
	//	* 예: DOT(Damage over Time) 효과, HoT(Heal over Time) 등 주기적으로 계산되는 효과
	//	* 지속 효과, 주기적 회복/피해, 실시간 변화 감지가 필요한 경우 사용
	//***/

	//RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition)

	RelevantAttributesToCapture.Add(GetCombatDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetCombatDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetCombatDamageCapture().DamageTakenDef);
}

// 커스텀 데미지 계산을 수행
void UGEExacCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{	
	// 현재 실행 중인 GameplayEffect의 스펙 정보를 가져옴
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	// 속성 계산에 사용할 태그 필터링 정보를 설정
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	// AttackPower 속성을 소스(공격자) 기준으로 캡처해서 SourceAttackPower에 저장
	// 레벨에 따른 플레이어의 기본 공격력
	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);

	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	// SetByCaller 값 추출
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value; // 레벨에 따른 무기 공격력
		}

		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
		}

		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
		}
	}

	// 방어자(타겟)의 방어력 계산
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);

	// 콤보 카운트에 따른 무기 공격력 증가
	if (UsedLightAttackComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.05f + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
	}

	if (UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncresePercentHeavy = UsedHeavyAttackComboCount * 0.15f + 1.f;

		BaseDamage *= DamageIncresePercentHeavy;
	}

	// 무기 공격력 * 플레이어 공격력 / 타깃 방어력
	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier( 
			FGameplayModifierEvaluatedData(	// 실제 계산된 데미지를 타겟에게 적용
				GetCombatDamageCapture().DamageTakenProperty, // 영향을 줄 타겟 속성 AttributeSet의 PostGameplayEffectExecute를 동작시킴
				EGameplayModOp::Override, // 적용 방식
				FinalDamageDone // 최종 계산된 데미지 값
			)
		);
	}
}
