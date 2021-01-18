// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskThrottle.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h>
#include "AIWheeledVehicleController.h"
#include "WheeledVehicleMovementComponent4W.h"

EBTNodeResult::Type UBTTaskThrottle::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AAIWheeledVehicleController* AIController = Cast<AAIWheeledVehicleController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		float Throttle;

		if (Blackboard->HasValidAsset())
		{
			Throttle = Blackboard->GetValueAsFloat("ThrottleValue");
		}
		else
		{
			Throttle = FMath::RandRange(-1.0f, 1.0f);
		}

		AIController->VehicleMovementComp->SetThrottleInput(Throttle);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTTaskThrottle::OnGameplayTaskActivated(UGameplayTask & task)
{
	//This method must be overridden
	//Otherwise, the linker will fail.
}


