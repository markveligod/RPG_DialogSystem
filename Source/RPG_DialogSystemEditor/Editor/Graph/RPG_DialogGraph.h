// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "RPG_DialogGraph.generated.h"

/**
 * 
 */
UCLASS()
class RPG_DIALOGSYSTEMEDITOR_API URPG_DialogGraph : public UEdGraph
{
    GENERATED_BODY()

public:
    
    /** Returns the Quest Dialog that contains this graph */
    class URPG_DialogObjectBase* GetDialogObject() const;

    virtual void PreSave(FObjectPreSaveContext SaveContext) override;
};
