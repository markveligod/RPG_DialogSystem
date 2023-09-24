#include "SDialogGraphNode.h"
#include "GraphEditorSettings.h"
#include "IDocumentation.h"
#include "SCommentBubble.h"
#include "SlateOptMacros.h"
#include "SLevelOfDetailBranchNode.h"
#include "TutorialMetaData.h"
#include "RPG_DialogSystem/RPG_DialogObject/Condition/RPG_DialogSettingsObject.h"
#include "RPG_DialogSystemEditor/Style/FRPG_DialogSystemStyle.h"

void SDialogGraphNode::Construct(const FArguments& InArgs, URPG_DialogGraphNode_Base* InNode)
{
    GraphNode = InNode;
    DialogGraphNode = InNode;
    SetCursor(EMouseCursor::CardinalCross);
    UpdateGraphNode();
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDialogGraphNode::UpdateGraphNode()
{
    InputPins.Empty();
    OutputPins.Empty();

    // Reset variables that are going to be exposed, in case we are refreshing an already setup node.
    RightNodeBox.Reset();
    LeftNodeBox.Reset();

    //
    //             ______________________
    //            |      TITLE AREA      |
    //            +-------+------+-------+
    //            | (>) L |      | R (>) |
    //            | (>) E |      | I (>) |
    //            | (>) F |      | G (>) |
    //            | (>) T |      | H (>) |
    //            |       |      | T (>) |
    //            |_______|______|_______|
    //
    TSharedPtr<SVerticalBox> MainVerticalBox;
    SetupErrorReporting();

    TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

    // Get node icon
    IconColor = FLinearColor::White;
    const FSlateBrush* IconBrush = nullptr;
    if (GraphNode != NULL && GraphNode->ShowPaletteIconOnNode())
    {
        IconBrush = GraphNode->GetIconAndTint(IconColor).GetOptionalIcon();
    }

    TSharedRef<SOverlay> DefaultTitleAreaWidget =
        SNew(SOverlay)
        + SOverlay::Slot()
        [
            SNew(SImage)
			.Image(FAppStyle::GetBrush("Graph.Node.TitleGloss"))
			.ColorAndOpacity(this, &SGraphNode::GetNodeTitleIconColor)
        ]
        + SOverlay::Slot()
          .HAlign(HAlign_Fill)
          .VAlign(VAlign_Center)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .HAlign(HAlign_Fill)
            [
                SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("Graph.Node.ColorSpill"))
				// The extra margin on the right
				// is for making the color spill stretch well past the node title
				.Padding(FMargin(10, 5, 30, 3))
				.BorderBackgroundColor(this, &SGraphNode::GetNodeTitleColor)
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                      .VAlign(VAlign_Top)
                      .Padding(FMargin(0.f, 0.f, 4.f, 0.f))
                      .AutoWidth()
                    [
                        SNew(SImage)
						.Image(IconBrush)
						.ColorAndOpacity(this, &SGraphNode::GetNodeTitleIconColor)
                    ]
                    + SHorizontalBox::Slot()
                    [
                        SNew(SVerticalBox)
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            CreateTitleWidget(NodeTitle)
                        ]
                        + SVerticalBox::Slot()
                        .AutoHeight()
                        [
                            NodeTitle.ToSharedRef()
                        ]
                    ]
                ]
            ]
            + SHorizontalBox::Slot()
              .HAlign(HAlign_Right)
              .VAlign(VAlign_Center)
              .Padding(0, 0, 5, 0)
              .AutoWidth()
            [
                CreateTitleRightWidget()
            ]
        ]
        + SOverlay::Slot()
        .VAlign(VAlign_Top)
        [
            SNew(SBorder)
			.Visibility(EVisibility::HitTestInvisible)
			.BorderImage(FAppStyle::GetBrush("Graph.Node.TitleHighlight"))
			.BorderBackgroundColor(this, &SGraphNode::GetNodeTitleIconColor)
            [
                SNew(SSpacer)
                .Size(FVector2D(20, 20))
            ]
        ];

    SetDefaultTitleAreaWidget(DefaultTitleAreaWidget);

    TSharedRef<SWidget> TitleAreaWidget =
        SNew(SLevelOfDetailBranchNode)
		.UseLowDetailSlot(this, &SDialogGraphNode::UseLowDetailNodeTitles)
		.LowDetail()
        [
            SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Graph.Node.ColorSpill"))
			.Padding(FMargin(75.0f, 22.0f)) // Saving enough space for a 'typical' title so the transition isn't quite so abrupt
			.BorderBackgroundColor(this, &SDialogGraphNode::GetNodeTitleColor)
        ]
        .HighDetail()
        [
            DefaultTitleAreaWidget
        ];

    if (!SWidget::GetToolTip().IsValid())
    {
        TSharedRef<SToolTip> DefaultToolTip = IDocumentation::Get()->CreateToolTip(TAttribute<FText>(this, &SGraphNode::GetNodeTooltip), NULL, GraphNode->GetDocumentationLink(),
            GraphNode->GetDocumentationExcerptName());
        SetToolTip(DefaultToolTip);
    }

    // Setup a meta tag for this node
    FGraphNodeMetaData TagMeta(TEXT("Graphnode"));
    PopulateMetaTag(&TagMeta);

    TSharedPtr<SVerticalBox> InnerVerticalBox;
    this->ContentScale.Bind(this, &SGraphNode::GetContentScale);

    InnerVerticalBox = SNew(SVerticalBox)
                       + SVerticalBox::Slot()
                         .AutoHeight()
                         .HAlign(HAlign_Fill)
                         .VAlign(VAlign_Top)
                         .Padding(Settings->GetNonPinNodeBodyPadding())
                       [
                           TitleAreaWidget
                       ]

                       + SVerticalBox::Slot()
                         .AutoHeight()
                         .HAlign(HAlign_Fill)
                         .VAlign(VAlign_Top)
                       [
                           CreateNodeContentArea()
                       ];

    TSharedPtr<SWidget> EnabledStateWidget = GetEnabledStateWidget();
    if (EnabledStateWidget.IsValid())
    {
        InnerVerticalBox->AddSlot()
                        .AutoHeight()
                        .HAlign(HAlign_Fill)
                        .VAlign(VAlign_Top)
                        .Padding(FMargin(2, 0))
        [
            EnabledStateWidget.ToSharedRef()
        ];
    }

    InnerVerticalBox->AddSlot()
                    .AutoHeight()
                    .Padding(Settings->GetNonPinNodeBodyPadding())
    [
        ErrorReporting->AsWidget()
    ];

    InnerVerticalBox->AddSlot()
                    .AutoHeight()
                    .Padding(Settings->GetNonPinNodeBodyPadding())
    [
        VisualWarningReporting->AsWidget()
    ];

    this->GetOrAddSlot(ENodeZone::Center)
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
    [
        SAssignNew(MainVerticalBox, SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SOverlay)
            .AddMetaData<FGraphNodeMetaData>(TagMeta)
            + SOverlay::Slot()
            .Padding(Settings->GetNonPinNodeBodyPadding())
            [
                SNew(SImage)
					.Image(GetNodeBodyBrush())
					.ColorAndOpacity(this, &SGraphNode::GetNodeBodyColor)
            ]
            + SOverlay::Slot()
            [
                InnerVerticalBox.ToSharedRef()
            ]
        ]
    ];

    bool SupportsBubble = true;
    if (GraphNode != nullptr)
    {
        SupportsBubble = GraphNode->SupportsCommentBubble();
    }

    if (SupportsBubble)
    {
        // Create comment bubble
        TSharedPtr<SCommentBubble> CommentBubble;
        const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

        SAssignNew(CommentBubble, SCommentBubble)
			.GraphNode(GraphNode)
			.Text(this, &SGraphNode::GetNodeComment)
			.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
			.OnToggled(this, &SGraphNode::OnCommentBubbleToggled)
			.ColorAndOpacity(CommentColor)
			.AllowPinning(true)
			.EnableTitleBarBubble(true)
			.EnableBubbleCtrls(true)
			.GraphLOD(this, &SGraphNode::GetCurrentLOD)
			.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

        GetOrAddSlot(ENodeZone::TopCenter)
            .SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
            .SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
            .AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
            .VAlign(VAlign_Top)
            [
                CommentBubble.ToSharedRef()
            ];
    }

    CreateBelowWidgetControls(MainVerticalBox);
    CreatePinWidgets();
    CreateInputSideAddButton(LeftNodeBox);
    CreateOutputSideAddButton(RightNodeBox);
    CreateBelowPinControls(InnerVerticalBox);
    CreateAdvancedViewArrow(InnerVerticalBox);
    CreateTextDialogBlock(InnerVerticalBox);
    CreateConditionIcon(LeftNodeBox);
    CreateEventIcon(RightNodeBox);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDialogGraphNode::UpdateOwnerEditorChange(FPropertyChangedEvent& PropertyChangedEvent)
{
    if (TextSDialog && DialogGraphNode && DialogGraphNode->GetDialogSettingsObject())
    {
        TextSDialog->SetText(DialogGraphNode->GetDialogSettingsObject()->GetTextDialog());
    }
}

