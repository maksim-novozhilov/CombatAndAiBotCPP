// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Character_Weapon.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_Character_Weapon : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBATANDAIBOTCPP_API IInterface_Character_Weapon
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void FlyToHand(USkeletalMeshComponent* MeshCaine);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void DropWeapon(USkeletalMeshComponent* MeshCaine);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void DetachWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void CLearWeaponInHand_Drop();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void Attack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void CanNextAttack(bool bSwitchCanNextAttack);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void ResetIsAttaking();

	
	//----------------INTERACT-----------------------------------------------------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void PlayMontagesSword_Sheathe(USkeletalMeshComponent* MeshCaine);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void PlayMontagesSword_Unsheathe(USkeletalMeshComponent* MeshCaine);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void AttachWeaponToHips();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void DetachWeaponToHips();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void SwapWeaponSlots();
};

//---------------------------------------------------------------------------------------------------------------------------------------------------