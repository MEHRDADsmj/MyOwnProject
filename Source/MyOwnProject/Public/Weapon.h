// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "MainCharacter.h"
#include "kismet/GameplayStatics.h"

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
		USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
		float Damage = 100.0f;

	UPROPERTY()
		TSubclassOf<UDamageType> DamageType;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void PlayComboAnimMontage();

	UFUNCTION()
		void DoDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
