// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"

#include "Weapon.generated.h"

UCLASS()
class MYOWNPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
		UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
		float Damage = 100.0f;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* MeshComp;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void PlayComboAnimMontage();

};
