// Fill out your copyright notice in the Description page of Project Settings.

#include "AIWheeledVehicle.h"
#include "VehicleTemplateWheelFront.h"
#include "VehicleTemplateWheelRear.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"

#include "CheckPoint.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "VehicleTemplateGameMode.h"
#include "VehicleTemplatePawn.h"

AAIWheeledVehicle::AAIWheeledVehicle()
{
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/Sedan/Sedan_AnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	// Simulation
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = UVehicleTemplateWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UVehicleTemplateWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UVehicleTemplateWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UVehicleTemplateWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);
}

void AAIWheeledVehicle::Tick(float Delta)
{
	Super::Tick(Delta);
	if (this->GetActorLocation().Z < 5 || this->GetActorRotation().Roll > 90)
	{
		this->Respawn();
	}
}

void AAIWheeledVehicle::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckPoint::StaticClass(), CheckPoints);
	NextCheckPoint = 1;
	SpawnLocation = this->GetActorLocation();
	SpawnRotation = this->GetActorRotation();
	AVehicleTemplateGameMode* GameMode = (AVehicleTemplateGameMode*)GetWorld()->GetAuthGameMode();
	if (GameMode->GetRaceType() == 1)
	{
		LapsRemaing = GameMode->GetRaceLaps();
	}
}

void AAIWheeledVehicle::Respawn()
{
	this->TeleportTo(SpawnLocation, SpawnRotation);
}

void AAIWheeledVehicle::SetSpawn(FVector Position, FRotator Rotation)
{
	this->SpawnLocation = Position;
	this->SpawnRotation = Rotation;
}

void AAIWheeledVehicle::CompleteLap()
{
	NextCheckPoint = 1;
	LapsRemaing--;
}

int AAIWheeledVehicle::GetLapsRemaining()
{
	return this->LapsRemaing;
}

void AAIWheeledVehicle::OnHit(AActor* SelfActor, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->GetClass() == AVehicleTemplatePawn::StaticClass())
	{
		AVehicleTemplatePawn* PlayerVehicle = Cast<AVehicleTemplatePawn>(OtherActor);
		float ImpulseMagnitude = 10;
		FVector Impulse = NormalImpulse * ImpulseMagnitude;
		PlayerVehicle->GetMesh()->AddImpulse(Impulse, TEXT(""), false);
		PlayerVehicle->UpdateAllReplicatedComponents();
	}
	else if (OtherActor->GetClass() == AAIWheeledVehicle::StaticClass())
	{
		AAIWheeledVehicle* AIVehicle = Cast<AAIWheeledVehicle>(OtherActor);
		UWheeledVehicleMovementComponent* MoveComp = AIVehicle->GetVehicleMovement();
		float ImpulseMagnitude = 10;
		FVector Impulse = NormalImpulse * ImpulseMagnitude;
		AIVehicle->GetMesh()->AddImpulse(Impulse, TEXT(""), false);
		AIVehicle->UpdateAllReplicatedComponents();
	}
}

FVector AAIWheeledVehicle::GetNextCheckPointPos()
{
	return CheckPoints[NextCheckPoint - 1]->GetActorLocation();
}

void AAIWheeledVehicle::SetNextCheckPoint()
{
	if (CheckPoints.Num() > NextCheckPoint)
	{
		NextCheckPoint++;
	}
}

int AAIWheeledVehicle::NumOfCheckPoints()
{
	return CheckPoints.Num();
}

int AAIWheeledVehicle::GetNextCheckPointNum()
{
	return this->NextCheckPoint;
}
