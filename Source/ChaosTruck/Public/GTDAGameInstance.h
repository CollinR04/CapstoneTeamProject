// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GTDAGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSTRUCK_API UGTDAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	/** Lifecycle function called after construction once the UWorld exists */
	virtual void Init() override;

public:
	void ChangeMap(FString MapName);
	UFUNCTION(BlueprintCallable)
	FString SetGameMode(const FString& SelectedGameMode, const FString& Mapname);

protected:
	/** Testing of Sentry.io Logging and Crash reporting */
	static bool VerifySentryLogging(const UGTDAGameInstance* GameInstance);

};
