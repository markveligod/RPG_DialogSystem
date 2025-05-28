#include "SDialogGraphNode.h"
#include "GraphEditorSettings.h"
#include "IDocumentation.h"
#include "SCommentBubble.h"
#include "SGraphPin.h"
#include "SlateOptMacros.h"
#include "SLevelOfDetailBranchNode.h"
#include "TutorialMetaData.h"
#include "RPG_DialogSystem/RPG_DialogObject/Nodes/RPG_DialogNodeBase.h"
#include "RPG_DialogSystem/RPG_DialogObject/Nodes/RPG_DialogNodeWork.h"
#include "RPG_DialogSystemEditor/Settings/RPG_DialogSystemConfigEditor.h"
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
    CenterNodeBox.Reset();

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
        SNew(SOverlay) + SOverlay::Slot()[SNew(SImage).Image(FAppStyle::GetBrush("Graph.Node.TitleGloss")).ColorAndOpacity(this, &SGraphNode::GetNodeTitleIconColor)] +
        SOverlay::Slot()
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Center)[SNew(SHorizontalBox) +
                                   SHorizontalBox::Slot().HAlign(HAlign_Fill)[SNew(SBorder)
                                           .BorderImage(FAppStyle::GetBrush("Graph.Node.ColorSpill"))
                                           // The extra margin on the right
                                           // is for making the color spill stretch well past the node title
                                           .Padding(FMargin(10, 5, 30, 3))
                                           .BorderBackgroundColor(
                                               this, &SGraphNode::GetNodeTitleColor)[SNew(SHorizontalBox) +
                                                                                     SHorizontalBox::Slot()
                                                                                         .VAlign(VAlign_Top)
                                                                                         .Padding(FMargin(0.f, 0.f, 4.f, 0.f))
                                                                                         .AutoWidth()[SNew(SImage).Image(IconBrush).ColorAndOpacity(this, &SGraphNode::GetNodeTitleIconColor)] +
                                                                                     SHorizontalBox::Slot()[SNew(SVerticalBox) + SVerticalBox::Slot().AutoHeight()[CreateTitleWidget(NodeTitle)] +
                                                                                                            SVerticalBox::Slot().AutoHeight()[NodeTitle.ToSharedRef()]]]] +
                                   SHorizontalBox::Slot().HAlign(HAlign_Right).VAlign(VAlign_Center).Padding(0, 0, 5, 0).AutoWidth()[CreateTitleRightWidget()]] +
        SOverlay::Slot().VAlign(VAlign_Top)[SNew(SBorder)
                .Visibility(EVisibility::HitTestInvisible)
                .BorderImage(FAppStyle::GetBrush("Graph.Node.TitleHighlight"))
                .BorderBackgroundColor(this, &SGraphNode::GetNodeTitleIconColor)[SNew(SSpacer).Size(FVector2D(20, 20))]];

    SetDefaultTitleAreaWidget(DefaultTitleAreaWidget);

    TSharedRef<SWidget> TitleAreaWidget = SNew(SLevelOfDetailBranchNode)
                                              .UseLowDetailSlot(this, &SDialogGraphNode::UseLowDetailNodeTitles)
                                              .LowDetail()[SNew(SBorder)
                                                      .BorderImage(FAppStyle::GetBrush("Graph.Node.ColorSpill"))
                                                      .Padding(FMargin(75.0f, 22.0f))  // Saving enough space for a 'typical' title so the transition isn't quite so abrupt
                                                      .BorderBackgroundColor(this, &SDialogGraphNode::GetNodeTitleColor)]
                                              .HighDetail()[DefaultTitleAreaWidget];

    if (!SWidget::GetToolTip().IsValid())
    {
        TSharedRef<SToolTip> DefaultToolTip =
            IDocumentation::Get()->CreateToolTip(TAttribute<FText>(this, &SGraphNode::GetNodeTooltip), NULL, GraphNode->GetDocumentationLink(), GraphNode->GetDocumentationExcerptName());
        SetToolTip(DefaultToolTip);
    }

    // Setup a meta tag for this node
    FGraphNodeMetaData TagMeta(TEXT("Graphnode"));
    PopulateMetaTag(&TagMeta);

    TSharedPtr<SVerticalBox> InnerVerticalBox;
    this->ContentScale.Bind(this, &SGraphNode::GetContentScale);

    InnerVerticalBox = SNew(SVerticalBox) + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Fill).VAlign(VAlign_Top).Padding(Settings->GetNonPinNodeBodyPadding())[TitleAreaWidget]

                       + SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Fill).VAlign(VAlign_Top)[CreateNodeContentArea()];

    TSharedPtr<SWidget> EnabledStateWidget = GetEnabledStateWidget();
    if (EnabledStateWidget.IsValid())
    {
        InnerVerticalBox->AddSlot().AutoHeight().HAlign(HAlign_Fill).VAlign(VAlign_Top).Padding(FMargin(2, 0))[EnabledStateWidget.ToSharedRef()];
    }

    InnerVerticalBox->AddSlot().AutoHeight().Padding(Settings->GetNonPinNodeBodyPadding())[ErrorReporting->AsWidget()];

    InnerVerticalBox->AddSlot().AutoHeight().Padding(Settings->GetNonPinNodeBodyPadding())[VisualWarningReporting->AsWidget()];

    this->GetOrAddSlot(ENodeZone::Center)
        .HAlign(HAlign_Center)
        .VAlign(
            VAlign_Center)[SAssignNew(MainVerticalBox, SVerticalBox) +
                           SVerticalBox::Slot().AutoHeight()[SNew(SOverlay).AddMetaData<FGraphNodeMetaData>(TagMeta) +
                                                             SOverlay::Slot().Padding(
                                                                 Settings->GetNonPinNodeBodyPadding())[SNew(SImage).Image(GetNodeBodyBrush()).ColorAndOpacity(this, &SGraphNode::GetNodeBodyColor)] +
                                                             SOverlay::Slot()[InnerVerticalBox.ToSharedRef()]]];

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
            .VAlign(VAlign_Top)[CommentBubble.ToSharedRef()];
    }

    CreateBelowWidgetControls(MainVerticalBox);
    CreatePinWidgets();
    CreateInputSideAddButton(LeftNodeBox);
    CreateOutputSideAddButton(RightNodeBox);
    CreateBelowPinControls(InnerVerticalBox);
    CreateAdvancedViewArrow(InnerVerticalBox);
    UpdateNPCTextData();
}

