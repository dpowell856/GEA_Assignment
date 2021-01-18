// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CheckPoint.h"
#include "AIWheeledVehicleController.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLETEMPLATE_API AAIWheeledVehicleController : public AAIController
{
    GENERATED_BODY()


public:
    class UWheeledVehicleMovementComponent* VehicleMovementComp;
    class UBehaviorTreeComponent* BehaviorTreeComp;
    class UBlackboardComponent* BlackboardComp;
    AAIWheeledVehicleController();
    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaSeconds) override;
    FVector GetNextCheckPointPos();
    void Respawn();
};
