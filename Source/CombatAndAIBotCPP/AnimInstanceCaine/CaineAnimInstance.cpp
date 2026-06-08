// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstanceCaine/CaineAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCaineAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CaineCharacter = Cast<ACharacter>(TryGetPawnOwner());
}

void UCaineAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CaineCharacter || !CaineCharacter->GetCharacterMovement()) return;

	UCharacterMovementComponent* MoveComp = CaineCharacter->GetCharacterMovement();

	SpeedXY = CaineCharacter->GetVelocity().Size2D();
	bIsFalling = MoveComp->IsFalling();

	float CurrentAcceleration = MoveComp->GetCurrentAcceleration().Size2D();
	float CharacterSprintSpeed = 470.0f;

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
			SprintResetDelayTimer -= DeltaSeconds;

			
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
		IdleAfterRunTimer -= DeltaSeconds;
		if (IdleAfterRunTimer <= 0.0f)
		{
			bCanIdleAfterRun = false;
		}
	}
}