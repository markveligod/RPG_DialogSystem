// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogSystem/RPG_Library/RPG_DialogFunctionLibrary.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "RPG_DialogSystem/RPG_DialogComponent/RPG_DialogComponentBase.h"

bool URPG_DialogFunctionLibrary::GetPlayerControllerByIDPlayer(UObject* WorldContextObject, const int32 IDPlayer, APlayerController*& PlayerController, FString& Reason)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        Reason = FString::Printf(TEXT("Invalid World Context"));
        return false;
    }

    AGameStateBase* GS = World->GetGameState();
    if (!GS)
    {
        Reason = FString::Printf(TEXT("Invalid Game State"));
        return false;
    }

    for (const auto& PS : GS->PlayerArray)
    {
        if (PS->GetPlayerId() != IDPlayer) continue;
        PlayerController = PS->GetOwner<APlayerController>();
        if (!PlayerController)
        {
            Reason = FString::Printf(TEXT("Invalid Player Controller"));
            return false;
        }
        return true;
    }

    Reason = FString::Printf(TEXT("ID: %i is not valid"), IDPlayer);
    return false;
}

APlayerController* URPG_DialogFunctionLibrary::GetPlayerControllerByIDPlayerCompact(UObject* WorldContextObject, const int32 IDPlayer)
{
    APlayerController* PlayerController = nullptr;
    FString StrError;
    if (GetPlayerControllerByIDPlayer(WorldContextObject, IDPlayer, PlayerController, StrError))
    {
        return PlayerController;
    }
    return nullptr;
}

bool URPG_DialogFunctionLibrary::GetPlayerStateByIDPlayer(UObject* WorldContextObject, const int32 IDPlayer, APlayerState*& PlayerState, FString& Reason)
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
    if (!World)
    {
        Reason = FString::Printf(TEXT("Invalid World Context"));
        return false;
    }

    AGameStateBase* GS = World->GetGameState();
    if (!GS)
    {
        Reason = FString::Printf(TEXT("Invalid Game State"));
        return false;
    }

    for (const auto& PS : GS->PlayerArray)
    {
        if (PS->GetPlayerId() != IDPlayer) continue;
        PlayerState = PS;
        return true;
    }

    Reason = FString::Printf(TEXT("ID: %i is not valid"), IDPlayer);
    return false;
}

APlayerState* URPG_DialogFunctionLibrary::GetPlayerStateByIDPlayerCompact(UObject* WorldContextObject, const int32 IDPlayer)
{
    APlayerState* PlayerState = nullptr;
    FString StrError;
    if (GetPlayerStateByIDPlayer(WorldContextObject, IDPlayer, PlayerState, StrError))
    {
        return PlayerState;
    }
    return nullptr;
}

bool URPG_DialogFunctionLibrary::GetPlayerNameByIDPlayer(UObject* WorldContextObject, const int32 IDPlayer, FString& PlayerName, FString& Reason)
{
    APlayerState* PS = nullptr;
    FString StrError;

    if (GetPlayerStateByIDPlayer(WorldContextObject, IDPlayer, PS, StrError))
    {
        PlayerName = PS->GetPlayerName();
        return true;
    }
    Reason = FString::Printf(TEXT("ID: %i is not valid"), IDPlayer);
    return false;
}

FString URPG_DialogFunctionLibrary::GetPlayerNameByIDPlayerCompact(UObject* WorldContextObject, const int32 IDPlayer)
{
    FString PlayerName;
    FString StrError;
    if (GetPlayerNameByIDPlayer(WorldContextObject, IDPlayer, PlayerName, StrError))
    {
        return PlayerName;
    }
    return {};
}

bool URPG_DialogFunctionLibrary::GetPlayerNameByPlayerController(UObject* WorldContextObject, APlayerController* PC, FString& PlayerName, FString& Reason)
{
    if (!PC)
    {
        Reason = FString::Printf(TEXT("Invalid Player Controller"));
        return false;
    }

    APlayerState* PS = PC->GetPlayerState<APlayerState>();
    if (!PS)
    {
        Reason = FString::Printf(TEXT("Invalid Player State"));
        return false;
    }

    PlayerName = PS->GetPlayerName();
    return true;
}

