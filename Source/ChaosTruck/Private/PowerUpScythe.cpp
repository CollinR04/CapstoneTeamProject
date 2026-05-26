// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpScythe.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
UPowerUpScythe::UPowerUpScythe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AActor> ScytheBP(TEXT("/Game/Blueprints/BP_ScytheActor"));
	if (ScytheBP.Succeeded())
	{
		ScytheActor = ScytheBP.Class;
	}

	PowerUpName = "Scythe";

	SetIsReplicatedByDefault(true);
}

// Called when the game starts or when spawned
void UPowerUpScythe::BeginPlay()
{
	Super::BeginPlay();
	
	SpinSpeed = 1200.0f;
	OrbitSpeed = 500.0f;
	OrbitRadius = 1500.0f;
	CurOrbitAngle = 0.0f;
	CurLocalAngle = 0.0f;
	HeightOffset = 400.0f;
	PowerUpDuration = 10.0f;
	TimeLeft = 0.0f;

	// UE_LOG(LogTemp, Warning, TEXT("Scythe is here"));
}

// Called every frame
void UPowerUpScythe::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	// Nothing to update
	if (!GetOwner() || !ScytheInstance)
	{
		return;
	}
	
	if (IsValid(ScytheInstance) == false)
	{
		return;
	}

	// Broadcasts that scythe is finished and destroys itself
	if (TimeLeft <= 0.0f) {
		OnPowerUpFinished.Broadcast();

		if (IsValid(ScytheInstance))
		{
			ScytheInstance->Destroy();
			ScytheInstance = nullptr;
		}
		
		DestroyComponent();
		return;
	}

	CurOrbitAngle = FMath::Fmod(CurOrbitAngle + OrbitSpeed * DeltaTime, 360.0f);
	CurLocalAngle = FMath::Fmod(CurLocalAngle + SpinSpeed * DeltaTime, 360.0f);

	// Spins in place
	// StaticMesh->AddLocalRotation(DeltaRotation);

	// AActor* Player = GetOwner();
	// if (!Player) return;

	// Gets current angles
	//CurOrbitAngle += OrbitSpeed * DeltaTime;
	//if (CurOrbitAngle >= 360.0f)
	//{
	//	CurOrbitAngle -= 360.0f;
	//}
	//CurLocalAngle += SpinSpeed * DeltaTime;
	//if (CurLocalAngle >= 360.0f)
	//{
	//	CurLocalAngle -= 360.0f;
	//}

	// Spins around vehicle
	FVector Anchor = GetOwner()->GetActorLocation() + FVector(0.0f, 0.0f, HeightOffset);
	// FRotator Rotation = FRotator(0.0f, CurOrbitAngle, 0.0f);
	FVector NewPos = FVector(OrbitRadius, 0.0f, 0.0f).RotateAngleAxis(CurOrbitAngle, FVector::UpVector);
	FRotator NewRot = FRotator(0.0f, CurLocalAngle, 0.0f);

	// Sets new position + rotation and decreases duration
	if (IsValid(ScytheInstance))
	{
		ScytheInstance->SetActorLocation(Anchor + NewPos);
		ScytheInstance->SetActorRotation(NewRot);
	}
	
	TimeLeft -= DeltaTime;

	// UE_LOG(LogTemp, Warning, TEXT("Time left: %f"), PowerUpDuration);
}

void UPowerUpScythe::UsePowerUp(bool Back)
{
	// UE_LOG(LogTemp, Warning, TEXT("Scythe activated"));
	
	if (!GetOwner()->HasAuthority()) return;

	TimeLeft = PowerUpDuration;
	CurOrbitAngle = 0.0f;
	CurLocalAngle = 0.0f;

	if (!ScytheActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ScytheActor not assigned"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bNoFail = true;
	SpawnParams.Instigator = Cast<APawn>(GetOwner());

	FVector StartLoc = GetOwner()->GetActorLocation() + FVector(OrbitRadius, 0, HeightOffset);
	FRotator StartRot = FRotator::ZeroRotator;

	ScytheInstance = GetWorld()->SpawnActor<AActor>(ScytheActor, StartLoc, StartRot, SpawnParams);

	if (ScytheInstance)
	{
		ScytheInstance->SetReplicates(true);
		ScytheInstance->SetReplicateMovement(true);

		//UPrimitiveComponent* ScytheCollision = Cast<UPrimitiveComponent>(ScytheInstance->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		USkeletalMeshComponent* ScytheCollision = Cast<USkeletalMeshComponent>(ScytheInstance->FindComponentByClass<USkeletalMeshComponent>());
		if (ScytheCollision)
		{
			AActor* Self = GetOwner();
			ScytheCollision->IgnoreActorWhenMoving(Self, true);
			ScytheCollision->SetNotifyRigidBodyCollision(true);
			ScytheCollision->SetGenerateOverlapEvents(true);
			ScytheCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			ScytheCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
			ScytheCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);


			if (!IsValid(ScytheCollision))
			{
				return;
			}
			if (!IsValid(this))
			{
				return;
			}
			ScytheCollision->OnComponentBeginOverlap.AddDynamic(this, &UPowerUpScythe::OnOverlapBegin);
		}
	}
}

void UPowerUpScythe::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor == GetOwner()) return; // skip owner

	//// Confirm it's another player
	//if (OtherActor->ActorHasTag("Player") || OtherActor->IsA(PlayerActor))
	//{
	//	UE_LOG(LogTemp, Log, TEXT("Scythe hit %s!"), *OtherActor->GetName());
	//	// apply damage or effect here
	//}

	UE_LOG(LogTemp, Warning, TEXT("Scythe hit!"));
}

