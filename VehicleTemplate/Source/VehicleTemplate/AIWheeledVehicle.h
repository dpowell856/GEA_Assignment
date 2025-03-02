// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "AIWheeledVehicle.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLETEMPLATE_API AAIWheeledVehicle : public AWheeledVehicle
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

public:
	AAIWheeledVehicle();

	// Begin Actor interface
	virtual void Tick(float Delta) override;
protected:
	virtual void BeginPlay() override;

	/* Are we on a 'slippery' surface */
	bool bIsLowFriction;


private:

	int NextCheckPoint;

	TArray<AActor*> CheckPoints;

	FVector SpawnLocation;
	FRotator SpawnRotation;

	int LapsRemaing;
	
public:
	FVector GetNextCheckPointPos();
	void SetNextCheckPoint();
	int NumOfCheckPoints();
	int GetNextCheckPointNum();
	void Respawn();
	void SetSpawn(FVector Position, FRotator Rotation);
	void CompleteLap();
	int GetLapsRemaining();

	UFUNCTION()
		void OnHit(AActor* SelfActor, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
