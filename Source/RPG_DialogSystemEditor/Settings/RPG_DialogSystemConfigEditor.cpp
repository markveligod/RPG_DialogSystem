// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogSystemConfigEditor.h"
#include "RPG_DialogSystem/RPG_DialogSystemDataTypes.h"

URPG_DialogSystemConfigEditor::URPG_DialogSystemConfigEditor()
{
    NodeTitleColors.Add(ERPG_TypeDialogNode::Start, FLinearColor::Red);
    NodeTitleColors.Add(ERPG_TypeDialogNode::Finish, FLinearColor::Red);
    NodeTitleColors.Add(ERPG_TypeDialogNode::Work, FLinearColor::Blue);
    NodeTitleColors.Add(ERPG_TypeDialogNode::Transfer, FLinearColor::Yellow);

    NodeBodyTintColors.Add(ERPG_TypeDialogNode::Start, FLinearColor::Red);
    NodeBodyTintColors.Add(ERPG_TypeDialogNode::Finish, FLinearColor::Red);
    NodeBodyTintColors.Add(ERPG_TypeDialogNode::Work, FLinearColor::Blue);
    NodeBodyTintColors.Add(ERPG_TypeDialogNode::Transfer, FLinearColor::Yellow);
}
