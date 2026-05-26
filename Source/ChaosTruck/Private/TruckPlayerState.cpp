// Fill out your copyright notice in the Description page of Project Settings.


#include "TruckPlayerState.h"
#include "Net/UnrealNetwork.h"

void ATruckPlayerState::RepTeamID_Implementation()
{
}

void ATruckPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Register simple replication
    DOREPLIFETIME(ATruckPlayerState, TeamID);
}
