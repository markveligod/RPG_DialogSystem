// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_DialogSystemConfigEditor.h"
#include "RPG_DialogSystem/RPG_DialogSystemDataTypes.h"

URPG_DialogSystemConfigEditor::URPG_DialogSystemConfigEditor()
{
    NodeTitleColor.Add(ERPG_TypeStateDialog::Entry, FLinearColor::Red);
    NodeTitleColor.Add(ERPG_TypeStateDialog::PlayerNode, FLinearColor::Blue);
    NodeTitleColor.Add(ERPG_TypeStateDialog::NPCNode, FLinearColor::Black);
    NodeTitleColor.Add(ERPG_TypeStateDialog::Transfer, FLinearColor::Yellow);

    NodeCommentColor.Add(ERPG_TypeStateDialog::Entry, FLinearColor::Red);
    NodeCommentColor.Add(ERPG_TypeStateDialog::PlayerNode, FLinearColor::Blue);
    NodeCommentColor.Add(ERPG_TypeStateDialog::NPCNode, FLinearColor::Black);
    NodeCommentColor.Add(ERPG_TypeStateDialog::Transfer, FLinearColor::Yellow);

    NodeBodyTintColor.Add(ERPG_TypeStateDialog::Entry, FLinearColor::Red);
    NodeBodyTintColor.Add(ERPG_TypeStateDialog::PlayerNode, FLinearColor::Blue);
    NodeBodyTintColor.Add(ERPG_TypeStateDialog::NPCNode, FLinearColor::Black);
    NodeBodyTintColor.Add(ERPG_TypeStateDialog::Transfer, FLinearColor::Yellow);

    NodeTitleText.Add(ERPG_TypeStateDialog::Entry, TEXT("Entry Node"));
    NodeTitleText.Add(ERPG_TypeStateDialog::PlayerNode, TEXT("Player Node"));
    NodeTitleText.Add(ERPG_TypeStateDialog::NPCNode, TEXT("NPC Node"));
    NodeTitleText.Add(ERPG_TypeStateDialog::Transfer, TEXT("Transfer Node"));

    NodeTooltipText.Add(ERPG_TypeStateDialog::Entry, TEXT("Entry Node"));
    NodeTooltipText.Add(ERPG_TypeStateDialog::PlayerNode, TEXT("Player Node"));
    NodeTooltipText.Add(ERPG_TypeStateDialog::NPCNode, TEXT("NPC Node"));
    NodeTooltipText.Add(ERPG_TypeStateDialog::Transfer, TEXT("Transfer Node"));
}
