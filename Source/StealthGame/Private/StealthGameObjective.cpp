// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGameObjective.h"
#include "../StealthGameCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AStealthGameObjective::AStealthGameObjective()
{

	//mesh component
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = meshComp;

	//sphere collider (trigger)
	collider = CreateDefaultSubobject<USphereComponent>(TEXT("collider"));
	collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //don't actively block
	collider->SetCollisionResponseToAllChannels(ECR_Ignore); //ignore with all classes
	collider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); //pawn classes have overlap response
	collider->SetupAttachment(meshComp);

}

// Called when the game starts or when spawned
void AStealthGameObjective::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStealthGameObjective::PlayEffects() {
	UGameplayStatics::SpawnEmitterAtLocation(this, pickupFX, GetActorLocation());
}

void AStealthGameObjective::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();
	
	//attempt to cast the other actor to a StealthGameCharacter. If success, then we've overlapped with the player.
	AStealthGameCharacter* playerChar = Cast<AStealthGameCharacter>(OtherActor);

	if (playerChar) {
		playerChar->bIsCarryingObjective = true;

		Destroy();
	}
	
	
}

