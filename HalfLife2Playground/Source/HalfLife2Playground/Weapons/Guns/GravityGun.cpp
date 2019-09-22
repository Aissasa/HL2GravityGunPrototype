// Fill out your copyright notice in the Description page of Project Settings.

#include "GravityGun.h"
#include "HalfLife2PlaygroundCharacter.h"
#include "Camera/CameraComponent.h"

// Engine Includes
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ArrowComponent.h"
#include "Engine/Engine.h"
#include "PhysicsEngine/RadialForceActor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"


AGravityGun::AGravityGun() :
	PrimaryFireAnimation(nullptr),
	ObjectGrabAnimation(nullptr),
	ObjectLaunchAnimation(nullptr),
	ObjectDropAnimation(nullptr),
	ObjectPullAnimation(nullptr),
	PrimaryFireSound(nullptr),
	ObjectGrabSound(nullptr),
	ObjectLaunchSound(nullptr),
	ObjectDropSound(nullptr),
	ObjectPullSound(nullptr),
	ExplosionParticleSystem(nullptr),
	NoHitScanMode(true),
	PrimaryFireSphereTraceRadius(20.f),
	PrimaryFireShootingRange(2000.f),
	ApplyExplosionAnyway(false),
	ExplosionRadius(250.f),
	ExplosionImpulseFallOffMode(ERadialImpulseFalloff::RIF_Linear),
	ExplosionImpulseStrengthBase(1000.f),
	ExplosionImpulseStrengthMultiplier(300.f),
	ObjectGrabRange(1000.f),
	ObjectGrabWidenessRange(20.f),
	ObjectPullRange(500.f),
	ObjectPullWidenessRange(20.f),
	ObjectPullImpulse(50.f),
	ObjectLaunchImpulse(5000.f),
	AccountForObjectMass(true),
	ObjectToGrabMassLimit(400.f),
	PhysicsHandleComponent(nullptr),
	GrabbedObjectLocation(nullptr),
	GrabbedObject(nullptr),
	ObjectsBeingPulled(),
	IsSecondaryFireStillPressed(false)
{
	PrimaryActorTick.bCanEverTick = true;

	ExplosionImpulseStrength = ExplosionImpulseStrengthBase * ExplosionImpulseStrengthMultiplier;

	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));
	PhysicsHandleComponent->RegisterComponent();

	GrabbedObjectLocation = CreateDefaultSubobject<USceneComponent>(TEXT("GrabbedObjectLocation"));
	GrabbedObjectLocation->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(MuzzleLocation);
}

void AGravityGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsGrabbingAnObject())
	{
		PhysicsHandleComponent->SetTargetLocationAndRotation(GrabbedObjectLocation->GetComponentLocation(), GrabbedObjectLocation->GetComponentRotation());
		return;
	}

	if (IsSecondaryFireStillPressed)
	{
		// this will keep looking for objects to grab or pull while holding right click.
		SecondaryFire();

		if (ObjectsBeingPulled.Num() > 0 && OwnerCharacter->IsValidLowLevel())
		{
			for (UPrimitiveComponent* component : ObjectsBeingPulled)
			{
				if (component->IsValidLowLevel() && component->GetOwner()->IsValidLowLevel() && component->GetOwner()->WasRecentlyRendered())
				{
					FVector direction = OwnerCharacter->GetActorLocation() - component->GetComponentLocation();
					direction.Normalize();
					component->AddImpulse(direction * ObjectPullImpulse, TEXT(""), true);
				}
			}
		}
	}
}

void AGravityGun::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//Get the name of the property that was changed  
	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// We test using GET_MEMBER_NAME_CHECKED so that if someone changes the property name .
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AGravityGun, ExplosionImpulseStrengthBase) || PropertyName == GET_MEMBER_NAME_CHECKED(AGravityGun, ExplosionImpulseStrengthMultiplier))
	{
		ExplosionImpulseStrength = ExplosionImpulseStrengthBase * ExplosionImpulseStrengthMultiplier;
	}
}

void AGravityGun::DropWeapon()
{
	if (IsGrabbingAnObject())
	{
		ReleaseGrabbedObject();
	}

	Super::DropWeapon();
}

bool AGravityGun::IsGrabbingAnObject() const
{
	return GrabbedObject->IsValidLowLevel();
}

void AGravityGun::BeginPlay()
{
	Super::BeginPlay();

}

void AGravityGun::PrimaryFire()
{
	if (IsGrabbingAnObject())
	{
		LaunchGrabbedObject();
		return;
	}

	if (OwnerCharacter->IsValidLowLevel())
	{
		FVector startLocation = OwnerCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector endLocation = startLocation + OwnerCharacter->GetFirstPersonCameraComponent()->GetForwardVector() * PrimaryFireShootingRange;

		FHitResult hitOut(ForceInit);
		TArray<AActor*> toIgnore{ this, OwnerCharacter };

		ETraceTypeQuery traceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_Visibility);
		bool hit;
		if (NoHitScanMode)
		{
			hit = UKismetSystemLibrary::SphereTraceSingle(this, startLocation, endLocation, PrimaryFireSphereTraceRadius, traceTypeQuery, false, toIgnore, EDrawDebugTrace::None, hitOut, true);
		}
		else
		{
			hit = UKismetSystemLibrary::LineTraceSingle(this, startLocation, endLocation, traceTypeQuery, false, toIgnore, EDrawDebugTrace::ForDuration, hitOut, true);
		}

		if (GetWorld())
		{
			if (hit)
			{
				PlayAnAnimation(PrimaryFireAnimation);
				PlayASound(PrimaryFireSound);

				CreateExplosion(hitOut.Location);
			}
			else
			{
				if (ApplyExplosionAnyway)
				{
					PlayAnAnimation(PrimaryFireAnimation);
					PlayASound(PrimaryFireSound);

					CreateExplosion(endLocation);
				}
			}
		}
	}
}

