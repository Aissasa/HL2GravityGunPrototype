// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseGun.h"

// Engine includes
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

#include "GravityGun.generated.h"

/** This class represents the gravity gun. It encapsulates its components and behaviors.
*	@see ABaseGun.
*/
UCLASS()
class HALFLIFE2PLAYGROUND_API AGravityGun : public ABaseGun
{
	GENERATED_BODY()
	
public:	

	/** Constructor.
	*/
	AGravityGun();

	/** Tick event.
	*/
	virtual void Tick(float DeltaTime) override;

	/** Updates the ExplosionImpulseStrength in the editor when ExplosionImpulseStrengthBase or ExplosionImpulseStrengthMultiplier is changed.
	*/
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/** Weapon Drop behavior.
	*/
	virtual void DropWeapon() override;

	/** Indicates if the gun has a grabbed object or not.
	*/
	bool IsGrabbingAnObject() const;

protected:

	/** Begin play event.
	*/
	virtual void BeginPlay() override;

	/** Gravity Gun primary fire behavior.
	*/
	virtual void PrimaryFire() override;

	/** Gravity Gun secondary fire behavior.
	*/
	virtual void SecondaryFire() override;

	/** Gravity Gun secondary fire released behavior.
	*/
	virtual void SecondaryFireReleased() override;

