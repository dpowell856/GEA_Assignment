// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskSteerVehicle.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "AIWheeledVehicleController.h"
#include "WheeledVehicleMovementComponent4W.h"


EBTNodeResult::Type UBTTaskSteerVehicle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIWheeledVehicleController* AIController = Cast<AAIWheeledVehicleController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		float Steering;

		if (Blackboard->HasValidAsset())
		{
			Steering = Blackboard->GetValueAsFloat("SteeringValue");
		}
		else
		{
			Steering = FMath::RandRange(-1.0f, 1.0f);
		}

		AIController->VehicleMovementComp->SetSteeringInput(Steering);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTTaskSteerVehicle::OnGameplayTaskActivated(UGameplayTask & task)
{
	//NB: this method must be overridden even if it has an empty body
	//otherwise the linker will fail
}
