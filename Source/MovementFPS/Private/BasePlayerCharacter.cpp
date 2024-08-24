#include "BasePlayerCharacter.h"

#include "AdvancedCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

ABasePlayerCharacter::ABasePlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UAdvancedCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	AdvancedCharacterMovementComponent = Cast<UAdvancedCharacterMovementComponent>(GetCharacterMovement());
	AdvancedCharacterMovementComponent->SetIsReplicated(true);

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);

	bUseControllerRotationPitch = bUseControllerRotationYaw = bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->AddRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;
}

void ABasePlayerCharacter::Jump()
{
	bPressedCustomJump = true;
	Super::Jump();

	//bPressedCustomJump = false;
	bPressedJump = false;

	UE_LOG(LogTemp, Warning, TEXT("Jump isserver:%d"), HasAuthority())
}

void ABasePlayerCharacter::StopJumping()
{
	bPressedCustomJump = false;
	Super::StopJumping();
}

bool ABasePlayerCharacter::CanJumpInternal_Implementation() const
{
	return JumpIsAllowedInternal();
}

// Input Interface implementations
void ABasePlayerCharacter::Move_Implementation(const FVector2D Vector)
{
	IPlayerActionInterface::Move_Implementation(Vector);

	if (Controller == nullptr)
		return;

	FVector normalizedVector = (GetActorRightVector() * Vector.X + GetActorForwardVector() * Vector.Y).GetSafeNormal();
    
	AddMovementInput(normalizedVector, AdvancedCharacterMovementComponent->MaxWalkSpeed);
}

void ABasePlayerCharacter::Look_Implementation(const FVector2D Vector)
{
	IPlayerActionInterface::Look_Implementation(Vector);
	AddControllerYawInput(Vector.X * TurnRate);// * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(Vector.Y * LookUpRate);// * GetWorld()->GetDeltaSeconds());
}

FCollisionQueryParams ABasePlayerCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}
