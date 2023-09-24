
#include "FRPG_DialogSystemStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Misc/Paths.h"

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon24x24(24.0f, 24.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon32x32(32.0f, 32.0f);
const FVector2D Icon64x64(64.0f, 64.0f);
const FVector2D Icon96x96(96.0f, 96.0f);

#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

TSharedPtr<FSlateStyleSet> FRPG_DialogSystemStyle::StyleSet = nullptr;

void FRPG_DialogSystemStyle::SetClassIcon(const FName ClassName, const WIDECHAR* RelativePath, const FLinearColor ClassColor)
{
    if (!StyleSet.IsValid()) return;

    StyleSet->Set(FName("ClassIcon." + ClassName.ToString()),
        new FSlateImageBrush(StyleSet->RootToContentDir(FString(RelativePath) + "_16x", TEXT(".png")), Icon16x16, ClassColor)
    );
    StyleSet->Set(FName("ClassThumbnail." + ClassName.ToString()),
        new FSlateImageBrush(StyleSet->RootToContentDir(FString(RelativePath) + "_64x", TEXT(".png")), Icon64x64, ClassColor)
    );
}

void FRPG_DialogSystemStyle::Initialize()
{
    if (StyleSet.IsValid()) return; 
    
    StyleSet = MakeShareable(new FSlateStyleSet("FRPG_DialogSystemStyle"));
    StyleSet->SetContentRoot(FPaths::ProjectPluginsDir() / TEXT("RPG_DialogSystem/Resources"));

    SetClassIcon("RPG_DialogObjectBase", TEXT("Dialogue"), FLinearColor::White);
    SetClassIcon("RPG_DialogComponentBase", TEXT("Dialogue"), FLinearColor::White);
    StyleSet->Set(GetIconEvent(), new IMAGE_BRUSH("DialogueEvent_16", Icon16x16));
    StyleSet->Set(GetIconCondition(), new IMAGE_BRUSH("DialogueCondition_16", Icon16x16));

    FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FRPG_DialogSystemStyle::Shutdown()
{
    if (!StyleSet.IsValid()) return;

    FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
    ensure(StyleSet.IsUnique());
    StyleSet.Reset();
}

FName FRPG_DialogSystemStyle::GetIconEvent()
{
    return FName(TEXT("DialogEditor.Icon.Event"));
}

FName FRPG_DialogSystemStyle::GetIconCondition()
{
    return FName(TEXT("DialogEditor.Icon.Condition"));
}
