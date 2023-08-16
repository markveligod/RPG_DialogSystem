#pragma once

#include "CoreMinimal.h"
#include "RPG_DialogSystemDataTypes.generated.h"

class URPG_DialogSettingsObject;

UENUM(BlueprintType)
enum class ERPG_TypeStateDialog : uint8
{
    None UMETA(Hidden),
    Entry,
    NPCNode,
    PlayerNode,
};

USTRUCT(BlueprintType)
struct FRPG_DialogNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 IndexNode{INDEX_NONE};

    UPROPERTY(EditAnywhere)
    FVector2D NodePosition{FVector2D::UnitVector};

    UPROPERTY(EditAnywhere)
    ERPG_TypeStateDialog TypeStateDialog{ERPG_TypeStateDialog::None};

    UPROPERTY(EditAnywhere)
    URPG_DialogSettingsObject* DialogConditionObject{nullptr};

    UPROPERTY(EditAnywhere)
    TArray<int32> OutNodes;

    bool operator==(const FRPG_DialogNode& Node) const { return Node.IndexNode == this->IndexNode; }
};

DECLARE_LOG_CATEGORY_CLASS(LogDialogSystem, All, All);

namespace DialogSystemSpace
{
inline bool IsLogPrint()
{
#if !UE_BUILD_SHIPPING
    const auto DialogSystemShowLog = IConsoleManager::Get().FindConsoleVariable(TEXT("DialogSystem.ShowLog"));
    return DialogSystemShowLog ? DialogSystemShowLog->GetBool() : false;
#endif
    return false;
}

inline bool ClogPrint(bool Cond, TCHAR* NameFunction, const FString& Text)
{
    if (Cond)
    {
        UE_LOG(LogDialogSystem, Error, TEXT("[%s] | TEXT:[%s]"), NameFunction, *Text);
    }
    return Cond;
}
}  // namespace DialogSystemSpace

#define LOG_DIALOG_SYSTEM(Verbosity, Format, ...)                                                                                                                                                      \
    {                                                                                                                                                                                                  \
        if (DialogSystemSpace::IsLogPrint())                                                                                                                                                           \
        {                                                                                                                                                                                              \
            const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__);                                                                                                                          \
            UE_LOG(LogDialogSystem, Verbosity, TEXT("[%s] | TEXT:[%s]"), ANSI_TO_TCHAR(__FUNCTION__), *Text);                                                                                          \
        }                                                                                                                                                                                              \
    }

#define CLOG_DIALOG_SYSTEM(Cond, Format, ...) DialogSystemSpace::ClogPrint(Cond, ANSI_TO_TCHAR(__FUNCTION__), FString::Printf(TEXT(Format), ##__VA_ARGS__))