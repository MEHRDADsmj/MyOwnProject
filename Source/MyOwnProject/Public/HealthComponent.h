// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, Health, class AController*, InstigatedBy);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYOWNPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere)
		float CurrentHealth;

	UPROPERTY(EditAnywhere)
		float DefaultHealth = 100.0f;

public:
	UFUNCTION(BlueprintCallable)
		float GetHealth();

	UFUNCTION()
		void HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY()
		FOnHealthChangedSignature OnHealthChanged;

};
