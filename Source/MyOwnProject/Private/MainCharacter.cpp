// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Attachments
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArm);

	AttackRangeComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	AttackRangeComp->SetupAttachment(RootComponent);
	AttackRangeComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	//Rotation Setup
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, RotationRate, 0.0f);

	SpringArm->bUsePawnControlRotation = true;

	CameraComp->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttackRangeComp->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::AddPossibleTarget);
	AttackRangeComp->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::RemovePossibleTarget);

	HealthComponent->OnHealthChanged.AddDynamic(this, &AMainCharacter::OnHealthChanged);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocked)
	{
		LockOnTarget();
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacter::Attack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::DoJump);
	PlayerInputComponent->BindAction("Lock", IE_Pressed, this, &AMainCharacter::RevertIsLocked);
	PlayerInputComponent->BindAction("NextTarget", IE_Pressed, this, &AMainCharacter::NextTarget);
	PlayerInputComponent->BindAction("PreviousTarget", IE_Pressed, this, &AMainCharacter::PreviousTarget);
	PlayerInputComponent->BindAxis("RightThumbStickXFunction", this, &AMainCharacter::RightThumbStickXFunction);
}

void AMainCharacter::MoveForward(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	if (IsJumping)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	else if (IsAttacking)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2;//Just to let it turn during attack
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
	if (Controller && AxisValue != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue * Speed);
	}
}

void AMainCharacter::MoveRight(float AxisValue)
{
	if (AxisValue == 0.0f)
	{
		return;
	}

	if (IsJumping)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	else if (IsAttacking)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2;//Just to let it turn during attack
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
	if (Controller && AxisValue != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue * Speed);
	}
}

void AMainCharacter::Attack()
{
	if (IsDuringAttack || IsJumping)
	{
		return;
	}

	if (!IsAttacking)
	{
		IsAttacking = true;
		//Resets combo
		Combo = 0;
	}
	IsDuringAttack = true;

	PlayWeaponCombo();
	Combo++;
}

void AMainCharacter::DoJump()
{
	if (IsJumping || IsDuringAttack)
	{
		return;
	}

	IsJumping = true;
	FRotator Rotation = GetActorRotation();
	Rotation.Normalize();
	//Gets player's movement direction
	FVector Direction = Rotation.Vector();
	Direction.X *= JumpXMultiplier;
	Direction.Y *= JumpYMultiplier;
	Direction.Z = JumpZ;
	//Launches player
	LaunchCharacter(Direction, false, false);

	PlayJumpAnimMontage();
}

void AMainCharacter::LockOnTarget()
{
	//Makes sure index is in array bound
	CurrentTargetIndex = FMath::Clamp(CurrentTargetIndex, 0, TargetActors.Num() - 1);

	if (TargetActors.Num() == 0)
	{
		//Sets everything back to normal
		LockedTarget = nullptr;
		SetIsLocked(false);
		CurrentTargetIndex = 0;
		return;
	}
	else if (LockedTarget == nullptr)
	{
		LockedTarget = TargetActors[CurrentTargetIndex];
	}

	ChangeCameraRotationToFace(LockedTarget);
}

//Turns camera so that player focuses on the input actor
void AMainCharacter::ChangeCameraRotationToFace(AActor* TargetActor)
{
	if (TargetActor == nullptr)
	{
		return;
	}
	//Gets player to target vector
	FVector PlayerLocation = GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector Direction = TargetLocation - PlayerLocation;
	//Creates a rotator from it
	FRotator NewRotation = Direction.Rotation();

	Controller->SetControlRotation(NewRotation);
}

void AMainCharacter::RevertIsLocked()
{
	IsLocked = !IsLocked;
}

void AMainCharacter::SetIsLocked(bool Value)
{
	IsLocked = Value;
}

//Increments target actors array's index
void AMainCharacter::NextTarget()
{
	LockedTarget = nullptr;
	CurrentTargetIndex++;
}

//Decrements target actors array's index
void AMainCharacter::PreviousTarget()
{
	LockedTarget = nullptr;
	CurrentTargetIndex--;
}

//Decides whether right thumb stick X axis should rotate camera or change target
void AMainCharacter::RightThumbStickXFunction(float Value)
{
	if (Value == 0.0f)
	{
		return;
	}

	if (IsLocked == true)
	{
		if (Value > 0)
		{
			NextTarget();
		}
		else
		{
			PreviousTarget();
		}
	}
	else
	{
		AddControllerYawInput(Value);
	}
}

void AMainCharacter::AddPossibleTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasSameBP(OtherActor, TargetLockClassFilter))
	{
		TargetActors.AddUnique(OtherActor);
	}
}

void AMainCharacter::RemovePossibleTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasSameBP(OtherActor, TargetLockClassFilter))
	{
		TargetActors.Remove(OtherActor);
	}
}

void AMainCharacter::OnHealthChanged(UHealthComponent* HealthComp, float Health, class AController* InstigatedBy)
{
	if (Health <= 0.0f)
	{
		Destroy();
	}
}

bool AMainCharacter::GetIsAttacking()
{
	return IsAttacking;
}
