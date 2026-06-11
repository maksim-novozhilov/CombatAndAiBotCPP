// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassWeaponBase/ClassWeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

// Sets default values
AClassWeaponBase::AClassWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	CollisionBox->SetupAttachment(WeaponMesh);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	CollisionSphere->SetupAttachment(WeaponMesh);

}



// Called when the game starts or when spawned
void AClassWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClassWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AClassWeaponBase::FlyToHand_Implementation(USkeletalMeshComponent* MeshCaine)
{
	
	if (MeshCaine)
	{
		GetCharacterMesh = MeshCaine;

		if (GetCharacterMesh)
		{
			GetWorldTimerManager().SetTimer(FlyWeaponTimerHandle, this, &AClassWeaponBase::FlyWeaponTick, 0.001f, true);//Запускаем таймер

			CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
			WeaponMesh->SetSimulatePhysics(false);

			if (PickUpMontage)
			{
				UAnimInstance* AnimInstance = GetCharacterMesh->GetAnimInstance();

				AnimInstance->Montage_Play(PickUpMontage, 1.0f);
				
			}
		}
	}
}



void AClassWeaponBase::FlyWeaponTick()
{
	
	//Координаты сокетв на руке
	FVector TargetLoc = GetCharacterMesh->GetSocketLocation(HandSocketSword);
	FRotator TargetRot = GetCharacterMesh->GetSocketRotation(HandSocketSword);

	FVector WeaponLocation = GetActorLocation();

	float Distance = FVector::Dist(TargetLoc, WeaponLocation);

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	if (Distance > AcceptableDistance)
	{
		
		
		CurrentFlySpeed += AccelerationRate * DeltaTime;

		FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLoc, DeltaTime, CurrentFlySpeed);

		FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRot, DeltaTime, CurrentFlySpeed);

		SetActorLocation(NewLocation);
		SetActorRotation(NewRotation);
	}

	else
	{
		//тут защелкиваем оружие в руке
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		this->AttachToComponent(GetCharacterMesh, AttachRules, HandSocketSword);

		GetWorldTimerManager().ClearTimer(FlyWeaponTimerHandle);//Выключаем таймер

		CurrentFlySpeed = 0.f;// обнуляем переменную скорости при защелкивании, что бы она не накаплмвалась

	}

}

void AClassWeaponBase::DropWeapon_Implementation(USkeletalMeshComponent* MeshCaine)
{
	if (MeshCaine)
	{
		GetCharacterMesh = MeshCaine;

		if (DropWeaponMontage)
		{
			UAnimInstance* AnimInstance = GetCharacterMesh->GetAnimInstance();

			AnimInstance->Montage_Play(DropWeaponMontage, 1.f);

			ResetIsAttaking_Implementation();
		}
	}
}

void AClassWeaponBase::DetachWeapon_Implementation()
{
	
	//открепляем оружие
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	this->DetachFromActor(DetachRules);

	if (WeaponMesh)
	{
		
		WeaponMesh->SetSimulatePhysics(true);
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CollisionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);

		
		WeaponMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		WeaponMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

		
		if (GetCharacterMesh)
		{

			FVector Forward = GetCharacterMesh->GetForwardVector();
			FVector Right = GetCharacterMesh->GetRightVector();
			FVector Up = GetCharacterMesh->GetUpVector();

			//ипульс перемещения
			FVector CustomImpulse = (Forward * (-60.0f)) + (Right * (-300.0f)) + (Up * (-200.0f));

			WeaponMesh->AddImpulse(CustomImpulse, NAME_None, true);
			
			//ипсульс вращения
			FVector RotationAxis = (Forward * -100.0f) + (Right * -600.0f);

			WeaponMesh->AddAngularImpulseInDegrees(RotationAxis, NAME_None, true);
			
		}
			
		GetCharacterMesh = nullptr;
	}

}

void AClassWeaponBase::PlayMontagesSword_Sheathe_Implementation(USkeletalMeshComponent* MeshCaine)
{
	if (MeshCaine)
	{
		GetCharacterMesh = MeshCaine;

		UAnimInstance* AnimInstance = GetCharacterMesh->GetAnimInstance();

		AnimInstance->Montage_Play(SheatheWeaponMontage, 1.f);

		ResetIsAttaking_Implementation();

		
	}
}

void AClassWeaponBase::PlayMontagesSword_Unsheathe_Implementation(USkeletalMeshComponent* MeshCaine)
{
	if (MeshCaine)
	{
		GetCharacterMesh = MeshCaine;

		UAnimInstance* AnimInstance = GetCharacterMesh->GetAnimInstance();

		AnimInstance->Montage_Play(UnsheatheWeaponMontage, 1.f);

	}
}

void AClassWeaponBase::AttachWeaponToHips_Implementation()
{
	if (GetCharacterMesh)
	{
		//тут защелкиваем оружие на поясе
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		this->AttachToComponent(GetCharacterMesh, AttachRules, HipsSocketSword);

		
		GetCharacterMesh = nullptr;

		
		
	}
}

void AClassWeaponBase::DetachWeaponToHips_Implementation()
{
	if (GetCharacterMesh)
	{
		//тут защелкиваем оружие на руке
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		this->AttachToComponent(GetCharacterMesh, AttachRules, HandSocketSword);


	}
}


//-----------------------ATTACK----------------------------------------------------------------------------------------------------------------------

void AClassWeaponBase::Attack_Implementation()
{
	if (GetCharacterMesh)
	{
		UAnimInstance* AnimInstance = GetCharacterMesh->GetAnimInstance();

		if (!bIsAttaking)
		{

			bIsAttaking = true;
			AttackIndex = 0;

			AnimInstance->Montage_Play(AttackMontages[AttackIndex], 1.f);

			AttackIndex++;
			return;

		}

		if (bIsAttaking && bCanNextAttack)
		{
			
			if (AttackIndex >= AttackMontages.Num())
			{
				AttackIndex = ResetAttackIndex;
			}
			
			AnimInstance->Montage_Play(AttackMontages[AttackIndex], 1.f);
			
			AttackIndex++;
			bCanNextAttack = false;
				
		}

	}
}


void AClassWeaponBase::CanNextAttack_Implementation(bool bSwitchCanNextAttack)
{
	bCanNextAttack = bSwitchCanNextAttack;
}

void AClassWeaponBase::ResetIsAttaking_Implementation()
{
	bIsAttaking = false;

	AttackIndex = 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------

	

			

		

			
		
