// liclem97 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATRPG_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	template<class T>
	T* GetOwningPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' Template 매개변수 GetPawn은 APawn에서 파생되어야 함.");
		return CastChecked<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template<class T>
	T* GetOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template 매개변수 GetController는 AController에서 파생되어야 함.");
		return GetOwningPawn<APawn>()->GetController<T>();
	}
};
