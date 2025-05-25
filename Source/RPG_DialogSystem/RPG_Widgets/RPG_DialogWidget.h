// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG_DialogWidget.generated.h"

class URPG_DialogPlayer;
class URPG_DialogNPC;
class URPG_DialogNodeBase;
class URPG_DialogObjectBase;
/**
 *
 */
UCLASS()
class RPG_DIALOGSYSTEM_API URPG_DialogWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** @public **/
    UFUNCTION(BlueprintCallable)
    void InitWidget(URPG_DialogObjectBase* DialogObject);

protected:
    /** @protected **/
    UFUNCTION(BlueprintCallable, meta = (BlueprintProtected = "true"))
    void NextDialogNPC();

    /** @protected **/
    UFUNCTION(BlueprintCallable, meta = (BlueprintProtected = "true"))
    bool MakeSelectDialogPlayer(int32 Index);

    /** @protected **/
    UFUNCTION(BlueprintImplementableEvent)
    void InitDialogWidget();

    /** @protected **/
    UFUNCTION(BlueprintImplementableEvent)
    void ChangeDialogNode();

    /** @protected **/
    UFUNCTION(BlueprintImplementableEvent)
    void FinishDialogObject();

    /** @protected **/
    UFUNCTION(BlueprintImplementableEvent)
    void ViewTextNPC(const FText& TextNPC);

    /** @protected **/
    UFUNCTION(BlueprintImplementableEvent)
    void ViewTextPlayer(const TMap<int32, FText>& TextPlayers);

private:
    /** @private **/
    void RegisterUpdateStateDialog(URPG_DialogObjectBase* DialogObject);

    /** @private **/
    TWeakObjectPtr<URPG_DialogObjectBase> TargetDialog;

    /** @private **/
    TWeakObjectPtr<URPG_DialogNodeBase> TargetDialogNode;

    /** @private **/
    TArray<TWeakObjectPtr<URPG_DialogNPC>> DialogNPC;

    /** @private **/
    int32 ViewDialogNPCIndex{INDEX_NONE};

    /** @private **/
    TArray<TWeakObjectPtr<URPG_DialogPlayer>> DialogPlayer;
};
