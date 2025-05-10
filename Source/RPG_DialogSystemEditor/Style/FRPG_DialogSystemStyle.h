#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FRPG_DialogSystemStyle
{
public:
    static void Initialize();
    static void Shutdown();

    static FName GetIconEvent();
    static FName GetIconCondition();
    static void SetClassIcon(const FName ClassName, const WIDECHAR* RelativePath, const FLinearColor ClassColor);
    static FORCEINLINE const FName& GetStyleSetName() { return StyleSet->GetStyleSetName(); }
    static FORCEINLINE TSharedPtr<ISlateStyle> Get() { return StyleSet; }

    static const FSlateBrush* GetBrush(FName PropertyName, const ANSICHAR* Specifier = nullptr) { return Get()->GetBrush(PropertyName, Specifier); }

private:
    static TSharedPtr<FSlateStyleSet> StyleSet;
};
