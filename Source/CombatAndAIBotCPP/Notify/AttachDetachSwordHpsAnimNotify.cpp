// Fill out your copyright notice in the Description page of Project Settings.

#include "Notify/AttachDetachSwordHpsAnimNotify.h"
#include "ClassCharacter/CharacterCaine.h"
#include "Interfaces/Interface_Character_Weapon.h"

void UAttachDetachSwordHpsAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    
    if (!MeshComp) return;

    if (ACharacterCaine* Character = Cast<ACharacterCaine>(MeshComp->GetOwner()))
    {
        
        if (AActor* WeaponInHand = Character->GetCurrentWeaponInHand())
        {
            IInterface_Character_Weapon::Execute_AttachWeaponToHips(WeaponInHand);
            IInterface_Character_Weapon::Execute_SwapWeaponSlots(Character);
        }
        
        else if (AActor* WeaponInHips = Character->GetCurrentEquippedWeaponInHips())
        {
            IInterface_Character_Weapon::Execute_DetachWeaponToHips(WeaponInHips);
            IInterface_Character_Weapon::Execute_SwapWeaponSlots(Character);
        }
    }
}


				
				

				
				
				;


				