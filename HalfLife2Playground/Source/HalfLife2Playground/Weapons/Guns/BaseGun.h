// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapons/BaseWeapon.h"

// Engine includes
#include "CoreMinimal.h"

#include "BaseGun.generated.h"

/** This class represents a base for the guns in the game.
*	@see ABaseWeapon.
*/
UCLASS()
class HALFLIFE2PLAYGROUND_API ABaseGun : public ABaseWeapon
{
	GENERATED_BODY()
	
public:	

	/** Constructor.
	*/
	ABaseGun();

protected:

	/** Muzzle location on the gun.
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
	class UArrowComponent* MuzzleLocation;

};
