// liclem97 All Rights Reserved.


#include "Characters/HeroCharacter.h"

#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "CombatGameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/Input/CombatInputComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "CombatDebugHelper.h"

AHeroCharacter::AHeroCharacter()
{	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

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
	FollowCamera->FieldOfView = DefaultFOV;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));

	bIsAiming = false;
}

UPawnCombatComponent* AHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

UPawnUIComponent* AHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 소프트 오브젝트가 유효한 리소스를 가리키고 있는지 확인
	if (!CharacterStartUpData.IsNull())
	{	
		// LoadSynchronous = 소프트 레퍼런스를 즉시 메모리에 로드하고, 로드된 실제 오브젝트 포인터를 반환
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

void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bIsAiming ? AimingFOV : DefaultFOV; // Aim시 카메라 조절
	FollowCamera->FieldOfView = FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, 10.0f);

	AimOffset(DeltaTime);
}

void AHeroCharacter::SetUseControllerRotationYaw(bool InUse)
{
	bUseControllerRotationYaw = InUse;
}

void AHeroCharacter::SetAiming(bool bAiming)
{	
	bIsAiming = bAiming;
	if (bAiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
}

float AHeroCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

void AHeroCharacter::AimOffset(float DeltaTime)
{
	if (HeroCombatComponent && !HeroCombatComponent->GetHeroCarriedWeaponByTag(CombatGameplayTags::Player_Weapon_Rifle)) return;
	
	float Speed = CalculateSpeed();

	if (Speed == 0.f)
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		TurnInPlace(DeltaTime);
	}
	else if (Speed > 0.f)
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
	CalculateAO_Pitch();
}

void AHeroCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// 피치 값을 [270, 360) 범위에서 [-90, 0) 범위로 매핑
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
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

void AHeroCharacter::TurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}


