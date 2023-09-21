﻿#pragma once

#include "SGraphNode.h"
#include "RPG_DialogSystemEditor/Editor/Graph/Nodes/RPG_DialogGraphNode_Base.h"

class RPG_DIALOGSYSTEMEDITOR_API SDialogGraphNode : public SGraphNode
{

public:

    SLATE_BEGIN_ARGS(SDialogGraphNode) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, URPG_DialogGraphNode_Base* InNode);


public:

    virtual void UpdateGraphNode() override;

    virtual void UpdateOwnerEditorChange(FPropertyChangedEvent& PropertyChangedEvent);

    virtual void CreateTextDialogBlock(TSharedPtr<SVerticalBox> MainBox);
protected:

    URPG_DialogGraphNode_Base* DialogGraphNode{nullptr};

private:

    TSharedPtr<STextBlock> TextSDialog;
};
