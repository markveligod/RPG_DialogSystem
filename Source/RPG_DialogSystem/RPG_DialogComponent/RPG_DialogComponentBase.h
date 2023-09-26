// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPG_DialogSystemDataTypes.h"
#include "RPG_DialogComponentBase.generated.h"

class URPG_DialogObjectBase;
UCLASS(ClassGroup = (Managers))
class RPG_DIALOGSYSTEM_API URPG_DialogComponentBase : public UActorComponent
{
    GENERATED_BODY()

#pragma region Default
    
public:
    
    // Sets default values for this component's properties
    URPG_DialogComponentBase();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
    
protected:
    
    // Called when the game starts
    virtual void BeginPlay() override;

#pragma endregion

#pragma region Action

public:

    /** @public **/
    UFUNCTION()
    void OnRep_TargetDialog();

#pragma endregion
    
#pragma region Data

private:

    /** @private **/
    UPROPERTY(ReplicatedUsing=OnRep_TargetDialog)
    URPG_DialogObjectBase* TargetDialog{nullptr};

#pragma endregion

#pragma region Signature

protected:

    /** @private **/
    virtual void NotifyUpdateTargetDialog();
    
    /** @private **/
    UPROPERTY(BlueprintAssignable)
    FUpdateTargetDialogSignature OnUpdateTargetDialog;
    
#pragma endregion
    
};
