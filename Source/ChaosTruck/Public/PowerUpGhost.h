// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpEffect.h"
#include "PowerUpGhost.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHAOSTRUCK_API UPowerUpGhost : public UPowerUpEffect
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SelfVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OtherVisible;

	UPROPERTY(ReplicatedUsing = DisplayVisiblity)
	bool bGhostActive;

public:
	// Sets default values for this actor's properties
	UPowerUpGhost();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Starts process; called by PlayerInventory
	virtual void UsePowerUp(bool Back) override;

	UFUNCTION(Server, Reliable)
	void ServerUseGhost();

	UFUNCTION(NetMulticast, Reliable)
	void DisplayVisiblity();

	UFUNCTION()
	void SetOpacity(float Opacity);

	UFUNCTION()
	void SetOriginal();

	UFUNCTION()
	void RestoreMaterialsFromLambda();

	UFUNCTION()
	bool CheckForLight(UStaticMeshComponent* Mesh);

private:
	UPROPERTY()
	TArray<AActor*> Shell;

	UPROPERTY()
	TArray<UStaticMeshComponent*> MeshParts;

	UPROPERTY()
	TArray<UStaticMeshComponent*> ArmorParts;

	UPROPERTY()
	TArray<UChildActorComponent*> Armor;

	UPROPERTY()
	TArray<UMaterialInterface*> OrigMaterials;

	UPROPERTY()
	TArray<UMaterialInterface*> ArmorMaterials;

	UPROPERTY()
	TArray<UMaterialInterface*> TireMaterials;

	UPROPERTY()
	TArray<UMaterialInterface*> FrameMaterials;

	TArray<TFunction<void()>> RestoreFunctions;

	UMaterialInterface* GhostMaterial;

	float TimeLeft;
};
