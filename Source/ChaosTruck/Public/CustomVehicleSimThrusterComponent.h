

#pragma once

#include "CoreMinimal.h"
#include "ChaosModularVehicle/VehicleSimThrusterComponent.h"
#include "CustomVehicleSimThrusterComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHAOSTRUCK_API UCustomVehicleSimThrusterComponent : public UVehicleSimThrusterComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCustomVehicleSimThrusterComponent();

	UFUNCTION(BlueprintCallable)
	void SetBoostMultiplier(float multiplier);

	UFUNCTION(BlueprintCallable)
	void SetMaxThrust(float max);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual Chaos::ISimulationModuleBase* CreateNewCoreModule() const override;
	
private:
	mutable Chaos::ISimulationModuleBase* coreModule;
};
