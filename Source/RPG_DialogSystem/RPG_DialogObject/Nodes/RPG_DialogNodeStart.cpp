// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogNodeStart.h"

URPG_DialogNodeStart::URPG_DialogNodeStart()
{
    SetTypeDialogNode(ERPG_TypeDialogNode::Start);
}

int32 URPG_DialogNodeStart::GetNextIDNode()
{
    return NextIDNode;
}

void URPG_DialogNodeStart::SetNextIDNode(int32 IDNode)
{
    NextIDNode = IDNode;
}