void SDialogGraphNode::CreateTextDialogBlock(TSharedPtr<SVerticalBox> MainBox)
{
    if (!DialogGraphNode) return;
    if (!DialogGraphNode->GetDialogSettingsObject()) return;
    if (DialogGraphNode->GetDialogSettingsObject()->TypeStateDialog != ERPG_TypeStateDialog::PlayerNode && DialogGraphNode->GetDialogSettingsObject()->TypeStateDialog != ERPG_TypeStateDialog::NPCNode) return;
    
    TextSDialog = SNew(STextBlock)
            .Text(DialogGraphNode->GetDialogSettingsObject()->GetTextDialog())
            .Visibility(EVisibility::Visible);
    
    MainBox->AddSlot()
        .AutoHeight()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Center)
        .Padding(5, 3, 5, 5)
        [
            TextSDialog.ToSharedRef()
        ];
}

void SDialogGraphNode::CreateConditionIcon(TSharedPtr<SVerticalBox> MainBox)
{
    if (!DialogGraphNode) return;
    if (!DialogGraphNode->GetDialogSettingsObject()) return;
    if (DialogGraphNode->GetDialogSettingsObject()->TypeStateDialog != ERPG_TypeStateDialog::PlayerNode && DialogGraphNode->GetDialogSettingsObject()->TypeStateDialog != ERPG_TypeStateDialog::NPCNode) return;

    CondImageSDialog = SNew(SImage)
    .Visibility(this, &SDialogGraphNode::GetVisibilityCondition)
    .Image(FRPG_DialogSystemStyle::GetBrush(FRPG_DialogSystemStyle::GetIconCondition()))
    .ColorAndOpacity(FSlateColor(FColor::Cyan));

    MainBox->AddSlot()
        .AutoHeight()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        .Padding(0, 0, 0, 0)
        [
            CondImageSDialog.ToSharedRef()
        ];
}

