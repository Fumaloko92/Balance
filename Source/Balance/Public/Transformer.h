// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "./BalanceBall.h"
#include "Transformer.generated.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

UCLASS()
class BALANCE_API ATransformer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATransformer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Transformer)
	EBallType TransformTo;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Trigger)
	AActor* TriggerVolume;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Effect)
	float TransformationTime;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Effect)
	AEmitter* ParticleEffect;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void RegisterDelegate();

	UFUNCTION()
	void OnBeginTriggerOverlap(class AActor* Trigger, class AActor* Actor);

	UFUNCTION()
	void OnEndTriggerOverlap(class AActor* Trigger, class AActor* Actor);
private:
	float _timer;
	bool _overlapping;
	AActor* _overlappingActor;

	void HandleTransformation(float Time);

	void EnableParticles();

	void DisableParticles();
};
