// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GTDAGamemode.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSTRUCK_API AGTDAGamemode : public AGameMode
{
	GENERATED_BODY()
	
public:
    AGTDAGamemode();

    // Selects a spawn point for the given controller. Returns the same point
    // if the controller has already been assigned one, preventing duplicate
    // slot consumption on respawn or listen server double-calls.
    virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

protected:
    // Populates spawn arrays before any player spawns. Must run before
    // BeginPlay to cover the listen server host's early spawn.
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

    // Logs spawn array state after initial spawning for debugging.
    virtual void BeginPlay() override;

    // Logs when a player joins. Spawn logic is handled by FindPlayerStart.
    virtual void PostLogin(APlayerController* NewPlayer) override;

    // Frees the disconnecting player's spawn slot back into the available pool.
    virtual void Logout(AController* Exiting) override;

private:
    // All PlayerStart actors found with tags 0-3, populated once in InitGame.
    UPROPERTY(VisibleInstanceOnly, Category = "Spawning")
    TArray<AActor*> AllSpawnPoints;

    // Spawn points not yet assigned to a player. Shrinks as players join.
    UPROPERTY(VisibleInstanceOnly, Category = "Spawning")
    TArray<AActor*> AvailableSpawnPoints;

    // Spawn points currently assigned to active players.
    UPROPERTY(VisibleInstanceOnly, Category = "Spawning")
    TArray<AActor*> UsedSpawnPoints;

    // Maps each controller to their assigned spawn point. Used to return
    // the same point on respawn and prevent duplicate slot consumption.
    UPROPERTY(VisibleInstanceOnly, Category = "Spawning")
    TMap<AController*, AActor*> AssignedSpawnPoints;

    // Scans the level for PlayerStart actors tagged 0-3 and fills AllSpawnPoints.
    void CollectTaggedPlayerStarts();

    // Resets AvailableSpawnPoints from AllSpawnPoints and clears UsedSpawnPoints.
    void RefreshAvailableSpawnPoints();

    // Team assignment
    void AssignTeam(APlayerController* NewPlayer);
    int32 GetTeamSize(int32 TeamID) const;

    // Kill tracking
    void OnPlayerKilled(AController* Killer, AController* Victim);
    void CheckWinCondition();

    // Scoring
    void AddTeamScore(int32 TeamID, int32 Amount);

    // Config (set these in your constructor or per-map)
    int32 KillLimit = 30;
    int32 TeamScores[2] = { 0, 0 };
};