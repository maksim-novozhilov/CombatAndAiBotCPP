// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimMontage.h"
#include "Interfaces/Interface_Character_Weapon.h"
#include "ClassWeaponBase.generated.h"

UCLASS()
class COMBATANDAIBOTCPP_API AClassWeaponBase : public AActor, public IInterface_Character_Weapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClassWeaponBase();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSetings")
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSetings")
	class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSetings")
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponSetings")
	float DamageWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetings")
	TArray<UAnimMontage*> AttackMontages;
	
	//Монтаж взятия оружия (меч)
	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetings")
	UAnimMontage* PickUpMontage;
	
	//Монтаж выбрасывания оружия (меч)
	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetings")
	UAnimMontage* DropWeaponMontage;
	
	//Монтаж прятанья оружия (меч)
	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetings")
	UAnimMontage* SheatheWeaponMontage;
	
	//Монтаж обнажения оружия (меч)
	UPROPERTY(EditDefaultsOnly, Category = "WeaponSetings")
	UAnimMontage* UnsheatheWeaponMontage;

	

protected:
	//Функции из интерфейса Interface_Character_Weapon
	virtual void FlyToHand_Implementation(USkeletalMeshComponent* MeshCaine) override;//функция для полета оружи в руку, запускается из персонажа из кнопки интерект
	virtual void DropWeapon_Implementation(USkeletalMeshComponent* MeshCaine) override;//Функция для выбрасывания оружия
	virtual void DetachWeapon_Implementation() override;//функция, которая разрешает открепить оружие от руки
	virtual void PlayMontagesSword_Sheathe_Implementation(USkeletalMeshComponent* MeshCaine) override;//функция для проигрываня монтажей Sheathe меча
	virtual void PlayMontagesSword_Unsheathe_Implementation(USkeletalMeshComponent* MeshCaine) override;//функция для проигрываня монтажей Unsheathe меча
	virtual void AttachWeaponToHips_Implementation() override;//Функция, которая приклепляет меч к поясу
	virtual void DetachWeaponToHips_Implementation() override;//функция, которая открепляет меч от пояса
	virtual void Attack_Implementation() override;//Функция атаки лкм
	virtual void CanNextAttack_Implementation(bool bSwitchCanNextAttack) override;//Функция, которая разрешает запустить некст атаку
	virtual void ResetIsAttaking_Implementation() override;//Функция сбрасывает переменную bIsAttaking, если игрок не успел нажать слудующий удар

protected:

	//Сюда сохраняем меш персонажа (для полета оружия в сокет на руке или для вызова аним инстанс)
	UPROPERTY(BlueprintReadOnly, Category = "WeaponSetings")
	class USkeletalMeshComponent* GetCharacterMesh;
	
	//Переменная текущей скорости. Эта переменная будет увеличиваться динамически в зависимости от AccelerationRate
	float CurrentFlySpeed = 0.0f;
	//Переменная ускорения
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSetings")
	float AccelerationRate = 20.0f;
	//Переменная расстояния защелкивания
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSetings")
	float AcceptableDistance = 5.0f;
	//Переменная сокет на руке
	UPROPERTY(EditAnywhere, Category = "WeaponSetings")
	FName HandSocketSword;
	//Переменная сокет на тазе
	UPROPERTY(EditAnywhere, Category = "WeaponSetings")
	FName HipsSocketSword;
	// Таймер
	FTimerHandle FlyWeaponTimerHandle;
	// Эвент, который запустит таймер
	UFUNCTION()
	void FlyWeaponTick();

protected:
	//Переменная, персонаж в процессе атаки
	UPROPERTY()
	bool bIsAttaking = false;
	//Переменная, которая разрешает нект атаку из массива
	UPROPERTY()
	bool bCanNextAttack = false;
	//Переменная, хранит текущий индекс атаки
	UPROPERTY()
	int AttackIndex = 0;
	//Переменная, сбрасывает тикущий индекс, если индекс атаки больше длинны массива (настраивается в бп оружия)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponSetings")
	int ResetAttackIndex;


	
	




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
