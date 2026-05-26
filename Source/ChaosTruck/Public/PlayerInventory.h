// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInventory.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHAOSTRUCK_API UPlayerInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, VisibleAnywhere)
	bool CanPickUp;

	UPROPERTY(Replicated, VisibleAnywhere)
	bool CanUse;

	UPROPERTY(Replicated, VisibleAnywhere)
	int CurItemID;

	UPROPERTY(Replicated, VisibleAnywhere)
	FName CurItemName;

	UPROPERTY(VisibleAnywhere)
	bool Back;

	UPROPERTY(EditDefaultsOnly, Category = "Ghost")
	UMaterialInterface* GhostMaterial;

public:

	// Checks ID of power-up and stores it
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	void PickUpItem(int ID, FName Name);

	// UseItem, on server side
	UFUNCTION(Server, Reliable)
	void ServerUseItem(bool Use, bool Other);

	// Checks ID of stored power-up and runs its respective script
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	void UseItem(bool Use, bool Other);

	// Checks if player is already holding a power-up
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	const bool GetPickUp() { return CanPickUp; }

	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	const bool GetUse() { return CanUse; }

	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	const int GetID() { return CurItemID; }

	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	const FName GetName() { return CurItemName; }

	// Activates ItemFinish if player is killed
	UFUNCTION(BlueprintCallable, Category = "PowerUp")
	void FinishEarly();

	// Allows player to pick-up power-ups again; also checks if player is already touching a power-up
	UFUNCTION()
	void ItemFinish();

	/*UFUNCTION(BlueprintCallable, Category = "PowerUp")
	AActor* CheckOverlap(AActor*);*/
};
