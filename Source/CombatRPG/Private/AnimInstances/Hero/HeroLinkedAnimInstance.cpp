// liclem97 All Rights Reserved.


#include "AnimInstances/Hero/HeroLinkedAnimInstance.h"

#include "AnimInstances/Hero/HeroAnimInstance.h"

UHeroAnimInstance* UHeroLinkedAnimInstance::GetHeroAnimInstance() const
{   
    return Cast<UHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
