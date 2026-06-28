// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "CaineAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class COMBATANDAIBOTCPP_API UCaineAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	
	class ACharacterCaine* CaineCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpeedXY")
	float SpeedXY;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpeedXY")
	float SpeedXYCombatWalking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpeedXY")
	float DirectionXY;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bHasWeapon")
	bool bHasWeapon;*/


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "bIsFalling")
	bool bIsFalling;

	UFUNCTION()
	void IdleAfterRun();

	//virtual void SetbHasWeapon_Implementation(bool SwitchbHasWeapon) override;//Функция переключает bool bHasWeapon по нотифаю

	//Переменная для разрешения перехода к состоянию айдл после бега
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bCanIdleAfterRun")
	bool bCanIdleAfterRun;

	//Переменная для управления длительностью айдла после бега
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timer Idle After Run")
	float TimeForIdleAfterRun = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bWeaponInHand")
	bool bWeaponInHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bIsAttaking")
	bool bIsAttaking;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bIsBlocking")
	bool bIsBlocking;



private:

	//Приватная переменная, будет копировать значение из float TimeForIdleAfterRun
	float IdleAfterRunTimer;

	//Приватная переменная, которая помнит, что в пред. кадре перс. бежал
	bool bWasSprintingLastFrame = false;

	float SprintResetDelayTimer = 0.0f;

	float CharacterSprintSpeed;

	class UCharacterMovementComponent* MoveComp;

	float DeltaTime;

private:
	FVector SmoothedVelocity = FVector::ZeroVector;
	
};
