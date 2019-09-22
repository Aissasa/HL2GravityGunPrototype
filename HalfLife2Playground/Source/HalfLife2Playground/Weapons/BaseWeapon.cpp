// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeapon.h"
#include "HalfLife2PlaygroundCharacter.h"

// Engine includes
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseWeapon::ABaseWeapon():
	WeaponMesh(nullptr),
	WeaponPickUpSound(nullptr),
	WeaponDropSound(nullptr),
	OwnerCharacter(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
}

void ABaseWeapon::PrimaryFire()
{

}

void ABaseWeapon::PrimaryFireReleased()
{
}

void ABaseWeapon::SecondaryFire()
{

}

void ABaseWeapon::SecondaryFireReleased()
{

}

void ABaseWeapon::PickUpWeapon()
{
	check(WeaponMesh);
	WeaponMesh->SetSimulatePhysics(false);
	SetActorEnableCollision(false);
	if (WeaponPickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponPickUpSound, GetActorLocation());
	}
}

void ABaseWeapon::DropWeapon()
{
	check(WeaponMesh);
	SetActorEnableCollision(true);
	WeaponMesh->SetSimulatePhysics(true);
	OwnerCharacter = nullptr;
	if (WeaponDropSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponDropSound, GetActorLocation());
	}
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (!OwnerCharacter)
	{
		SetActorEnableCollision(true);
		WeaponMesh->SetSimulatePhysics(true);
	}
}
