// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Interfaces/Interface_Character_Weapon.h"
#include "CharacterCaine.generated.h"

UCLASS()
class COMBATANDAIBOTCPP_API ACharacterCaine : public ACharacter, public IInterface_Character_Weapon
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Interact;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Sheathe_Unsheathe;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_LBM;

	void MoveForward(const struct FInputActionValue& Value);
	void MoveRight(const struct FInputActionValue& Value);
	void Turn(const struct FInputActionValue& Value);
	void LookUp(const struct FInputActionValue& Value);
	void SprintStart();
	void SprintStop();
	void Roll();
	void Interact();
	void Sheathe_Unsheathe();
	void LBM();

protected: //Функции из интерфейса Interface_Character_Weapon
	
	//функция для обнуления переменной AActor* ActorInHand во времы выбрасывания оружия
	virtual void CLearWeaponInHand_Drop_Implementation() override;
	//Функция для обновления переменных AActor* WeaponInHand; AActor* EquippedWeaponInHips;
	virtual void SwapWeaponSlots_Implementation() override;

//Переменные для изменения скорости
protected: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Settings")
	float DefaultSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Settings")
	float SprintSpeed;

	

	bool bIsSprinting = false;




//Функции для оверлапа
	UFUNCTION()
	void OnWeaponOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Переменная, хранит в себе актора, которого задетектил персонаж
	UPROPERTY()
	AActor* DetectedActor;

	//Переменная, хранит в себе актора, которого подобрал персонаж и сейчас имеет в себе (в руках)
	UPROPERTY()
	AActor* WeaponInHand;
	//Переменная, хранит в себе оружие на поясе
	UPROPERTY()
	AActor* EquippedWeaponInHips;
	//Функция, переключает режим персонажа с не боевгого в боевой
	UFUNCTION()
	void SwitcherCharacterMode();

	//функция гетер, для передачи текущего оружия в нотифай
public: class AActor* GetCurrentWeaponInHand() const { return WeaponInHand; }
	//функция гетер, для передачи текущего оружия на поясе в нотифай
public: class AActor* GetCurrentEquippedWeaponInHips() const { return EquippedWeaponInHips; }

	  //Массив инвентарь
	  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Settings")
	  TArray<AActor*> WeaponInventory;

	



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
