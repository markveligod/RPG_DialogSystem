// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_DialogSystem/RPG_DialogSystemDataTypes.h"
#include "RPG_DialogNodeBase.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class RPG_DIALOGSYSTEM_API URPG_DialogNodeBase : public UObject
{
    GENERATED_BODY()

#pragma region ActionDialog

public:
    /** @public  **/
    virtual int32 GetNextIDNode();

    /** @public  **/
    virtual void SetNextIDNode(int32 IDNode);

    /** @public  **/
    virtual void ResetNode();

    /** @public  **/
    virtual void InitNode();

    /** @public  **/
    int32 GetIndexNode() const;

    /** @public  **/
    void SetIndexNode(const int32 Index);

    /** @public  **/
    ERPG_TypeDialogNode GetTypeDialogNode() const;

    /** @public  **/
    void SetTypeDialogNode(const ERPG_TypeDialogNode Type);

#if WITH_EDITOR

    /** @public  **/
    FVector2D GetNodePosition() const;

    /** @public  **/
    void SetNodePosition(const FVector2D& Pos);

#endif

#pragma endregion

#pragma region DataNode

private:
    /** @private The index of the dialog node for finding the current object **/
    UPROPERTY()
    int32 IndexNode{INDEX_NONE};

    /** @private **/
    UPROPERTY()
    ERPG_TypeDialogNode TypeDialogNode{ERPG_TypeDialogNode::None};

#if WITH_EDITORONLY_DATA
    /** @private To determine the location in the editor **/
    UPROPERTY()
    FVector2D NodePosition;
#endif

#pragma endregion
};
