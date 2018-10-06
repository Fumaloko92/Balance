// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.


#include "Balance.h"
#include "BalanceBall.h"


ABalanceBall::ABalanceBall()
{
	BallsDict = TMap<EBallType, int>();
	BallProps = TMap<EBallType, FBallProps>();
	BallProps.Add(EBallType::BE_Paper, FBallProps(EBallType::BE_Paper));
	BallProps.Add(EBallType::BE_Rock, FBallProps(EBallType::BE_Rock));
	BallProps.Add(EBallType::BE_Iron, FBallProps(EBallType::BE_Iron));
	BallProps.Add(EBallType::BE_Wood, FBallProps(EBallType::BE_Wood));
	int counter = 0;
	ChosenProperties = EBallType::BE_Wood;
	for (auto& prop : BallProps)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(*prop.Value.GetPathToMesh());
		BallsDict.Add(prop.Key, counter);
		BallMeshes.Add(BallMesh.Object);
		counter++;
	}
	UpdateBallProps();
	

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 600.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
											 // Set up forces

	MaxZoomOut = 1200;
	MaxZoomIn = 150;
	ZoomSpeed = 50;

	RotateL = false; RotateR = false;
	TorqueVector = FVector::ZeroVector;

}

void ABalanceBall::UpdateBallProps()
{
	if (Ball == nullptr)
		Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetSimulatePhysics(true);
	Ball->SetStaticMesh(BallMeshes[*BallsDict.Find(ChosenProperties)]);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetAngularDamping(BallProps.Find(ChosenProperties)->GetAngularDamping());
	Ball->SetLinearDamping(BallProps.Find(ChosenProperties)->GetLinearDamping());
	Ball->SetAllMassScale(BallProps.Find(ChosenProperties)->GetMassScale());
	Ball->SetPhysicsMaxAngularVelocity(BallProps.Find(ChosenProperties)->GetMaxAngularVelocity());

	
	RollTorque = BallProps.Find(ChosenProperties)->GetRollTorque();
	RootComponent = Ball;
}

void ABalanceBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &ABalanceBall::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABalanceBall::MoveForward);

	PlayerInputComponent->BindAxis("Zoom", this, &ABalanceBall::Zoom);

	PlayerInputComponent->BindAxis("CameraUpDown", this, &ABalanceBall::CameraUpDown);
	PlayerInputComponent->BindAxis("CameraLeftRight", this, &ABalanceBall::CameraLeftRight);

}

void ABalanceBall::MoveRight(float Val)
{
	TorqueVector.X = -Val * RollTorque;

}

void ABalanceBall::MoveForward(float Val)
{
	TorqueVector.Y = Val*RollTorque;

}

void ABalanceBall::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Ball->AddTorque(SpringArm->RelativeRotation.RotateVector(TorqueVector));
	if (RotateL)
		SpringArm->AddWorldRotation(FRotator(0, -1, 0));
	if (RotateR)
		SpringArm->AddWorldRotation(FRotator(0, 1, 0));
}

void ABalanceBall::Zoom(float Val)
{
	if ((Val > 0 && SpringArm->TargetArmLength < MaxZoomOut) || (Val < 0 && SpringArm->TargetArmLength > MaxZoomIn))
		SpringArm->TargetArmLength += Val*ZoomSpeed;

}

void ABalanceBall::CameraUpDown(float Val)
{

	if (Val != 0)
	{
		FRotator r = SpringArm->RelativeRotation;
		if (r.Pitch + Val < 0 && r.Pitch + Val > -90) {
			r.Roll = 0;
			r.Add(Val, 0, 0);
			SpringArm->SetRelativeRotation(r);
		}
	}
}

void ABalanceBall::CameraLeftRight(float Val)
{
	if (Val != 0)
	{
		FRotator r = SpringArm->RelativeRotation;
		r.Roll = 0;
		r.Add(0, Val, 0);
		SpringArm->SetRelativeRotation(r);
	}
}

void ABalanceBall::Transform(EBallType Target)
{
	
	if (this != nullptr && Target != ChosenProperties)
	{
		ChosenProperties = Target;
		UpdateBallProps();
	}
}