// Fill out your copyright notice in the Description page of Project Settings.
#include "GTDAGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

AGTDAGamemode::AGTDAGamemode()
{
}

// InitGame runs before any player spawns, including the listen server host.
// This is intentionally earlier than BeginPlay to ensure spawn arrays are
// populated before FindPlayerStart is ever called.
void AGTDAGamemode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    CollectTaggedPlayerStarts();
    RefreshAvailableSpawnPoints();
    UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: InitGame - AllSpawnPoints found: %d"), AllSpawnPoints.Num());
}

// BeginPlay is used only to verify the spawn arrays are still intact
// after the initial host player has already been assigned a spawn point.
void AGTDAGamemode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: BeginPlay - AllSpawnPoints: %d, Available: %d"),
        AllSpawnPoints.Num(), AvailableSpawnPoints.Num());
}

// Scans the level for PlayerStart actors tagged "0", "1", "2", "3" in order.
// Only the first PlayerStart found per tag is added, giving us one slot per player.
// The full actor list is logged first so we can debug missing or untagged actors.
void AGTDAGamemode::CollectTaggedPlayerStarts()
{
    AllSpawnPoints.Empty();

    const TArray<FName> OrderedTags = { TEXT("0"), TEXT("1"), TEXT("2"), TEXT("3") };

    // Log every PlayerStart in the level regardless of tags so we can
    // catch cases where actors exist but have no tags assigned.
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        APlayerStart* PS = *It;
        if (PS)
        {
            FString TagList = TEXT("none");
            if (PS->Tags.Num() > 0)
            {
                TagList = TEXT("");
                for (const FName& T : PS->Tags)
                    TagList += T.ToString() + TEXT(" ");
            }
            UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: Found PlayerStart [%s] - Tags: [%s]"),
                *PS->GetName(), *TagList);
        }
    }

    // Add one PlayerStart per tag in order (0, 1, 2, 3) so spawn slots
    // always map predictably to player indices.
    for (const FName& Tag : OrderedTags)
    {
        for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
        {
            APlayerStart* PS = *It;
            if (PS && PS->Tags.Contains(Tag))
            {
                AllSpawnPoints.Add(PS);
                UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: Added PlayerStart [%s] for tag [%s]"),
                    *PS->GetName(), *Tag.ToString());
                break;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: CollectTaggedPlayerStarts complete - %d/%d slots filled"),
        AllSpawnPoints.Num(), OrderedTags.Num());
}

// Resets the available pool back to the full AllSpawnPoints list and clears
// the used list. Called at init and whenever the available pool runs dry.
void AGTDAGamemode::RefreshAvailableSpawnPoints()
{
    AvailableSpawnPoints = AllSpawnPoints;
    UsedSpawnPoints.Empty();
    UE_LOG(LogTemp, Log, TEXT("GTDAGameMode: Spawn point list refreshed with %d points"), AvailableSpawnPoints.Num());
}

// Overrides UE's default spawn point selection.
// Key behaviours:
//   - If a controller already has an assigned spawn (e.g. on respawn or a
//     second FindPlayerStart call from the listen server host), reuse it.
//     This prevents the Blueprint respawn flow and UE's internal double-call
//     on listen server from consuming extra slots.
//   - Otherwise, take the next available slot, move it to UsedSpawnPoints,
//     and record the controller -> spawn mapping in AssignedSpawnPoints.
//   - If no slots remain, refresh the pool and try again before falling
//     back to UE's default behaviour.
AActor* AGTDAGamemode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
    // Check if this controller was already given a spawn point.
    // This handles: listen server host double-call, Blueprint respawn via
    // K2_FindPlayerStart, and any other re-entry for the same controller.
    if (AssignedSpawnPoints.Contains(Player))
    {
        UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: FindPlayerStart - Controller [%s] already assigned: YES - reusing [%s]"),
            *Player->GetName(), *AssignedSpawnPoints[Player]->GetName());
        return AssignedSpawnPoints[Player];
    }

    UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: FindPlayerStart - Controller [%s] already assigned: NO - assigning new"),
        *Player->GetName());
    UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: FindPlayerStart called - Available: %d, Used: %d"),
        AvailableSpawnPoints.Num(), UsedSpawnPoints.Num());

    // If the pool is empty, attempt a refresh before giving up.
    // This can happen if all 4 slots were used and a 5th player joins.
    if (AvailableSpawnPoints.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: Available spawn points exhausted - refreshing list"));
        RefreshAvailableSpawnPoints();
    }

    // If still empty after refresh, no tagged PlayerStarts exist in the level.
    // Fall back to UE's default which picks any PlayerStart it can find.
    if (AvailableSpawnPoints.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("GTDAGameMode: No tagged PlayerStart actors found - falling back to default"));
        return Super::FindPlayerStart_Implementation(Player, IncomingName);
    }

    // Take the first available spawn point, move it to the used list,
    // and record the assignment so this controller reuses it on respawn.
    AActor* ChosenSpawn = AvailableSpawnPoints[0];
    AvailableSpawnPoints.RemoveAt(0);
    UsedSpawnPoints.Add(ChosenSpawn);
    AssignedSpawnPoints.Add(Player, ChosenSpawn);

    UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: Player [%s] assigned to spawn [%s] | Remaining: %d"),
        *Player->GetName(), *ChosenSpawn->GetName(), AvailableSpawnPoints.Num());

    // On-screen confirmation during development so we can verify assignments
    // without opening the output log.
    if (GEngine)
    {
        FString Msg = FString::Printf(TEXT("Player %s spawned at %s | Remaining: %d"),
            *Player->GetName(),
            *ChosenSpawn->Tags[0].ToString(),
            AvailableSpawnPoints.Num());
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);
    }

    return ChosenSpawn;
}

// Called when a player successfully connects. Super handles the actual
// spawn flow which will trigger FindPlayerStart, so no spawn logic lives here.
void AGTDAGamemode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: PostLogin - Player [%s] joined"), *NewPlayer->GetName());
}

// Called when a player disconnects. Frees their assigned spawn point back
// into the available pool so it can be reused by a new player joining.
void AGTDAGamemode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    if (AssignedSpawnPoints.Contains(Exiting))
    {
        AActor* FreedSpawn = AssignedSpawnPoints[Exiting];
        AssignedSpawnPoints.Remove(Exiting);
        UsedSpawnPoints.Remove(FreedSpawn);
        AvailableSpawnPoints.Add(FreedSpawn);
        UE_LOG(LogTemp, Warning, TEXT("GTDAGameMode: Player [%s] left - freed spawn [%s]"),
            *Exiting->GetName(), *FreedSpawn->GetName());
    }
}