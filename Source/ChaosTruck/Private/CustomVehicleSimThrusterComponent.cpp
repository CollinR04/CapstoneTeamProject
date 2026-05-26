


#include "CustomVehicleSimThrusterComponent.h"
#include "SimModule/ThrusterModule.h"


// Sets default values for this component's properties
UCustomVehicleSimThrusterComponent::UCustomVehicleSimThrusterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCustomVehicleSimThrusterComponent::SetBoostMultiplier(float multiplier)
{
	Chaos::FThrusterSimModule* module = (Chaos::FThrusterSimModule*)(this->coreModule);
	if (module != nullptr) {
		module->AccessSetup().BoostMultiplier = multiplier;
	}
}

void UCustomVehicleSimThrusterComponent::SetMaxThrust(float max)
{
	Chaos::FThrusterSimModule* module = (Chaos::FThrusterSimModule*)(this->coreModule);
	if (module != nullptr) {
		module->AccessSetup().MaxThrustForce = max;
	}
}


// Called when the game starts
void UCustomVehicleSimThrusterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCustomVehicleSimThrusterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

Chaos::ISimulationModuleBase* UCustomVehicleSimThrusterComponent::CreateNewCoreModule() const
{
	return this->coreModule = Super::CreateNewCoreModule();
}