FString URPG_DialogFunctionLibrary::GetPlayerNameByPlayerControllerCompact(UObject* WorldContextObject, APlayerController* PC)
{
    FString PlayerName;
    FString StrError;
    if (GetPlayerNameByPlayerController(WorldContextObject, PC, PlayerName, StrError))
    {
        return PlayerName;
    }
    return {};
}

bool URPG_DialogFunctionLibrary::GetPlayerIDByPlayerController(UObject* WorldContextObject, APlayerController* PC, int32& IDPlayer, FString& Reason)
{
    if (!PC)
    {
        Reason = FString::Printf(TEXT("Invalid Player Controller"));
        return false;
    }
    APlayerState* PS = PC->GetPlayerState<APlayerState>();
    if (!PS)
    {
        Reason = FString::Printf(TEXT("Invalid Player State"));
        return false;
    }
    IDPlayer = PS->GetPlayerId();
    return true;
}

int32 URPG_DialogFunctionLibrary::GetPlayerIDByPlayerControllerCompact(UObject* WorldContextObject, APlayerController* PC)
{
    int32 IDPlayer = INDEX_NONE;
    FString StrError;
    if (GetPlayerIDByPlayerController(WorldContextObject, PC, IDPlayer, StrError))
    {
        return IDPlayer;
    }
    return INDEX_NONE;
}

void URPG_DialogFunctionLibrary::RunDialog(UObject* WorldContextObject, URPG_DialogComponentBase* DialogComponent)
{
    if (!DialogComponent) return;
    DialogComponent->RunDialog();
}

bool URPG_DialogFunctionLibrary::GetDialogComponent(UObject* WorldContextObject, AActor* ActorRun, URPG_DialogComponentBase*& DialogComponent, FString& Reason)
{
    if (!ActorRun)
    {
        Reason = FString::Printf(TEXT("Invalid Actor Run"));
        return false;
    }

    DialogComponent = ActorRun->FindComponentByClass<URPG_DialogComponentBase>();
    if (!DialogComponent)
    {
        Reason = FString::Printf(TEXT("Invalid Dialog Component"));
        return false;
    }
    return true;
}

URPG_DialogComponentBase* URPG_DialogFunctionLibrary::GetDialogComponentCompact(UObject* WorldContextObject, AActor* ActorRun)
{
    URPG_DialogComponentBase* DialogComponent = nullptr;
    FString StrError;

    GetDialogComponent(WorldContextObject, ActorRun, DialogComponent, StrError);
    return DialogComponent;
}

bool URPG_DialogFunctionLibrary::GetDialogObject(UObject* WorldContextObject, AActor* ActorRun, URPG_DialogObjectBase*& DialogObject, FString& Reason)
{
    if (!ActorRun)
    {
        Reason = FString::Printf(TEXT("Invalid Actor Run"));
        return false;
    }

    URPG_DialogComponentBase* DialogComponent = ActorRun->FindComponentByClass<URPG_DialogComponentBase>();
    if (!DialogComponent)
    {
        Reason = FString::Printf(TEXT("Invalid Dialog Component"));
        return false;
    }

    DialogObject = DialogComponent->GetTargetDialog();
    if (!DialogObject)
    {
        Reason = FString::Printf(TEXT("Invalid Dialog Object"));
        return false;
    }
    return true;
}

URPG_DialogObjectBase* URPG_DialogFunctionLibrary::GetDialogObjectCompact(UObject* WorldContextObject, AActor* ActorRun)
{
    URPG_DialogObjectBase* DialogObject = nullptr;
    FString StrError;
    GetDialogObject(WorldContextObject, ActorRun, DialogObject, StrError);
    return DialogObject;
}