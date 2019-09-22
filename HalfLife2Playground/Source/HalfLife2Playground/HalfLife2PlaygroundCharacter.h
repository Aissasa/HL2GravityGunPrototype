// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

// Engine include
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "HalfLife2PlaygroundCharacter.generated.h"

// Forward declarations
class UCameraComponent;
class ABaseWeapon;

UCLASS(config=Game)
class AHalfLife2PlaygroundCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	/** Constructor.
	*/
	AHalfLife2PlaygroundCharacter();

	/** Mesh1P inline getter. 
	*/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	/** FirstPersonCameraComponent inline getter. 
	*/
	FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:

	/** Begin play event.
	*/
	virtual void BeginPlay() override;

	/** Allows a Pawn to set up custom input bindings. Called upon possession by a PlayerController, using the InputComponent created by CreatePlayerInputComponent(). 
	*	@param InputComponent The InputComponent to use for bindings.
	*/
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** Handles moving forward/backward.
	*	@param Val This is a normalized value of the movement on the Y axis, i.e. 1.0 means 100% of desired forward movement.
	*/
	void MoveForward(float Val);

	/** Handles lateral movement, left and right.
	*	@param Val This is a normalized value of the movement on the X axis, i.e. 1.0 means 100% of desired right movement.
	*/
	void MoveRight(float Val);

	/** Called via input to turn at a given rate.
	*	@param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate.
	*/
	void TurnAtRate(float Rate);

	/** Called via input to turn look up/down at a given rate.
	*	@param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate.
	*/
	void LookUpAtRate(float Rate);
	
	/** The PrimaryFire pressed behavior. 
	*/
	void OnPrimaryFirePressed();

	/** The PrimaryFire Released behavior.
	*/
	void OnPrimaryFireReleased();

	/** The SecondaryFire pressed behavior.
	*/
	void OnSecondaryFirePressed();

	/** The SecondaryFire released behavior.
	*/
	void OnSecondaryFireReleased();

	/** The PickUp/Drop weapon behavior.
	*/
	void OnPickUpOrDropWeapon();

	/** Pawn mesh: 1st person view (arms; seen only by self)
	*/
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** First person camera .
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** The radius in which the player is allowed to pickup the weapon.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (ClampMin = 50.f, ClampMax = 1000.f))
	float WeaponPickUpRadius;

	/** Determines whether to spawn a default weapon when the character is spawned or not.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	bool CreateDefaultWeaponWhenSpawned;

	/** Represents the default weapon to equip. This is unlocked only when CreateDefaultWeaponWhenSpawned is true.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon, meta = (EditCondition = CreateDefaultWeaponWhenSpawned))
	TSubclassOf<ABaseWeapon> DefaultWeaponType;

	/** Represents the currently equipped weapon.
	*/
	UPROPERTY()
	ABaseWeapon* EquippedWeapon;

};

