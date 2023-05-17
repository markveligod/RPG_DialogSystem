// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Library/RPG_DialogFunctionLibrary.h"

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<bool> EnableD_DialogSystemLog(TEXT("RPGDialogSystem.ShowLog"), false, TEXT("RPGDialogSystem.ShowLog [true/false]"), ECVF_Cheat);
#endif

void URPG_DialogFunctionLibrary::Print_Log(const TEnumAsByte<ELogVerbosity::Type> LogVerb, const FString& Text, const int Line, const char* Function)
{

#if !UE_EDITOR && (UE_BUILD_DEVELOPMENT || UE_BUILD_DEBUG)
    if (!EnableD_DialogSystemLog.GetValueOnGameThread()) return;
#endif

    if (LogVerb == ELogVerbosity::Display)
    {
        UE_LOG(LogDialogSystem, Display, TEXT("[%s][%d] TEXT:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
    }
    else if (LogVerb == ELogVerbosity::Warning)
    {
        UE_LOG(LogDialogSystem, Warning, TEXT("[%s][%d] TEXT:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
    }
    else if (LogVerb == ELogVerbosity::Error)
    {
        UE_LOG(LogDialogSystem, Error, TEXT("[%s][%d] TEXT:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
    }
}

bool URPG_DialogFunctionLibrary::Print_CLog(const bool bCond, const TEnumAsByte<ELogVerbosity::Type> LogVerb, const FString& Text, const int Line, const char* Function)
{
    if (bCond)
    {
        URPG_DialogFunctionLibrary::Print_Log(LogVerb, Text, Line, Function);
    }
    return bCond;
}

FString URPG_DialogFunctionLibrary::GetNetModeToString(AActor* InActor)
{
    if (!InActor) return FString("NM_Client");

    const ENetMode NetMode = InActor->GetNetMode();

    switch (NetMode)
    {
        case NM_Standalone: return FString("NM_Standalone");
        case NM_DedicatedServer: return FString("NM_DedicatedServer");
        case NM_Client: return FString("NM_Client");
        case NM_ListenServer: return FString("NM_ListenServer");
    }
    return FString("nullptr");
}
