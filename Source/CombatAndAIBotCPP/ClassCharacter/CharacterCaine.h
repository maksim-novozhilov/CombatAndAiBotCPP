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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_RBM;


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
	void BlockStart();
	void BlockStop();

protected: //Функции из интерфейса Interface_Character_Weapon
	
	//функция для обнуления переменной AActor* ActorInHand во времы выбрасывания оружия
	virtual void CLearWeaponInHand_Drop_Implementation() override;
	//Функция для обновления переменных AActor* WeaponInHand; AActor* EquippedWeaponInHips;
	virtual void SwapWeaponSlots_Implementation() override;
	//Функция для обновления переменной bHasWeapon (значения приходят из оружия)
	virtual void SetbHasWeapon_Implementation(bool SetbHasWeapon) override;
	//функция означает, что персонаж атакует
	virtual void SetCharIsAttacing_Implementation(bool SwitchSetCharIsAttacing) override;
	//функция перключает мовемент мод в персонаже (например во время спринт атаки)
	virtual void SetMovementMode_Implementation(bool bSwitMovementhMode) override;

protected: //Монтажи для кувырков
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Character Settings_Montages")
	class UAnimMontage* Montage_RollForward;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Character Settings_Montages")
	class UAnimMontage* Montage_RollBack;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Character Settings_Montages")
	class UAnimMontage* Montage_RollLeft;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Character Settings_Montages")
	class UAnimMontage* Montage_RollRight;

	//Переменные для изменения скорости
protected: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Settings")
	float DefaultSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Settings")
	float SprintSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Settings")
	float CombatWalkingSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Settings")
	float CombatSprintSpeed;

	bool bIsSprinting = false;

private:
	//Тукущая скорость
	UPROPERTY()
	float CurrentSpeedXY;



protected:
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
	void SwitcherCharacterCameraModeAndSpeed();
	//Булевая переменная, означает что в руках есть оружие,  (нужна для переключения режимов ходьбы)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Settings")
	bool bHasWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Settings")
	bool bCharIsAttacing;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Settings")
	bool bCharIsBlocking = false;
	//Функция, которая на wasd направляет персонажа во время атаки
	UFUNCTION()
	void UpdateRotateDirection(float DeltaTime);

	//функция гетер, для передачи текущего оружия в нотифай
public: class AActor* GetCurrentWeaponInHand() const { return WeaponInHand; }
	//функция гетер, для передачи текущего оружия на поясе в нотифай
public: class AActor* GetCurrentEquippedWeaponInHips() const { return EquippedWeaponInHips; }
	//Функция гетер, для передачи значения SprintSpeed
public: float GetSprintSpeed() const { return SprintSpeed; }
	  //Функция гетер, для передачи значения bHasWeapon
public: bool GetbHasWeapon() const { return bHasWeapon; }
	  //Функция гетер, для передачи значения CharIsAttacing
public: bool GetbCharIsAttacingn() const { return bCharIsAttacing; }
	  //Функция гетер, для передачи значения bCharIsBlocking
public: bool GetbCharIsBlocking() const { return bCharIsBlocking; }

	
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
