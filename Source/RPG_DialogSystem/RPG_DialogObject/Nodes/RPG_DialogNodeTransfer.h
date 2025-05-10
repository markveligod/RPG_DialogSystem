// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_DialogNodeBase.h"
#include "RPG_DialogNodeTransfer.generated.h"

/**
 *
 */
UCLASS()
class RPG_DIALOGSYSTEM_API URPG_DialogNodeTransfer : public URPG_DialogNodeBase
{
    GENERATED_BODY()

#pragma region ActionDialog

public:
    /** @public **/
    URPG_DialogNodeTransfer();

    /** @public  **/
    virtual int32 GetNextIDNode() override;

    /** @public  **/
    virtual void SetNextIDNode(int32 IDNode) override;

#pragma endregion

#pragma region DataNode

private:
    /** @private **/
    int32 NextIDNode{INDEX_NONE};

#pragma endregion
};
