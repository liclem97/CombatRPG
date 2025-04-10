#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_Left UMETA(DisplayName = "Turning Left"),
	ETIP_Right UMETA(DisplayName = "Turning Right"),
	ETIP_NotTurning UMETA(DisplayName = "Not Turning"),

	ETIP_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM()
enum class ECombatConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class ECombatValidType : uint8
{
	Valid,
	InValid
};

UENUM()
enum class ECombatSuccessType : uint8
{
	Successful,
	Failed
};