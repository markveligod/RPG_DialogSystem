// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPG_DialogSystem/RPG_DialogObject/RPG_DialogObjectBase.h"
#include "RPG_DialogComponentBase.generated.h"

class URPG_DialogObjectBase;
UCLASS(ClassGroup = (Managers), meta = (BlueprintSpawnableComponent),
    HideCategories = ("Variable", "Sockets", "Tags", "ComponentTick", "ComponentReplication", "Activation", "Cooking", "Events", "AssetUserData", "Replication", "Navigation"))
class RPG_DIALOGSYSTEM_API URPG_DialogComponentBase : public UActorComponent
{
    GENERATED_BODY()

#pragma region Default

public:
    // Sets default values for this component's properties
    URPG_DialogComponentBase();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

#pragma endregion

#pragma region Action

public:
    /** @public  **/
    void RunDialog();

    /** @public  **/
    URPG_DialogObjectBase* GetTargetDialog() const { return TargetDialog; }

protected:
    /** @protected **/
    virtual void RunDialog_Internal();

    /** @protected **/
    virtual void RegisterUpdateDialogObject(URPG_DialogObjectBase* DialogObject);

#pragma endregion

#pragma region Data

private:
    /** @private **/
    UPROPERTY(EditAnywhere, Category = "Dialog Settings")
    TObjectPtr<URPG_DialogObjectBase> TargetDialog;

#pragma endregion

#pragma region Signature

public:
    /** @public **/
    template <typename UserClass>
    FDelegateHandle SignUpdateTargetDialog(UserClass* Object, void (UserClass::*Method)(URPG_DialogComponentBase*))
    {
        static_assert(TIsDerivedFrom<UserClass, UObject>::IsDerived, "SignUpdateTargetDialog<UserClass>: UserClass must be a UObject");
        return OnUpdateTargetDialog.AddUObject(Object, Method);
    }

    /** @public **/
    void UnSignUpdateTargetDialog(const FDelegateHandle& Handle) { OnUpdateTargetDialog.Remove(Handle); }

protected:
    /** @protected **/
    virtual void NotifyUpdateTargetDialog();

    /** @protected **/
    FUpdateTargetDialogSignature OnUpdateTargetDialog;

#pragma endregion
};
