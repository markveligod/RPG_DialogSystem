// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Logging/LogVerbosity.h"
#include "RPG_DialogFunctionLibrary.generated.h"

class URPG_DialogObjectBase;
class URPG_DialogComponentBase;
/**
 *
 */
UCLASS()
class RPG_DIALOGSYSTEM_API URPG_DialogFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static bool GetPlayerControllerByIDPlayer(UObject* WorldContextObject, int32 IDPlayer, APlayerController*& PlayerController, FString& Reason);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static APlayerController* GetPlayerControllerByIDPlayerCompact(UObject* WorldContextObject, int32 IDPlayer);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static bool GetPlayerStateByIDPlayer(UObject* WorldContextObject, int32 IDPlayer, APlayerState*& PlayerState, FString& Reason);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static APlayerState* GetPlayerStateByIDPlayerCompact(UObject* WorldContextObject, int32 IDPlayer);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static bool GetPlayerNameByIDPlayer(UObject* WorldContextObject, int32 IDPlayer, FString& PlayerName, FString& Reason);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static FString GetPlayerNameByIDPlayerCompact(UObject* WorldContextObject, int32 IDPlayer);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static bool GetPlayerNameByPlayerController(UObject* WorldContextObject, APlayerController* PC, FString& PlayerName, FString& Reason);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static FString GetPlayerNameByPlayerControllerCompact(UObject* WorldContextObject, APlayerController* PC);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static bool GetPlayerIDByPlayerController(UObject* WorldContextObject, APlayerController* PC, int32& IDPlayer, FString& Reason);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static int32 GetPlayerIDByPlayerControllerCompact(UObject* WorldContextObject, APlayerController* PC);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static void RunDialog(UObject* WorldContextObject, URPG_DialogComponentBase* DialogComponent);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static bool GetDialogComponent(UObject* WorldContextObject, AActor* ActorRun, URPG_DialogComponentBase*& DialogComponent, FString& Reason);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static URPG_DialogComponentBase* GetDialogComponentCompact(UObject* WorldContextObject, AActor* ActorRun);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static bool GetDialogObject(UObject* WorldContextObject, AActor* ActorRun, URPG_DialogObjectBase*& DialogObject, FString& Reason);

    /** @public **/
    UFUNCTION(BlueprintCallable, Category = "DialogSystemLibrary", meta = (WorldContext = "WorldContextObject"))
    static URPG_DialogObjectBase* GetDialogObjectCompact(UObject* WorldContextObject, AActor* ActorRun);
};
