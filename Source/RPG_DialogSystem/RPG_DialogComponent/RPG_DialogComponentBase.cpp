// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_DialogComponent/RPG_DialogComponentBase.h"

// Sets default values for this component's properties
URPG_DialogComponentBase::URPG_DialogComponentBase()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void URPG_DialogComponentBase::BeginPlay()
{
    Super::BeginPlay();

    // ...
}

// Called every frame
void URPG_DialogComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}
