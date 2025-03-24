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
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' Template �Ű����� GetPawn�� APawn���� �Ļ��Ǿ�� ��.");
		return CastChecked<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template<class T>
	T* GetOwningController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template �Ű����� GetController�� AController���� �Ļ��Ǿ�� ��.");
		return GetOwningPawn<APawn>()->GetController<T>();
	}
};
