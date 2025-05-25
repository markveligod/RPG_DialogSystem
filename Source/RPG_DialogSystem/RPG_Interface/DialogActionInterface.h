// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogActionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDialogActionInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class RPG_DIALOGSYSTEM_API IDialogActionInterface
{
    GENERATED_BODY()

public:
    /** @public **/
    UFUNCTION(BlueprintNativeEvent)
    void RunDialog(AActor* ActorRunDialog);

    /** @public **/
    UFUNCTION(BlueprintNativeEvent)
    void CompleteDialog(AActor* ActorRunDialog);
};