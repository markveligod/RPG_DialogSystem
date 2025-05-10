// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_DialogNodeBase.h"
#include "RPG_DialogNodeWork.generated.h"

/**
 *
 */
UCLASS(DisplayName = "DialogCondition", EditInlineNew, Abstract, Blueprintable)
class RPG_DIALOGSYSTEM_API URPG_DialogCondition : public UObject
{
    GENERATED_BODY()

#pragma region Action

public:
    virtual bool IsCondition(APlayerController* PC);

protected:
    UFUNCTION(BlueprintNativeEvent)
    bool IsConditionNative(APlayerController* PC);

#pragma endregion
};

/**
 *
 */
UCLASS(DisplayName = "DialogEvent", EditInlineNew, Abstract, Blueprintable)
class RPG_DIALOGSYSTEM_API URPG_DialogEvent : public UObject
{
    GENERATED_BODY()

#pragma region Action

public:
    virtual void TriggerEvent(APlayerController* PC);

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void Trigger_Event(APlayerController* PC);

#pragma endregion
};

/**
 *
 */
UCLASS()
class RPG_DIALOGSYSTEM_API URPG_DialogText : public UObject
{
    GENERATED_BODY()

public:
    /** @public **/
    virtual void Reset();

    /** @public **/
    FText& GetText();

    /** @public **/
    bool IsHaveSomeCondition() const;

    /** @public **/
    bool IsHaveSomeEvent() const;

    /** @public **/
    virtual bool IsValidCondition(APlayerController* PlayerController) const;

    /** @public **/
    virtual void ExecuteEvents(APlayerController* PlayerController);

private:
    /** @private **/
    UPROPERTY(EditAnywhere, Category = "Settings")
    FText Text;

    /** @private  **/
    UPROPERTY(EditAnywhere, Instanced, Category = "Settings")
    TArray<URPG_DialogCondition*> ArrayCondition;

    /** @private  **/
    UPROPERTY(EditAnywhere, Instanced, Category = "Settings")
    TArray<URPG_DialogEvent*> ArrayEvent;
};

/**
 *
 */
UCLASS(DisplayName = "DialogNPC", EditInlineNew)
class RPG_DIALOGSYSTEM_API URPG_DialogNPC : public URPG_DialogText
{
    GENERATED_BODY()
};

/**
 *
 */
UCLASS(DisplayName = "DialogPlayer", EditInlineNew)
class RPG_DIALOGSYSTEM_API URPG_DialogPlayer : public URPG_DialogText
{
    GENERATED_BODY()

public:
    /** @public **/
    virtual void Reset() override;

    /** @public **/
    int32 GetNextIDNode() const;

    /** @public **/
    void SetNextIDNode(const int32 ID);

private:
    /** @private **/
    UPROPERTY(VisibleAnywhere, Category = "Settings")
    int32 NextIDNode{INDEX_NONE};
};

/**
 *
 */
UCLASS()
class RPG_DIALOGSYSTEM_API URPG_DialogNodeWork : public URPG_DialogNodeBase
{
    GENERATED_BODY()

#pragma region ActionDialog

public:
    /** @public **/
    URPG_DialogNodeWork();

    /** @public  **/
    virtual int32 GetNextIDNode() override;

    /** @public  **/
    virtual void InitNode() override;

    /** @public  **/
    virtual void ResetNode() override;

    /** @public  **/
    void MakeSelectPlayerNode(uint16 SelectIndex);

    /** @public  **/
    TArray<URPG_DialogNPC*> GetNPCDialogs(APlayerController* PlayerController);

    /** @public  **/
    TArray<URPG_DialogPlayer*> GetPlayerDialogs(APlayerController* PlayerController);

#if WITH_EDITOR

    /** @public  **/
    const TArray<URPG_DialogPlayer*>& GetOutputPinDialogs();

    /** @public  **/
    const TArray<URPG_DialogNPC*>& GetOutputNPCDialogs();

    /** @public  **/
    int32 GetNextIDNodeByDialogPlayer(const uint16 IndexArray);

    /** @public  **/
    void SetNextIDNodeByDialogPlayer(const uint16 IndexArray, const int32 ID);

#endif

#pragma endregion

#pragma region DataNode

private:
    /** @private  **/
    UPROPERTY(EditAnywhere, Instanced, Category = "Settings")
    TArray<URPG_DialogNPC*> ArrayDialogNPC;

    /** @private  **/
    UPROPERTY(EditAnywhere, Instanced, Category = "Settings")
    TArray<URPG_DialogPlayer*> ArrayDialogPlayer;

    /** @private  **/
    uint16 IndexSelectPlayerNode{0};

#pragma endregion
};
