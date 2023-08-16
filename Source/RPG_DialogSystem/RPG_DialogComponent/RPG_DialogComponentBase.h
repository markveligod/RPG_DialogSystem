// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPG_DialogComponentBase.generated.h"


UCLASS(ClassGroup=(Managers))
class RPG_DIALOGSYSTEM_API URPG_DialogComponentBase : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URPG_DialogComponentBase();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
