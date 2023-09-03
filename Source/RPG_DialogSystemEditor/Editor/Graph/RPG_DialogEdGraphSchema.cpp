// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_DialogEdGraphSchema.h"
#include "Actions/RPG_DialogGraphActions.h"

#define LOCTEXT_NAMESPACE "DialogSchema"

void URPG_DialogEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    // For action graph
    Super::GetGraphContextActions(ContextMenuBuilder);

    const TSharedPtr<FRPG_DialogAction_NewNPCNode> CreateNPCAction(new FRPG_DialogAction_NewNPCNode());
    ContextMenuBuilder.AddAction(CreateNPCAction);

    const TSharedPtr<FRPG_DialogAction_NewPlayerNode> CreatePlayerAction(new FRPG_DialogAction_NewPlayerNode());
    ContextMenuBuilder.AddAction(CreatePlayerAction);

}

void URPG_DialogEdGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
    // For Node
    Super::GetContextMenuActions(Menu, Context);

}

#undef LOCTEXT_NAMESPACE