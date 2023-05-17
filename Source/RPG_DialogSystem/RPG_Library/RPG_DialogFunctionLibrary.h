// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Logging/LogVerbosity.h"
#include "RPG_DialogFunctionLibrary.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogDialogSystem, All, All);

/**
 * 
 */
UCLASS()
class RPG_DIALOGSYSTEM_API URPG_DialogFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    /** @public Write a log **/
    static void Print_Log(const TEnumAsByte<ELogVerbosity::Type> LogVerb, const FString& Text, const int Line, const char* Function);

    /** @public Write a clog **/
    static bool Print_CLog(const bool bCond, const TEnumAsByte<ELogVerbosity::Type> LogVerb, const FString& Text, const int Line, const char* Function);

    /** @public Net mode to string **/
    UFUNCTION(BlueprintCallable)
    static FString GetNetModeToString(AActor* InActor);
};
