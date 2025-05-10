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

#pragma region Default

public:
    URPG_DialogObjectBase();

    /**
     * Handles reading, writing, and reference collecting using FArchive.
     * This implementation handles all FProperty serialization, but can be overridden for native variables.
     */
    virtual void Serialize(FArchive& Ar) override;

#pragma endregion

#pragma region NetworkInterface

public:
    /**
     * @public Return the space this function should be called. Checks to see if this function should
     * be called locally, remotely, or simply absorbed under the given conditions
     *
     * @param Function function to call
     * @param Stack stack frame for the function call
     * @return bitmask representing all callspaces that apply to this UFunction in the given context
     */
    virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;

    /**
     * @public Call the actor's function remotely
     *
     * @param1 Function function to call
     * @param2 Parameters arguments to the function call
     * @param3 Stack stack frame for the function call
     */
    virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override;

    /** @public Returns properties that are replicated for the lifetime of the actor channel */
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** @public IsSupportedForNetworking means an object can be referenced over the network */
    virtual bool IsSupportedForNetworking() const override { return true; }

#pragma endregion

#pragma region Action

#pragma region FindNode

public:
    /** @public Node search by index **/
    URPG_DialogNodeBase* FindNodeByIndex(int32 IndexNode);

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

protected:
    /** @protected Search for a free number index **/
    int32 GetFreeIndexNumSlot() const;

#endif

#pragma endregion

public:
    /** @public Initialization of internal parameters **/
    virtual bool InitDialog(APlayerController* PlayerController);

    /** @public Get next dialog settings **/
    void NextDialog();

    /** @public **/
    void ResetDialog();

    /** @public Get all dialog objects **/
    const TArray<URPG_DialogNodeBase*>& GetArrayDialogNode() { return ArrayDialogNode; }

    /** @public  **/
    bool IsSomeHaveOutPlayerNode(const TArray<int32>& OutNodes);

    /** @public  **/
    void RemoveIndexNode(int32 IndexNode);

    /** @public  **/
    bool IsNetworkUpdate() const { return bNetworkUpdate; }

    /** @public  **/
    void EnableNetworkUpdate() { bNetworkUpdate = true; }

    /** @public  **/
    void ResetNetworkUpdate() { bNetworkUpdate = false; }

    /** @public  **/
    UFUNCTION()
    void OnRep_TargetIDNode();

#pragma endregion

#pragma region DataDialog

protected:
    /** @protected Array of dialog nodes **/
    UPROPERTY(SaveGame, Replicated)
    TArray<URPG_DialogNodeBase*> ArrayDialogNode;

    /** @protected Owner player controller **/
    UPROPERTY()
    APlayerController* OwnerPC{nullptr};

    /** @protected Owner player controller **/
    UPROPERTY(ReplicatedUsing = OnRep_TargetIDNode)
    int32 TargetIDNode{INDEX_NONE};

private:
    /** @private **/
    bool bNetworkUpdate{false};

#pragma endregion

#pragma region Signature

private:
    FCompleteDialogSignature OnCompleteDialog;

#pragma endregion
};
