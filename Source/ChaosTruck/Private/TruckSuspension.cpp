// Fill out your copyright notice in the Description page of Project Settings.


#include "TruckSuspension.h"

// Sets default values for this component's properties
UTruckSuspension::UTruckSuspension()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    this->SetIsReplicatedByDefault(true);

	// Create wheel
	//Wheel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wheel"));
	//Wheel->SetWorldLocation(GetComponentLocation());
	//Wheel->SetRelativeScale3D(FVector(0.25f));
	//FRotator WheelRotator(0.0f, 90.0f, 0.0f);
	//Wheel->SetRelativeRotation(WheelRotator);
}


bool UTruckSuspension::GetHit() const
{
	return bHit;
}

void UTruckSuspension::ResetWheelMesh()
{
	Wheel->DestroyComponent();
	Wheel = NewObject<UStaticMeshComponent>(this);
	Wheel->RegisterComponent();
	Wheel->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
    Wheel->SetWorldLocation(GetComponentLocation());
}

void UTruckSuspension::SetWheelMesh(UStaticMesh* Mesh)
{
	Wheel->SetStaticMesh(Mesh);
}

// Called when the game starts
void UTruckSuspension::BeginPlay()
{
	Super::BeginPlay();

	// Set chasis reference
	Chasis = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	
}

bool UTruckSuspension::Server_ApplySuspension_Validate(double SuspensionForce)
{
	return true;
}

void UTruckSuspension::Server_ApplySuspension_Implementation(double SuspensionForce)
{
	Chasis->AddForceAtLocation(ImpactNormal * SuspensionForce, GetComponentLocation());
}


// Called every frame
void UTruckSuspension::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Raycast to see how far from the ground the top of the suspension is
	FVector TraceStart = GetComponentLocation();
	FVector TraceEnd = TraceStart + GetUpVector() * -SuspensionDistance;
	FCollisionQueryParams TraceParams(FName(TEXT("SuspensionTrace")), false, GetOwner());
	FHitResult TraceResult;
	bHit = GetWorld()->LineTraceSingleByChannel(TraceResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	// Draw for debugging
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.0f, 0, 1.0f);

    // Percentage of compressed suspension
    CompressionAmount = (SuspensionDistance - TraceResult.Distance) / SuspensionDistance;

	// Move wheels
	MaxExtension =  SuspensionDistance * 0.3;
	//Wheel->SetRelativeLocation(FVector(0.0, 0.0,  FMath::Lerp<float>(MaxExtension, 0.0, CompressionAmount)));
	//UE_LOG(LogTemp, Warning, TEXT("MaxExtension: %f"), MaxExtension);

	// Apply upward force based on how compressed the suspension is
	if (bHit)
	{
        ImpactNormal = TraceResult.ImpactNormal;
        ImpactPoint = TraceResult.ImpactPoint;

	//	UE_LOG(LogTemp, Warning, TEXT("Hit"));
	//	Wheel->SetWorldLocation(this->GetComponentLocation());
 //       // Store for other calculations
 //       //UE_LOG(LogTemp, Warning, TEXT("ImpactPoint: %f"), TraceResult.ImpactPoint.Z);

	//	// Percentage of compressed suspension
	//	float CompressionAmount{ (SuspensionDistance - TraceResult.Distance) / SuspensionDistance };
	//	UE_LOG(LogTemp, Warning, TEXT("Compression percentage: %f"), CompressionAmount); // For debugging

	//	// Apply force from where this component is located
	//	if (Chasis)
	//	{
	//		// F = -kx - bv
	//		// k = Spring's "strength"
	//		// x = compression amount
	//		// b = damping coefficient
	//		// v = velocity at point of movement
	//		double SuspensionForce{DeltaTime * (CompressionForce * CompressionAmount - DampingCoefficient * GetComponentVelocity().Z) };
 //           Server_ApplySuspension(SuspensionForce);
	//	}
	//	
	}
	else
	{
		ImpactNormal = FVector(0.0f, 0.0f, 1.0f);
	}

	// Move wheels to reflect compression state


}

