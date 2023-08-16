// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPG_DialogSettingsObject.generated.h"

/**
 *
 */
UCLASS(DisplayName = "DialogEvent", EditInlineNew)
class RPG_DIALOGSYSTEM_API URPG_DialogConditionEvent : public UObject
{
    GENERATED_BODY()

#pragma region ActionDialogEvent

public:
    virtual void TriggerCondition();

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void TriggerCondition_Event();

#pragma endregion
};

/**
 *
 */
UCLASS(DisplayName = "DialogTask", EditInlineNew)
class RPG_DIALOGSYSTEM_API URPG_DialogConditionTask : public UObject
{
    GENERATED_BODY()

#pragma region ActionDialogTask

public:
    virtual bool IsConditionTask();

protected:
    UFUNCTION(BlueprintNativeEvent)
    bool IsConditionTask_Event();

#pragma endregion
};

/**
 *
 */
UCLASS()
class RPG_DIALOGSYSTEM_API URPG_DialogSettingsObject : public UObject
{
    GENERATED_BODY()

#pragma region ActionDialog

public:
    UFUNCTION(BlueprintCallable)
    void SetupNewTextDialog(const FText& NewText) { TextDialog = NewText; }

    UFUNCTION(BlueprintCallable)
    const FText& GetTextDialog() const { return TextDialog; }

#pragma endregion

#pragma region DataDialog

private:
    UPROPERTY(EditAnywhere)
    FText TextDialog;

    UPROPERTY(EditAnywhere, Instanced)
    TArray<URPG_DialogConditionTask*> ArrayConditionTask;

    UPROPERTY(EditAnywhere, Instanced)
    TArray<URPG_DialogConditionEvent*> ArrayConditionEvent;

#pragma endregion
};
