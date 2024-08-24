// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AdvancedCharacterMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashStartDelegate);

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_Prone			UMETA(DisplayName = "Prone"),
	CMOVE_WallRun		UMETA(DisplayName = "Wall Run"),
	CMOVE_MAX			UMETA(Hidden),
};


UCLASS()
class MOVEMENTFPS_API UAdvancedCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

class FSavedMove_Custom : public FSavedMove_Character
	{
	
		typedef  FSavedMove_Character Super;

	public:
		enum CompressedFlags
		{
			FLAG_Sprint			= 0x10,
			FLAG_Dash			= 0x20,
			FLAG_Custom_2		= 0x40,
			FLAG_Custom_3		= 0x80,
		};
		// Flags
		uint8 Saved_bWantsToSprint:1;
		uint8 Saved_bWantsToDash:1;
		uint8 Saved_bPressedCustomJump:1;

		// Other Variables
		uint8 Saved_bWantsToProne:1;
		uint8 Saved_bWantsToCrouch:1;
		uint8 Saved_bHadAnimRootMotion:1;
		uint8 Saved_bTransitionFinished:1;
		uint8 Saved_bWallRunIsRight:1;
		

	public:
		FSavedMove_Custom();
	private:
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Custom : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Custom(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;
		
		virtual FSavedMovePtr AllocateNewMove() override;
	};

	// Parameters
	public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General") float MaxSprintSpeed = 900.f;
	private:
	// Slide
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float MinSlideSpeed=400.f;
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float MaxSlideSpeed=400.f;
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float SlideEnterImpulse=400.f;
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float SlideGravityForce=4000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float SlideFrictionFactor=.06f;
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float BrakingDecelerationSliding=1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Slide") float SlideAccelerationMultiplier=.1f;

	// Prone
	UPROPERTY(EditDefaultsOnly, Category = "Prone") float ProneEnterHoldDuration=.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Prone") float ProneSlideEnterImpulse=300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Prone") float MaxProneSpeed=300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Prone") float BrakingDecelerationProning=2500.f;

	// Dash
	UPROPERTY(EditDefaultsOnly, Category = "Dash") bool DashEnabled = false;
	UPROPERTY(EditDefaultsOnly, Category = "Dash") float DashImpulse = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Dash") float DashCooldownDuration = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Dash") float AuthDashCooldownDuration = .9f;

	// Mantle
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MantleMaxDistance = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MantleReachHeight = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MinMantleDepth = 30.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MantleMinWallSteepnessAngle = 75.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MantleMaxSurfaceAngle = 40.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") float MantleMaxAlignmentAngle = 45.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* TallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* TransitionTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* ProxyTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* ShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* TransitionShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Mantle") UAnimMontage* ProxyShortMantleMontage;

	// Wall Run
	UPROPERTY(EditDefaultsOnly, Category = "Wall Run") bool WallRunEnabled = false;
	UPROPERTY(EditDefaultsOnly, Category = "Wall Run") float MinWallRunSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wall Run") float MaxWallRunSpeed = 800.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wall Run") float MaxVerticalWallRunSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wall Run") float WallRunPullAwayAngle = 75.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wall Run") float WallAttractionForce = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wall Run") float MinWallRunHeight = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Wall Run") UCurveFloat* WallRunGravityScaleCurve;
	UPROPERTY(EditDefaultsOnly, Category = "Wall Run") float WallJumpOffForce = 300.f;

	// Transient
	UPROPERTY(Transient) ABasePlayerCharacter* BasePlayerCharacterOwner;

	// Flags
	bool Safe_bWantsToSprint;
	bool Safe_bWantsToProne;
	bool Safe_bWantsToDash;

	bool Safe_bHadAnimRootMotion;
	bool Safe_bWantsToCrouch;
	float DashStartTime;
	FTimerHandle TimerHandle_EnterProne;
	FTimerHandle TimerHandle_DashCooldown;

	bool Safe_bTransitionFinished;
	TSharedPtr<FRootMotionSource_MoveToForce> TransitionRMS;
	UPROPERTY(Transient) UAnimMontage* TransitionQueuedMontage;
	float TransitionQueuedMontageSpeed;
	int TransitionRMS_ID;

	bool Safe_bWallRunIsRight;

	// Replication
	UPROPERTY(ReplicatedUsing=OnRep_DashStart) bool Proxy_bDashStart;

	UPROPERTY(ReplicatedUsing=OnRep_ShortMantle) bool Proxy_bShortMantle;
	UPROPERTY(ReplicatedUsing=OnRep_TallMantle) bool Proxy_bTallMantle;

	// Delegates
public:
	UPROPERTY(BlueprintAssignable) FDashStartDelegate DashStartDelegate;
	
public:
	UAdvancedCharacterMovementComponent();

	// Actor Component
protected:
	virtual void InitializeComponent() override;

	// Character Movement Component
public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;

	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bReplayingMoves) override;
	
protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	
public:
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

protected:
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	
	// Slide
private:
	void EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitSlide();
	bool CanSlide() const;
	void PhysSlide(float deltaTime, int32 Iterations);

	
	// Prone
private:
	void TryEnterProne() { Safe_bWantsToProne = true; }
	UFUNCTION(Server, Reliable) void Server_EnterProne();
	
	void EnterProne(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitProne();
	bool CanProne();
	void PhysProne(float deltaTime, int32 Iterations);

	
	// Dash
private:
	void OnDashCooldownFinished();
	bool CanDash() const;
	void PerformDash();

	
	// Mantle
private:
	bool TryMantle();
	FVector GetMantleStartLocation(FHitResult FrontHit, FHitResult SurfaceHit, bool bTallMantle) const;


	// Wall Run
private:
	bool TryWallRun();
	void PhysWallRun(float deltaTime, int32 Iterations);
	

	// Helpers
private:
	bool IsServer() const;
	float CapR() const;
	float CapHH() const;

	// Input
public:
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	UFUNCTION(BlueprintCallable) void CrouchPressed();
	UFUNCTION(BlueprintCallable) void CrouchReleased();

	UFUNCTION(BlueprintCallable) void DashPressed();
	UFUNCTION(BlueprintCallable) void DashReleased();

	
	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	UFUNCTION(BlueprintPure) bool IsMovementMode(EMovementMode InMovementMode) const;

	UFUNCTION(BlueprintPure) bool IsWallRunning() const { return IsCustomMovementMode(CMOVE_WallRun); }
	UFUNCTION(BlueprintPure) bool WallRunningIsRight() const { return Safe_bWallRunIsRight; }



	// Proxy Replication
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private: 
	UFUNCTION() void OnRep_DashStart();
	UFUNCTION() void OnRep_ShortMantle();
	UFUNCTION() void OnRep_TallMantle();
};
