// Fill out your copyright notice in the Description page of Project Settings.


#include "GTDAGameInstance.h"

#include "SentrySubsystem.h"
#include "SentrySettings.h"

void UGTDAGameInstance::Init()
{
	// Calls base class init function to default initialization behavior
	Super::Init();

	// Do the logging verification
	if (!UGTDAGameInstance::VerifySentryLogging(this))
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance: Failed to setup Sentry logging."));
	}
}

void UGTDAGameInstance::ChangeMap(FString MapName)
{
	GetWorld()->ServerTravel(MapName, false, false);
}


FString UGTDAGameInstance::SetGameMode(const FString& SelectedGameMode, const FString& Mapname)
{
	{
		if (SelectedGameMode == "TeamDeathmatch")
		{
			if (Mapname == "RaveYard")  return "Raveyard_TDM";
			if (Mapname == "SandMap")   return "SandMap_TDM";
			if (Mapname == "HighWayHell")      return "HighWayHell_TDM";

			UE_LOG(LogTemp, Warning, TEXT("[SetGameMode] TDM - Unknown map: %s"), *Mapname);
			return FString();
		}

		if (SelectedGameMode == "FreeForAll")
		{
			if (Mapname == "RaveYard")  return "RaveYard";
			if (Mapname == "SandMap")   return "SandMap";
			// Non-game mode maps
			if (Mapname == "cemetary")         return "cemetary";
			if (Mapname == "LobbyMenu")        return "LobbyMenu";
			if (Mapname == "TutorialCemetary") return "TutorialCemetary";
			if (Mapname == "HighWayHell")      return "HighWayHell";

			UE_LOG(LogTemp, Warning, TEXT("[SetGameMode] FFA - Unknown map: %s"), *Mapname);
			return FString();
		}

		UE_LOG(LogTemp, Warning, TEXT("[SetGameMode] No match - Map: %s"), *Mapname);
		return FString();
	}
}

bool UGTDAGameInstance::VerifySentryLogging(const UGTDAGameInstance* GameInstance)
{
	if (IsValid(GameInstance))
	{
		// Grab from global engine reference
		USentrySubsystem* SentrySubsystem = GEngine->GetEngineSubsystem<USentrySubsystem>();

		// Create settings with logging enabled
		SentrySubsystem->InitializeWithSettings(FConfigureSettingsNativeDelegate::CreateLambda(
			[=](USentrySettings* Settings)
			{
				Settings->EnableStructuredLogging = true;

				// Enable specific severity levels for UE_LOG forwarding
				Settings->StructuredLoggingLevels.bOnDebugLog = false;
				Settings->StructuredLoggingLevels.bOnInfoLog = false;
				Settings->StructuredLoggingLevels.bOnWarningLog = true;
				Settings->StructuredLoggingLevels.bOnErrorLog = true;
				Settings->StructuredLoggingLevels.bOnFatalLog = true;
				Settings->bSendBreadcrumbsWithStructuredLogging = false; // Send as structured logs instead of breadcrumbs			

				// Only forward logs from specific categories
				TArray<FString> AllowedCategories;
				AllowedCategories.Add(TEXT("LogBlueprintUserMessages"));
				AllowedCategories.Add(TEXT("LogTemp"));
				Settings->StructuredLoggingCategories = AllowedCategories;
			}));

		return true;
	}

	return false;
}
