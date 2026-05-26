// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Armor.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class CHAOSTRUCK_API UArmor : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Default constructor
	UArmor();

	// Calculate and set new health after given damage. If damage taken if greater than total
	// health, return remaining untaken damage
	UFUNCTION(BlueprintCallable)
	float TakeDamage(float Damage);

	// Return maximum health
	float GetMaxHealth() const;

	// Return armor piece's current health
	float GetCurrentHealth() const;

	// Return armor piece's break order, lowest breaking first
	int GetBreakPriority() const;

	// Override operator< to sort in destruction order
	friend bool operator<(const UArmor& l, const UArmor& r);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	// RPC to destroy on the server
	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_BreakOff(UArmor* Armor);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_BreakOff(UArmor* Armor);

	// Change component's mesh to the broken version
	void SwapToBroken();

	// Remove mesh and component
	void BreakOff();

	// Maximum health for this armor piece, replicated
	UPROPERTY(EditAnywhere, Replicated)
	float MaxHealth;

	// Current health for this armor piece, replicated
	UPROPERTY(BlueprintReadOnly, Replicated/*ReplicatedUsing = OnRep_CurrentHealth*/)
	float CurrentHealth;

	void SetCurrentHealth(float NewHealth) { CurrentHealth = NewHealth; }

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_SetCurrentHealth(UArmor* Armor, float NewHealth);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_SetMaxHealth(UArmor* Armor, float NewMaxHealth);

	//UFUNCTION()
	//void OnRep_CurrentHealth();

	// Determines break order when in a collection
	UPROPERTY(EditAnywhere)
	int BreakPriority;

	// Partially damaged mesh for showing reduced health
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* DamagedMesh;
	
};

