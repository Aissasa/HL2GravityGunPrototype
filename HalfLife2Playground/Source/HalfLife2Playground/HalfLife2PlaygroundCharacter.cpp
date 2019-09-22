// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HalfLife2PlaygroundCharacter.h"
#include "Weapons/Guns/GravityGun.h"
#include "HalfLife2PlaygroundProjectile.h"

// Engine includes
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

AHalfLife2PlaygroundCharacter::AHalfLife2PlaygroundCharacter():
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	WeaponPickUpRadius(250.f),
	CreateDefaultWeaponWhenSpawned(true),
	DefaultWeaponType(nullptr),
	EquippedWeapon(nullptr)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

void AHalfLife2PlaygroundCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CreateDefaultWeaponWhenSpawned && !EquippedWeapon->IsValidLowLevel() && GetWorld())
	{
		EquippedWeapon = Cast<ABaseWeapon>(GetWorld()->SpawnActor(DefaultWeaponType));
		if (EquippedWeapon->IsValidLowLevel())
		{
			EquippedWeapon->SetOwnerCharacter(this);
			EquippedWeapon->PickUpWeapon();
			EquippedWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
	}
}

void AHalfLife2PlaygroundCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire events
	PlayerInputComponent->BindAction("PrimaryFire", IE_Pressed, this, &AHalfLife2PlaygroundCharacter::OnPrimaryFirePressed);
	PlayerInputComponent->BindAction("PrimaryFire", IE_Released, this, &AHalfLife2PlaygroundCharacter::OnPrimaryFireReleased);
	PlayerInputComponent->BindAction("SecondaryFire", IE_Pressed, this, &AHalfLife2PlaygroundCharacter::OnSecondaryFirePressed);
	PlayerInputComponent->BindAction("SecondaryFire", IE_Released, this, &AHalfLife2PlaygroundCharacter::OnSecondaryFireReleased);
	PlayerInputComponent->BindAction("CollectOrDropWeapon", IE_Pressed, this, &AHalfLife2PlaygroundCharacter::OnPickUpOrDropWeapon);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AHalfLife2PlaygroundCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHalfLife2PlaygroundCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHalfLife2PlaygroundCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHalfLife2PlaygroundCharacter::LookUpAtRate);
}

void AHalfLife2PlaygroundCharacter::OnPrimaryFirePressed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->PrimaryFire();
	}
}

void AHalfLife2PlaygroundCharacter::OnPrimaryFireReleased()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->PrimaryFireReleased();
	}
}

void AHalfLife2PlaygroundCharacter::OnSecondaryFirePressed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SecondaryFire();
	}
}

void AHalfLife2PlaygroundCharacter::OnSecondaryFireReleased()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->SecondaryFireReleased();
	}
}

void AHalfLife2PlaygroundCharacter::OnPickUpOrDropWeapon()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->DropWeapon();
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedWeapon = nullptr;
	}
	else
	{
		FVector startLocation = GetActorLocation();
		FVector endLocation = startLocation;

		FHitResult hitOut(ForceInit);
		TArray<AActor*> toIgnore{ this };

		ETraceTypeQuery traceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4); // weapon trace channel
		bool hit = UKismetSystemLibrary::SphereTraceSingle(this, startLocation, endLocation, WeaponPickUpRadius, traceTypeQuery, false, toIgnore, EDrawDebugTrace::None, hitOut, true);
		if (hit)
		{
			ABaseWeapon* weapon = Cast<ABaseWeapon>(hitOut.GetActor());
			if (weapon)
			{
				weapon->SetOwnerCharacter(this);
				weapon->PickUpWeapon();
				weapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
				EquippedWeapon = weapon;
			}
		}
	}
}

void AHalfLife2PlaygroundCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AHalfLife2PlaygroundCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AHalfLife2PlaygroundCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHalfLife2PlaygroundCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
