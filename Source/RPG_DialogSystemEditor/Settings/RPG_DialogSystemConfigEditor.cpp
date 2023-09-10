// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_DialogSystemConfigEditor.h"
#include "RPG_DialogSystem/RPG_DialogSystemDataTypes.h"

URPG_DialogSystemConfigEditor::URPG_DialogSystemConfigEditor()
{
    NodeTitleColor.Add(ERPG_TypeStateDialog::Entry, FLinearColor::Red);
    NodeTitleColor.Add(ERPG_TypeStateDialog::PlayerNode, FLinearColor::Blue);
    NodeTitleColor.Add(ERPG_TypeStateDialog::NPCNode, FLinearColor::Black);

    NodeCommentColor.Add(ERPG_TypeStateDialog::Entry, FLinearColor::Red);
    NodeCommentColor.Add(ERPG_TypeStateDialog::PlayerNode, FLinearColor::Blue);
    NodeCommentColor.Add(ERPG_TypeStateDialog::NPCNode, FLinearColor::Black);

    NodeBodyTintColor.Add(ERPG_TypeStateDialog::Entry, FLinearColor::Red);
    NodeBodyTintColor.Add(ERPG_TypeStateDialog::PlayerNode, FLinearColor::Blue);
    NodeBodyTintColor.Add(ERPG_TypeStateDialog::NPCNode, FLinearColor::Black);
}