void SDialogGraphNode::CreateStandardPinWidget(UEdGraphPin* Pin)
{
    const bool bShowPin = ShouldPinBeHidden(Pin);

    if (bShowPin)
    {
        TSharedPtr<SGraphPin> NewPin = CreatePinWidget(Pin);
        check(NewPin.IsValid());
        const FSlateBrush* PinConnect = FAppStyle::GetBrush(TEXT("Graph.ExecPin.Connected"));
        const FSlateBrush* PinDisConnect = FAppStyle::GetBrush(TEXT("Graph.ExecPin.Disconnected"));
        NewPin->SetCustomPinIcon(PinConnect, PinDisConnect);

        this->AddPin(NewPin.ToSharedRef());
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDialogGraphNode::UpdateOwnerEditorChange(FPropertyChangedEvent& PropertyChangedEvent)
{
    if (CenterNodeBox.IsValid())
    {
        UpdateNPCTextData();
    }
}

TSharedRef<SWidget> SDialogGraphNode::CreateNodeContentArea()
{
    if (DialogGraphNode && DialogGraphNode->GetOwnerNode() && DialogGraphNode->GetOwnerNode()->GetTypeDialogNode() == ERPG_TypeDialogNode::Work)
    {
        return SNew(SBorder)
            .BorderImage(FAppStyle::GetBrush("NoBorder"))
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            .Padding(FMargin(0,
                3))[SNew(SHorizontalBox)

                    // ─── LEFT ────────────────────────────────────────────────────────────────────
                    + SHorizontalBox::Slot().HAlign(HAlign_Left).AutoWidth().MinWidth(15.0f)[SAssignNew(LeftNodeBox, SVerticalBox)]

                    // separator
                    + SHorizontalBox::Slot().AutoWidth().MinWidth(5.0f)[SNew(SSeparator).Orientation(EOrientation::Orient_Vertical).Thickness(2.0f)]

                    // ─── CENTER ──────────────────────────────────────────────────────────────────
                    + SHorizontalBox::Slot()
                          .HAlign(HAlign_Center)
                          .AutoWidth()
                          .MinWidth(50.0f)[SNew(SVerticalBox)
                                           // Header для центральной части
                                           + SVerticalBox::Slot().AutoHeight().Padding(FMargin(5, 0, 5,
                                                 5))[SNew(STextBlock).Text(FText::FromString(TEXT("Dialog NPCs:"))).Font(FAppStyle::Get().GetFontStyle("BoldFont")).Justification(ETextJustify::Center)]

                                           // Содержимое центральной части
                                           + SVerticalBox::Slot().FillHeight(1.0f)[SAssignNew(CenterNodeBox, SVerticalBox)

        ]]
                    // separator
                    + SHorizontalBox::Slot().AutoWidth().MinWidth(5.0f)[SNew(SSeparator).Orientation(EOrientation::Orient_Vertical).Thickness(2.0f)]

                    // ─── RIGHT ───────────────────────────────────────────────────────────────────
                    + SHorizontalBox::Slot().AutoWidth().MinWidth(50.0f).HAlign(
                          HAlign_Right)[SNew(SVerticalBox)
                                        // Header для правой части
                                        + SVerticalBox::Slot().AutoHeight().Padding(FMargin(5, 0, 5,
                                              5))[SNew(STextBlock).Text(FText::FromString(TEXT("Dialog Players:"))).Font(FAppStyle::Get().GetFontStyle("BoldFont")).Justification(ETextJustify::Center)]
                                        // Содержимое правой части
                                        + SVerticalBox::Slot().FillHeight(1.0f)[SAssignNew(RightNodeBox, SVerticalBox)]]];
    }
    return SGraphNode::CreateNodeContentArea();
}

void SDialogGraphNode::UpdateNPCTextData()
{
    if (!DialogGraphNode) return;
    URPG_DialogNodeWork* WorkNode = Cast<URPG_DialogNodeWork>(DialogGraphNode->GetOwnerNode());
    if (!WorkNode) return;
    TArray<URPG_DialogNPC*> ArrayNPC = WorkNode->GetOutputNPCDialogs();
    CenterNodeBox->ClearChildren();

    for (int32 i = 0; i < ArrayNPC.Num(); ++i)
    {
        if (!ArrayNPC.IsValidIndex(i)) continue;
        URPG_DialogNPC* DialogNPC = ArrayNPC[i];
        if (!DialogNPC) continue;
        bool bVisibleCond = DialogNPC->IsHaveSomeCondition();
        bool bVisibleEvent = DialogNPC->IsHaveSomeEvent();

        // Prepare the text
        FText SetupText = FText::FromString(FString::Printf(TEXT("#%i | %s"), i, *DialogNPC->GetText().ToString()));

        const FSlateBrush* ConditionImageBrush = FRPG_DialogSystemStyle::GetBrush(FRPG_DialogSystemStyle::GetIconCondition());
        const FSlateBrush* EventImageBrush = FRPG_DialogSystemStyle::GetBrush(FRPG_DialogSystemStyle::GetIconEvent());

        CenterNodeBox->AddSlot()
            .AutoHeight()
            .HAlign(HAlign_Fill)
            .Padding(
                3)[SNew(SHorizontalBox)

                   // Condition icon, fixed size
                   +
                   SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)[SNew(SBox).WidthOverride(16.0f).HeightOverride(16.0f)
                           [SNew(SBorder).BorderImage(ConditionImageBrush).BorderBackgroundColor(FLinearColor(FColor::Cyan)).Visibility(bVisibleCond ? EVisibility::Visible : EVisibility::Collapsed)]]

                   // Event icon, fixed size
                   + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)[SNew(SBox).WidthOverride(16.0f).HeightOverride(
                         16.0f)[SNew(SBorder).BorderImage(EventImageBrush).BorderBackgroundColor(FLinearColor(FColor::Red)).Visibility(bVisibleEvent ? EVisibility::Visible : EVisibility::Collapsed)]]

                   // Text slot, wrapped in a fixed-width box
                   + SHorizontalBox::Slot().FillWidth(1.0f).Padding(
                         FMargin(5, 0))[SNew(SBox).WidthOverride(180.0f)[SNew(STextBlock).Text(SetupText).AutoWrapText(true).Justification(ETextJustify::Left)]]];
    }
}
