// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogNodeTransfer.h"

URPG_DialogNodeTransfer::URPG_DialogNodeTransfer()
{
    SetTypeDialogNode(ERPG_TypeDialogNode::Transfer);
}

int32 URPG_DialogNodeTransfer::GetNextIDNode()
{
    return NextIDNode;
}

void URPG_DialogNodeTransfer::SetNextIDNode(int32 IDNode)
{
    NextIDNode = IDNode;
}