void AGravityGun::SecondaryFire()
{
	if (IsGrabbingAnObject())
	{
		ReleaseGrabbedObject();
		return;
	}

	if (OwnerCharacter->IsValidLowLevel())
	{
		IsSecondaryFireStillPressed = true;

		FVector startLocation = OwnerCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector endLocation = startLocation + OwnerCharacter->GetFirstPersonCameraComponent()->GetForwardVector() * ObjectGrabRange;

		FHitResult hitOut(ForceInit);
		TArray<AActor*> toIgnore{ this, OwnerCharacter };

		ETraceTypeQuery traceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_Visibility);
		bool hit = UKismetSystemLibrary::SphereTraceSingle(this, startLocation, endLocation, ObjectGrabWidenessRange, traceTypeQuery, false, toIgnore, EDrawDebugTrace::None, hitOut, true);

		if (hit && hitOut.GetComponent()->IsValidLowLevel() && hitOut.GetComponent()->IsSimulatingPhysics() && (!AccountForObjectMass || AccountForObjectMass && hitOut.GetComponent()->GetMass() < ObjectToGrabMassLimit))
		{
			check(PhysicsHandleComponent);

			PlayAnAnimation(ObjectGrabAnimation);
			PlayASound(ObjectGrabSound);

			GrabbedObject = hitOut.GetComponent();
			PhysicsHandleComponent->GrabComponentAtLocationWithRotation(hitOut.GetComponent(), TEXT(""), GrabbedObject->GetComponentLocation(), GrabbedObject->GetComponentRotation());
		}
		else
		{
			// making sure it's empty
			ObjectsBeingPulled.Empty();

			endLocation += OwnerCharacter->GetFirstPersonCameraComponent()->GetForwardVector() * ObjectPullRange;
			TArray<FHitResult> hitOuts;
			traceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2); // my custom channel that added that does overlap, so I can get multiple objects.
			UKismetSystemLibrary::SphereTraceMulti(this, startLocation, endLocation, ObjectPullWidenessRange, traceTypeQuery, false, toIgnore, EDrawDebugTrace::None, hitOuts, true);

			for (FHitResult hitResult : hitOuts)
			{
				if (hitResult.GetComponent()->IsValidLowLevel() && hitResult.GetComponent()->IsSimulatingPhysics() &&
					(!AccountForObjectMass || AccountForObjectMass && hitResult.GetComponent()->GetMass() < ObjectToGrabMassLimit))
				{
					ObjectsBeingPulled.Add(hitResult.GetComponent());
				}
			}

			if (ObjectsBeingPulled.Num() > 0)
			{
				PlayAnAnimation(ObjectPullAnimation);
				PlayASound(ObjectPullSound, true);
			}
		}
	}
}

void AGravityGun::SecondaryFireReleased()
{
	IsSecondaryFireStillPressed = false;
	ObjectsBeingPulled.Empty();
}

void AGravityGun::CreateExplosion(const FVector& ExplosionLocation) const
{
	ARadialForceActor* radialForceActor = Cast<ARadialForceActor>(GetWorld()->SpawnActor(ARadialForceActor::StaticClass(), &ExplosionLocation));
	URadialForceComponent* radialForceComponent = radialForceActor->GetForceComponent();
	radialForceComponent->ImpulseStrength = ExplosionImpulseStrength;
	radialForceComponent->Radius = ExplosionRadius;
	radialForceComponent->Falloff = ExplosionImpulseFallOffMode;
	radialForceComponent->ForceStrength = 0.f;
	radialForceComponent->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // added to ignore the player
	radialForceActor->FireImpulse();
	if (ExplosionParticleSystem)
	{
		FTransform transform(radialForceActor->GetActorRotation(), radialForceActor->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleSystem, transform, true);
	}
	radialForceActor->Destroy();
}

void AGravityGun::LaunchGrabbedObject()
{
	check(GrabbedObject);
	check(PhysicsHandleComponent);
	check(GrabbedObjectLocation);

	PlayAnAnimation(ObjectLaunchAnimation);
	PlayASound(ObjectLaunchSound);

	PhysicsHandleComponent->ReleaseComponent();
	if (OwnerCharacter)
	{
		GrabbedObject->AddImpulse(OwnerCharacter->GetFirstPersonCameraComponent()->GetForwardVector() * ObjectLaunchImpulse, TEXT(""), true);
	}
	GrabbedObject = nullptr;
}

void AGravityGun::ReleaseGrabbedObject()
{
	check(GrabbedObject);
	check(PhysicsHandleComponent);

	PlayAnAnimation(ObjectDropAnimation);
	PlayASound(ObjectDropSound);

	PhysicsHandleComponent->ReleaseComponent();
	GrabbedObject = nullptr;
}

void AGravityGun::PlayAnAnimation(UAnimMontage* AnimationToPlay) const
{
	if (AnimationToPlay && OwnerCharacter->IsValidLowLevel() && OwnerCharacter->GetMesh1P())
	{
		// Get the animation object for the character arms mesh
		UAnimInstance* animInstance = OwnerCharacter->GetMesh1P()->GetAnimInstance();
		if (animInstance)
		{
			animInstance->Montage_Play(AnimationToPlay);
		}
	}
}

void AGravityGun::PlayASound(USoundBase* SoundToPlay, bool PlayOnAudioComponent) const
{
	if (SoundToPlay)
	{
		if (!PlayOnAudioComponent)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, MuzzleLocation->GetComponentLocation());
		}
		else
		{
			if (!AudioComponent->IsPlaying())
			{
				AudioComponent->SetSound(SoundToPlay);
				AudioComponent->Play();
			}
		}
	}
}