void SDialogGraphNode::CreateEventIcon(TSharedPtr<SVerticalBox> MainBox)
{
    if (!DialogGraphNode) return;
    if (!DialogGraphNode->GetDialogSettingsObject()) return;
    if (DialogGraphNode->GetDialogSettingsObject()->TypeStateDialog != ERPG_TypeStateDialog::PlayerNode && DialogGraphNode->GetDialogSettingsObject()->TypeStateDialog != ERPG_TypeStateDialog::NPCNode) return;

    EventImageSDialog = SNew(SImage)
    .Visibility(this, &SDialogGraphNode::GetVisibilityEvent)
    .Image(FRPG_DialogSystemStyle::GetBrush(FRPG_DialogSystemStyle::GetIconEvent()))
    .ColorAndOpacity(FSlateColor(FColor::Red));

    MainBox->AddSlot()
        .AutoHeight()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        .Padding(0, 0, 0, 0)
        [
            EventImageSDialog.ToSharedRef()
        ];
}

EVisibility SDialogGraphNode::GetVisibilityCondition() const
{
    if (DialogGraphNode && DialogGraphNode->GetDialogSettingsObject())
    {
        return DialogGraphNode->GetDialogSettingsObject()->IsHaveSomeCondition() ? EVisibility::Visible : EVisibility::Hidden;
    }
    return EVisibility::Hidden;
}

EVisibility SDialogGraphNode::GetVisibilityEvent() const
{
    if (DialogGraphNode && DialogGraphNode->GetDialogSettingsObject())
    {
        return DialogGraphNode->GetDialogSettingsObject()->IsHaveSomeEvent() ? EVisibility::Visible : EVisibility::Hidden;
    }
    return EVisibility::Hidden;
}
