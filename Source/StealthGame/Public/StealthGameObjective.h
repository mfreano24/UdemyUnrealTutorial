// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StealthGameObjective.generated.h"

class USphereComponent;
class UParticleSystem;

UCLASS()
class STEALTHGAME_API AStealthGameObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStealthGameObjective();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* meshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* collider;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* pickupFX;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffects();

public:	

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
