// liclem97 All Rights Reserved.


#include "Characters/HeroCharacter.h"

#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "CombatGameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/Input/CombatInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "CombatDebugHelper.h"

AHeroCharacter::AHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
}

void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 소프트 오브젝트 레퍼런스 이므로 항상 유효하지 않기에 Null 체크로 확인함
	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(CombatAbilitySystemComponent);
		}
	}
}

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	checkf(InputConfigDataAsset, TEXT("Check assign a valid data asset as input config"));
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(SubSystem);

	SubSystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UCombatInputComponent* CombatInputComponent = CastChecked<UCombatInputComponent>(PlayerInputComponent);
	CombatInputComponent->BindNativeInputAction(InputConfigDataAsset, CombatGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &AHeroCharacter::Input_Move);
	CombatInputComponent->BindNativeInputAction(InputConfigDataAsset, CombatGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &AHeroCharacter::Input_Look);

	CombatInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &AHeroCharacter::Input_AbilityInputPressed, &AHeroCharacter::Input_AbilityInputReleased);
}

void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroCharacter::SetUseControllerRotationYaw(bool InUse)
{
	bUseControllerRotationYaw = InUse;
}

void AHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	CombatAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	CombatAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}


