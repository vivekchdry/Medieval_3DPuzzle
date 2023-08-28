// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
//#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle NULL"));
		return;
	}
	FVector TargetLocation = GetComponentLocation() + (GetForwardVector() * HoldDistance);
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
}

void UGrabber::TriggerGrab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle NULL"));
		return;
	}
	FVector Start = GetComponentLocation();
	FVector End = Start + (GetForwardVector() * MaxGrabDistance);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	// DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Green,false,2.f);
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	bool HasHit;
	FHitResult HitResult;
	HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);
	//UE_LOG(LogTemp,Display,TEXT("HasHit "),HasHit);
	if (HasHit)
	{
		//DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Green,false,2.f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Blue, false, 2.f);
		//AActor* HitActor = HitResult.GetActor();
		//UE_LOG(LogTemp, Display, TEXT("HitActor %s"), *HitActor->GetActorNameOrLabel());
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation());
	}
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("NO HIT"));
	}
}

void UGrabber::ReleaseGrab()
{
	UE_LOG(LogTemp, Display, TEXT("Grabber Released "));
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* ReturnPhysicsHandle = GetOwner()->FindComponentByClass <UPhysicsHandleComponent>();
	if(ReturnPhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReturnPhysicsHandle NULL"));
	}
	return ReturnPhysicsHandle;
}
