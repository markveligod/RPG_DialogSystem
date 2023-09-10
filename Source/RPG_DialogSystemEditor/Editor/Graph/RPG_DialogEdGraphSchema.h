// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "RPG_DialogEdGraphSchema.generated.h"

/**
 * 
 */
UCLASS()
class RPG_DIALOGSYSTEMEDITOR_API URPG_DialogEdGraphSchema : public UEdGraphSchema
{
    GENERATED_BODY()

public:
    //~ Begin EdGraphSchema Interface
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
    virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
    virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
    virtual void AutoConnectNodeByDefault(UEdGraph& Graph) const;
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
    virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
    virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
    virtual FLinearColor GetSecondaryPinTypeColor(const FEdGraphPinType& PinType) const override;
    //~ End EdGraphSchema Interface


};
