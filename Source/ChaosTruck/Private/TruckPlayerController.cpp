// Fill out your copyright notice in the Description page of Project Settings.
#include "TruckPlayerController.h"
#include "Kismet/GameplayStatics.h"

void ATruckPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
    UE_LOG(LogTemp, Warning, TEXT("[OnPossess] Fired on: %s | Level: %s"),
        HasAuthority() ? TEXT("Server") : TEXT("Client"),
        *CurrentLevel);

    if (CurrentLevel == "cemetary")
    {
        UE_LOG(LogTemp, Warning, TEXT("[OnPossess] Correct level, sending Client_SetLobbyView RPC"));
        Client_SetLobbyView();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[OnPossess] Wrong level: %s, skipping"), *CurrentLevel);
    }
}

void ATruckPlayerController::OnRep_Pawn()
{
    Super::OnRep_Pawn();

    UE_LOG(LogTemp, Warning, TEXT("[OnRep_Pawn] Fired on client"));

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ATruckPlayerController::SetUpLobby, 0.5f, false);
    UE_LOG(LogTemp, Warning, TEXT("[OnRep_Pawn] Timer set, SetUpLobby will fire in 0.5s"));
}

void ATruckPlayerController::SetUpLobby()
{
    FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
    UE_LOG(LogTemp, Warning, TEXT("[SetUpLobby] Fired on: %s | Level: %s"),
        HasAuthority() ? TEXT("Server") : TEXT("Client"),
        *CurrentLevel);

    if (CurrentLevel != "cemetary")
    {
        UE_LOG(LogTemp, Warning, TEXT("[SetUpLobby] Wrong level, returning early"));
        return;
    }

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("LobbyCamera"), FoundActors);
    UE_LOG(LogTemp, Warning, TEXT("[SetUpLobby] Found %d actors with LobbyCamera tag"), FoundActors.Num());

    if (FoundActors.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("[SetUpLobby] No LobbyCamera found! Check: 1) Actor has LobbyCamera tag 2) Replicates is ticked on the Blueprint"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("[SetUpLobby] Using camera actor: %s"), *FoundActors[0]->GetName());
    SetViewTargetWithBlend(FoundActors[0]);
    UE_LOG(LogTemp, Warning, TEXT("[SetUpLobby] SetViewTargetWithBlend called"));
}

void ATruckPlayerController::Client_SetLobbyView_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("[Client_SetLobbyView] RPC received on: %s"),
        HasAuthority() ? TEXT("Server/Host") : TEXT("Client"));

    SetUpLobby();
}