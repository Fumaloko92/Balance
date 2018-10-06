// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "GameFramework/Pawn.h"
#include "BalanceBall.generated.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

UENUM(BlueprintType)		
enum class EBallType : uint8
{
	BE_Paper 	UMETA(DisplayName = "Paper"),
	BE_Rock 	UMETA(DisplayName = "Rock"),
	BE_Iron		UMETA(DisplayName = "Iron"),
	BE_Wood		UMETA(DisplayName = "Wood")
};

USTRUCT()
struct FBallProps
{
	GENERATED_USTRUCT_BODY()

private:
	float RollTorque;
	FString PathToMesh;
	float AngularDamping;
	float LinearDamping;
	float MassScale;
	float MaxAngularVelocity;
		
public:
	float GetRollTorque() { return RollTorque; }
	FString GetPathToMesh() { return PathToMesh; }
	float GetAngularDamping() { return AngularDamping; }
	float GetLinearDamping() { return LinearDamping; }
	float GetMassScale () { return MassScale; }
	float GetMaxAngularVelocity() { return MaxAngularVelocity; }
	FBallProps(){}
	FBallProps(EBallType type)
	{
		switch (type)
		{
		case EBallType::BE_Paper:
			PathToMesh = FString("/Game/Meshes/CrumpledPaper.CrumpledPaper");
			AngularDamping = 0.5f;
			LinearDamping = 0.1f;
			MassScale = 0.5f;
			MaxAngularVelocity =500.0f;
			RollTorque = 750000.0f;
			break;
		case EBallType::BE_Rock:
			PathToMesh = FString("/Game/Meshes/Rock.Rock");
			AngularDamping = 0.5f;
			LinearDamping = 0.1f;
			MassScale = 5.0f;
			MaxAngularVelocity = 250.0f;
			RollTorque = 6000000.0f;
			break;
		case EBallType::BE_Iron:
			PathToMesh = FString("/Game/Meshes/Iron.Iron");
			AngularDamping = 0.5f;
			LinearDamping = 0.1f;
			MassScale = 50.0f;
			MaxAngularVelocity = 250.0f;
			RollTorque = 33000000.0f;
			break;
		case EBallType::BE_Wood:
			PathToMesh = FString("/Game/Meshes/Wood.Wood");
			AngularDamping = 0.5f;
			LinearDamping = 0.1f;
			MassScale = 2.0f;
			MaxAngularVelocity = 250.0f;
			RollTorque = 6000000.0f;
			break;
		}
	}

};

UCLASS()
class ABalanceBall : public APawn
{
	GENERATED_BODY()
public:
	ABalanceBall();

	void Tick(float);

	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }

	void Transform(EBallType target);

	TMap<EBallType, struct FBallProps> BallProps;

	EBallType ChosenProperties;
protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);

	void Zoom(float Val);

	void CameraUpDown(float Val);

	void CameraLeftRight(float Val);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	float RollTorque;

	float ZoomSpeed;
	UPROPERTY(Transient)
	TArray<UStaticMesh *> BallMeshes;

	FString PathToMesh;
	float AngularDamping;
	float LinearDamping;
	float MassScale;
	float MaxAngularVelocity;
private:
	void UpdateBallProps();

	float MaxZoomOut; 
	float MaxZoomIn;

	bool RotateL;
	bool RotateR;

	FVector TorqueVector;

	TMap<EBallType, int> BallsDict;
};
