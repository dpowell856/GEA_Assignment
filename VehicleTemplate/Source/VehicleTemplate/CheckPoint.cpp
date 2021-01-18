// Fill out your copyright notice in the Description page of Project Settings.
  

#include "CheckPoint.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "AIWheeledVehicle.h"
#include "VehicleTemplatePawn.h"
#include "VehicleTemplateGameMode.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));

	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));

	CollisionBox->SetCollisionProfileName("Trigger");

	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverLapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACheckPoint::OnOverLapEnd);

}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	AVehicleTemplateGameMode* GameMode = (AVehicleTemplateGameMode*)GetWorld()->GetAuthGameMode();
	RaceType = GameMode->GetRaceType();
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int ACheckPoint::GetCheckPointOrder()
{
	return CheckPointOrder;
}

void ACheckPoint::OnOverLapBegin(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass() == AAIWheeledVehicle::StaticClass()) 
	{
		AAIWheeledVehicle* AIVehicle = Cast<AAIWheeledVehicle>(OtherActor);
		if(AIVehicle->GetNextCheckPointNum() == this->CheckPointOrder)
		{
			AIVehicle->SetSpawn(this->GetActorLocation(), this->GetActorRotation());
			AIVehicle->SetNextCheckPoint();
		}
	}
	else if (OtherActor->GetClass() == AVehicleTemplatePawn::StaticClass())
	{
		AVehicleTemplatePawn* PlayerVehicle = Cast<AVehicleTemplatePawn>(OtherActor);
		if (PlayerVehicle->GetNextCheckPointNum() == this->CheckPointOrder)
		{
			PlayerVehicle->SetSpawn(this->GetActorLocation(), this->GetActorRotation());
			PlayerVehicle->SetNextCheckPoint();
		}
	}
}

void ACheckPoint::OnOverLapEnd(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass() == AAIWheeledVehicle::StaticClass())
	{
		AAIWheeledVehicle* AIVehicle = Cast<AAIWheeledVehicle>(OtherActor);
		if (this->GetCheckPointOrder() == AIVehicle->NumOfCheckPoints() && AIVehicle->GetNextCheckPointNum() == AIVehicle->NumOfCheckPoints())
		{
			if(RaceType == 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName() + " Has passed checpoint the Final Checkpoint and completed the Race");
				OtherActor->Destroy();
			}
			else if(RaceType == 1)
			{
				AIVehicle->CompleteLap();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName() + " Has completed a lap of the Race");
				if(AIVehicle->GetLapsRemaining() == 0)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName() + " Has passed checpoint the Final Checkpoint and completed the Race");
					OtherActor->Destroy();
				}
			}
		}
	}
	else if (OtherActor->GetClass() == AVehicleTemplatePawn::StaticClass())
	{
		AVehicleTemplatePawn* PlayerVehicle = Cast<AVehicleTemplatePawn>(OtherActor);
		if (this->GetCheckPointOrder() == PlayerVehicle->NumOfCheckPoints()  && PlayerVehicle->GetNextCheckPointNum()  == PlayerVehicle->NumOfCheckPoints())
		{
			if (RaceType == 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName() + " Has passed checpoint the Final Checkpoint and completed the Race");
				OtherActor->Destroy();
			}
			else if (RaceType == 1)
			{
				PlayerVehicle->CompleteLap();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName() + " Has completed a lap of the Race");
				if (PlayerVehicle->GetLapsRemaining() == 0)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, OtherActor->GetName() + " Has passed checpoint the Final Checkpoint and completed the Race");
					OtherActor->Destroy();
				}
			}
		}
	}
}

