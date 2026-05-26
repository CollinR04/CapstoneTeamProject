// Fill out your copyright notice in the Description page of Project Settings.


#include "TruckMovementComponent.h"

// Sets default values for this component's properties
UTruckMovementComponent::UTruckMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Required for RPC calls
	SetIsReplicatedByDefault(true);

}

// Called every frame
void UTruckMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// Do forward movement
	//Accelerate(DeltaTime);
    AActor* Owner{ GetOwner() };
	if (Owner && Owner->HasAuthority())
	{
        Server_Accelerate(DeltaTime);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Current vel: %f"), MovementAnchor->GetPhysicsLinearVelocity().Length() );

}

void UTruckMovementComponent::SetMovementAnchor(UPrimitiveComponent* MovementComponent)
{
	this->MovementAnchor = MovementComponent;
}

void UTruckMovementComponent::SetMass(float Mass)
{
	OwnerMass = Mass;
}

void UTruckMovementComponent::SetGroundNormal(FVector Normal)
{
	GroundNormal = Normal;
}

void UTruckMovementComponent::AddAcceleration(float ActionValue)
{
	// Add easing to acceleration input
	AccelerationInput = ActionValue;// FMath::FInterpTo(AccelerationInput, ActionValue, GetWorld()->GetDeltaSeconds(), AccelTime); <- Old version w/ too much easing
	
}

void UTruckMovementComponent::Steer(float ActionValue)
{
	// Add torque to steer vehicle
	float ReverseModifier{ 1.0f };
	if (AccelerationInput < 0)
	{
		ReverseModifier = -1.0f;
	}
	float UpsideDownModifier{ 1.0f };
	// Check if vehicle is upside down as can happen on arena walls
	//UE_LOG(LogTemp, Warning, TEXT("upsidedown %f"), MovementAnchor->GetForwardVector().Dot(FVector(0.0f, 0.0f, 1.0f)));
	if (MovementAnchor->GetUpVector().Dot(FVector(0.0f, 0.0f, 1.0f)) < 0.5f // 0.5f is when the truck is perpendicular to the ground
		 && (MovementAnchor->GetForwardVector().Dot(FVector(0.0f, 0.0f, 1.0f)) > 0.5f || MovementAnchor->GetForwardVector().Dot(FVector(0.0f, 0.0f, 1.0f)) < -0.5f)) // Makes controls still intuitive on ramps
	{
		UpsideDownModifier = -1.0f;
	}
	MovementAnchor->AddTorqueInRadians(FVector(SteeringTorque.X, SteeringTorque.Y, SteeringTorque.Z * ActionValue * ReverseModifier * UpsideDownModifier));
}

float UTruckMovementComponent::GetBoostDecay()
{
	return BoostDrain;
}

bool UTruckMovementComponent::Server_Boost_Validate(float BoostNum)
{
	if (BoostNum < 0.0)
	{
		return false;
	}
	return true;
}

void UTruckMovementComponent::Server_Boost_Implementation(float BoostNum)
{
	if (BoostNum > 0.0)
	{
		MaxSpeed = BaseMaxSpeed * BoostMult;
        Speed = BaseSpeed * BoostMult;

	}
	else
	{
		MaxSpeed = BaseMaxSpeed;
		Speed = BaseSpeed;
	}
}

void UTruckMovementComponent::Server_UnBoost_Implementation()
{
	MaxSpeed = BaseMaxSpeed;
	Speed = BaseSpeed;
}

bool UTruckMovementComponent::Server_UnBoost_Validate()
{
	return true;
}

void UTruckMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTruckMovementComponent, Speed);
	DOREPLIFETIME(UTruckMovementComponent, AccelerationInput);
	DOREPLIFETIME(UTruckMovementComponent, Acceleration);
	DOREPLIFETIME(UTruckMovementComponent, MaxAccel);
	//DOREPLIFETIME(UTruckMovementComponent, DecelTime);
	DOREPLIFETIME(UTruckMovementComponent, BaseSpeed);
	DOREPLIFETIME(UTruckMovementComponent, BaseMaxSpeed);
	DOREPLIFETIME(UTruckMovementComponent, MaxSpeed);
	DOREPLIFETIME(UTruckMovementComponent, BoostMult);
	DOREPLIFETIME(UTruckMovementComponent, BoostDrain);
	DOREPLIFETIME(UTruckMovementComponent, OwnerMass);
	DOREPLIFETIME(UTruckMovementComponent, Traction);
}

FVector UTruckMovementComponent::CalculateAcceleration(float DeltaTime)
{
	// Increase acceleration using AccelerationInput
	Acceleration = FMath::Lerp<float>(0.0f, MaxAccel, AccelerationInput);

	// Decelerate over time with easing
	AccelerationInput = 0.0f;//FMath::FInterpTo(AccelerationInput, 0.0f, DeltaTime, DecelTime);
	//if (GetOwner()->HasAuthority())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Authority"));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("No Authority"));
	//}

	// Calculate and return forward force
    return FVector::VectorPlaneProject(MovementAnchor->GetForwardVector(), GroundNormal) * Speed * Acceleration * OwnerMass;
}

FVector UTruckMovementComponent::CalculateFrictionForce()
{
	// Get perpendicular from the forward vector
	FVector TractionDirection{ MovementAnchor->GetRightVector() };
	FVector TractionForce{ MovementAnchor->GetComponentVelocity().ProjectOnTo(TractionDirection) * Traction };
	return TractionForce;
}

// Called when the game starts
void UTruckMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	SetIsReplicated(true);

	// Set speed vars
	Speed = BaseSpeed;
	MaxSpeed = BaseMaxSpeed;
}

void UTruckMovementComponent::Accelerate(float DeltaTime)
{
	if (MovementAnchor)
	{
		if (MovementAnchor->GetPhysicsLinearVelocity().Length() < MaxSpeed)
		{
			auto a = CalculateAcceleration(DeltaTime);
			if (a.Length() > 0.001)
			{
				MovementAnchor->AddForceAtLocation(a, MovementAnchor->GetCenterOfMass());
			}
		}
		if (bIsGrounded)
		{
            MovementAnchor->AddForceAtLocation(CalculateFrictionForce(), MovementAnchor->GetCenterOfMass());
		}
		// Shift CoM to better feel the acceleration
		//MovementAnchor->SetCenterOfMass(CoMOffset * AccelerationInput);
	}
}

bool UTruckMovementComponent::Server_Accelerate_Validate(float DeltaTime)
{
	return true;
}

void UTruckMovementComponent::Server_Accelerate_Implementation(float DeltaTime)
{
	Accelerate(DeltaTime);
}

