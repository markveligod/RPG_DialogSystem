// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogSystem/RPG_DialogObject/Nodes/RPG_DialogNodeBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"

#pragma region ActionDialog

int32 URPG_DialogNodeBase::GetNextIDNode()
{
    return INDEX_NONE;
}

void URPG_DialogNodeBase::SetNextIDNode(int32 IDNode) {}

void URPG_DialogNodeBase::ResetNode()
{
    IndexNode = INDEX_NONE;
    TypeDialogNode = ERPG_TypeDialogNode::None;
    NodePosition = FVector2D::ZeroVector;
}

void URPG_DialogNodeBase::InitNode() {}

int32 URPG_DialogNodeBase::GetIndexNode() const
{
    return IndexNode;
}

void URPG_DialogNodeBase::SetIndexNode(const int32 Index)
{
    IndexNode = Index;
}

ERPG_TypeDialogNode URPG_DialogNodeBase::GetTypeDialogNode() const
{
    return TypeDialogNode;
}

void URPG_DialogNodeBase::SetTypeDialogNode(const ERPG_TypeDialogNode Type)
{
    TypeDialogNode = Type;
}

#if WITH_EDITOR

FVector2D URPG_DialogNodeBase::GetNodePosition() const
{
    return NodePosition;
}

void URPG_DialogNodeBase::SetNodePosition(const FVector2D& Pos)
{
    NodePosition = Pos;
}

#endif

#pragma endregion
