// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "MainCharacter.generated.h"

UCLASS()
class MYOWNPROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
		float RotationRate = 720.0f;

	UPROPERTY(EditAnywhere)
		float Speed = 3.0f;

	UPROPERTY(EditAnywhere)
		float MaxWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere)
		float JumpXMultiplier = 10.0f;

	UPROPERTY(EditAnywhere)
		float JumpYMultiplier = 10.0f;

	UPROPERTY(EditAnywhere)
		float JumpZ = 100.0f;

	UPROPERTY(BlueprintReadWrite)
		bool IsAttacking = false;

	UPROPERTY(BlueprintReadWrite)
		bool IsDuringAttack = false;

	UPROPERTY(BlueprintReadWrite)
		bool IsJumping = false;

	UPROPERTY(BlueprintReadWrite)
		uint8 Combo = 1;

public:
	UFUNCTION()
		void MoveForward(float AxisValue);

	UFUNCTION()
		void MoveRight(float AxisValue);

	UFUNCTION()
		void Attack();

	UFUNCTION()
		void DoJump();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayComboAnimMontage();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayJumpAnimMontage();
};
