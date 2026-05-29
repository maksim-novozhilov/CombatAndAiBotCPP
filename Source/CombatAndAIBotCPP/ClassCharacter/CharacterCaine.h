// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "CharacterCaine.generated.h"

UCLASS()
class COMBATANDAIBOTCPP_API ACharacterCaine : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterCaine();

	
	//SETUP INPUT MAPPING CONTEXT
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void SetupInputMappingContext();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputMappingContext")
	class UInputMappingContext* CaineMappingContext;

	//INPUTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_MoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_MoveRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Turn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_LookUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Roll;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Sprint;
	

	void MoveForward(const struct FInputActionValue& Value);
	void MoveRight(const struct FInputActionValue& Value);
	void Turn(const struct FInputActionValue& Value);
	void LookUp(const struct FInputActionValue& Value);
	void SprintStart();
	void SprintStop();
	void Roll();


//Переменные для изменения скорости
protected: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Settings")
	float DefaultSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Settings")
	float SprintSpeed;

	

	bool bIsSprinting = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
