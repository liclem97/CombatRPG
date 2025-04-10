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
	/* ��ũ�� ���� ĸó�ϴ� ��� */
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
	//	* GE�� ó�� ����� �� �ش� �Ӽ����� �� ���� ĸ��
	//	* ���� �ش� GE�� �����Ǵ� ����, �Ӽ����� �ٲ� ������ ���� ����
	//	* �ַ� ������ �����, �ʱ� ������ ��� ��� ���

	//InSnapShot == false >
	//	* GE�� ��(evaluation)�� ������ �ǽð����� �ش� �Ӽ����� ����
	//	* ��: DOT(Damage over Time) ȿ��, HoT(Heal over Time) �� �ֱ������� ���Ǵ� ȿ��
	//	* ���� ȿ��, �ֱ��� ȸ��/����, �ǽð� ��ȭ ������ �ʿ��� ��� ���
	//***/

	//RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition)

	RelevantAttributesToCapture.Add(GetFCombatDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetFCombatDamageCapture().DefensePowerDef);
}
