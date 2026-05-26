// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "TruckPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSTRUCK_API ATruckPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, ReplicatedUsing = RepTeamID)
    int32 TeamID = -1;

    UFUNCTION(BlueprintNativeEvent, Category = "MyEvents")
    void RepTeamID();
    //Ignore this error it is dumb function that is confused

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};


