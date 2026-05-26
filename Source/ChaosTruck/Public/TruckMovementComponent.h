// Fill out your copyright notice in the Description page of Project Settings.

// Concept inspiration taken from Space Dust Racing https://youtu.be/LG1CtlFRmpU?si=UL0N57OtK9T6or8X
// Acceleration code adapted from Ryan Laley's tutorial blueprints https://www.youtube.com/watch?v=sdXEEeTReO0

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "TruckMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHAOSTRUCK_API UTruckMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTruckMovementComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetMovementAnchor(UPrimitiveComponent* MovementComponent);

	// Set mass before using component
	void SetMass(float Mass);

	UFUNCTION(BlueprintCallable)
	void SetGroundNormal(FVector Normal);

    // Call when throttle EnhancedInput is triggered
	UFUNCTION(BlueprintCallable)
	void AddAcceleration(float ActionValue);

	// Call when steering EnhancedInput is triggered
	UFUNCTION(BlueprintCallable)
	void Steer(float ActionValue);

	UFUNCTION(BlueprintCallable)
	float GetBoostDecay();

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_Boost(float BoostNum);

	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_UnBoost();

	// Replicate properties
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	float BaseMaxSpeed{ 5000 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	float MaxSpeed{ 5000 };

	UPROPERTY(BlueprintReadWrite)
	bool bIsGrounded{ true };

private:	
	
	// RPC to move on the server
	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_Accelerate(float DeltaTime);

	// Move truck forward/backward
	UFUNCTION()
	void Accelerate(float DeltaTime);

	// RPC to steer on the server
	//UFUNCTION(Server, Unreliable, WithValidation)
	//void Server_Steer();

	// Helper funcction to handle natural deceleration and max accceleration
	FVector CalculateAcceleration(float DeltaTime);

	// Calcultae force to apply for steering traction
	FVector CalculateFrictionForce();

	// Amount to accelerate by
	UPROPERTY(VisibleAnywhere, Replicated)
	float AccelerationInput{ 0.0f };

	// Current acceleration
	UPROPERTY(VisibleAnywhere, Replicated)
	float Acceleration{ 0.0f };

	// Normal of ground plane
	FVector GroundNormal;

	UPROPERTY(EditAnywhere, Replicated)
	float MaxAccel{ 2000.0f };

	// Higher values = faster acceleration
	//UPROPERTY(EditAnywhere)
	//float AccelTime{ 2.0 };

	// Higher values = faster deceleration. Should be less than AccelTime
	//UPROPERTY(EditAnywhere, Replicated)
	//float DecelTime{ 1.5 };

	// Movement speed scalar
	UPROPERTY(EditAnywhere, Replicated)
	float BaseSpeed{ 1000 };

	UPROPERTY(VisibleAnywhere, Replicated)
	float Speed;


	// How much faster the truck moves while boosting
	UPROPERTY(EditAnywhere, Replicated)
	float BoostMult{ 1.5 };

	// How quick does boost meter drain
	UPROPERTY(EditAnywhere, Replicated)
	float BoostDrain{ 0.1 };

	// Total mass of attached pawn
	UPROPERTY(VisibleAnywhere, Replicated)
	float OwnerMass{ 0.1 };

	// Maximum center of mass offset
	UPROPERTY(EditAnywhere)
	FVector CoMOffset{ 50.0, 0.0, 0.0 };

	UPROPERTY(EditAnywhere)
	FVector SteeringTorque{ 0.0, 0.0, 100000.0 };

	// Scalar for how much friction is experienced during a turn (should be negative)
	UPROPERTY(EditAnywhere, Replicated)
	float Traction{ -100.0 };

	// Owning actor's component to be moved
	UPrimitiveComponent* MovementAnchor;

};
