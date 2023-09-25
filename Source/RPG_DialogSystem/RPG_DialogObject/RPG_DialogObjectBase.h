// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_DialogSystem/RPG_DialogSystemDataTypes.h"
#include "RPG_DialogObjectBase.generated.h"

/**
 * @class Dialog object for storing nodes
 */
UCLASS()
class RPG_DIALOGSYSTEM_API URPG_DialogObjectBase : public UObject
{
    GENERATED_BODY()

#pragma region Default

public:

    URPG_DialogObjectBase();
    
    /** @public Initialization of internal parameters **/
    UFUNCTION(BlueprintCallable)
    virtual bool InitDialog(APlayerController* PlayerController);

    /** @public Get next dialog settings **/
    UFUNCTION(BlueprintCallable)
    URPG_DialogSettingsObject* NextDialog();

    UFUNCTION(BlueprintCallable)
    void ResetDialog();
    
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

public:
    /** @public Node search by index **/
    UFUNCTION(BlueprintCallable)
    URPG_DialogSettingsObject* FindNodeByIndex(int32 IndexNode);

    /** @public Find start node **/
    UFUNCTION(BlueprintCallable)
    URPG_DialogSettingsObject* FindStartNode();

    /** @public Get all dialog objects **/
    UFUNCTION(BlueprintCallable)
    const TArray<URPG_DialogSettingsObject*>& GetArrayDialogNode() { return ArrayDialogNode; }

    /** @public  **/
    UFUNCTION(BlueprintCallable)
    bool IsSomeHaveOutPlayerNode(const TArray<int32>& OutNodes);

    /** @public  **/
    void RemoveIndexNode(int32 IndexNode);

#if WITH_EDITOR

public:

    /** @public Creating a new dialog node **/
    URPG_DialogSettingsObject* CreateNewDialogNode(const ERPG_TypeStateDialog& TypeStateDialog, FVector2D NodePosition);

private:

    /** @private Search for a free number index **/
    int32 GetFreeIndexNumSlot() const;

#endif

#pragma endregion

#pragma region DataDialog

private:
    /** @private Array of dialog nodes **/
    UPROPERTY(SaveGame, Replicated)
    TArray<URPG_DialogSettingsObject*> ArrayDialogNode;

    /** @private Owner player controller **/
    UPROPERTY()
    APlayerController* OwnerPC{nullptr};

    /** @private Owner player controller **/
    int32 TargetIDNode{INDEX_NONE};

#pragma endregion
};
