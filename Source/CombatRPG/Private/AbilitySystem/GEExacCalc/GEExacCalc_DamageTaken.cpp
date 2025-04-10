// liclem97 All Rights Reserved.


#include "AbilitySystem/GEExacCalc/GEExacCalc_DamageTaken.h"

#include "AbilitySystem/CombatAttributeSet.h"

struct FCombatDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)

	FCombatDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, DefensePower, Target, false)
	}
};

static const FCombatDamageCapture& GetFCombatDamageCapture()
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

	RelevantAttributesToCapture.Add(GetFCombatDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetFCombatDamageCapture().DefensePowerDef);
}