	/** AnimMontage to play each time we use PrimaryFire.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrimaryFire|Animation")
	UAnimMontage* PrimaryFireAnimation;

	/** AnimMontage to play each time we grab an object.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SecondaryFire|Animation")
	UAnimMontage* ObjectGrabAnimation;

	/** AnimMontage to play each time we launch an object.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SecondaryFire|Animation")
	UAnimMontage* ObjectLaunchAnimation;

	/** AnimMontage to play each time we drop an object.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SecondaryFire|Animation")
	UAnimMontage* ObjectDropAnimation;

	/** AnimMontage to play each time we pull objects.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SecondaryFire|Animation")
	UAnimMontage* ObjectPullAnimation;

	/** Sound to play each time we use PrimaryFire.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrimaryFire|Audio")
	USoundBase* PrimaryFireSound;

	/** Sound to play each time we grab an object.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SecondaryFire|Audio")
	USoundBase* ObjectGrabSound;

	/** Sound to play each time we launch an object.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SecondaryFire|Audio")
	USoundBase* ObjectLaunchSound;

	/** Sound to play each time we drop an object.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SecondaryFire|Audio")
	USoundBase* ObjectDropSound;

	/** Sound to play each time we pull objects.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SecondaryFire|Audio")
	USoundBase* ObjectPullSound;

	/** Particle system that spawns where the primary fire explosion happens.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrimaryFire|Particles")
	UParticleSystem* ExplosionParticleSystem;

	/** Represents if the primary fire should use hitscan to decide the location of the explosions or not.
	*	If it's not hitscan, a sphere trace will be applied, and this will unlock the PrimaryFireSphereTraceRadius property.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PrimaryFire|Shooting")
	bool NoHitScanMode;

	/** Represents the radius of the sphere trace done when firing. The bigger the radius, the wider the fire range is.
	*	Unlocked if the PrimaryFireHitScanShootingMode is off.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PrimaryFire|Shooting", meta =(ClampMin = 1.f, ClampMax = 100.f, EditCondition = NoHitScanMode))
	float PrimaryFireSphereTraceRadius;

	/** Represents the primary fire shooting range.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PrimaryFire|Shooting", meta = (ClampMin = 100.f, ClampMax = 10000.f))
	float PrimaryFireShootingRange;

	/** Represents if the primary fire should trigger an explosion even if no object was in range.
	*	The explosion will be triggered where the Shooting range would reach.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PrimaryFire|Shooting")
	bool ApplyExplosionAnyway;

	/** Represents the primary fire explosion radius.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PrimaryFire|Explosion", meta = (ClampMin = 100.f, ClampMax = 5000.f))
	float ExplosionRadius;

	/** Represents the radial force fall off mode.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PrimaryFire|Explosion", meta = (ClampMin = 100.f, ClampMax = 5000.f))
	TEnumAsByte<enum ERadialImpulseFalloff> ExplosionImpulseFallOffMode;

	/** Represents the base strength of the radial force applied during the primary fire explosion.
	*	This will be multiplied with the ExplosionImpulseStrengthMultiplier to give the final value of the Explosion radial force strength, called ExplosionImpulseStrength.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrimaryFire|Explosion", meta = (ClampMin = 100.f, ClampMax = 5000.f))
	float ExplosionImpulseStrengthBase;

	/** Represents the explosion radial force multiplier .
	*	This will be multiplied with the ExplosionImpulseStrengthBase to give the final value of the Explosion radial force strength, called ExplosionImpulseStrength.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PrimaryFire|Explosion", meta = (ClampMin = 0.f, ClampMax = 1000.f))
	float ExplosionImpulseStrengthMultiplier;

	/** Represents the final value of the radial force applied during the primary fire explosion.
	*	This is the result of the multiplication of ExplosionImpulseStrengthBase and ExplosionImpulseStrengthMultiplier.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PrimaryFire|Explosion")
	float ExplosionImpulseStrength;

	/** Represents the object grab range.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SecondaryFire|ObjectGrab", meta = (ClampMin = 100.f, ClampMax = 5000.f))
	float ObjectGrabRange;

	/** Represents the object grab wideness range.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SecondaryFire|ObjectGrab", meta = (ClampMin = 1.f, ClampMax = 1000.f))
	float ObjectGrabWidenessRange;

	/** Represents the object pull range. This is the additional distance added to the ObjectGrabRange, to find objects to pull when the grab fails.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SecondaryFire|ObjectPull", meta = (ClampMin = 100.f, ClampMax = 5000.f))
	float ObjectPullRange;

	/** Represents the object pull wideness range.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SecondaryFire|ObjectPull", meta = (ClampMin = 1.f, ClampMax = 1000.f))
	float ObjectPullWidenessRange;

	/** Represents the impulse used to pull objects that are out of the grab reach.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SecondaryFire|ObjectPull", meta = (ClampMin = 1.f, ClampMax = 500.f))
	float ObjectPullImpulse;

	/** Represents the object pull wideness range.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SecondaryFire", meta = (ClampMin = 100.f, ClampMax = 80000.f))
	float ObjectLaunchImpulse;

	/** Determines if the object mass should be accounted for when grabbing or pulling an object.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SecondaryFire|ObjectGrab")
	bool AccountForObjectMass;

	/** Represents the mass limit allowed to grab or pull an object. Accounted for only when AccountForObjectMass is true.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SecondaryFire|ObjectGrab", meta = (ClampMin = 50.f, ClampMax = 5000.f, EditCondition = AccountForObjectMass))
	float ObjectToGrabMassLimit;

	/** The Physics handle component that would allow us to grab objects with the secondary fire.
	*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "SecondaryFire")
	class UPhysicsHandleComponent* PhysicsHandleComponent;

	/** The location where the grabbed object would hover around. Made as scene component to allow easier customization.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SecondaryFire")
	USceneComponent* GrabbedObjectLocation;

	/** The grabbed object component.
	*/
	UPrimitiveComponent* GrabbedObject;

	/** Array of the objects being pulled to the player by the secondary fire.
	*/
	TArray<UPrimitiveComponent*> ObjectsBeingPulled;

	/** Audio component that handles recurring audio.
	*/
	class UAudioComponent* AudioComponent;

private:

	/** Handles the creation of the primary fire explosion.
	*/
	void CreateExplosion(const FVector& ExplosionLocation) const;

	/** Launches the grabbed object forward.
	*/
	void LaunchGrabbedObject();

	/** Releases the grabbed object.
	*/
	void ReleaseGrabbedObject();

	/** Play the passed animation.
	*/
	void PlayAnAnimation(UAnimMontage* AnimationToPlay) const;

	/** Play the passed sound.
	*/
	void PlayASound(USoundBase* SoundToPlay, bool PlayOnAudioComponent = false) const;

	/** Represents if the secondary fire input is still pressed.
	*/
	bool IsSecondaryFireStillPressed;

};
