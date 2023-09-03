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
    //~ End EdGraphSchema Interface


};
