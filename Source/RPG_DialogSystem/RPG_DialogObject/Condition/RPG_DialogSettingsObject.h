// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_DialogSystem/RPG_DialogSystemDataTypes.h"
#include "RPG_DialogSettingsObject.generated.h"

/**
 *
 */
UCLASS(DisplayName = "DialogEvent", EditInlineNew, Abstract, Blueprintable)
class RPG_DIALOGSYSTEM_API URPG_DialogEvent : public UObject
{
    GENERATED_BODY()

#pragma region Action

public:
    virtual void TriggerEvent(APlayerController* PC);

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void Trigger_Event(APlayerController* PC);

#pragma endregion
};

/**
 *
 */
UCLASS(DisplayName = "DialogCondition", EditInlineNew, Abstract, Blueprintable)
class RPG_DIALOGSYSTEM_API URPG_DialogCondition : public UObject
{
    GENERATED_BODY()

#pragma region Action

public:
    virtual bool IsCondition(APlayerController* PC);

protected:
    UFUNCTION(BlueprintNativeEvent)
    bool IsConditionNative(APlayerController* PC);

#pragma endregion
};

/**
 *
 */
UCLASS(BlueprintType)
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
    int32 GetIndexNode() const { return IndexNode; }

    UFUNCTION(BlueprintCallable)
    bool IsHaveSomeCondition() const { return ArrayCondition.Num() != 0; }

    UFUNCTION(BlueprintCallable)
    bool IsHaveSomeEvent() const { return ArrayEvent.Num() != 0; }

    UFUNCTION()
    TArray<FName> GetAllDialogPlayerAndNPCNames() const;

    UFUNCTION(BlueprintCallable)
    int32 GetIndexTransferNode() const;

    UFUNCTION(BlueprintCallable)
    bool IsValidCondition(APlayerController* PlayerController) const;

    UFUNCTION(BlueprintCallable)
    void ExecuteEvents(APlayerController* PlayerController);
    
#pragma endregion

#pragma region DataDialog

public:

    UPROPERTY(VisibleAnywhere)
    int32 IndexNode{INDEX_NONE};

    UPROPERTY(VisibleAnywhere)
    FVector2D NodePosition{FVector2D::UnitVector};

    UPROPERTY(VisibleAnywhere)
    ERPG_TypeStateDialog TypeStateDialog{ERPG_TypeStateDialog::None};

    UPROPERTY(VisibleAnywhere)
    TArray<int32> OutNodes;
    
private:
    UPROPERTY(EditAnywhere, meta = (MultiLine = true, EditCondition = "TypeStateDialog == ERPG_TypeStateDialog::NPCNode || TypeStateDialog == ERPG_TypeStateDialog::PlayerNode", EditConditionHides))
    FText TextDialog;

    UPROPERTY(EditAnywhere, Instanced, meta = (EditCondition = "TypeStateDialog == ERPG_TypeStateDialog::NPCNode || TypeStateDialog == ERPG_TypeStateDialog::PlayerNode", EditConditionHides))
    TArray<URPG_DialogCondition*> ArrayCondition;

    UPROPERTY(EditAnywhere, Instanced, meta = (EditCondition = "TypeStateDialog == ERPG_TypeStateDialog::NPCNode || TypeStateDialog == ERPG_TypeStateDialog::PlayerNode", EditConditionHides))
    TArray<URPG_DialogEvent*> ArrayEvent;

    UPROPERTY(EditAnywhere, meta = (EditCondition = "TypeStateDialog == ERPG_TypeStateDialog::Transfer", EditConditionHides, GetOptions = "GetAllDialogPlayerAndNPCNames"))
    FName NameTransfer;

#pragma endregion
};
