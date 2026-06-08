// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/PickUpAndDropWeaponAnimNotify.h"
#include "ClassCharacter/CharacterCaine.h"
#include "Interfaces/Interface_Character_Weapon.h"

void UPickUpAndDropWeaponAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ACharacterCaine* Character = Cast<ACharacterCaine>(MeshComp->GetOwner()))
		{
			if (Character->GetCurrentWeaponInHand())
			{
				AActor* GetWeapon = Character->GetCurrentWeaponInHand();

				IInterface_Character_Weapon::Execute_DetachWeapon(GetWeapon);//отщелкивает оружие от сокета руки
				IInterface_Character_Weapon::Execute_CLearWeaponInHand_Drop(Character);//обнуляет переменную ActorInHand в персонаже
			}
		}
	}
}

				
