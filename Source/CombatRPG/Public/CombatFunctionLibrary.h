// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatFunctionLibrary.generated.h"

class UCombatAbilitySystemComponent;

UENUM()
enum class ECombatConfirmType : uint8
{
	Yes,
	No
};
/**
 * 
 */
UCLASS()
class COMBATRPG_API UCombatFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UCombatAbilitySystemComponent* NativeGetCombatASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Combat|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, ECombatConfirmType& OutConfirmType);
};
