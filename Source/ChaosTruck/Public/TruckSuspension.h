// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"
#include "TruckSuspension.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class CHAOSTRUCK_API UTruckSuspension : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTruckSuspension();

	UFUNCTION(BlueprintCallable)
	FVector GetImpactPoint() const { return ImpactPoint; }

	UFUNCTION(BlueprintCallable)
	FVector GetImpactNormal() const { return ImpactNormal; }

	// Return whether the wheel is on the ground
	UFUNCTION(BlueprintCallable)
	bool GetHit() const;

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetWheel() const { return Wheel; }

	UFUNCTION(BlueprintCallable)
	void SetWheel(UStaticMeshComponent* NewWheel) { this->Wheel = NewWheel; }

	UFUNCTION(BlueprintCallable)
	void ResetWheelMesh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Wheel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CompressionAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double MaxExtension;

	UFUNCTION(BlueprintCallable)
	void SetWheelMesh(UStaticMesh* Mesh);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_ApplySuspension(double SuspensionForce);

	UPROPERTY(EditAnywhere)
	float SuspensionDistance{ 100 };

	//UPROPERTY(EditAnywhere)
	//float CompressionForce{ 100 };

	//UPROPERTY(EditAnywhere)
	//float DampingCoefficient{ 0.05 };

	UPrimitiveComponent* Chasis;

	bool bHit{ false };

	FVector ImpactPoint;
	FVector ImpactNormal;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
