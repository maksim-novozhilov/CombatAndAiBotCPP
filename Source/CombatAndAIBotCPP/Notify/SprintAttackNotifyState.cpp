// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/SprintAttackNotifyState.h"
#include "ClassCharacter/CharacterCaine.h"
#include "Interfaces/Interface_Character_Weapon.h"

void USprintAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ACharacterCaine* Character = Cast<ACharacterCaine>(MeshComp->GetOwner()))

	{
		if (AActor* WeaponInHand = Character->GetCurrentWeaponInHand())

		{
			IInterface_Character_Weapon::Execute_SetMovementMode(Character, true);

			IInterface_Character_Weapon::Execute_BlokingAttack(WeaponInHand, true);

		}

	}
	
}

void USprintAttackNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference & EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ACharacterCaine* Character = Cast<ACharacterCaine>(MeshComp->GetOwner()))
	{
		if (AActor* WeaponInHand = Character->GetCurrentWeaponInHand())
		{
			IInterface_Character_Weapon::Execute_SetMovementMode(Character, false);

			IInterface_Character_Weapon::Execute_BlokingAttack(WeaponInHand, false);

		}
		

	}


}
