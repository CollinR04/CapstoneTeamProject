// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TruckPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSTRUCK_API ATruckPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;

	UFUNCTION(BlueprintCallable)
	void SetUpLobby();

	UFUNCTION(Client, Reliable)
	void Client_SetLobbyView();
	
};
