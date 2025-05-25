#pragma once

#include "CoreMinimal.h"
#include "RPG_DialogSystemDataTypes.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogDialogSystem, All, All);

namespace DialogSystemSpace
{
inline bool IsLogPrint()
{
#if !UE_BUILD_SHIPPING
    const auto DialogSystemShowLog = IConsoleManager::Get().FindConsoleVariable(TEXT("DialogSystem.ShowLog"));
    if (DialogSystemShowLog)
    {
        return DialogSystemShowLog->GetBool();
    }
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
            UE_LOG(LogDialogSystem, Verbosity, TEXT("[%s] | TEXT:[%s]"), ANSI_TO_TCHAR(__FUNCTION__), *Msg);                                                                                           \
        }                                                                                                                                                                                              \
    }

#define CLOG_DIALOG_SYSTEM(Cond, Format, ...) DialogSystemSpace::ClogPrint(Cond, ANSI_TO_TCHAR(__FUNCTION__), FString::Printf(TEXT(Format), ##__VA_ARGS__))

class URPG_DialogObjectBase;
class URPG_DialogComponentBase;

DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateTargetDialogSignature, URPG_DialogComponentBase*);
DECLARE_DELEGATE_OneParam(FUpdateStateDialogSignature, URPG_DialogObjectBase*);

UENUM(BlueprintType)
enum class ERPG_TypeDialogNode : uint8
{
    None = 0,
    Start,
    Work,
    Finish,
    Transfer,
};

UENUM()
enum class ERPG_DialogObjectState : uint8
{
    None = 0,
    Init,
    Run,
    Finish,
};