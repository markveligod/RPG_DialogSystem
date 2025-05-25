// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_DialogSystem/RPG_DialogSystemDataTypes.h"
#include "RPG_DialogObjectBase.generated.h"

class URPG_DialogNodeBase;
/**
 * @class Dialog object for storing nodes
 */
UCLASS(BlueprintType)
class RPG_DIALOGSYSTEM_API URPG_DialogObjectBase : public UObject
{
    GENERATED_BODY()

    friend class URPG_DialogComponentBase;
    friend class URPG_DialogWidget;

#pragma region Default

public:
    URPG_DialogObjectBase();

    /**
     * Handles reading, writing, and reference collecting using FArchive.
     * This implementation handles all FProperty serialization, but can be overridden for native variables.
     */
    virtual void Serialize(FArchive& Ar) override;

    virtual UWorld* GetWorld() const override;

#pragma endregion

#pragma region Action

#pragma region FindNode

public:
    /** @public Node search by index **/
    URPG_DialogNodeBase* FindNodeByIndex(int32 IndexNode);

    /** @public Node search by target Index **/
    URPG_DialogNodeBase* FindTargetNode();

    /** @public Find start node **/
    URPG_DialogNodeBase* FindStartNode();

    /** @public Find start node **/
    TArray<URPG_DialogNodeBase*> FindNodeByType(ERPG_TypeDialogNode TypeDialogNode);

#pragma endregion

#pragma region ActionNode

#if WITH_EDITOR

public:
    /** @public **/
    virtual URPG_DialogNodeBase* CreateNode(ERPG_TypeDialogNode TypeNode, const FVector2D& NodePosition);

    /** @public **/
    virtual void RemoveNode(const int32 IndexNode);

    /** @public **/
    virtual bool Modify(bool bAlwaysMarkDirty = true) override;

protected:
    /** @protected Search for a free number index **/
    int32 GetFreeIndexNumSlot() const;

#endif

#pragma endregion

public:
    /** @public Initialization of internal parameters **/
    virtual bool InitDialog(AActor* ActorOwner);

    /** @public  **/
    virtual void RunDialog();

    /** @public  **/
    virtual void CompleteDialog();

    /** @public **/
    virtual void NextDialog();

    /** @public **/
    void ResetDialog();

    /** @public Get all dialog objects **/
    const TArray<URPG_DialogNodeBase*>& GetArrayDialogNode() { return ArrayDialogNode; }

    /** @public  **/
    ERPG_DialogObjectState GetDialogState() const { return DialogState; }

    /** @public  **/
    AActor* GetOwner() const { return Owner.Get(); }

protected:
    /** @protected  **/
    virtual void UpdateTargetIDNode();

    /** @protected  **/
    void ChangeDialogState(ERPG_DialogObjectState NewState);

#pragma endregion

#pragma region DataDialog

protected:
    /** @protected Array of dialog nodes **/
    UPROPERTY(SaveGame)
    TArray<URPG_DialogNodeBase*> ArrayDialogNode;

    /** @protected Owner player controller **/
    int32 TargetIDNode{INDEX_NONE};

private:
    /** @private **/
    TWeakObjectPtr<AActor> Owner{nullptr};

    /** @private **/
    ERPG_DialogObjectState DialogState{ERPG_DialogObjectState::None};

#pragma endregion

#pragma region Signature

protected:
    /** @protected **/
    void UpdateStateDialog() { OnUpdateDialog.ExecuteIfBound(this); }

    /** @protected **/
    FUpdateStateDialogSignature OnUpdateDialog;

#pragma endregion
};
