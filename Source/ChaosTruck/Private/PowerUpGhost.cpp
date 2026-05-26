// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpGhost.h"
#include "Net/UnrealNetwork.h"
#include "Components/SpotLightComponent.h"

UPowerUpGhost::UPowerUpGhost()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	PowerUpName = "Ghost";
	SelfVisible = 0.5f;
	OtherVisible = 0.0f;
	PowerUpDuration = 15.0f;
	TimeLeft = 0.0f;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(
		TEXT("/Game/Materials/m_ghost")
	);

	if (MatFinder.Succeeded())
	{
		GhostMaterial = MatFinder.Object;
	}
}

void UPowerUpGhost::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->GetComponents<UStaticMeshComponent>(MeshParts);

	for (UStaticMeshComponent* Mesh : MeshParts)
	{
		if (!CheckForLight(Mesh))
		{
			continue;
		}
		for (int i = 0; i < Mesh->GetNumMaterials(); i++)
		{
			auto Material = Mesh->GetMaterial(i);
			RestoreFunctions.Add([Mesh, Material, i]()
				{
					Mesh->SetMaterial(i, Material);
				});
		}
	}

	GetOwner()->GetComponents<UChildActorComponent>(Armor);

	GetOwner()->GetAttachedActors(Shell);

	for (AActor* Actor : Shell)
	{
		if (!Actor)
		{
			continue;
		}

		Actor->GetComponents<UStaticMeshComponent>(ArmorParts);

		for (UStaticMeshComponent* Mesh : ArmorParts)
		{
			if (!CheckForLight(Mesh))
			{
				continue;
			}

			for (int i = 0; i < Mesh->GetNumMaterials(); i++)
			{
				auto Material = Mesh->GetMaterial(i);
				RestoreFunctions.Add([Mesh, Material, i]()
					{
						Mesh->SetMaterial(i, Material);
					});
			}
		}
	}
}

// Called every frame
void UPowerUpGhost::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TimeLeft <= 0.0f)
	{
		return;
	}

	TimeLeft -= DeltaTime;

	if (TimeLeft <= 0.0f)
	{	
		bGhostActive = false;
		DisplayVisiblity();
		OnPowerUpFinished.Broadcast();
	}
}

void UPowerUpGhost::UsePowerUp(bool Back)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		ServerUseGhost();
	}
	else
	{
		ServerUseGhost();
	}
}

void UPowerUpGhost::ServerUseGhost_Implementation()
{
	bGhostActive = true;
	TimeLeft = PowerUpDuration;

	DisplayVisiblity();
}

void UPowerUpGhost::DisplayVisiblity_Implementation()
{
	if (bGhostActive)
	{
		const bool bIsSelf = Cast<APawn>(GetOwner()) && Cast<APawn>(GetOwner())->IsLocallyControlled();

		if (bIsSelf)
		{
			SetOpacity(SelfVisible);
		}
		else
		{
			SetOpacity(OtherVisible);
		}
	}
	else
	{
		RestoreMaterialsFromLambda();
	}
}

void UPowerUpGhost::SetOpacity(float Opacity)
{
	if (!GhostMaterial)
	{
		return;
	}

	for (UStaticMeshComponent* Mesh : MeshParts)
	{
		if (!CheckForLight(Mesh))
		{
			continue;
		}

		for (int i = 0; i < Mesh->GetNumMaterials(); i++)
		{
			Mesh->SetMaterial(i, GhostMaterial);
			UMaterialInstanceDynamic* MatInst = Mesh->CreateAndSetMaterialInstanceDynamic(i);
			if (MatInst)
			{
				MatInst->SetScalarParameterValue(FName("OpacityValue"), Opacity);
			}
		}
	}

	for (UStaticMeshComponent* Mesh : ArmorParts)
	{
		if (!CheckForLight(Mesh))
		{
			continue;
		}

		for (int i = 0; i < Mesh->GetNumMaterials(); i++)
		{
			Mesh->SetMaterial(i, GhostMaterial);
			UMaterialInstanceDynamic* MatInst = Mesh->CreateAndSetMaterialInstanceDynamic(i);
			if (MatInst)
			{
				MatInst->SetScalarParameterValue(FName("OpacityValue"), Opacity);
			}
		}
	}
}

void UPowerUpGhost::SetOriginal()
{
	for (UStaticMeshComponent* Mesh : MeshParts)
	{
		if (!CheckForLight(Mesh))
		{
			continue;
		}
		if (Mesh->GetNumMaterials() == 1)
		{
			for (int i = 0; i < Mesh->GetNumMaterials(); i++)
			{
				Mesh->SetMaterial(i, OrigMaterials[i]);
			}
		}
		else if (Mesh->GetNumMaterials() == 2)
		{
			for (int i = 0; i < Mesh->GetNumMaterials(); i++)
			{
				Mesh->SetMaterial(i, TireMaterials[i]);
			}
		}
		else
		{
			for (int i = 0; i < Mesh->GetNumMaterials(); i++)
			{
				Mesh->SetMaterial(i, FrameMaterials[i]);
			}
		}
	}

	for (UChildActorComponent* Actor : Armor)
	{
		if (!Actor)
		{
			continue;
		}

		for (UStaticMeshComponent* Mesh : ArmorParts)
		{
			if (!CheckForLight(Mesh))
			{
				continue;
			}

			for (int i = 0; i < Mesh->GetNumMaterials(); i++)
			{
				Mesh->SetMaterial(i, ArmorMaterials[i]);
			}
		}
	}
}

void UPowerUpGhost::RestoreMaterialsFromLambda()
{
	for (auto& Func : RestoreFunctions)
	{
		Func();
	}
}

bool UPowerUpGhost::CheckForLight(UStaticMeshComponent* Mesh)
{
	if (!Mesh) return false;

	if (Mesh->GetAttachParent() && Mesh->GetAttachParent()->IsA<USpotLightComponent>()) return false;

	return true;
}

void UPowerUpGhost::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPowerUpGhost, bGhostActive);
}
