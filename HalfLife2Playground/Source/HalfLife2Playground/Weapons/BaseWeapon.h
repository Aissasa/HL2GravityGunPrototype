// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BaseWeapon.generated.h"

// Forward declarations
class AHalfLife2PlaygroundCharacter;
class UAnimMontage;
class USoundBase;

/** This class represents a base for the weapons in the game.
*/
UCLASS()
class HALFLIFE2PLAYGROUND_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:

	/** Constructor.
	*/
	ABaseWeapon();

	/** Weapon primary fire behavior.
	*/
	virtual void PrimaryFire();

	/** Weapon primary fire released behavior.
	*/
	virtual void PrimaryFireReleased();

	/** Weapon secondary fire behavior.
	*/
	virtual void SecondaryFire();

	/** Weapon secondary fire released behavior.
	*/
	virtual void SecondaryFireReleased();

	/** Weapon Pick up behavior.
	*/
	virtual void PickUpWeapon();

	/** Weapon Drop behavior.
	*/
	virtual void DropWeapon();

	/** Setter for the character that is holding the weapon.
	*/
	FORCEINLINE void SetOwnerCharacter(AHalfLife2PlaygroundCharacter* TheOwnerCharacter) { OwnerCharacter = TheOwnerCharacter; }

	/** Weapon Mesh.
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	class USkeletalMeshComponent* WeaponMesh;

protected:

	/** Begin play event.
	*/
	virtual void BeginPlay() override;

	/** Sound to play when the weapon is picked up.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundBase* WeaponPickUpSound;

	/** Sound to play when the weapon is dropped.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundBase* WeaponDropSound;

	/** A pointer to the character that is holding the weapon. This is nullptr when the weapon is dropped.
	*/
	AHalfLife2PlaygroundCharacter* OwnerCharacter;

};
