// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_DialogSystem/RPG_DialogSystemDataTypes.h"
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

    UFUNCTION(BlueprintCallable)
    bool IsSomeHaveOutPlayerNode();
    
#pragma endregion

#pragma region DataDialog

#if WITH_EDITORONLY_DATA

public:

    UPROPERTY(VisibleAnywhere)
    int32 IndexNode{INDEX_NONE};

    UPROPERTY(VisibleAnywhere)
    FVector2D NodePosition{FVector2D::UnitVector};

    UPROPERTY(VisibleAnywhere)
    ERPG_TypeStateDialog TypeStateDialog{ERPG_TypeStateDialog::None};

    UPROPERTY(VisibleAnywhere)
    TArray<int32> OutNodes;

#endif
    
private:
    UPROPERTY(EditAnywhere)
    FText TextDialog;

    UPROPERTY(EditAnywhere, Instanced)
    TArray<URPG_DialogConditionTask*> ArrayConditionTask;

    UPROPERTY(EditAnywhere, Instanced)
    TArray<URPG_DialogConditionEvent*> ArrayConditionEvent;

#pragma endregion
};
