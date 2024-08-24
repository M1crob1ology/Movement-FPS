// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerActionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerActionInterface : public UInterface
{
	GENERATED_BODY()
};

class MOVEMENTFPS_API IPlayerActionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Move(const FVector2D Vector);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Look(const FVector2D Vector);
};
