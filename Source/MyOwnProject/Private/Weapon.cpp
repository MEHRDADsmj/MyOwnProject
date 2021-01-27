// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::DoDamage);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::DoDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* Character = Cast<AMainCharacter>(GetOwner());
	if (Character)
	{
		if (Character->GetIsAttacking())
		{
			AMainCharacter* ActorToAttack = Cast<AMainCharacter>(OtherActor);
			if (ActorToAttack)
			{
				return;
			}
			else
			{
				UGameplayStatics::ApplyDamage(OtherActor, Damage, GetOwner()->GetInstigatorController(), this, DamageType);
			}
		}
	}
}
