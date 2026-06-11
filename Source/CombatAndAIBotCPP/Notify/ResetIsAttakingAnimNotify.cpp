// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/ResetIsAttakingAnimNotify.h"
#include "ClassCharacter/CharacterCaine.h"
#include "Interfaces/Interface_Character_Weapon.h"

void UResetIsAttakingAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);
	
        if (ACharacterCaine* Character = Cast<ACharacterCaine>(MeshComp->GetOwner()))
        {

            if (AActor* WeaponInHand = Character->GetCurrentWeaponInHand())
            {
                IInterface_Character_Weapon::Execute_ResetIsAttaking(WeaponInHand);
            }

        }
}

