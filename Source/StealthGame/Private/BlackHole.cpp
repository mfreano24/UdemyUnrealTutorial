// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/SphereComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//force collider
	forceTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("forceTrigger"));
	forceTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	forceTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	forceTrigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	
	RootComponent = forceTrigger;

	//destroy collider
	destroyTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("destroyTrigger"));
	destroyTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	destroyTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	destroyTrigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	destroyTrigger->SetupAttachment(forceTrigger);


	



}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();

	forceTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::RadialOverlap);
	destroyTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::DestroyOverlap);
	forceTrigger->GetOverlappingComponents(forceTargets);
	UE_LOG(LogTemp, Warning, TEXT("Size of forceTargets: %d"), forceTargets.Num())
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	for(int i = 0; i < forceTargets.Num(); i++)
	{
		if (forceTargets[i]->IsSimulatingPhysics()) {
			FVector vec = GetActorLocation() - forceTargets[i]->GetComponentLocation();
			vec.Normalize();
			
			forceTargets[i]->AddForce(vec, NAME_None, true);
		}
		
	}
	

}

void ABlackHole::RadialOverlap(UPrimitiveComponent* comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsSimulatingPhysics()) {
		forceTargets.AddUnique(OtherComp);
		UE_LOG(LogTemp, Warning, TEXT("Size of forceTargets: %d"), forceTargets.Num())
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("RadialOverlap failed - component is not simulating physics."))
	}
}

void ABlackHole::EndRadialOverlap(UPrimitiveComponent* comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherComp->IsSimulatingPhysics()) {
		forceTargets.Remove(OtherComp);
	}
}

void ABlackHole::DestroyOverlap(UPrimitiveComponent* comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OtherActor->Destroy();
}

