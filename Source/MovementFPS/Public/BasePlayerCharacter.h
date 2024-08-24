#pragma once


#include "CoreMinimal.h"
#include "FPSClasses.h"
#include "GameFramework/Character.h"
#include "PlayerActionInterface.h"
#include "GameplayTagContainer.h"
#include "BasePlayerCharacter.generated.h"

enum class CustomAbilityID : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ABasePlayerCharacter*, Character);


UCLASS()
class MOVEMENTFPS_API ABasePlayerCharacter : public ACharacter, public IPlayerActionInterface
{
	GENERATED_BODY()

	// Components
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Movement) UAdvancedCharacterMovementComponent* AdvancedCharacterMovementComponent;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRate{45.0f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float LookUpRate{45.0f};

public:
	bool bPressedCustomJump;
	bool bFreeCamEnabled {false};
	
public:
	ABasePlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual bool CanJumpInternal_Implementation() const override;

	// Input
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Move(const FVector2D Vector);
	void Move_Implementation(const FVector2D Vector) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Look(const FVector2D Vector);
	void Look_Implementation(const FVector2D Vector) override;

public:
	FORCEINLINE UCameraComponent* GetFirstPersonCamera() const
	{
		return FirstPersonCamera;
	}
	UFUNCTION(BlueprintPure) FORCEINLINE UAdvancedCharacterMovementComponent* GetAdvancedCharacterMovement() const { return AdvancedCharacterMovementComponent; }

	FCollisionQueryParams GetIgnoreCharacterParams() const;
};
