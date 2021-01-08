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
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
}

void AMainCharacter::MoveForward(float AxisValue)
{
	if (IsJumping)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	else if (IsAttacking)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2;
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
	if (IsJumping)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	else if (IsAttacking)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2;
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
	FVector Direction = Rotation.Vector();
	Direction.X *= JumpXMultiplier;
	Direction.Y *= JumpYMultiplier;
	Direction.Z = JumpZ;
	LaunchCharacter(Direction, false, false);
	
	PlayJumpAnimMontage();
}
