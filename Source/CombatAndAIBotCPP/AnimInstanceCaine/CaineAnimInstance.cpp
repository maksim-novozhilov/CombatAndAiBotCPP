// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstanceCaine/CaineAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ClassCharacter/CharacterCaine.h"


void UCaineAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CaineCharacter = Cast<ACharacterCaine>(TryGetPawnOwner());
	
}


void UCaineAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CaineCharacter || !CaineCharacter->GetCharacterMovement()) return;

	DeltaTime = DeltaSeconds;
	
	MoveComp = CaineCharacter->GetCharacterMovement();

	//Получаем доступ к текущей скорости спринта
	CharacterSprintSpeed = CaineCharacter ? CaineCharacter->GetSprintSpeed() : 0.f;
	
	
	//OLD (Направление персонажа для блендспейса)
	/*//Скорость персонажа
	SpeedXY = CaineCharacter->GetVelocity().Size2D();
	//Направление персонажа
	FVector Velocity = CaineCharacter->GetVelocity();
	FRotator ActorRotation = CaineCharacter->GetActorRotation();
	DirectionXY = CalculateDirection(Velocity, ActorRotation);*/

	// Направление персонажа для блендспейса (сглаженное)
	
	FVector Velocity = CaineCharacter->GetVelocity();
	SmoothedVelocity = FMath::VInterpTo(SmoothedVelocity, Velocity, DeltaSeconds, 9.0f);
	
	SpeedXYCombatWalking = SmoothedVelocity.Size2D();
	SpeedXY = CaineCharacter->GetVelocity().Size2D();
	
	FRotator ActorRotation = CaineCharacter->GetActorRotation();
	DirectionXY = CalculateDirection(SmoothedVelocity, ActorRotation);

	

	

	
	
	
	//Булевая переменная в аним инстансе, которая означает, что в руке есть оружие (для переключения в боевую походку)
	bWeaponInHand = (CaineCharacter->GetbHasWeapon() != false);
	//Булевая переменная в аним инстансе, которая означает, персонаж сейчас атакует
	bIsAttaking = (CaineCharacter->GetbCharIsAttacingn() != false);
	
	bIsFalling = MoveComp->IsFalling();

	IdleAfterRun();


}

	
	

void UCaineAnimInstance::IdleAfterRun()
{
	float CurrentAcceleration = MoveComp->GetCurrentAcceleration().Size2D();

	if (SpeedXY >= CharacterSprintSpeed - 10.0f)
	{
		bWasSprintingLastFrame = true;


		SprintResetDelayTimer = 0.70f;
	}
	else if (SpeedXY < CharacterSprintSpeed - 10.0f && CurrentAcceleration > 0.0f)
	{

		bCanIdleAfterRun = false;

		if (bWasSprintingLastFrame)
		{
			SprintResetDelayTimer -= DeltaTime;


			if (SprintResetDelayTimer <= 0.0f)
			{

				bWasSprintingLastFrame = false;
			}
		}

	}


	if (SpeedXY <= 1.0f && bWasSprintingLastFrame)
	{
		bCanIdleAfterRun = true;
		IdleAfterRunTimer = TimeForIdleAfterRun;

		bWasSprintingLastFrame = false;
	}


	if (bCanIdleAfterRun)
	{
		IdleAfterRunTimer -= DeltaTime;
		if (IdleAfterRunTimer <= 0.0f)
		{
			bCanIdleAfterRun = false;
		}
	}
}

//void UCaineAnimInstance::SetbHasWeapon_Implementation(bool SwitchbHasWeapon)
//{
//	bHasWeapon = SwitchbHasWeapon;
//}

