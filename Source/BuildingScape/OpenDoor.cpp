// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();	
	Owner = GetOwner();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName());
	}
}


void UOpenDoor::OpenDoor()
{
	if (!Owner) { return; }
	// Create a rotator
	//FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);

	// Set door rotation
	//Owner->SetActorRotation(NewRotation);
	OnOpenRequest.Broadcast();
}


void UOpenDoor::CloseDoor()
{
	// Create a rotator
	FRotator NewRotation = FRotator(0.0f, CloseAngle, 0.0f);

	// Set door rotation
	if (!Owner) { return; }
	Owner->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If ActorThatOpens is in volume
	if (GetMassOfActorOnPlate() > 35.f)
	{
		OpenDoor();
		LastOpenDoorTime = GetWorld()->GetTimeSeconds();
	} 
	else if (GetWorld()->GetTimeSeconds() - LastOpenDoorTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetMassOfActorOnPlate() const
{
	float TotalMass = 0.f;

	// Find all the overlaping actors
	TArray<AActor*> OverlapingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlapingActors);

	// Iterate through them adding their masses
	for (auto *Actor : OverlapingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Name of actor: %s"), *Actor->GetName());
	}

	return TotalMass;
}

