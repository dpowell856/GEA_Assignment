// Fill out your copyright notice in the Description page of Project Settings.

#include "BTSteeringService.h"
#include <EngineUtils.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "VehicleTemplatePawn.h"
#include "AIWheeledVehicleController.h"

void UBTSteeringService::OnGameplayTaskActivated(UGameplayTask & task)
{
	//nothing here
	//This method must be overridden from the inherited interface
	//Otherwise, the linker will fail.
}

void UBTSteeringService::OnGameplayTaskDeactivated(UGameplayTask & task)
{
	//nothing here
	//This method must be overridden from the inherited interface
	//Otherwise, the linker will fail.
}

void UBTSteeringService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	{
		APawn* OwnerPawn;
		AAIWheeledVehicleController* AIVehicle = Cast<AAIWheeledVehicleController>(OwnerComp.GetAIOwner());
		OwnerPawn = AIVehicle->GetPawn();

		FVector ForwardVector = OwnerPawn->GetActorForwardVector();
		FVector RightVector = OwnerPawn->GetActorRightVector();
		FVector NextCheckPointLocation = AIVehicle->GetNextCheckPointPos();
		FVector TargetDirection = NextCheckPointLocation - OwnerPawn->GetActorLocation();
		float Distance = FVector::DistSquared(NextCheckPointLocation, OwnerPawn->GetActorLocation());
		float Throttle = FMath::Clamp<float>(0.0f, 1.0f, Distance / 750.0f);
		TargetDirection.Normalize();
		float AngleValue = FVector::DotProduct(ForwardVector, TargetDirection);
		if (AngleValue < -0.5)
		{
			AIVehicle->Respawn();
		}
		float SteerValue = FVector::DotProduct(RightVector, TargetDirection) < 0.0f ? -(1.0f - AngleValue) : (1.0f - AngleValue);
		AIVehicle->BlackboardComp->SetValueAsVector("PlayerLocation", OwnerPawn->GetActorLocation());
		AIVehicle->BlackboardComp->SetValueAsFloat("SteeringValue", SteerValue);
		AIVehicle->BlackboardComp->SetValueAsFloat("ThrottleValue", AngleValue < 0 ? -.15f : Throttle);
	}
}