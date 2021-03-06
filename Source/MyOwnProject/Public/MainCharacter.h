// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "HealthComponent.h"

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
	//Components
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
		USphereComponent* AttackRangeComp;

	UPROPERTY(EditAnywhere)
		UHealthComponent* HealthComponent;

	//Variables
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
		uint8 Combo = 0;

	UPROPERTY()
		bool IsLocked = false;

	UPROPERTY()
		AActor* LockedTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<AActor> TargetLockClassFilter;

	UPROPERTY()
		TArray<AActor*> TargetActors;

	UPROPERTY()
		int32 CurrentTargetIndex = 0;

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
		void PlayWeaponCombo();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayJumpAnimMontage();

	UFUNCTION()
		void LockOnTarget();

	UFUNCTION()
		void ChangeCameraRotationToFace(AActor* TargetActor);

	UFUNCTION()
		void RevertIsLocked();

	UFUNCTION()
		void SetIsLocked(bool Value);

	UFUNCTION()
		void NextTarget();

	UFUNCTION()
		void PreviousTarget();

	UFUNCTION()
		void RightThumbStickXFunction(float Value);

	UFUNCTION()
		void AddPossibleTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void RemovePossibleTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
		bool HasSameBP(AActor* FirstActor, TSubclassOf<AActor> ClassFilter);

	UFUNCTION()
		void OnHealthChanged(UHealthComponent* HealthComp, float Health, class AController* InstigatedBy);

	UFUNCTION()
		bool GetIsAttacking();

};
