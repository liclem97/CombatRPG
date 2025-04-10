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

	RelevantAttributesToCapture.Add(GetCombatDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetCombatDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetCombatDamageCapture().DamageTakenDef);
}

// Ŀ���� ������ ����� ����
void UGEExacCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{	
	// ���� ���� ���� GameplayEffect�� ���� ������ ������
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	// �Ӽ� ��꿡 ����� �±� ���͸� ������ ����
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	// AttackPower �Ӽ��� �ҽ�(������) �������� ĸó�ؼ� SourceAttackPower�� ����
	// ������ ���� �÷��̾��� �⺻ ���ݷ�
	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);

	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	// SetByCaller �� ����
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(CombatGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value; // ������ ���� ���� ���ݷ�
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

	// �����(Ÿ��)�� ���� ���
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);

	// �޺� ī��Ʈ�� ���� ���� ���ݷ� ����
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

	// ���� ���ݷ� * �÷��̾� ���ݷ� / Ÿ�� ����
	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier( 
			FGameplayModifierEvaluatedData(	// ���� ���� �������� Ÿ�ٿ��� ����
				GetCombatDamageCapture().DamageTakenProperty, // ������ �� Ÿ�� �Ӽ� AttributeSet�� PostGameplayEffectExecute�� ���۽�Ŵ
				EGameplayModOp::Override, // ���� ���
				FinalDamageDone // ���� ���� ������ ��
			)
		);
	}
}
