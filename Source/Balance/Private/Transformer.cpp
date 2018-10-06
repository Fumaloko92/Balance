// Fill out your copyright notice in the Description page of Project Settings.

#include "Balance.h"
#include "./BalanceBall.h"
#include "./Public/Transformer.h"


// Sets default values
ATransformer::ATransformer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATransformer::BeginPlay()
{
	Super::BeginPlay();
	RegisterDelegate();
}

// Called every frame
void ATransformer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (_overlapping)
		HandleTransformation(DeltaTime);
}

void ATransformer::RegisterDelegate()
{
	TScriptDelegate<FWeakObjectPtr> del_begin;
	del_begin.BindUFunction(this, FName(TEXT("OnBeginTriggerOverlap")));
	TriggerVolume->OnActorBeginOverlap.Add(del_begin);

	TScriptDelegate<FWeakObjectPtr> del_end;
	del_end.BindUFunction(this, FName(TEXT("OnEndTriggerOverlap")));
	TriggerVolume->OnActorEndOverlap.Add(del_end);
}

void ATransformer::OnBeginTriggerOverlap(class AActor* Trigger, class AActor* Actor)
{
	EnableParticles();
	_overlappingActor = Actor;
	_timer = TransformationTime;
	_overlapping = true;
}


void ATransformer::OnEndTriggerOverlap(class AActor* Trigger, class AActor* Actor)
{
	DisableParticles();
	if(Actor == _overlappingActor)
		_overlappingActor = nullptr;
}

void ATransformer::HandleTransformation(float Time)
{
	_timer -= Time;
	if (_timer <= 0)
	{
		Cast<ABalanceBall>(_overlappingActor)->Transform(TransformTo);
		DisableParticles();
		_overlapping = false;
	}
}

void ATransformer::EnableParticles()
{
	ParticleEffect->Activate();
}

void ATransformer::DisableParticles()
{
	ParticleEffect->Deactivate();
}
